/*
 * Copyright 2018-2019 OneCube
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

// Sunday, January 07, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_FUNCTION_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_FUNCTION_SYMBOL_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/compiler/symbol-table/Symbol.h>
#include <com/onecube/zen/compiler/symbol-table/FunctionSignature.h>

/*******************************************************************************
 * FunctionSymbol                                                              *
 *******************************************************************************/

/**
 * @class FunctionSymbol
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_FunctionSymbol_t {
    zen_Symbol_t* m_symbol;
    /* Signatures are destroyed by the destructor. */
    jtk_ArrayList_t* m_signatures; /* <zen_FunctionSignature_t*> */
    int32_t m_parameterThreshold;
};

/**
 * @memberof FunctionSymbol
 */
typedef struct zen_FunctionSymbol_t zen_FunctionSymbol_t;

// Constructor

/**
 * @memberof FunctionSymbol
 */
zen_FunctionSymbol_t* zen_FunctionSymbol_new(zen_ASTNode_t* identifier, zen_Scope_t* enclosingScope);

// Destructor

/**
 * @memberof FunctionSymbol
 */
void zen_FunctionSymbol_delete(zen_FunctionSymbol_t* symbol);

// Signature

/**
 * @memberof FunctionSymbol
 */
jtk_ArrayList_t* zen_FunctionSymbol_getSignatures(zen_FunctionSymbol_t* symbol);

/**
 * @memberof FunctionSymbol
 */
void zen_FunctionSymbol_addSignature(zen_FunctionSymbol_t* symbol, zen_FunctionSignature_t* signature);

// Parameter Threshold

/**
 * @memberof FunctionSymbol
 */
int32_t zen_FunctionSymbol_getParameterThreshold(zen_FunctionSymbol_t* symbol);

/**
 * @memberof FunctionSymbol
 */
void zen_FunctionSymbol_setParameterThreshold(zen_FunctionSymbol_t* symbol, int32_t parameterThreshold);

// Symbol

/**
 * @memberof FunctionSymbol
 */
zen_Symbol_t* zen_FunctionSymbol_getSymbol(zen_FunctionSymbol_t* symbol);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_FUNCTION_SYMBOL_H */