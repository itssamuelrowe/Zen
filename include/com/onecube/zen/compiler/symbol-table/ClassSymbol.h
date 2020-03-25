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

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_CLASS_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_CLASS_SYMBOL_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/compiler/symbol-table/Symbol.h>
#include <com/onecube/zen/compiler/symbol-table/Modifier.h>

// Forward reference
typedef struct zen_ClassScope_t zen_ClassScope_t;

/*******************************************************************************
 * ClassSymbol                                                                 *
 *******************************************************************************/

/**
 * @class ClassSymbol
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ClassSymbol_t {
    zen_Symbol_t* m_symbol;

    /* The superclasses are added in the resolution phase. */
    jtk_ArrayList_t* m_superClasses; /* <zen_ClassSymbol_t*> */

    zen_Scope_t* m_classScope;

    uint8_t* m_qualifiedName;
    int32_t m_qualifiedNameSize;
};

/**
 * @memberof ClassSymbol
 */
typedef struct zen_ClassSymbol_t zen_ClassSymbol_t;

// Constructor

/**
 * @memberof ClassSymbol
 */
zen_ClassSymbol_t* zen_ClassSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope, zen_Scope_t* m_classScope,
    uint8_t* qualifiedName, int32_t qualifiedNameSize);

// Destructor

/**
 * @memberof ClassSymbol
 */
void zen_ClassSymbol_delete(zen_ClassSymbol_t* symbol);

// Class Scope

/**
 * @memberof ClassSymbol
 */
zen_Scope_t* zen_ClassSymbol_getClassScope(zen_ClassSymbol_t* symbol);

// Modifier

/**
 * @memberof ClassSymbol
 */
void zen_ClassSymbol_addModifier(zen_ClassSymbol_t* symbol, zen_Modifier_t modifier,
    zen_ASTNode_t* node);

/**
 * @memberof ClassSymbol
 */
bool zen_ClassSymbol_hasModifier(zen_ClassSymbol_t* symbol, zen_Modifier_t modifier);

// Superclasses

/**
 * @memberof ClassSymbol
 */
jtk_ArrayList_t* zen_ClassSymbol_getSuperClasses(zen_ClassSymbol_t* symbol);

// Symbol

/**
 * @memberof ClassSymbol
 */
zen_Symbol_t* zen_ClassSymbol_getSymbol(zen_ClassSymbol_t* symbol);

// Qualified Name

/**
 * @memberof ClassSymbol
 */
uint8_t* zen_ClassSymbol_getQualifiedName(zen_ClassSymbol_t* symbol);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_CLASS_SYMBOL_H */