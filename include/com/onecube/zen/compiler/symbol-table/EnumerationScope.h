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

// Friday, March 09, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_ENUMERATION_SCOPE_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_ENUMERATION_SCOPE_H

#include <jtk/collection/map/HashMap.h>
#include <com/onecube/zen/compiler/symbol-table/Scope.h>

/*******************************************************************************
 * EnumerationScope                                                            *
 *******************************************************************************/

/**
 * @class EnumerationScope
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EnumerationScope_t {

    zen_Scope_t* m_scope;

    /**
TODO: Use OrderedMap here.
     */
    jtk_HashMap_t* m_enumerates;

    zen_Symbol_t* m_enumerationSymbol;
};

/**
 * @memberof EnumerationScope
 */
typedef struct zen_EnumerationScope_t zen_EnumerationScope_t;

// Constructor

/**
 * @memberof EnumerationScope
 */
zen_EnumerationScope_t* zen_EnumerationScope_new(zen_Scope_t* enclosingScope);

// Destructor

/**
 * @memberof EnumerationScope
 */
void zen_EnumerationScope_delete(zen_EnumerationScope_t* classScope);

// Children Symbols

/**
 * @memberof EnumerationScope
 */
void zen_EnumerationScope_getChildrenSymbols(zen_EnumerationScope_t* scope, jtk_ArrayList_t* childrenSymbols);

// Define

/**
 * @memberof EnumerationScope
 */
void zen_EnumerationScope_define(zen_EnumerationScope_t* scope, zen_Symbol_t* symbol);

// Enumeration Symbol

/**
 * @memberof EnumerationScope
 */
zen_Symbol_t* zen_EnumerationScope_getEnumerationSymbol(zen_EnumerationScope_t* classScope);

// Resolve

/**
 * @memberof EnumerationScope
 */
zen_Symbol_t* zen_EnumerationScope_resolve(zen_EnumerationScope_t* scope, const uint8_t* identifier);

// Scope

/**
 * @memberof EnumerationScope
 */
zen_Scope_t* zen_EnumerationScope_getScope(zen_EnumerationScope_t* scope);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_ENUMERATION_SCOPE_H */