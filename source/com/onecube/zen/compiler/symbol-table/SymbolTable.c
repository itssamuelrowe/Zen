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

// Sunday, January 07, 2018

#include <jtk/collection/stack/LinkedStack.h>

#include <com/onecube/zen/compiler/Compiler.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolTable.h>

zen_SymbolTable_t* zen_SymbolTable_new(zen_Compiler_t* compiler) {
    zen_SymbolTable_t* symbolTable = zen_Memory_allocate(zen_SymbolTable_t, 1);
    symbolTable->m_currentScope = NULL;
    symbolTable->m_compiler = compiler;

    return symbolTable;
}

void zen_SymbolTable_delete(zen_SymbolTable_t* symbolTable) {
    jtk_Assert_assertObject(symbolTable, "The specified symbol table is null.");

    jtk_Memory_deallocate(symbolTable);
}

void zen_SymbolTable_setCurrentScope(zen_SymbolTable_t* symbolTable, zen_Scope_t* currentScope) {
    jtk_Assert_assertObject(symbolTable, "The specified symbol table is null.");

    jtk_Logger_t* logger = symbolTable->m_compiler->m_logger;
    jtk_Logger_debug(logger, "<enter> %s", zen_Scope_getName(currentScope));
    symbolTable->m_currentScope = currentScope;
}

zen_Scope_t* zen_SymbolTable_getCurrentScope(zen_SymbolTable_t* symbolTable) {
    jtk_Assert_assertObject(symbolTable, "The specified symbol table is null.");
    return symbolTable->m_currentScope;
}

void zen_SymbolTable_invalidateCurrentScope(zen_SymbolTable_t* symbolTable) {
    jtk_Assert_assertObject(symbolTable, "The specified symbol table is null.");

    jtk_Logger_t* logger = symbolTable->m_compiler->m_logger;
    jtk_Logger_debug(logger, "<exit> %s", zen_Scope_getName(symbolTable->m_currentScope));
    symbolTable->m_currentScope = zen_Scope_getEnclosingScope(symbolTable->m_currentScope);
}

void zen_SymbolTable_define(zen_SymbolTable_t* symbolTable, zen_Symbol_t* symbol) {
    jtk_Assert_assertObject(symbolTable, "The specified symbol table is null.");
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    zen_Scope_define(symbolTable->m_currentScope, symbol);
}

/* Resolve */

zen_Symbol_t* zen_SymbolTable_resolve(zen_SymbolTable_t* symbolTable, const uint8_t* identifier) {
    jtk_Assert_assertObject(symbolTable, "The specified symbol table is null.");
    jtk_Assert_assertObject(identifier, "The specified identifier is null.");

    zen_Scope_t* scope = symbolTable->m_currentScope;
    zen_Symbol_t* symbol = NULL;
    while (scope != NULL) {
        symbol = zen_Scope_resolve(scope, identifier);
        if (symbol != NULL) {
            break;
        }
        else {
            scope = zen_Scope_getEnclosingScope(scope);
        }
    }

    if ((symbol != NULL) && zen_Symbol_isExternal(symbol)) {
        symbol = symbol->m_context.m_asExternal;
    }

    return symbol;
}