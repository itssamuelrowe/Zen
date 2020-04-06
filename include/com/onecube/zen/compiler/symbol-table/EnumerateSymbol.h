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

// Friday, March 09, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_ENUMERATE_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_ENUMERATE_SYMBOL_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/symbol-table/Symbol.h>
#include <com/onecube/zen/compiler/symbol-table/Modifier.h>

/*******************************************************************************
 * EnumerateSymbol                                                             *
 *******************************************************************************/

/**
 * @class EnumerateSymbol
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EnumerateSymbol_t {
    zen_Symbol_t* m_symbol;
};

/**
 * @memberof EnumerateSymbol
 */
typedef struct zen_EnumerateSymbol_t zen_EnumerateSymbol_t;

// Constructor

/**
 * @memberof EnumerateSymbol
 */
zen_EnumerateSymbol_t* zen_EnumerateSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope);

// Destructor

/**
 * @memberof EnumerateSymbol
 */
void zen_EnumerateSymbol_delete(zen_EnumerateSymbol_t* symbol);

// Symbol

/**
 * @memberof EnumerateSymbol
 */
zen_Symbol_t* zen_EnumerateSymbol_getSymbol(zen_EnumerateSymbol_t* symbol);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_ENUMERATE_SYMBOL_H */