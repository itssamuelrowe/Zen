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

// Saturday, November 25, 2017

#ifndef ZEN_SCOPE_H
#define ZEN_SCOPE_H

#include <jtk/core/String.h>
#include <jtk/collection/map/HashMap.h>

#include <com/onecube/zen/compiler/symbol-table/ScopeType.h>

// Forward references

typedef struct zen_Symbol_t zen_Symbol_t;

typedef zen_Symbol_t* (*zen_Scope_ResolveSymbolFunction_t)(void* context, const uint8_t* identifier);
typedef zen_Symbol_t* (*zen_Scope_DefineSymbolFunction_t)(void* context, zen_Symbol_t* symbol);

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
    jtk_String_t* m_name;
    zen_ScopeType_t m_type;
    zen_Scope_t* m_enclosingScope;
    void* m_context;
    zen_Scope_ResolveSymbolFunction_t m_resolveSymbol;
    zen_Scope_DefineSymbolFunction_t m_defineSymbol;
};

// Constructor

/**
 * @memberof Scope
 */
zen_Scope_t* zen_Scope_new(const uint8_t* name, zen_ScopeType_t type,
    zen_Scope_t* enclosingScope, void* context);

// Destructor

/**
 * @memberof Scope
 */
void zen_Scope_delete(zen_Scope_t* scope);

// Class Scope

/**
 * @memberof Scope
 */
bool zen_Scope_isClassScope(zen_Scope_t* scope);

// Compilation Unit

/**
 * @memberof Scope
 */
bool zen_Scope_isCompilationUnitScope(zen_Scope_t* scope);

// Define

/**
 * @memberof Scope
 */
void zen_Scope_define(zen_Scope_t* scope, zen_Symbol_t* symbol);

// Enclosing Scope

/**
 * @memberof Scope
 */
zen_Scope_t* zen_Scope_getEnclosingScope(zen_Scope_t* scope);

// Local Scope

/**
 * @memberof Scope
 */
bool zen_Scope_isLocalScope(zen_Scope_t* scope);

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

// Type

/**
 * @memberof Scope
 */
zen_ScopeType_t zen_Scope_getType(zen_Scope_t* scope);

#endif /* ZEN_SCOPE_H */