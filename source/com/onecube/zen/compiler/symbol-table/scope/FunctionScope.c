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

// Tuesday, February 27, 2018

#include <jtk/core/CString.h>
#include <jtk/core/Object.h>

#include <com/onecube/zen/compiler/lexer/Token.h>
#include <com/onecube/zen/compiler/symbol-table/FunctionScope.h>

/*******************************************************************************
 * FunctionScope                                                               *
 *******************************************************************************/

static const uint8_t zen_FunctionScope_name[] = { 'f',  'u',  'n',  'c',  't',  'i',  'o', 'n', '\0' };

zen_FunctionScope_t* zen_FunctionScope_new(zen_Scope_t* enclosingScope) {
    zen_FunctionScope_t* functionScope = zen_Memory_allocate(zen_FunctionScope_t, 1);

    zen_Scope_t* scope = zen_Scope_new(zen_FunctionScope_name, ZEN_SCOPE_FUNCTION, enclosingScope, functionScope);
    scope->m_resolveSymbol = (zen_Scope_ResolveSymbolFunction_t)zen_FunctionScope_resolve;
    scope->m_defineSymbol = (zen_Scope_DefineSymbolFunction_t)zen_FunctionScope_define;
    scope->m_getChildrenSymbols = (zen_Scope_GetChildrenSymbolsFunction_t)zen_FunctionScope_getChildrenSymbols;

    functionScope->m_scope = scope;
    functionScope->m_fixedParameters = jtk_ArrayList_new();
    functionScope->m_variableParameter = NULL;

    return functionScope;
}

void zen_FunctionScope_delete(zen_FunctionScope_t* scope) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    zen_Scope_delete(scope->m_scope);
    jtk_ArrayList_delete(scope->m_fixedParameters);
    jtk_Memory_deallocate(scope);
}

zen_Scope_t* zen_FunctionScope_getScope(zen_FunctionScope_t* scope) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    return scope->m_scope;
}

// Children Symbols

void zen_FunctionScope_getChildrenSymbols(zen_FunctionScope_t* scope,
    jtk_ArrayList_t* childrenSymbols) {
    
    jtk_ArrayList_addAll(childrenSymbols, JTK_COLLECTION_ARRAY_LIST, scope->m_fixedParameters);
    jtk_ArrayList_addPredicatively(childrenSymbols, scope->m_variableParameter, jtk_Object_isNotNull);
}

void zen_FunctionScope_define(zen_FunctionScope_t* scope, zen_Symbol_t* symbol) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    if (zen_Symbol_isConstant(symbol)) {
        zen_ConstantSymbol_t* constantSymbol = (zen_ConstantSymbol_t*)symbol->m_context;
        if (zen_ConstantSymbol_hasModifier(constantSymbol, ZEN_MODIFIER_VARIABLE_PARAMETER)) {
            if (scope->m_variableParameter != NULL) {
                fprintf(stderr, "[internal error] Declaring multiple variable parameters in the same function scope.");
            }
            scope->m_variableParameter = symbol;
        }
        else {
#warning "TODO: Why aren't we checking for an internal error?"
            jtk_ArrayList_add(scope->m_fixedParameters, symbol);
        }
    }
    else {
        fprintf(stderr, "[internal error] The specified function parameters are not constants.");
    }
}

zen_Symbol_t* zen_FunctionScope_resolve(zen_FunctionScope_t* scope, const uint8_t* identifier) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    zen_Symbol_t* result = NULL;
    int32_t size = jtk_ArrayList_getSize(scope->m_fixedParameters);
    int32_t i;
    for (i = 0; i < size; i++) {
        zen_Symbol_t* symbol = (zen_Symbol_t*)jtk_ArrayList_getValue(scope->m_fixedParameters, i);
        const uint8_t* identifier0 = zen_Token_getText((zen_Token_t*)(symbol->m_identifier->m_context));
        if (jtk_CString_equals(identifier, jtk_CString_getSize(identifier), identifier0, jtk_CString_getSize(identifier0))) {
            result = symbol;
            break;
        }
    }

    if ((result == NULL) && (scope->m_variableParameter != NULL)) {
        const uint8_t* identifier0 = zen_Token_getText((zen_Token_t*)(scope->m_variableParameter->m_identifier->m_context));
        if (jtk_CString_equals(identifier, jtk_CString_getSize(identifier), identifier0, jtk_CString_getSize(identifier0))) {
            result = scope->m_variableParameter;
        }
    }

    return result;
}
