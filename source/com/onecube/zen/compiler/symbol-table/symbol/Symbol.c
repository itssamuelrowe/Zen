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

// Monday, January 08, 2018

#include <com/onecube/zen/compiler/symbol-table/Symbol.h>

/*******************************************************************************
 * Symbol                                                                      *
 *******************************************************************************/

zen_Symbol_t* zen_Symbol_new(zen_SymbolCategory_t category,
    zen_ASTNode_t* identifier, zen_Scope_t* enclosingScope, void* context) {
    zen_Symbol_t* symbol = zen_Memory_allocate(zen_Symbol_t, 1);
    symbol->m_category = category;
    symbol->m_identifier = identifier;
    symbol->m_enclosingScope = enclosingScope;
    symbol->m_explicitModifiers = jtk_ArrayList_new();
    symbol->m_modifiers = 0;
    // TODO

    return symbol;
}

void zen_Symbol_delete(zen_Symbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    jtk_ArrayList_delete(symbol->m_explicitModifiers);
    jtk_Memory_deallocate(symbol);
}

// Category

zen_SymbolCategory_t zen_Symbol_getCategory(zen_Symbol_t* symbol) {
    return symbol->m_category;
}

bool zen_Symbol_isEnumeration(zen_Symbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");
    return (symbol->m_category == ZEN_SYMBOL_CATEGORY_ENUMERATION);
}

bool zen_Symbol_isEnumerate(zen_Symbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");
    return (symbol->m_category == ZEN_SYMBOL_CATEGORY_ENUMERATE);
}

bool zen_Symbol_isFunction(zen_Symbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");
    return (symbol->m_category == ZEN_SYMBOL_CATEGORY_FUNCTION);
}

bool zen_Symbol_isConstant(zen_Symbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");
    return (symbol->m_category == ZEN_SYMBOL_CATEGORY_CONSTANT);
}

bool zen_Symbol_isVariable(zen_Symbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");
    return (symbol->m_category == ZEN_SYMBOL_CATEGORY_VARIABLE);
}

bool zen_Symbol_isClass(zen_Symbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");
    return (symbol->m_category == ZEN_SYMBOL_CATEGORY_CLASS);
}

zen_Scope_t* zen_Symbol_getEnclosingScope(zen_Symbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");
    return symbol->m_enclosingScope;
}

zen_ASTNode_t* zen_Symbol_getIdentifier(zen_Symbol_t* symbol) {
    return symbol->m_identifier;
}

/* Modifier */

void zen_Symbol_addExplicitModifiers(zen_Symbol_t* symbol, uint32_t modifiers,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    if (node != NULL) {
        jtk_ArrayList_add(symbol->m_explicitModifiers, node);
    }
    symbol->m_modifiers |= modifiers;
}

void zen_Symbol_addModifiers(zen_Symbol_t* symbol, uint32_t modifiers) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    symbol->m_modifiers |= modifiers;
}

bool zen_Symbol_hasModifiers(zen_Symbol_t* symbol, uint32_t modifiers) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    return (symbol->m_modifiers & modifiers) == modifiers;
}