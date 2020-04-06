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

#include <com/onecube/zen/compiler/symbol-table/EnumerateSymbol.h>

/*******************************************************************************
 * EnumerateSymbol                                                             *
 *******************************************************************************/

#warning "TODO: Add ordinal to this symbol."

zen_EnumerateSymbol_t* zen_EnumerateSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope) {
    zen_EnumerateSymbol_t* enumerateSymbol = zen_Memory_allocate(zen_EnumerateSymbol_t, 1);

    zen_Symbol_t* symbol = zen_Symbol_new(ZEN_SYMBOL_CATEGORY_ENUMERATE, identifier, enclosingScope, enumerateSymbol);

    enumerateSymbol->m_symbol = symbol;

    return enumerateSymbol;
}

void zen_EnumerateSymbol_delete(zen_EnumerateSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    zen_Symbol_delete(symbol->m_symbol);
    jtk_Memory_deallocate(symbol);
}

zen_Symbol_t* zen_EnumerateSymbol_getSymbol(zen_EnumerateSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    return symbol->m_symbol;
}