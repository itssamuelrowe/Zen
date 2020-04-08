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

#include <jtk/core/CString.h>
#include <com/onecube/zen/compiler/symbol-table/Scope.h>

zen_Scope_t* zen_Scope_new(const uint8_t* name, zen_ScopeType_t type,
    zen_Scope_t* enclosingScope, void* context) {
    jtk_Assert_assertObject(name, "The specified name is null.");

    zen_Scope_t* scope = jtk_Memory_allocate(zen_Scope_t, 1);
    scope->m_name = jtk_CString_new(name); // Fix this.
    scope->m_type = type;
    scope->m_enclosingScope = enclosingScope;
    // TODO

    return scope;
}

void zen_Scope_delete(zen_Scope_t* scope) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    jtk_CString_delete(scope->m_name);
    jtk_Memory_deallocate(scope);
}

// Children Symbols

void zen_Scope_getChildrenSymbols(zen_Scope_t* scope, jtk_ArrayList_t* childrenSymbols) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");
    jtk_Assert_assertObject(childrenSymbols, "The specified list is null.");

    // TODO
}

bool zen_Scope_isEnumerationScope(zen_Scope_t* scope) {
    return scope->m_type == ZEN_SCOPE_ENUMERATION;
}

bool zen_Scope_isClassScope(zen_Scope_t* scope) {
    return scope->m_type == ZEN_SCOPE_CLASS;
}

bool zen_Scope_isFunctionScope(zen_Scope_t* scope) {
    return scope->m_type == ZEN_SCOPE_FUNCTION;
}

bool zen_Scope_isCompilationUnitScope(zen_Scope_t* scope) {
    return scope->m_type == ZEN_SCOPE_COMPILATION_UNIT;
}

bool zen_Scope_isLocalScope(zen_Scope_t* scope) {
    return scope->m_type == ZEN_SCOPE_LOCAL;
}

zen_Scope_t* zen_Scope_getEnclosingScope(zen_Scope_t* scope) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");
    return scope->m_enclosingScope;
}

void zen_Scope_define(zen_Scope_t* scope, zen_Symbol_t* symbol) {
    // TODO
}

zen_Symbol_t* zen_Scope_resolve(zen_Scope_t* scope, uint8_t* identifier) {
    // TODO
    return NULL;
}

const uint8_t* zen_Scope_getName(zen_Scope_t* scope) {
    return scope->m_name;
}

zen_ScopeType_t zen_Scope_getType(zen_Scope_t* scope) {
    return scope->m_type;
}

zen_Scope_t* zen_Scope_resolveQualifiedSymbol(zen_Scope_t* scope, const uint8_t* name,
    int32_t nameSize) {
    return NULL;
}