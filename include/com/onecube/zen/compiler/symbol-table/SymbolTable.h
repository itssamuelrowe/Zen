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

// Satuday, November 25, 2017

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_TABLE_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_TABLE_H

#include <com/onecube/zen/compiler/symbol-table/Scope.h>
#include <com/onecube/zen/compiler/symbol-table/Symbol.h>

struct zen_Compiler_t;
typedef struct zen_Compiler_t zen_Compiler_t;

/*******************************************************************************
 * SymbolTable                                                                 *
 *******************************************************************************/

/**
 * @class SymbolTable
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_SymbolTable_t {
    zen_Scope_t* m_currentScope;
    zen_Compiler_t* m_compiler;
};

/**
 * @memberof SymbolTable
 */
typedef struct zen_SymbolTable_t zen_SymbolTable_t;

// Constructor

/**
 * @memberof SymbolTable
 */
zen_SymbolTable_t* zen_SymbolTable_new(zen_Compiler_t* compiler);

// Destructor

/**
 * @memberof SymbolTable
 */
void zen_SymbolTable_delete(zen_SymbolTable_t* symbolTable);

// Current Scope

/**
 * @memberof SymbolTable
 */
void zen_SymbolTable_setCurrentScope(zen_SymbolTable_t* symbolTable, zen_Scope_t* currentScope);

/**
 * @memberof SymbolTable
 */
zen_Scope_t* zen_SymbolTable_getCurrentScope(zen_SymbolTable_t* symbolTable);

/**
 * @memberof SymbolTable
 */
void zen_SymbolTable_invalidateCurrentScope(zen_SymbolTable_t* symbolTable);

// Define

/**
 * @memberof SymbolTable
 */
void zen_SymbolTable_define(zen_SymbolTable_t* symbolTable, zen_Symbol_t* symbol);

/**
 * @memberof SymbolTable
 */
void zen_SymbolTable_defineClassMember(zen_SymbolTable_t* symbolTable, zen_Scope_t* classScope, zen_Symbol_t* symbol);

// Resolve

/**
 * @memberof SymbolTable
 */
zen_Symbol_t* zen_SymbolTable_resolve(zen_SymbolTable_t* symbolTable, const uint8_t* identifier);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_TABLE_H */