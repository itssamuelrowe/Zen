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

zen_FunctionSignature_t* zen_FunctionSignature_new(jtk_ArrayList_t* fixedParameters,
    zen_ASTNode_t* variableParameter, uint16_t modifiers) {
    zen_FunctionSignature_t* signature = zen_Memory_allocate(zen_FunctionSignature_t, 1);
    signature->m_fixedParameters = fixedParameters;
    signature->m_variableParameter = variableParameter;
    signature->m_modifiers = modifiers;
    signature->m_tableIndex = -1;

    jtk_StringBuilder_t* builder = jtk_StringBuilder_new();
    jtk_StringBuilder_appendEx_z(builder, "(zen/core/Object):", 18);
    int32_t argumentCount = jtk_ArrayList_getSize(fixedParameters);
    jtk_StringBuilder_multiply_z(builder, "(zen/core/Object)", 17, argumentCount);
    if (variableParameter != NULL) {
        jtk_StringBuilder_appendEx_z(builder, "@(zen/core/Object)", 18);
    }
    signature->m_descriptor = jtk_StringBuilder_toCString(builder,
        &signature->m_descriptorSize);
    jtk_StringBuilder_delete(builder);

    return signature;
}

zen_FunctionSignature_t* zen_FunctionSignature_newEx(const uint8_t* descriptor,
    int32_t descriptorSize, uint16_t modifiers) {
    zen_FunctionSignature_t* signature = zen_Memory_allocate(zen_FunctionSignature_t, 1);
    signature->m_fixedParameters = NULL;
    signature->m_variableParameter = NULL;
    signature->m_descriptor = jtk_CString_make(descriptor, &descriptorSize);
    signature->m_descriptorSize = descriptorSize;
    signature->m_tableIndex = -1;
    signature->m_modifiers = modifiers;

    return signature;
}

void zen_FunctionSignature_delete(zen_FunctionSignature_t* signature) {
    jtk_Assert_assertObject(signature, "The specified signature is null.");

    jtk_CString_delete(signature->m_descriptor);
    jtk_Memory_deallocate(signature);
}