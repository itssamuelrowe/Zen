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
#include <jtk/collection/stack/LinkedStack.h>
#include <jtk/core/CStringObjectAdapter.h>

#include <com/onecube/zen/compiler/symbol-table/Scope.h>
#include <com/onecube/zen/compiler/symbol-table/Symbol.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/lexer/Token.h>

zen_Scope_t* zen_Scope_new(const uint8_t* name, int32_t nameSize,
    zen_ScopeType_t type, zen_Scope_t* enclosingScope, zen_Symbol_t* symbol) {
    // jtk_Assert_assertObject(name, "The specified name is null.");

    jtk_ObjectAdapter_t* stringObjectAdapter = jtk_CStringObjectAdapter_getInstance();

    zen_Scope_t* scope = jtk_Memory_allocate(zen_Scope_t, 1);
    scope->m_name = NULL; // jtk_CString_make(name, &nameSize);
    scope->m_nameSize = nameSize;
    scope->m_type = type;
    scope->m_enclosingScope = enclosingScope;
    scope->m_symbols = jtk_HashMap_new(stringObjectAdapter, NULL);
    scope->m_nextTicket = 0;
    scope->m_symbol = symbol;

    return scope;
}

zen_Scope_t* zen_Scope_forCompilationUnit() {
    return zen_Scope_new(NULL, 0, ZEN_SCOPE_COMPILATION_UNIT, NULL, NULL);
}

zen_Scope_t* zen_Scope_forFunction(zen_Scope_t* enclosingScope) {
    return zen_Scope_new(NULL, 0, ZEN_SCOPE_FUNCTION, enclosingScope, NULL);
}

zen_Scope_t* zen_Scope_forLocal(zen_Scope_t* enclosingScope) {
    return zen_Scope_new(NULL, 0, ZEN_SCOPE_LOCAL, enclosingScope, NULL);
}

zen_Scope_t* zen_Scope_forClass(zen_Scope_t* enclosingScope) {
    return zen_Scope_new(NULL, 0, ZEN_SCOPE_CLASS, enclosingScope, NULL);
}

void zen_Scope_delete(zen_Scope_t* scope) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    jtk_HashMap_delete(scope->m_symbols);
    jtk_CString_delete(scope->m_name);
    jtk_Memory_deallocate(scope);
}

// Children Symbols

void zen_Scope_getChildrenSymbols(zen_Scope_t* scope, jtk_ArrayList_t* childrenSymbols) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");
    jtk_Assert_assertObject(childrenSymbols, "The specified list is null.");

    jtk_Iterator_t* iterator = jtk_HashMap_getValueIterator(scope->m_symbols);
    jtk_ArrayList_addAllFromIterator(childrenSymbols, iterator);
    jtk_Iterator_delete(iterator);
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
    const uint8_t* text = ((zen_Token_t*)symbol->m_identifier->m_context)->m_text;
    zen_Scope_defineEx(scope, text, -1, symbol);
}

void zen_Scope_defineEx(zen_Scope_t* scope, const uint8_t* descriptor,
    int32_t descriptorSize, zen_Symbol_t* symbol) {
    if (!jtk_HashMap_putStrictly(scope->m_symbols, (void*)descriptor, symbol)) {
        fprintf(stderr, "[internal error] zen_Scope_define() invoked to redefine a symbol.\n");
    }
}

/*
 * Resolving a class member
 * ------------------------
 *
 * 0. We are given a class scope and an identifier.
 * 1. Retrieve the class symbol corresponding to class scope.
 * 2. The super classes of the class symbol are stored in an
 *    tree. Therefore, create a linked-stack for its traversal.
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
zen_Symbol_t* zen_Scope_resolveClassMember(zen_Scope_t* scope,
    const uint8_t* identifier) {
    jtk_Assert_assertObject(scope, "The specified class scope is null.");
    jtk_Assert_assertObject(identifier, "The specified identifier is null.");

    zen_Symbol_t* result = NULL;
    zen_Symbol_t* symbol = scope->m_symbol;

    jtk_LinkedStack_t* stack = jtk_LinkedStack_new();
    /* The things you have to do when you have no inheritance. (-__-) */
    jtk_LinkedStack_push(stack, symbol);

    while (!jtk_LinkedStack_isEmpty(stack)) {
        symbol = (zen_Symbol_t*)jtk_LinkedStack_pop(stack);

        result = (zen_Symbol_t*)jtk_HashMap_getValue(scope->m_symbols, identifier);
        if (result != NULL) {
            break;
        }
        else {
            jtk_ArrayList_t* superClasses = symbol->m_context.m_asClass.m_superClasses;
            int32_t size = jtk_ArrayList_getSize(superClasses);
            int32_t i;
            for (i = 0; i < size; i++) {
                zen_Symbol_t* superClassSymbol = jtk_ArrayList_getValue(superClasses, i);
                jtk_LinkedStack_push(stack, superClassSymbol);
            }
        }
    }

    /* Destroy the stack; not required anymore. */
    jtk_LinkedStack_delete(stack);

    return result;
}

zen_Symbol_t* zen_Scope_resolve(zen_Scope_t* scope, uint8_t* identifier) {
    zen_Symbol_t* result = NULL;
    switch (scope->m_type) {
        case ZEN_SCOPE_COMPILATION_UNIT:
        case ZEN_SCOPE_FUNCTION:
        case ZEN_SCOPE_LOCAL: {
            result = jtk_HashMap_getValue(scope->m_symbols, (void*)identifier);
            break;
        }

        case ZEN_SCOPE_CLASS: {
            result = zen_Scope_resolveClassMember(scope, identifier);
            break;
        }
    }
    return result;
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