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

// Tuesday, February 27, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_LOCAL_SCOPE_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_LOCAL_SCOPE_H

#include <jtk/collection/map/HashMap.h>
#include <com/onecube/zen/compiler/symbol-table/Scope.h>

/*******************************************************************************
 * LocalScope                                                                  *
 *******************************************************************************/

/**
 * @class LocalScope
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_LocalScope_t {
    zen_Scope_t* m_scope;
    zen_HashMap_t* m_symbols;
};

/**
 * @memberof LocalScope
 */
typedef struct zen_LocalScope_t zen_LocalScope_t;

// Constructor

/**
 * @memberof LocalScope
 */
zen_LocalScope_t* zen_LocalScope_new(zen_Scope_t* enclosingScope);

// Destructor

/**
 * @memberof LocalScope
 */
void zen_LocalScope_delete(zen_LocalScope_t* scope);

// Define

/**
 * @memberof LocalScope
 */
void zen_LocalScope_define(zen_LocalScope_t* scope, zen_Symbol_t* symbol);

// Resolve

/**
 * @memberof LocalScope
 */
zen_Symbol_t* zen_LocalScope_resolve(zen_LocalScope_t* scope, const uint8_t* identifier);

// Scope

/**
 * @memberof LocalScope
 */
zen_Scope_t* zen_LocalScope_getScope(zen_LocalScope_t* scope);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_LOCAL_SCOPE_H */