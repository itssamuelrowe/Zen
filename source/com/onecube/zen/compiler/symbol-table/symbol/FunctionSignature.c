/*
 * Copyright 2017-2020 Samuel Rowe
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

// Saturday, February 24, 2018

#include <jtk/core/StringBuilder.h>
#include <com/onecube/zen/compiler/symbol-table/FunctionSignature.h>
#include <jtk/core/CString.h>

// TODO: Constructors require v for their return type!
zen_FunctionSignature_t* zen_FunctionSignature_new(jtk_ArrayList_t* fixedParameters,
    zen_ASTNode_t* variableParameter, uint16_t modifiers) {
    int32_t parameterCount = jtk_ArrayList_getSize(fixedParameters);

    zen_FunctionSignature_t* signature = zen_Memory_allocate(zen_FunctionSignature_t, 1);
    signature->m_fixedParameters = fixedParameters;
    signature->m_variableParameter = variableParameter;
    signature->m_modifiers = modifiers;
    signature->m_tableIndex = -1;
    signature->m_fixedParameterCount = parameterCount;
    signature->m_variableParameter = false; // TODO

    jtk_StringBuilder_t* builder = jtk_StringBuilder_new();
    jtk_StringBuilder_appendEx_z(builder, "(zen/core/Object):", 18);
    if (parameterCount > 0) {
        jtk_StringBuilder_multiply_z(builder, "(zen/core/Object)", 17, parameterCount);
    }
    else {
        jtk_StringBuilder_appendEx_z(builder, "v", 1);
    }
    if (variableParameter != NULL) {
        jtk_StringBuilder_appendEx_z(builder, "@(zen/core/Object)", 18);
    }
    signature->m_descriptor = jtk_StringBuilder_toCString(builder,
        &signature->m_descriptorSize);
    jtk_StringBuilder_delete(builder);

    return signature;
}

int32_t zen_FunctionSignature_countParameters(const uint8_t* descriptor,
    int32_t size) {
    int32_t afterColon = -1;
    int32_t parameterCount = 0;
    int32_t i;
    /* The following function descriptor parser is a naive implementation.
     * It assumes that the given function descriptor is not malformed.
     * The purpose of this parser is to simply extract relevant information
     * as quickly as possible.
     */
    for (i = 0; i < size; i++) {
        switch (descriptor[i]) {
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
                while (descriptor[++i] != ')');
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
                printf("[error] Malformed function descriptor '%s'\n",
                    descriptor);
            }
        }
    }

    int32_t lastKnownVoid = -1;
    int32_t parameterIndex;
    for (i = 0, parameterIndex = -1;
        (i < size) && (parameterIndex < parameterCount);
        i++, parameterIndex++) {
        int32_t startIndex = i;
        switch (descriptor[i]) {
            case '@': {
                /* Arrays are also considered as reference types. */
                while (descriptor[++i] == '@');
                if (descriptor[i] == '(') {
                    while (descriptor[++i] != ')');
                }
                else {
                    /* Skip the primitive type. */
                    i++;
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
            case 'd': {
                break;
            }

            case 'v': {
                lastKnownVoid = i;
                break;
            }

            case '(': {
                while (descriptor[++i] != ')');
                break;
            }

            default: {
            }
        }
        if (parameterIndex < 0) {
            /* Skip the colon after the return type. */
            i++;
        }
    }

    if (lastKnownVoid >= afterColon) {
        if (parameterCount == 1) {
            parameterCount = 0;
        }
        else {
            printf("[error] Malformed function descriptor '%s'.\n",
                descriptor);
        }
    }

    return parameterCount;
}

zen_FunctionSignature_t* zen_FunctionSignature_newEx(const uint8_t* descriptor,
    int32_t descriptorSize, uint16_t modifiers, uint16_t tableIndex) {
    zen_FunctionSignature_t* signature = zen_Memory_allocate(zen_FunctionSignature_t, 1);
    signature->m_fixedParameters = NULL;
    signature->m_variableParameter = NULL;
    signature->m_descriptor = jtk_CString_make(descriptor, &descriptorSize);
    signature->m_descriptorSize = descriptorSize;
    signature->m_tableIndex = tableIndex;
    signature->m_modifiers = modifiers;
    signature->m_variableParameter = false; // TODO
    signature->m_fixedParameterCount = zen_FunctionSignature_countParameters(descriptor, descriptorSize);

    return signature;
}

void zen_FunctionSignature_delete(zen_FunctionSignature_t* signature) {
    jtk_Assert_assertObject(signature, "The specified signature is null.");

    jtk_CString_delete(signature->m_descriptor);
    jtk_Memory_deallocate(signature);
}