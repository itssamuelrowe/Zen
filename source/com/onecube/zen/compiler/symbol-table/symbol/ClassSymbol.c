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

// Saturday, February 24, 2018

#include <com/onecube/zen/st/ClassSymbol.h>

/*******************************************************************************
 * ClassSymbol                                                                 *
 *******************************************************************************/

zen_ClassSymbol_t* zen_ClassSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope, zen_Scope_t* classScope) {
    zen_ClassSymbol_t* classSymbol = zen_Memory_allocate(zen_ClassSymbol_t, 1);

    zen_Symbol_t* symbol = zen_Symbol_new(ZEN_SYMBOL_CATEGORY_CLASS, identifier, enclosingScope, classSymbol);

    classSymbol->m_symbol = symbol;
    classSymbol->m_superClasses = zen_ArrayList_new();
    classSymbol->m_explicitModifiers = zen_ArrayList_new();
    classSymbol->m_modifiers = 0;
    classSymbol->m_classScope = classScope;

    return classSymbol;
}

void zen_ClassSymbol_delete(zen_ClassSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    zen_Symbol_delete(symbol->m_symbol);
    zen_ArrayList_delete(symbol->m_superClasses);
    zen_ArrayList_delete(symbol->m_explicitModifiers);
    zen_Memory_deallocate(symbol);
}

zen_Scope_t* zen_ClassSymbol_getClassScope(zen_ClassSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    return symbol->m_classScope;
}

jtk_ArrayList_t* zen_ClassSymbol_getSuperClasses(zen_ClassSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    return symbol->m_superClasses;
}

zen_Symbol_t* zen_ClassSymbol_getSymbol(zen_ClassSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    return symbol->m_symbol;
}

void zen_ClassSymbol_addModifier(zen_ClassSymbol_t* symbol, zen_Modifier_t modifier,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");
    
    if (node != NULL) {
        zen_ArrayList_add(symbol->m_explicitModifiers, node);
    }
    symbol->m_modifiers |= (int32_t)modifier;
}

bool zen_ClassSymbol_hasModifier(zen_ClassSymbol_t* symbol, zen_Modifier_t modifier) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");
    
    return (symbol->m_modifiers & (int32_t)modifier) != 0;
}