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

// Thursday, March 08, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_ENUMERATION_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_ENUMERATION_SYMBOL_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/compiler/symbol-table/Symbol.h>
#include <com/onecube/zen/compiler/symbol-table/Modifier.h>
#include <com/onecube/zen/compiler/symbol-table/ClassSymbol.h>

/*******************************************************************************
 * EnumerationSymbol                                                           *
 *******************************************************************************/

/**
 * @class EnumerationSymbol
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EnumerationSymbol_t {
    zen_Symbol_t* m_symbol;

    /* The superclass attribute is updated during the resolution phase. */
    zen_ClassSymbol_t* m_superclass;

    jtk_ArrayList_t* m_explicitModifiers;

    int32_t m_modifiers;

    zen_Scope_t* m_enumerationScope;
};

/**
 * @memberof EnumerationSymbol
 */
typedef struct zen_EnumerationSymbol_t zen_EnumerationSymbol_t;

// Constructor

/**
 * @memberof EnumerationSymbol
 */
zen_EnumerationSymbol_t* zen_EnumerationSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope, zen_Scope_t* m_enumerationScope);

// Destructor

/**
 * @memberof EnumerationSymbol
 */
void zen_EnumerationSymbol_delete(zen_EnumerationSymbol_t* symbol);

// Modifier

/**
 * @memberof EnumerationSymbol
 */
void zen_EnumerationSymbol_addModifier(zen_EnumerationSymbol_t* symbol,
    zen_Modifier_t modifier, zen_ASTNode_t* node);

/**
 * @memberof EnumerationSymbol
 */
bool zen_EnumerationSymbol_hasModifier(zen_EnumerationSymbol_t* symbol,
    zen_Modifier_t modifier);

// Scope

/**
 * @memberof EnumerationSymbol
 */
zen_Scope_t* zen_EnumerationSymbol_getEnumerationScope(zen_EnumerationSymbol_t* symbol);

// Superclass

/**
 * @memberof EnumerationSymbol
 */
zen_ClassSymbol_t* zen_EnumerationSymbol_getSuperclass(zen_EnumerationSymbol_t* symbol);

// Symbol

/**
 * @memberof EnumerationSymbol
 */
zen_Symbol_t* zen_EnumerationSymbol_getSymbol(zen_EnumerationSymbol_t* symbol);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_ENUMERATION_SYMBOL_H */