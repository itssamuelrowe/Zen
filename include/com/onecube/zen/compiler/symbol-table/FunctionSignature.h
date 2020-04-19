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

// Monday, January 08, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_FUNCTION_SIGNATURE_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_FUNCTION_SIGNATURE_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * FunctionSignature                                                           *
 *******************************************************************************/

/**
 * @class FunctionSignature
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_FunctionSignature_t {
    jtk_ArrayList_t* m_fixedParameters;
    zen_ASTNode_t* m_variableParameter;
    uint8_t* m_descriptor;
    int32_t m_descriptorSize;
    uint16_t m_modifiers;
    uint16_t m_tableIndex;
    int32_t m_fixedParameterCount;
    bool m_hasVariableParameter;
};

/**
 * @memberof FunctionSignature
 */
typedef struct zen_FunctionSignature_t zen_FunctionSignature_t;

// Constructor

/**
 * @memberof FunctionSignature
 */
zen_FunctionSignature_t* zen_FunctionSignature_new(jtk_ArrayList_t* fixedParameters,
    zen_ASTNode_t* variableParameter, uint16_t modifiers);

zen_FunctionSignature_t* zen_FunctionSignature_newEx(const uint8_t* descriptor,
    int32_t descriptorSize, uint16_t modifiers, uint16_t tableIndex);

// Destructor

/**
 * @memberof FunctionSignature
 */
void zen_FunctionSignature_delete(zen_FunctionSignature_t* signature);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_FUNCTION_SIGNATURE_H */