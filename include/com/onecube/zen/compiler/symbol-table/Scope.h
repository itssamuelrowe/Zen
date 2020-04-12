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

// Saturday, November 25, 2017

#ifndef ZEN_SCOPE_H
#define ZEN_SCOPE_H

#include <jtk/collection/list/ArrayList.h>
#include <jtk/collection/map/HashMap.h>

#include <com/onecube/zen/compiler/symbol-table/ScopeType.h>

typedef struct zen_Symbol_t zen_Symbol_t;

/*******************************************************************************
 * Scope                                                                       *
 *******************************************************************************/

/**
 * @memberof Scope
 */
typedef struct zen_Scope_t zen_Scope_t;

/**
 * @class Scope
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_Scope_t {
    uint8_t* m_name;
    int32_t m_nameSize;
    zen_ScopeType_t m_type;
    zen_Scope_t* m_enclosingScope;
    jtk_HashMap_t* m_symbols;
    int32_t m_nextTicket;
    zen_Symbol_t* m_symbol;
};

// Constructor

/**
 * @memberof Scope
 */
zen_Scope_t* zen_Scope_new(const uint8_t* name, int32_t nameSize,
    zen_ScopeType_t type, zen_Scope_t* enclosingScope, zen_Symbol_t* symbol);

/**
 * @memberof Scope
 */
zen_Scope_t* zen_Scope_forCompilationUnit();

/**
 * @memberof Scope
 */
zen_Scope_t* zen_Scope_forFunction(zen_Scope_t* enclosingScope);

/**
 * @memberof Scope
 */
zen_Scope_t* zen_Scope_forLocal(zen_Scope_t* enclosingScope);

/**
 * @memberof Scope
 */
zen_Scope_t* zen_Scope_forClass(zen_Scope_t* enclosingScope);

// Destructor

/**
 * @memberof Scope
 */
void zen_Scope_delete(zen_Scope_t* scope);

// Children Symbols

void zen_Scope_getChildrenSymbols(zen_Scope_t* scope, jtk_ArrayList_t* childrenSymbols);

// Context

void* zen_Scope_getContext(zen_Scope_t* scope);

// Scope Type

/**
 * @memberof Scope
 */
bool zen_Scope_isEnumerationScope(zen_Scope_t* scope);

/**
 * @memberof Scope
 */
bool zen_Scope_isClassScope(zen_Scope_t* scope);

/**
 * @memberof Scope
 */
bool zen_Scope_isFunctionScope(zen_Scope_t* scope);

/**
 * @memberof Scope
 */
bool zen_Scope_isCompilationUnitScope(zen_Scope_t* scope);

/**
 * @memberof Scope
 */
bool zen_Scope_isLocalScope(zen_Scope_t* scope);

// Define

/**
 * @memberof Scope
 */
void zen_Scope_define(zen_Scope_t* scope, zen_Symbol_t* symbol);

/**
 * @memberof Scope
 */
void zen_Scope_defineEx(zen_Scope_t* scope, const uint8_t* descriptor,
    int32_t descriptorSize, zen_Symbol_t* symbol);

// Enclosing Scope

/**
 * @memberof Scope
 */
zen_Scope_t* zen_Scope_getEnclosingScope(zen_Scope_t* scope);

// Name

/**
 * @memberof Scope
 */
const uint8_t* zen_Scope_getName(zen_Scope_t* scope);

// Resolve

/**
 * @memberof Scope
 */
zen_Symbol_t* zen_Scope_resolve(zen_Scope_t* scope, uint8_t* identifier);

/**
 * @memberof Scope
 */
zen_Scope_t* zen_Scope_resolveQualifiedSymbol(zen_Scope_t* scope,
    const uint8_t* name, int32_t nameSize);

// Type

/**
 * @memberof Scope
 */
zen_ScopeType_t zen_Scope_getType(zen_Scope_t* scope);

#endif /* ZEN_SCOPE_H */