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

// Friday, June 08, 2018

#include <jtk/collection/array/Arrays.h>
#include <jtk/core/VariableArguments.h>
#include <jtk/core/CString.h>
#include <jtk/core/StringBuilder.h>
#include <jtk/core/StringObjectAdapter.h>

#include <com/onecube/zen/virtual-machine/ExceptionManager.h>
#include <com/onecube/zen/virtual-machine/VirtualMachine.h>
#include <com/onecube/zen/virtual-machine/processor/Interpreter.h>
#include <com/onecube/zen/virtual-machine/loader/EntityLoader.h>
#include <com/onecube/zen/virtual-machine/object/Class.h>

/* Object */

/**
 * The layout of the object in memory is shown below.
 * | Object Header | Object Body |
 *
 * The object header is constant, that is, the size is fixed based on the
 * implementation of the virtual machine. On the other hand, the size of the
 * object body is variable, contingent on both the implementation and the
 * fields declared in the class whose instance the object is.
 *
 * [Object Header]
 * struct jtk_ObjectHeader_t {
 *     uintptr_t m_class;       // Depends on the platform.
 *     uint32_t m_hashCode;     // 4 bytes
 * };
 */
#define ZEN_OBJECT_HEADER_CLASS_SIZE sizeof (uintptr_t)
#define ZEN_OBJECT_HEADER_HASH_CODE_SIZE sizeof (uint32_t)
#define ZEN_OBJECT_HEADER_SIZE (ZEN_OBJECT_HEADER_CLASS_SIZE + \
                               ZEN_OBJECT_HEADER_HASH_CODE_SIZE)
#define ZEN_OBJECT_HEADER_CLASS_OFFSET 0
#define ZEN_OBJECT_HEADER_HASH_CODE_OFFSET ZEN_OBJECT_HEADER_CLASS_SIZE

zen_Class_t* zen_Object_getClass(zen_Object_t* object);
bool zen_VirtualMachine_isInstance(zen_VirtualMachine_t* virtualMachine,
    zen_Object_t* object, jtk_String_t* classDescriptor);
int32_t zen_VirtualMachine_getStringSize(zen_VirtualMachine_t* virtualMachine, zen_Object_t* string);
uint8_t* zen_VirtualMachine_getStringBytes(zen_VirtualMachine_t* virtualMachine, zen_Object_t* string);

// Identity Hash

int32_t zen_VirtualMachine_identityHash(zen_Object_t* object) {
    if (sizeof (uintptr_t) == 4) {
        return (int32_t)object;
    }
    else if (sizeof (uintptr_t) == 8) {
        return (int32_t)((uintptr_t)object ^ ((uintptr_t)object >> 32));
    }
    else {
        return (int32_t)((uintptr_t)object);
    }
}

// ** Native Functions **

zen_Class_t* zen_Object_getClass(zen_Object_t* object) {
    jtk_Assert_assertObject(object, "The specified object is null.");

    zen_Class_t** classField = ((uint8_t*)object + ZEN_OBJECT_HEADER_CLASS_OFFSET);
    return *classField;
}

bool zen_VirtualMachine_isInstance(zen_VirtualMachine_t* virtualMachine,
    zen_Object_t* object, jtk_String_t* classDescriptor) {
    zen_Class_t* class0 = zen_VirtualMachine_getClass(virtualMachine, classDescriptor);
    zen_Class_t** classField = ((uint8_t*)object + ZEN_OBJECT_HEADER_CLASS_OFFSET);
    return *classField == class0;
}

void zen_print(zen_VirtualMachine_t* virtualMachine, jtk_Array_t* arguments) {
    jtk_String_t* stringDescriptor = jtk_String_newEx("zen/core/String", 15);
    jtk_String_t* integerDescriptor = jtk_String_newEx("zen/core/Integer", 16);

    zen_Object_t* argument = (zen_Object_t*)jtk_Array_getValue(arguments, 0);;

    if (zen_VirtualMachine_isInstance(virtualMachine, argument, stringDescriptor)) {
        zen_Object_t* format = argument;
        zen_Object_t* array = zen_VirtualMachine_getObjectField(virtualMachine,
            format, "value", 5);
        uint8_t* values = (uint8_t*)zen_VirtualMachine_getObjectField(virtualMachine,
            array, "values", 6);
        int32_t size = (int32_t)zen_VirtualMachine_getObjectField(virtualMachine,
            array, "size", 4);
        fwrite(values, 1, size, stdout);
        puts("");
    }
    else if (zen_VirtualMachine_isInstance(virtualMachine, argument, integerDescriptor)) {
        zen_Object_t* value = (zen_Object_t*)jtk_Array_getValue(arguments, 0);
        int64_t value0 = (int64_t)zen_VirtualMachine_getObjectField(virtualMachine,
            value, "value", 5);
        printf("%ld\n", value0);
    }
    else {
        printf("Object <%lX>\n", argument);
    }

    fflush(stdout);
}

void zen_Integer_initialize(zen_VirtualMachine_t* virtualMachine,
    zen_Object_t* self, jtk_Array_t* arguments) {
    const uint8_t* valueDescriptor = "value";
    int32_t valueDescriptorSize = 5;
    zen_Object_t* value = (zen_Object_t*)jtk_Array_getValue(arguments, 0);
    zen_VirtualMachine_setObjectField(virtualMachine, self, valueDescriptor,
        valueDescriptorSize, value);
}

zen_Object_t* zen_Integer_add(zen_VirtualMachine_t* virtualMachine,
    jtk_Array_t* arguments) {
    zen_Object_t* operand1 = (zen_Object_t*)jtk_Array_getValue(arguments, 0);
    zen_Object_t* operand2 = (zen_Object_t*)jtk_Array_getValue(arguments, 1);
    
    int64_t value1 = (int64_t)zen_VirtualMachine_getObjectField(virtualMachine,
            operand1, "value", 5);
    int64_t value2 = (int64_t)zen_VirtualMachine_getObjectField(virtualMachine,
            operand2, "value", 5);

    int64_t result = value1 + value2;
    printf("%ld\n", result);

    return result;
}


void zen_String_initialize(zen_VirtualMachine_t* virtualMachine,
    zen_Object_t* self, jtk_VariableArguments_t arguments) {
    const uint8_t* valueDescriptor = "value";
    int32_t valueDescriptorSize = 5;
    zen_Object_t* value = jtk_VariableArguments_argument(arguments, zen_Object_t*);
    zen_VirtualMachine_setObjectField(virtualMachine, self, valueDescriptor,
        valueDescriptorSize, value);
}

jtk_String_t* zen_String_add(zen_VirtualMachine_t* virtualMachine,
    jtk_Array_t* arguments) {
    zen_Object_t* self = (zen_Object_t*)jtk_Array_getValue(arguments, 0);
    zen_Object_t* other = (zen_Object_t*)jtk_Array_getValue(arguments, 1);
    
    uint8_t* selfBytes = zen_VirtualMachine_getStringBytes(virtualMachine, self);
    uint8_t* otherBytes = zen_VirtualMachine_getStringBytes(virtualMachine, other);
    
    uint8_t* selfSize = zen_VirtualMachine_getStringSize(virtualMachine, self);
    uint8_t* otherSize = zen_VirtualMachine_getStringSize(virtualMachine, other);

    jtk_String_t* result = jtk_String_newFromJoinEx(selfBytes, selfSize,
        otherBytes, otherSize);

    printf("%.*s\n", result->m_size, result->m_value);

    return result;
}

int32_t zen_String_getSize(zen_VirtualMachine_t* virtualMachine, jtk_Array_t* arguments) {
    return 0;
}

int32_t zen_VirtualMachine_getStringSize(zen_VirtualMachine_t* virtualMachine, zen_Object_t* string) {
    zen_Object_t* array = zen_VirtualMachine_getObjectField(virtualMachine, string, "value", 5);
    return (int32_t)zen_VirtualMachine_getObjectField(virtualMachine, array, "size", 4);
}

uint8_t* zen_VirtualMachine_getStringBytes(zen_VirtualMachine_t* virtualMachine, zen_Object_t* string) {
    zen_Object_t* array = zen_VirtualMachine_getObjectField(virtualMachine, string, "value", 5);
    uint8_t* bytes = zen_VirtualMachine_getObjectField(virtualMachine, array, "values", 6);
    return bytes;
}

jtk_String_t* zen_ByteArray_toJTKString(zen_VirtualMachine_t* virtualMachine,
    zen_Object_t* array) {
    uint8_t* bytes = zen_VirtualMachine_getObjectField(virtualMachine, array, "values", 6);
    int32_t size = (int32_t)zen_VirtualMachine_getObjectField(virtualMachine, array, "size", 4);
    return jtk_String_newEx(bytes, size);
}

jtk_String_t* zen_String_toJTKString(zen_VirtualMachine_t* virtualMachine,
    zen_Object_t* string) {
    zen_Object_t* value = zen_VirtualMachine_getObjectField(virtualMachine, string, "value", 5);
    return zen_ByteArray_toJTKString(virtualMachine, value);
}

void zen_ZenKernel_invokeStatic(zen_VirtualMachine_t* virtualMachine,
    jtk_Array_t* arguments) {
    zen_Object_t* entity = (zen_Object_t*)jtk_Array_getValue(arguments, 0);
    zen_Object_t* targetFunctionName = (zen_Object_t*)jtk_Array_getValue(arguments, 1);
    jtk_Array_t* targetArguments = (jtk_Array_t*)jtk_Array_getValue(arguments, 2);
    jtk_StringBuilder_t* builder = jtk_StringBuilder_new();
    jtk_StringBuilder_appendEx_z(builder, "(zen/core/Object):", 18);
    int32_t i;
    int32_t parameterCount = jtk_Array_getSize(targetArguments);
    for (i = 0; i < parameterCount; i++) {
        jtk_StringBuilder_appendEx_z(builder, "(zen/core/Object)", 17);
    }

    jtk_String_t* targetFunctionDescriptor = jtk_StringBuilder_toString(builder);
    jtk_String_t* entity0 = zen_String_toJTKString(virtualMachine, entity);
    jtk_String_t* targetFunctionName0 = zen_String_toJTKString(virtualMachine, targetFunctionName);

    zen_Class_t* targetClass = zen_VirtualMachine_getClass(virtualMachine,
        entity0);
    zen_Function_t* targetFunction = zen_VirtualMachine_getStaticFunction(virtualMachine,
        targetClass, targetFunctionName0, targetFunctionDescriptor);

    jtk_String_delete(targetFunctionName0);
    jtk_String_delete(entity0);
    jtk_String_delete(targetFunctionDescriptor);

    zen_Interpreter_invokeStaticFunction(virtualMachine->m_interpreter, targetFunction,
        targetArguments);
}

/* TODO: The evaluate function builds a cache of operator-function pairs
 * as the program runs. This helps in reducing lookup time for mapping operators
 * to functions.
 */
void* zen_ZenKernel_evaluate(zen_VirtualMachine_t* virtualMachine,
    jtk_Array_t* arguments) {
    zen_Object_t* operand1 = (zen_Object_t*)jtk_Array_getValue(arguments, 0);
    zen_Object_t* operand2 = (zen_Object_t*)jtk_Array_getValue(arguments, 1);
    zen_Object_t* symbol = (zen_Object_t*)jtk_Array_getValue(arguments, 2);

    uint8_t* symbolBytes = zen_VirtualMachine_getStringBytes(virtualMachine, symbol);
    int32_t symbolSize = zen_VirtualMachine_getStringSize(virtualMachine, symbol);
    zen_Class_t* class0 = zen_Object_getClass(operand1);

    jtk_String_t* targetFunctionName = NULL;
    jtk_String_t* targetFunctionSignature = jtk_String_newEx("(zen/core/Object):(zen/core/Object)(zen/core/Object)", 52);

    switch (symbolSize) {
        case 1: {
            if (jtk_CString_equals(symbolBytes, symbolSize, "+", 1)) {
                targetFunctionName = jtk_String_newEx("add", 3);
            }
            break;
        }
    }

    void* result = NULL;
    if (targetFunctionName != NULL) {
        zen_Function_t* targetFunction = zen_VirtualMachine_getStaticFunction(virtualMachine, class0, targetFunctionName, targetFunctionSignature);

        /* NOTE: The arguments received by the evaluate() function are forwarded to the
         * target function as is, assuming that the target function uses only the
         * first two values.
         */
        /* result = */ zen_Interpreter_invokeStaticFunction(virtualMachine->m_interpreter, targetFunction,
            arguments);

        jtk_String_delete(targetFunctionSignature);
        jtk_String_delete(targetFunctionName);
    }
    else {
        printf("[error] An exception was thrown.\n"
            "[error] UnknownOperatorException: Cannot find a matching method for symbol '%.*s'\n",
            symbolSize, symbolBytes);
    }

    return result;
}

/*******************************************************************************
 * VirtualMachine                                                              *
 *******************************************************************************/

// Constructor

zen_VirtualMachine_t* zen_VirtualMachine_new(zen_VirtualMachineConfiguration_t* configuration) {
    jtk_Assert_assertObject(configuration, "The specified virtual machine configuration is null.");

    jtk_ObjectAdapter_t* stringObjectAdapter = jtk_StringObjectAdapter_getInstance();
    jtk_Iterator_t* entityDirectoryIterator = jtk_ArrayList_getIterator(configuration->m_entityDirectories);

    zen_VirtualMachine_t* virtualMachine = zen_Memory_allocate(zen_VirtualMachine_t, 1);
    virtualMachine->m_configuration = configuration;
    virtualMachine->m_entityLoader = zen_EntityLoader_newWithEntityDirectories(entityDirectoryIterator);
    virtualMachine->m_classLoader = zen_ClassLoader_new(virtualMachine->m_entityLoader);
    virtualMachine->m_interpreter = zen_Interpreter_new(NULL, virtualMachine, NULL);
    virtualMachine->m_nativeFunctions = jtk_HashMap_newEx(stringObjectAdapter, NULL,
        JTK_HASH_MAP_DEFAULT_CAPACITY, JTK_HASH_MAP_DEFAULT_LOAD_FACTOR);

    zen_VirtualMachine_loadDefaultLibraries(virtualMachine);

    return virtualMachine;
}

// Destructor

void zen_VirtualMachine_delete(zen_VirtualMachine_t* virtualMachine) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");

    zen_VirtualMachine_unloadLibraries(virtualMachine);

    zen_ClassLoader_delete(virtualMachine->m_classLoader);
    zen_EntityLoader_delete(virtualMachine->m_entityLoader);
    jtk_Memory_deallocate(virtualMachine);
}

// Array

zen_ObjectArray_t* zen_VirtualMachine_newObjectArray(zen_VirtualMachine_t* virtualMachine,
    zen_Class_t* class0, int32_t size) {
    return NULL;
}

zen_ObjectArray_t* zen_VirtualMachine_newByteArray(
    zen_VirtualMachine_t* virtualMachine, int8_t* bytes, int32_t size) {
    const uint8_t* arrayDescriptor = "zen/core/Array";
    int32_t arrayDescriptorSize = 14;
    const uint8_t* constructorDescriptor = "v:v";
    int32_t constructorDescriptorSize = 3;

    zen_Object_t* result = zen_VirtualMachine_newObject(virtualMachine,
        arrayDescriptor, arrayDescriptorSize, constructorDescriptor,
        constructorDescriptorSize);
    uint8_t* values = jtk_Arrays_clone_b(bytes, size);

    zen_VirtualMachine_setObjectField(virtualMachine, result, "values", 6, values);
    // TODO: Change setObjectField to setIntegerField!
    zen_VirtualMachine_setObjectField(virtualMachine, result, "size", 4, size);

    return result;
}

// Class

zen_Class_t* zen_VirtualMachine_getClass(zen_VirtualMachine_t* virtualMachine,
    jtk_String_t* descriptor) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    jtk_Assert_assertObject(descriptor, "The specified class descriptor is null.");

    zen_Class_t* class0 = zen_ClassLoader_findClass(
        virtualMachine->m_classLoader, descriptor);

    if (class0 == NULL) {
        zen_VirtualMachine_raiseClassNotFoundException(virtualMachine, descriptor);
    }

    return class0;
}

// Clear

bool zen_VirtualMachine_isClear(zen_VirtualMachine_t* virtualMachine) {
    return true;
}

// Entity Loader

zen_EntityLoader_t* zen_VirtualMachine_getEntityLoader(
    zen_VirtualMachine_t* virtualMachine) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");

    return virtualMachine->m_entityLoader;
}

// Field

void hexDump(const uint8_t* description, void* address, const int length) {
    printf("Dumping object at 0x%X\n", address);
    int i;
    unsigned char buffer[17];
    const unsigned char* iterator = (const unsigned char *)address;

    /* Print the description if given. */
    if (description != NULL) {
        printf ("%s:\n", description);
    }

    if (length == 0) {
        printf("[warning] The specified object is empty, that is, has zero length.\n");
    }
    else if (length < 0) {
        printf("[error] The specified object has negative length %d!\n", length);
    }
    else {
        /* Process each byte in the specified object. */
        for (i = 0; i < length; i++) {
            /* If the current index is a multiple of 16, print new line with line
             * offset.
             */
            if ((i % 16) == 0) {
                /* Do not print ASCII bufferer for the "zeroth" line. */
                if (i != 0) {
                    printf ("  %s\n", buffer);
                }

                /* Print the offset. */
                printf (" [%04x] ", i);
            }

            /* Print the hexadecimal code for the current byte. */
            printf(" %02x", iterator[i]);

            /* Append a printable ASCII character to the buffer. */
            /* TODO: isprint() may be better. */
            if ((iterator[i] < 0x20) || (iterator[i] > 0x7e)) {
                buffer[i % 16] = '.';
            }
            else {
                buffer[i % 16] = iterator[i];
            }
            buffer[(i % 16) + 1] = '\0';
        }

        /* Pad the last line if there are lesser than 16 characters. */
        while ((i % 16) != 0) {
            printf("   ");
            i++;
        }

        /* Print the final ASCII representation of the bytes. */
        printf("  %s\n", buffer);
    }
}

void zen_VirtualMachine_setObjectField(zen_VirtualMachine_t* virtualMachine,
    zen_Object_t* object, const uint8_t* fieldDescriptor, int32_t fieldDescriptorSize,
    zen_Object_t* value) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");

    if (object == NULL) {
        zen_VirtualMachine_raiseNullReferenceException(virtualMachine);
    }
    else {
        zen_Class_t* class0 = *((zen_Class_t**)(object + ZEN_OBJECT_HEADER_CLASS_OFFSET));
        int32_t offset = zen_Class_findFieldOffset(class0, fieldDescriptor, fieldDescriptorSize);
        if (offset < 0) {
            zen_VirtualMachine_raiseUnknownFieldException(virtualMachine, fieldDescriptor,
                fieldDescriptorSize);
        }
        else {
            *((zen_Object_t**)((uint8_t*)object + ZEN_OBJECT_HEADER_SIZE + offset)) = value;
            // memcpy((uint8_t*)object + ZEN_OBJECT_HEADER_SIZE + offset, &value, sizeof (zen_Object_t*));
        }
        // hexDump(NULL, object, class0->m_memoryRequirement + ZEN_OBJECT_HEADER_CLASS_SIZE);
    }

}

zen_Object_t* zen_VirtualMachine_getObjectField(zen_VirtualMachine_t* virtualMachine,
    zen_Object_t* object, uint8_t* fieldName, int32_t fieldNameSize) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");

    zen_Object_t* result = NULL;
    if (object == NULL) {
        zen_VirtualMachine_raiseNullReferenceException(virtualMachine);
    }
    else {
        zen_Class_t** classField = (zen_Class_t**)((uint8_t*)object + ZEN_OBJECT_HEADER_CLASS_OFFSET);
        zen_Class_t* class0 = *classField;
        int32_t offset = zen_Class_findFieldOffset(class0, fieldName, fieldNameSize);
        if (offset < 0) {
            zen_VirtualMachine_raiseUnknownFieldException(virtualMachine, fieldName,
                fieldNameSize);
        }
        else {
            result = *((zen_Object_t**)((uint8_t*)object + ZEN_OBJECT_HEADER_SIZE + offset));
        }
        // hexDump(NULL, object, class0->m_memoryRequirement + ZEN_OBJECT_HEADER_CLASS_SIZE);
    }

    return result;
}

// Libraries

void zen_VirtualMachine_loadDefaultLibraries(zen_VirtualMachine_t* virtualMachine) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");

    /*
     * TODO: Should we associate native functions with their respective classes?
     * As of now, only the classes are associated with the native functions.
     * In other words, there exists a unidirectional relationship between classes
     * and native functions, with the class owning the native function.
     */

    // Object Test.print(Object format)
    zen_VirtualMachine_registerNativeFunction(virtualMachine, "Test", 4,
        "print", 5, "(zen/core/Object):(zen/core/Object)", 35, zen_print);

    // Object ZenKernel.invokeStatic(Object className, Object functionName, Object ... arguments)
    zen_VirtualMachine_registerNativeFunction(virtualMachine, "ZenKernel", 9,
        "invokeStatic", 12, "(zen/core/Object):(zen/core/Object)(zen/core/Object)@(zen/core/Object)",
        70, zen_ZenKernel_invokeStatic);

    // Object ZenKernel.evaluate(Object operator, Object operand1, Object operand2)
    zen_VirtualMachine_registerNativeFunction(virtualMachine, "ZenKernel", 9,
        "evaluate", 8, "(zen/core/Object):(zen/core/Object)(zen/core/Object)(zen/core/Object)",
        69, zen_ZenKernel_evaluate);

    // void String.new(value)
    zen_VirtualMachine_registerNativeFunction(virtualMachine, "String", 6,
        "<initialize>", 12, "v:(zen/core/Object)", 19, zen_String_initialize);

    // String String.add(value1, value2)
    zen_VirtualMachine_registerNativeFunction(virtualMachine, "String", 6,
        "add", 3, "(zen/core/Object):(zen/core/Object)(zen/core/Object)", 52, zen_String_add);

    // void Integer.new(value)
    zen_VirtualMachine_registerNativeFunction(virtualMachine, "Integer", 7,
        "<initialize>", 12, "v:(zen/core/Object)", 19, zen_Integer_initialize);

    // Integer Integer.add(value1, value2)
    zen_VirtualMachine_registerNativeFunction(virtualMachine, "Integer", 7,
        "add", 3, "(zen/core/Object):(zen/core/Object)(zen/core/Object)", 52, zen_Integer_add);

    // TODO: Unload native functions
}

void zen_VirtualMachine_unloadLibraries(zen_VirtualMachine_t* virtualMachine) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");

    jtk_Iterator_t* entryIterator = jtk_HashMap_getEntryIterator(virtualMachine->m_nativeFunctions);
    while (jtk_Iterator_hasNext(entryIterator)) {
        jtk_HashMapEntry_t* entry = (jtk_HashMapEntry_t*)jtk_Iterator_getNext(entryIterator);
        jtk_String_t* key = (jtk_String_t*)jtk_HashMapEntry_getKey(entry);
        zen_NativeFunction_t* value = (zen_NativeFunction_t*)jtk_HashMapEntry_getValue(entry);

        jtk_String_delete(key);
        zen_NativeFunction_delete(value);
    }
}

// Native Function

zen_NativeFunction_t* zen_VirtualMachine_getNativeFunction(
    zen_VirtualMachine_t* virtualMachine, jtk_String_t* className,
    jtk_String_t* functionName, jtk_String_t* functionDescriptor) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    jtk_Assert_assertObject(className, "The specified class name is null.");
    jtk_Assert_assertObject(functionName, "The specified function name is null.");
    jtk_Assert_assertObject(functionDescriptor, "The specified function descriptor is null.");

    const uint8_t* strings[] = {
        className->m_value,
        functionName->m_value,
        functionDescriptor->m_value
    };
    int32_t sizes[] = {
        className->m_size,
        functionName->m_size,
        functionDescriptor->m_size
    };
    jtk_String_t* key = jtk_String_fromJoin(strings, sizes, 3);
    zen_NativeFunction_t* result = (zen_NativeFunction_t*)jtk_HashMap_getValue(
        virtualMachine->m_nativeFunctions, key);
    jtk_String_delete(key);

    return result;
}

zen_NativeFunction_t* zen_VirtualMachine_registerNativeFunction(
    zen_VirtualMachine_t* virtualMachine, const uint8_t* className,
    int32_t classNameSize, const uint8_t* functionName, int32_t functionNameSize,
    const uint8_t* functionDescriptor, int32_t functionDescriptorSize,
    zen_NativeFunction_InvokeFunction_t function) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    jtk_Assert_assertObject(className, "The specified class name is null.");
    jtk_Assert_assertTrue(classNameSize > 0, "The specified class name size is invalid.");
    jtk_Assert_assertObject(functionName, "The specified function name is null.");
    jtk_Assert_assertTrue(functionNameSize > 0, "The specified function name size is invalid.");
    jtk_Assert_assertObject(functionDescriptor, "The specified function descriptor is null.");
    jtk_Assert_assertTrue(functionDescriptorSize > 0, "The specified function descriptor size is invalid.");

    zen_NativeFunction_t* nativeFunction = zen_NativeFunction_new(function);
    const uint8_t* strings[] = {
        className,
        functionName,
        functionDescriptor
    };
    int32_t sizes[] = {
        classNameSize,
        functionNameSize,
        functionDescriptorSize
    };
    jtk_String_t* key = jtk_String_fromJoin(strings, sizes, 3);
    jtk_HashMap_put(virtualMachine->m_nativeFunctions, key, nativeFunction);

    return nativeFunction;
}

// Object

zen_Object_t* zen_VirtualMachine_allocateObject(zen_VirtualMachine_t* virtualMachine,
    zen_Class_t* class0) {
    /* The allocation should be clean such that all the bits are initialized
     * to 0.
     */
    zen_Object_t* object = calloc(ZEN_OBJECT_HEADER_SIZE + class0->m_memoryRequirement, sizeof (uint8_t));
    uintptr_t* classField = (uintptr_t*)((uint8_t*)object + ZEN_OBJECT_HEADER_CLASS_OFFSET);
    int32_t* hashField = (int32_t*)((uint8_t*)object + ZEN_OBJECT_HEADER_HASH_CODE_OFFSET);
    
    *classField = (uintptr_t)class0;
    *hashField = zen_VirtualMachine_identityHash(object);

    return object;
}

zen_Object_t* zen_VirtualMachine_makeObjectEx(zen_VirtualMachine_t* virtualMachine,
    zen_Function_t* constructor, jtk_VariableArguments_t arguments) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    jtk_Assert_assertObject(constructor, "The specified constructor is null.");

    zen_Class_t* class0 = constructor->m_class;
    /* The allocation should be clean such that all the bits are initialized
     * to 0.
     */
    zen_Object_t* object = calloc(ZEN_OBJECT_HEADER_SIZE + class0->m_memoryRequirement, sizeof (uint8_t));
    *((uintptr_t*)object + ZEN_OBJECT_HEADER_CLASS_OFFSET) = (uintptr_t)class0;
    *((uint32_t*)object + ZEN_OBJECT_HEADER_HASH_CODE_OFFSET) = zen_VirtualMachine_identityHash(object);

    zen_Interpreter_invokeConstructor(virtualMachine->m_interpreter, object,
        constructor, arguments);

    /* Chances are an exception may have been thrown in the constructor. Further,
     * a reference to the object may be stored somewhere by the constructor.
     * Therefore, the object should not be immediately deallocated. Let the garbage
     * collector do its job.
     */

    return object;
}

zen_Object_t* zen_VirtualMachine_makeObject(zen_VirtualMachine_t* virtualMachine,
    zen_Function_t* constructor, ...) {
    jtk_VariableArguments_t arguments;
    jtk_VariableArguments_start(arguments, constructor);
    zen_Object_t* result = zen_VirtualMachine_makeObjectEx(virtualMachine,
        constructor, arguments);
    jtk_VariableArguments_end(arguments);
    return result;
}

zen_Object_t* zen_VirtualMachine_newObjectEx(zen_VirtualMachine_t* virtualMachine,
    const uint8_t* classDescriptor, int32_t classDescriptorSize,
    const uint8_t* constructorDescriptor, int32_t constructorDescriptorSize,
    jtk_VariableArguments_t arguments) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    jtk_Assert_assertObject(classDescriptor, "The specified class descriptor is null.");
    jtk_Assert_assertObject(constructorDescriptor, "The specified constructor descriptor is null.");

    jtk_String_t* classDescriptor0 = jtk_String_newEx(classDescriptor, classDescriptorSize);
    zen_Class_t* class0 = zen_VirtualMachine_getClass(virtualMachine, classDescriptor0);
    jtk_String_delete(classDescriptor0);

    zen_Object_t* result = NULL;
    if (zen_VirtualMachine_isClear(virtualMachine)) {
        jtk_String_t* constructorDescriptor0 = jtk_String_newEx(constructorDescriptor, constructorDescriptorSize);
        zen_Function_t* constructor = zen_Class_getConstructor(class0, constructorDescriptor0);
        if (zen_VirtualMachine_isClear(virtualMachine)) {

            result = zen_VirtualMachine_makeObjectEx(virtualMachine, constructor, arguments);
        }
    }

    return result;
}

zen_Object_t* zen_VirtualMachine_newObject(zen_VirtualMachine_t* virtualMachine,
    const uint8_t* classDescriptor, int32_t classDescriptorSize,
    const uint8_t* constructorDescriptor, int32_t constructorDescriptorSize, ...) {
    jtk_VariableArguments_t arguments;
    jtk_VariableArguments_start(arguments, constructorDescriptorSize);
    zen_Object_t* object = zen_VirtualMachine_newObjectEx(virtualMachine,
        classDescriptor, classDescriptorSize, constructorDescriptor,
        constructorDescriptorSize, arguments);
    jtk_VariableArguments_end(arguments);
    return object;
}

// Raise Exception

void zen_VirtualMachine_raiseException(zen_VirtualMachine_t* virtualMachine,
    zen_Object_t* exception) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    jtk_Assert_assertObject(exception, "The specified exception is null.");

    printf("[debug] An exception was raised.\n");
}

void zen_VirtualMachine_raiseUnknownFieldException(zen_VirtualMachine_t* virtualMachine,
    const uint8_t* fieldDescriptor, int32_t fieldDescriptorSize) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    jtk_Assert_assertObject(fieldDescriptor, "The specified field descriptor is null.");

    printf("[error] UnknownFieldException was thrown!\n");
}

void zen_VirtualMachine_raiseNullReferenceException(zen_VirtualMachine_t* virtualMachine) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");

    printf("[error] NullReferenceException was thrown!\n");
}

void zen_VirtualMachine_raiseClassNotFoundException(zen_VirtualMachine_t* virtualMachine,
    const uint8_t* reason) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    jtk_Assert_assertObject(reason, "The specified class descriptor is null.");

    /* Find the `ClassNotFoundException` class. */
    // const uint8_t* descriptor = "zen.core.ClassNotFoundException";

    // zen_Class_t* handle = NULL;

    // zen_EntityLoader_findClass(entityLoader, descriptor);
    // zen_EntityLoader_findEntity(virtualMachine->m_entityLoader, descriptor);

    /* The message of the exception. */
    // zen_Object_t* message = zen_VirtualMachine_newString(virtualMachine, reason);

    /* The descriptor of the constructor which will initializer the exception
     * object.
     */
    // const uint8_t* constructor = "(Czen.core.String;)";

    /* Create an instance of the `zen.core.ClassNotFoundException` class. */
    // zen_Object_t* exception = zen_VirtualMachine_newObject(virtualMachine, descriptor, constructor, message);

    /* Throw the exception. */
    // zen_VirtualMachine_raiseException(virtualMachine, exception);
}

void zen_VirtualMachine_raiseFunctionNotFoundException(zen_VirtualMachine_t* virtualMachine,
    const uint8_t* identifier, const uint8_t* reason) {
    // jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    // jtk_Assert_assertObject(reason, "The specified class descriptor is null.");

    /* Find the `FunctionNotFoundException` class. */
    // const uint8_t* descriptor = "zen.core.FunctionNotFoundException";
    // zen_Class_t* handle = NULL;

    // zen_EntityLoader_findClass(entityLoader, descriptor);
    // zen_EntityLoader_findEntity(virtualMachine->m_entityLoader, descriptor);

    /* The message of the exception. */
    // zen_Object_t* message = zen_VirtualMachine_newString(virtualMachine, reason);

    /* The descriptor of the constructor which will initializer the exception
     * object.
     */
    // const uint8_t* constructor = "(Czen.core.String;)";

    /* Create an instance of the `zen.core.ClassNotFoundException` class. */
    // zen_Object_t* exception = zen_VirtualMachine_newObject(virtualMachine, descriptor, constructor, message);

    /* Throw the exception. */
    // zen_VirtualMachine_raiseException(virtualMachine, exception);
}

// Static Function

zen_Function_t* zen_VirtualMachine_getStaticFunction(zen_VirtualMachine_t* virtualMachine,
    zen_Class_t* handle, jtk_String_t* identifier, jtk_String_t* signature) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    jtk_Assert_assertObject(handle, "The specified class is null.");
    jtk_Assert_assertObject(identifier, "The specified identifier is null.");
    jtk_Assert_assertObject(signature, "The specified signature is null.");

    zen_Function_t* function = zen_Class_getStaticFunction(handle, identifier, signature);
    if (function == NULL) {
        zen_VirtualMachine_raiseFunctionNotFoundException(virtualMachine, identifier, signature);
    }
    return function;
}

// Start

void zen_VirtualMachine_start(zen_VirtualMachine_t* virtualMachine,
    zen_Function_t* function, ...) {
    jtk_VariableArguments_t variableArguments;
    jtk_VariableArguments_start(variableArguments, function);

    zen_Interpreter_invokeStaticFunctionEx(virtualMachine->m_interpreter,
        function, variableArguments);

    jtk_VariableArguments_end(variableArguments);
}

// String

zen_Object_t* zen_VirtualMachine_getEmptyString(zen_VirtualMachine_t* virtualMachine) {
    return zen_VirtualMachine_newStringFromUtf8(virtualMachine, NULL, 0);
}

zen_Object_t* zen_VirtualMachine_newStringFromUtf8(zen_VirtualMachine_t* virtualMachine,
    const uint8_t* string, int32_t size) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    /*
    size = (size < 0)? jtk_Utf8String_getSize(string) : size;
    const uint8_t* descriptor

    return result;
    */
    return NULL;
}

zen_Object_t* zen_VirtualMachine_newString(zen_VirtualMachine_t* virtualMachine,
    const uint8_t* string) {
    return NULL;
}

// Shutdown

void zen_VirtualMachine_shutDown(zen_VirtualMachine_t* virtualMachine) {
    /* In C, the process is terminated if the main thread exits. Therefore, we
     * need to wait for the threads spawned by the virtual machine to complete.
     */
    zen_VirtualMachine_waitForThreads(virtualMachine);

    /* Notify all the registered listeners of the "shut down" event. */
    zen_VirtualMachine_notifyShutDown(virtualMachine);

    /* The listeners may have spawned threads. The virtual machine has to wait
     * for their completion.
     */
    zen_VirtualMachine_waitForThreads(virtualMachine);

    /* Tear down the virtual machine. */
    zen_VirtualMachine_tearDown(virtualMachine);
}

void zen_VirtualMachine_notifyShutDown(zen_VirtualMachine_t* virtualMachine) {
}

void zen_VirtualMachine_tearDown(zen_VirtualMachine_t* virtualMachine) {
}

// Thread

void zen_VirtualMachine_waitForThreads(zen_VirtualMachine_t* virtualMachine) {
}