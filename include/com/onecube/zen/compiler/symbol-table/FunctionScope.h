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

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_FUNCTION_SCOPE_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_FUNCTION_SCOPE_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/symbol-table/ConstantSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/Scope.h>

/*******************************************************************************
 * FunctionScope                                                               *
 *******************************************************************************/

/**
 * @class FunctionScope
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_FunctionScope_t {
    zen_Scope_t* m_scope;

    /* TODO: Use jtk_RBTreeMap_t here for improving performance. */
    jtk_ArrayList_t* m_fixedParameters; /* <zen_Symbol*> */

    zen_Symbol_t* m_variableParameter;
};

/**
 * @memberof FunctionScope
 */
typedef struct zen_FunctionScope_t zen_FunctionScope_t;

// Constructor

/**
 * @memberof FunctionScope
 */
zen_FunctionScope_t* zen_FunctionScope_new(zen_Scope_t* enclosingScope);

// Destructor

/**
 * @memberof FunctionScope
 */
void zen_FunctionScope_delete(zen_FunctionScope_t* scope);

// Children Symbols

/**
 * @memberof FunctionScope
 */
void zen_FunctionScope_getChildrenSymbols(zen_FunctionScope_t* scope, jtk_ArrayList_t* childrenSymbols);

// Define

/**
 * @memberof FunctionScope
 */
void zen_FunctionScope_define(zen_FunctionScope_t* scope, zen_Symbol_t* symbol);

// Resolve

/**
 * @memberof FunctionScope
 */
zen_Symbol_t* zen_FunctionScope_resolve(zen_FunctionScope_t* scope, const uint8_t* identifier);

// Scope

/**
 * @memberof FunctionScope
 */
zen_Scope_t* zen_FunctionScope_getScope(zen_FunctionScope_t* scope);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_FUNCTION_SCOPE_H */