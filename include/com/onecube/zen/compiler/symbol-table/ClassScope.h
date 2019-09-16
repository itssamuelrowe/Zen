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

// Monday, January 08, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_CLASS_SCOPE_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_CLASS_SCOPE_H

#include <jtk/collection/map/HashMap.h>
#include <com/onecube/zen/compiler/symbol-table/Scope.h>

// Forward reference
typedef struct zen_ClassSymbol_t zen_ClassSymbol_t;

/*******************************************************************************
 * ClassScope                                                                  *
 *******************************************************************************/

/**
 * @class ClassScope
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ClassScope_t {
    zen_Scope_t* m_scope;
    jtk_HashMap_t* m_symbols;
    zen_Symbol_t* m_classSymbol;
};

/**
 * @memberof ClassScope
 */
typedef struct zen_ClassScope_t zen_ClassScope_t;

// Constructor

/**
 * @memberof ClassScope
 */
zen_ClassScope_t* zen_ClassScope_new(zen_Scope_t* enclosingScope);

// Destructor

/**
 * @memberof ClassScope
 */
void zen_ClassScope_delete(zen_ClassScope_t* classScope);

// Class Symbol

/**
 * @memberof ClassScope
 */
zen_Symbol_t* zen_ClassScope_getClassSymbol(zen_ClassScope_t* classScope);

// Define

/**
 * @memberof ClassScope
 */
void zen_ClassScope_define(zen_ClassScope_t* scope, zen_Symbol_t* symbol);

// Resolve

/**
 * @memberof ClassScope
 */
zen_Symbol_t* zen_ClassScope_resolve(zen_ClassScope_t* scope, const uint8_t* identifier);

// Scope

/**
 * @memberof ClassScope
 */
zen_Scope_t* zen_ClassScope_getScope(zen_ClassScope_t* scope);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_CLASS_SCOPE_H */