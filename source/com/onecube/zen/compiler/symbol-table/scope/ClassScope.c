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

// Monday, January 08, 2018

#include <jtk/collection/list/ArrayList.h>
#include <jtk/collection/stack/LinkedStack.h>
#include <jtk/core/StringObjectAdapter.h>

#include <com/onecube/zen/compiler/lexer/Token.h>
#include <com/onecube/zen/compiler/symbol-table/ClassScope.h>
#include <com/onecube/zen/compiler/symbol-table/ClassSymbol.h>

static const char* zen_ClassScope_name = "class";

zen_ClassScope_t* zen_ClassScope_new(zen_Scope_t* enclosingScope) {
    zen_ClassScope_t* classScope = zen_Memory_allocate(zen_ClassScope_t, 1);

    zen_Scope_t* scope = zen_Scope_new(zen_ClassScope_name, ZEN_SCOPE_CLASS, enclosingScope, classScope);
    scope->m_resolveSymbol = (zen_Scope_ResolveSymbolFunction_t)zen_ClassScope_resolve;
    scope->m_defineSymbol = (zen_Scope_DefineSymbolFunction_t)zen_ClassScope_define;

    classScope->m_scope = scope;
    classScope->m_symbols = jtk_HashMap_new(zen_StringObjectAdapter_getInstance(), NULL);
    classScope->m_classSymbol = NULL;
#warning "classScope->m_classSymbol must be set by the ClassSymbol class."

    return classScope;
}

void zen_ClassScope_delete(zen_ClassScope_t* scope) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    zen_Scope_delete(scope->m_scope);
    jtk_HashMap_delete(scope->m_symbols);
    zen_Memory_deallocate(scope);
}

zen_Scope_t* zen_ClassScope_getScope(zen_ClassScope_t* scope) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    return scope->m_scope;
}

zen_Symbol_t* zen_ClassScope_getClassSymbol(zen_ClassScope_t* scope) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    return scope->m_classSymbol;
}

void zen_ClassScope_define(zen_ClassScope_t* scope, zen_Symbol_t* symbol) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    if (zen_Symbol_isFunction(symbol) || zen_Symbol_isConstant(symbol) ||
        zen_Symbol_isVariable(symbol) || zen_Symbol_isEnumeration(symbol)) {
        const uint8_t* text = zen_Token_getText((zen_Token_t*)zen_ASTNode_getContext(zen_Symbol_getIdentifier(symbol)));
        if (!jtk_HashMap_putStrictly(scope->m_symbols, (void*)text, symbol)) {
            fprintf(stderr, "[internal error] zen_ClassScope_define() invoked to redefine a symbol.\n");
        }
    }
    else {
        fprintf(stderr, "[internal error] Unknown symbol definition in ClassScope.\n");
    }
}

/* Resolve */

/*
 * Resolving a class member
 * ------------------------
 *
 * 0. We are given a class scope and an identifier.
 * 1. Retrieve the class symbol corresponding to class scope.
 * 2. The super classes of the class symbol are stored in an
 *    n-ary tree. Therefore, create a linked-stack for its traversal.
 * 3. Push the class symbol on to the stack.
 * 4. Repeatedly process the elements on the stack.
 * 5. Retrieve the class symbol on top of the stack and remove it.
 * 6. Retrieve the class scope associated with the class symbol.
 * 7. Look for a function, variable, constant, enumeration, or subclass symbol
 *    within the class scope.
 * 8. If such a symbol exists, retrieve the corresponding symbol. Fall
 *    through to the end.
 * 9. Otherwise, retrieve the super classes of the current class.
 *     Push the super classes on to the stack. GOTO step 4.
 * 10. Return the symbol, if found. Otherwise, null.
 */
zen_Symbol_t* zen_ClassScope_resolve(zen_ClassScope_t* scope,
    const uint8_t* identifier) {
    jtk_Assert_assertObject(scope, "The specified class scope is null.");
    jtk_Assert_assertObject(identifier, "The specified identifier is null.");

    zen_ClassSymbol_t* classSymbol = (zen_ClassSymbol_t*)(zen_ClassScope_getClassSymbol(scope)->m_context);
    zen_Symbol_t* symbol = NULL;

    jtk_LinkedStack_t* stack = jtk_LinkedStack_new();
    /* The things you have to do when you have no inheritance. (-__-) */
    jtk_LinkedStack_push(stack, classSymbol);

    while (!jtk_LinkedStack_isEmpty(stack)) {
        classSymbol = (zen_ClassSymbol_t*)jtk_LinkedStack_pop(stack);
        scope = (zen_ClassScope_t*)zen_ClassSymbol_getClassScope(classSymbol)->m_context;

        symbol = (zen_Symbol_t*)jtk_HashMap_getValue(scope->m_symbols, identifier);
        if (symbol != NULL) {
            break;
        }
        else {
            jtk_ArrayList_t* superClasses = zen_ClassSymbol_getSuperClasses(classSymbol);
            int32_t size = zen_ArrayList_getSize(superClasses);
            int32_t i;
            for (i = 0; i < size; i++) {
                zen_Symbol_t* rawSuperClassSymbol = zen_ArrayList_get(superClasses, i);
                zen_ClassSymbol_t* superClassSymbol = (zen_ClassSymbol_t*)rawSuperClassSymbol->m_context;
                jtk_LinkedStack_push(stack, superClassSymbol);
            }
        }
    }

    /* Destroy the stack; not required anymore. */
    jtk_LinkedStack_delete(stack);

    return symbol;
}