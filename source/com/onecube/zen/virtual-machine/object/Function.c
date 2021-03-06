/*
 * Copyright 2018-2020 Samuel Rowe
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Tuesday, March 26, 2019

#include <jtk/core/CString.h>
#include <com/onecube/zen/virtual-machine/feb/EntityFlag.h>
#include <com/onecube/zen/virtual-machine/object/Function.h>
#include <com/onecube/zen/virtual-machine/object/Class.h>
#include <com/onecube/zen/virtual-machine/VirtualMachine.h>
#include <com/onecube/zen/virtual-machine/feb/attribute/PredefinedAttribute.h>

/*******************************************************************************
 * Function                                                                    *
 *******************************************************************************/

// Constructor

/* A native function can be loaded in two ways. If a native
 * function is registered before the program begins execution,
 * the native function is automatically associated with the
 * function object when it is being loaded from the binary entity
 * format. This ensures that native functions are always present
 * throughtout the life cycle of a class. This is called as cold
 * registration.
 * 
 * Alternatively, a native function can be registered after the
 * class has been loaded. This is called as hot registration.
 */

zen_Function_t* zen_Function_new(zen_VirtualMachine_t* virtualMachine,
    zen_Class_t* class0, zen_FunctionEntity_t* functionEntity) {
    zen_ConstantPool_t* constantPool = &class0->m_entityFile->m_constantPool;
    zen_ConstantPoolUtf8_t* nameEntry = constantPool->m_entries[functionEntity->m_nameIndex];
    zen_ConstantPoolUtf8_t* descriptorEntry = constantPool->m_entries[functionEntity->m_descriptorIndex];

    zen_Function_t* function = jtk_Memory_allocate(zen_Function_t, 1);
    function->m_name = jtk_CString_newEx(nameEntry->m_bytes, nameEntry->m_length);
    function->m_nameSize = nameEntry->m_length;
    function->m_descriptor = jtk_CString_newEx(descriptorEntry->m_bytes, descriptorEntry->m_length);
    function->m_descriptorSize = descriptorEntry->m_length;
    function->m_class = class0;
    function->m_flags = 0;
    function->m_functionEntity = functionEntity;
    function->m_nativeFunction = zen_VirtualMachine_getNativeFunction(virtualMachine,
        class0->m_descriptor, class0->m_descriptorSize,
        function->m_name, function->m_nameSize,
        function->m_descriptor, function->m_descriptorSize);

    // type, startIndex
    int32_t afterColon = -1;
    int32_t parameterCount = 0;
    int32_t i;
    /* The following function descriptor parser is a naive implementation.
     * It assumes that the given function descriptor is not malformed.
     * The purpose of this parser is to simply extract relevant information
     * as quickly as possible.
     */
    for (i = 0; i < descriptorEntry->m_length; i++) {
        switch (descriptorEntry->m_bytes[i]) {
            /* When an array type is encountered, skip all the '@' symbols. The
             * base type of the array will be counted as the parameter.
             */
            case '@': {
                continue;
            }

            /* When the colon symbol is encountered, ignore it. */
            case ':': {
                /* Do not count the return type as a parameter. */
                afterColon = i + 1;
                continue;
            }

            case '(': {
                while (descriptorEntry->m_bytes[++i] != ')');
                if (afterColon >= 0) {
                    parameterCount++;
                }
                break;
            }

            case 'z':
            case 'b':
            case 'c':
            case 's':
            case 'i':
            case 'l':
            case 'f':
            case 'd':
            case 'v': {
                if (afterColon >= 0) {
                    /* Note that parameter count is incremented for void only for the sake of parsing.
                    * It is adjusted in the next section.
                    */
                    parameterCount++;
                }
                break;
            }

            default: {
                printf("[error] Malformed function descriptor '%.*s'\n",
                    descriptorEntry->m_length, descriptorEntry->m_bytes);
            }
        }
    }

    int32_t lastKnownVoid = -1;
    zen_Type_t returnType = ZEN_TYPE_UNKNOWN;
    int32_t* parameters = jtk_Memory_allocate(int32_t, parameterCount * 2);
    zen_Type_t type;
    int32_t parameterIndex;
    for (i = 0, parameterIndex = -1;
        (i < descriptorEntry->m_length) && (parameterIndex < parameterCount);
        i++, parameterIndex++) {
        int32_t startIndex = i;
        switch (descriptorEntry->m_bytes[i]) {
            case '@': {
                /* Arrays are also considered as reference types. */
                type = ZEN_TYPE_REFERENCE;
                while (descriptorEntry->m_bytes[++i] == '@');
                if (descriptorEntry->m_bytes[i] == '(') {
                    while (descriptorEntry->m_bytes[++i] != ')');
                }
                else {
                    /* Skip the primitive type. */
                    i++;
                }
                break;
            }
            
            case 'z': {
                type = ZEN_TYPE_BOOLEAN;
                break;
            }

            case 'b': {
                type = ZEN_TYPE_INTEGER_8;
                break;
            }

            case 'c': {
                type = ZEN_TYPE_CHARACTER;
                break;
            }

            case 's': {
                type = ZEN_TYPE_INTEGER_16;
                break;
            }

            case 'i': {
                type = ZEN_TYPE_INTEGER_32;
                break;
            }

            case 'l': {
                type = ZEN_TYPE_INTEGER_64;
                break;
            }

            case 'f': {
                type = ZEN_TYPE_DECIMAL_32;
                break;
            }

            case 'd': {
                type = ZEN_TYPE_DECIMAL_64;
                break;
            }

            case 'v': {
                type = ZEN_TYPE_VOID;
                lastKnownVoid = i;
                break;
            }

            case '(': {
                type = ZEN_TYPE_REFERENCE;
                while (descriptorEntry->m_bytes[++i] != ')');
                break;
            }

            default: {
            }
        }
        if (parameterIndex < 0) {
            /* Skip the colon after the return type. */
            returnType = type;
            i++;
        }
        else {
            int32_t index = parameterIndex * 2;
            parameters[index] = type;
            parameters[index + 1] = startIndex;
        }
    }

    if (lastKnownVoid >= afterColon) {
        if (parameterCount == 1) {
            parameterCount = 0;
        }
        else {
            printf("[error] Malformed function descriptor '%.*s'.\n",
                descriptorEntry->m_length, descriptorEntry->m_bytes);
        }
    }

    function->m_returnType = returnType;
    function->m_parameterCount = parameterCount;
    function->m_parameters = parameters;

    function->m_instructionAttribute = NULL;
    if ((function->m_functionEntity->m_flags & (1 << 7)) == 0) {
        int32_t limit = functionEntity->m_attributeTable.m_size;
        for (i = 0; i < limit; i++) {
            zen_Attribute_t* attribute = functionEntity->m_attributeTable.m_attributes[i];
            zen_ConstantPoolUtf8_t* nameEntry =
                (zen_ConstantPoolUtf8_t*)constantPool->m_entries[attribute->m_nameIndex];

            if (jtk_CString_equals(nameEntry->m_bytes, nameEntry->m_length,
                ZEN_PREDEFINED_ATTRIBUTE_INSTRUCTION, ZEN_PREDEFINED_ATTRIBUTE_INSTRUCTION_SIZE)) {
                function->m_instructionAttribute = (zen_InstructionAttribute_t*)attribute;
                // maxStackSize = instructionAttribute->m_maxStackSize;

                break;
            }
        }
    }


    return function;
}

// Destructor

void zen_Function_delete(zen_Function_t* function) {
    jtk_Memory_deallocate(function->m_parameters);
    jtk_CString_delete(function->m_name);
    jtk_CString_delete(function->m_descriptor);
    jtk_Memory_deallocate(function);
}

// Class

zen_Class_t* zen_Function_getClass(zen_Function_t* function) {
    return function->m_class;
}

// Native

bool zen_Function_isNative(zen_Function_t* function) {
    return (function->m_functionEntity->m_flags & (1 << 7)) != 0;
}