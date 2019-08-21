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

#include <com/onecube/zen/core/StringObjectAdapter.h>
#include <jtk/collection/map/HashMap.h>
#include <com/onecube/zen/st/Scope.h>
#include <com/onecube/zen/st/EnumerationScope.h>
#include <com/onecube/zen/st/Symbol.h>
#include <com/onecube/zen/Token.h>

static const char* zen_EnumerationScope_name = "enumeration";

/*******************************************************************************
 * EnumerationScope                                                            *
 *******************************************************************************/

zen_EnumerationScope_t* zen_EnumerationScope_new(zen_Scope_t* enclosingScope) {
    zen_EnumerationScope_t* enumerationScope = zen_Memory_allocate(zen_EnumerationScope_t, 1);
    
    zen_Scope_t* scope = zen_Scope_new(zen_EnumerationScope_name, ZEN_SCOPE_ENUMERATION, enclosingScope, enumerationScope);
    scope->m_resolveSymbol = (zen_Scope_ResolveSymbolFunction_t)zen_EnumerationScope_resolve;
    scope->m_defineSymbol = (zen_Scope_DefineSymbolFunction_t)zen_EnumerationScope_define;

    enumerationScope->m_scope = scope;
    enumerationScope->m_enumerates = zen_HashMap_new(zen_StringObjectAdapter_getInstance(), NULL);
    enumerationScope->m_enumerationSymbol = NULL;
#warning "enumerationScope->m_enumerationSymbol must be set by the EnumerationSymbol class."

    return enumerationScope;
}

void zen_EnumerationScope_delete(zen_EnumerationScope_t* scope) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    zen_Scope_delete(scope->m_scope);
    zen_HashMap_delete(scope->m_enumerates);
    zen_Memory_deallocate(scope);
}

zen_Scope_t* zen_EnumerationScope_getScope(zen_EnumerationScope_t* scope) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    return scope->m_scope;
}

zen_Symbol_t* zen_EnumerationScope_getEnumerationSymbol(zen_EnumerationScope_t* scope) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    return scope->m_enumerationSymbol;
}

void zen_EnumerationScope_define(zen_EnumerationScope_t* scope, zen_Symbol_t* symbol) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    if (zen_Symbol_isEnumerate(symbol)) {
        const uint8_t* text = zen_Token_getText((zen_Token_t*)zen_ASTNode_getContext(zen_Symbol_getIdentifier(symbol)));
        if (!jtk_HashMap_putStrictly(scope->m_enumerates, (void*)text, symbol)) {
            fprintf(stderr, "[internal error] zen_ClassScope_define() invoked to redefine a symbol.\n");
        }
    }
    else {
        fprintf(stderr, "[internal error] Unknown symbol definition in EnumerationScope.\n");
    }
}

zen_Symbol_t* zen_EnumerationScope_resolve(zen_EnumerationScope_t* scope, const uint8_t* identifier) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    return zen_HashMap_getValue(scope->m_enumerates, (void*)identifier);
}