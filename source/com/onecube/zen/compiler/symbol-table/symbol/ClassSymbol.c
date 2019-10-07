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

#include <com/onecube/zen/compiler/symbol-table/ClassSymbol.h>

/*******************************************************************************
 * ClassSymbol                                                                 *
 *******************************************************************************/

zen_ClassSymbol_t* zen_ClassSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope, zen_Scope_t* classScope, jtk_String_t* qualifiedName) {
    zen_ClassSymbol_t* classSymbol = zen_Memory_allocate(zen_ClassSymbol_t, 1);

    zen_Symbol_t* symbol = zen_Symbol_new(ZEN_SYMBOL_CATEGORY_CLASS, identifier, enclosingScope, classSymbol);

    classSymbol->m_symbol = symbol;
    classSymbol->m_superClasses = jtk_ArrayList_new();
    classSymbol->m_explicitModifiers = jtk_ArrayList_new();
    classSymbol->m_modifiers = 0;
    classSymbol->m_classScope = classScope;
    classSymbol->m_qualifiedName = jtk_String_clone(qualifiedName);

    return classSymbol;
}

void zen_ClassSymbol_delete(zen_ClassSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    jtk_String_delete(symbol->m_qualifiedName);
    zen_Symbol_delete(symbol->m_symbol);
    jtk_ArrayList_delete(symbol->m_superClasses);
    jtk_ArrayList_delete(symbol->m_explicitModifiers);
    jtk_Memory_deallocate(symbol);
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
        jtk_ArrayList_add(symbol->m_explicitModifiers, node);
    }
    symbol->m_modifiers |= (int32_t)modifier;
}

bool zen_ClassSymbol_hasModifier(zen_ClassSymbol_t* symbol, zen_Modifier_t modifier) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");
    
    return (symbol->m_modifiers & (int32_t)modifier) != 0;
}

jtk_String_t* zen_ClassSymbol_getQualifiedName(zen_ClassSymbol_t* symbol) {
    /* To retrieve the fully qualified name a few resolution steps must be taken.
     * Therefore, a fully qualified name is evaluated and stored during the instantiation
     * of this class.
     *
     * Classes that are automatically made available by the compiler are represented
     * by the ImplicitClassSymbol class. This allows us to use the ASTNode and
     * be sure that the node originates from the source file. Therefore, we do
     * not worry about implicitly imported classes here.
     */
    return symbol->m_qualifiedName;
}