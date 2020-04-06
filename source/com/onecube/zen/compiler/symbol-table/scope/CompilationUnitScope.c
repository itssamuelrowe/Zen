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

// Sunday, February 25, 2018

#include <jtk/core/CString.h>
#include <jtk/core/CStringObjectAdapter.h>

#include <com/onecube/zen/compiler/symbol-table/CompilationUnitScope.h>
#include <com/onecube/zen/compiler/lexer/Token.h>

/*******************************************************************************
 * CompilationUnitScope                                                        *
 *******************************************************************************/

static const uint8_t zen_CompilationUnitScope_name[] = { 'c',  'o',  'm',  'p',  'i',  'l',  'a', 't', 'i', 'o', 'n', '_',  'u', 'n', 'i', 't', '\0' };

zen_CompilationUnitScope_t* zen_CompilationUnitScope_new() {
    zen_CompilationUnitScope_t* compilationUnitScope = zen_Memory_allocate(zen_CompilationUnitScope_t, 1);

    zen_Scope_t* scope = zen_Scope_new(zen_CompilationUnitScope_name, ZEN_SCOPE_COMPILATION_UNIT, NULL, compilationUnitScope);
    scope->m_resolveSymbol = (zen_Scope_ResolveSymbolFunction_t)zen_CompilationUnitScope_resolve;
    scope->m_defineSymbol = (zen_Scope_DefineSymbolFunction_t)zen_CompilationUnitScope_define;
    scope->m_getChildrenSymbols = (zen_Scope_GetChildrenSymbolsFunction_t)zen_CompilationUnitScope_getChildrenSymbols;

    compilationUnitScope->m_scope = scope;
    /* The value adapter is null because the HashMap is not required to test
     * any values.
     */
    compilationUnitScope->m_symbols = jtk_HashMap_new(jtk_CStringObjectAdapter_getInstance(), NULL);

    return compilationUnitScope;
}

void zen_CompilationUnitScope_delete(zen_CompilationUnitScope_t* scope) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    zen_Scope_delete(scope->m_scope);
    jtk_HashMap_delete(scope->m_symbols);
    jtk_Memory_deallocate(scope);
}

// Children Symbols

void zen_CompilationUnitScope_getChildrenSymbols(zen_CompilationUnitScope_t* scope,
    jtk_ArrayList_t* childrenSymbols) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");
    jtk_Assert_assertObject(childrenSymbols, "The specified list is null.");

    jtk_Iterator_t* iterator = jtk_HashMap_getValueIterator(scope->m_symbols);
    jtk_ArrayList_addAllFromIterator(childrenSymbols, iterator);
    jtk_Iterator_delete(iterator);
}

zen_Scope_t* zen_CompilationUnitScope_getScope(zen_CompilationUnitScope_t* scope) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    return scope->m_scope;
}

void zen_CompilationUnitScope_define(zen_CompilationUnitScope_t* scope, zen_Symbol_t* symbol) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    /* NOTE: The key string is owned by the ASTNode. */
    zen_ASTNode_t* identifier = zen_Symbol_getIdentifier(symbol);
    zen_Token_t* identifierToken = (zen_Token_t*)zen_ASTNode_getContext(identifier);
    const uint8_t* text = zen_Token_getText(identifierToken);
    if (!jtk_HashMap_putStrictly(scope->m_symbols, text, symbol)) {
        fprintf(stderr, "[internal error] zen_CompilationUnitScope_define() invoked to redefine a symbol.");
    }
}

zen_Symbol_t* zen_CompilationUnitScope_resolve(zen_CompilationUnitScope_t* scope, const uint8_t* identifier) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    return jtk_HashMap_getValue(scope->m_symbols, (void*)identifier);
}