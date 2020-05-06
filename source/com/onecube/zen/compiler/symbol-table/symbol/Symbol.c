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
#include <com/onecube/zen/compiler/lexer/Token.h>
#include <jtk/core/CString.h>

/*******************************************************************************
 * Symbol                                                                      *
 *******************************************************************************/

zen_Symbol_t* zen_Symbol_new(zen_SymbolCategory_t category,
    zen_ASTNode_t* identifier, zen_Scope_t* enclosingScope) {
    zen_Symbol_t* symbol = zen_Memory_allocate(zen_Symbol_t, 1);
    symbol->m_category = category;
    symbol->m_identifier = identifier;
    symbol->m_enclosingScope = enclosingScope;
    symbol->m_modifiers = 0;
    symbol->m_ticket = (enclosingScope != NULL)? enclosingScope->m_nextTicket++ : -1;
    symbol->m_index = -1;
    symbol->m_flags = 0;
    if (identifier != NULL) {
        zen_Token_t* token = (zen_Token_t*)identifier->m_context;
        symbol->m_name = token->m_text;
        symbol->m_nameSize = token->m_length;
    }

    if (category == ZEN_SYMBOL_CATEGORY_FUNCTION) {
        zen_FunctionSymbol_initialize(&symbol->m_context.m_asFunction);
    }
    else if (category == ZEN_SYMBOL_CATEGORY_CLASS) {
        zen_ClassSymbol_initialize(&symbol->m_context.m_asClass);
    }

    return symbol;
}

zen_Symbol_t* zen_Symbol_forConstant(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope) {
    return zen_Symbol_new(ZEN_SYMBOL_CATEGORY_CONSTANT, identifier, enclosingScope);
}

zen_Symbol_t* zen_Symbol_forVariable(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope) {
    return zen_Symbol_new(ZEN_SYMBOL_CATEGORY_VARIABLE, identifier, enclosingScope);
}

zen_Symbol_t* zen_Symbol_forFunction(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope) {
    zen_Symbol_t* symbol = zen_Symbol_new(ZEN_SYMBOL_CATEGORY_FUNCTION,
        identifier, enclosingScope);
    return symbol;
}

zen_Symbol_t* zen_Symbol_forClass(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope, zen_Scope_t* classScope, const uint8_t* name,
    int32_t nameSize, const uint8_t* package, int32_t packageSize) {
    uint8_t* qualifiedName = NULL;
    int32_t qualifiedNameSize = -1;
    if (package != NULL) {
        uint8_t* strings[] = {
            package,
            ".",
            name
        };
        int32_t sizes[] = {
            packageSize,
            1,
            nameSize
        };
        qualifiedName = jtk_CString_joinAll(strings, sizes, 3, &qualifiedNameSize);
    }
    else {
        qualifiedName = jtk_CString_newEx(name, nameSize);
        qualifiedNameSize = nameSize;
    }

    uint8_t* descriptor = jtk_CString_newEx(qualifiedName, qualifiedNameSize);
    jtk_Arrays_replace_b(descriptor, qualifiedNameSize, '.', '/');

    zen_Symbol_t* symbol = zen_Symbol_new(ZEN_SYMBOL_CATEGORY_CLASS, identifier, enclosingScope);
    zen_ClassSymbol_t* classSymbol = &symbol->m_context.m_asClass;
    classSymbol->m_qualifiedName = qualifiedName;
    classSymbol->m_qualifiedNameSize = qualifiedNameSize;
    classSymbol->m_descriptor = descriptor;
    classSymbol->m_descriptorSize = qualifiedNameSize;
    classSymbol->m_classScope = classScope;

    return symbol;
}

zen_Symbol_t* zen_Symbol_forClassAlt(zen_Scope_t* classScope, const uint8_t* descriptor,
    int32_t descriptorSize) {
    uint8_t* qualifiedName = jtk_CString_newEx(descriptor,
        descriptorSize);
    jtk_Arrays_replace_b(qualifiedName, descriptorSize, '/', '.');

    zen_Symbol_t* symbol = zen_Symbol_new(ZEN_SYMBOL_CATEGORY_CLASS, NULL, NULL);
    zen_ClassSymbol_t* classSymbol = &symbol->m_context.m_asClass;
    classSymbol->m_qualifiedName = qualifiedName;
    classSymbol->m_qualifiedNameSize = descriptorSize;
    classSymbol->m_descriptor = descriptor;
    classSymbol->m_descriptorSize = descriptorSize;
    classSymbol->m_classScope = classScope;

    return symbol;
}

zen_Symbol_t* zen_Symbol_forLabel(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope) {
    return zen_Symbol_new(ZEN_SYMBOL_CATEGORY_LABEL, identifier, enclosingScope);
}

zen_Symbol_t* zen_Symbol_forExternal(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope, zen_Symbol_t* other) {
    zen_Symbol_t* result = zen_Symbol_new(ZEN_SYMBOL_CATEGORY_EXTERNAL,
        identifier, enclosingScope);
    result->m_context.m_asExternal = other;

    return result;
}

void zen_Symbol_delete(zen_Symbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    if (symbol->m_category == ZEN_SYMBOL_CATEGORY_FUNCTION) {
        zen_FunctionSymbol_destroy(&symbol->m_context.m_asFunction);
    }
    else if (symbol->m_category == ZEN_SYMBOL_CATEGORY_CLASS) {
        zen_ClassSymbol_destroy(&symbol->m_context.m_asClass);
    }

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

bool zen_Symbol_isExternal(zen_Symbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");
    return (symbol->m_category == ZEN_SYMBOL_CATEGORY_EXTERNAL);
}

zen_Scope_t* zen_Symbol_getEnclosingScope(zen_Symbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");
    return symbol->m_enclosingScope;
}

zen_ASTNode_t* zen_Symbol_getIdentifier(zen_Symbol_t* symbol) {
    return symbol->m_identifier;
}

/* Modifier */

void zen_Symbol_addModifiers(zen_Symbol_t* symbol, uint32_t modifiers) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    symbol->m_modifiers |= modifiers;
}

bool zen_Symbol_hasModifiers(zen_Symbol_t* symbol, uint32_t modifiers) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    return (symbol->m_modifiers & modifiers) == modifiers;
}

bool zen_Symbol_isStatic(zen_Symbol_t* symbol) {
    return (symbol->m_modifiers & ZEN_MODIFIER_STATIC) != 0;
}

zen_FunctionSignature_t* zen_Symbol_getFunctionSignature(zen_Symbol_t* symbol,
    int32_t argumentCount) {
    zen_FunctionSymbol_t* functionSymbol = &symbol->m_context.m_asFunction;
    if ((argumentCount >= functionSymbol->m_parameterThreshold) &&
        (functionSymbol->m_parameterThreshold >= 0)) {
        argumentCount = functionSymbol->m_parameterThreshold;
    }

    zen_FunctionSignature_t* result = NULL;
    int32_t i;
    int32_t count = jtk_ArrayList_getSize(functionSymbol->m_signatures);
    for (i = 0; i < count; i++) {
        zen_FunctionSignature_t* signature = (zen_FunctionSignature_t*)
            jtk_ArrayList_getValue(functionSymbol->m_signatures, i);
        int32_t parameterCount = signature->m_fixedParameterCount; // jtk_ArrayList_getSize(signature->m_fixedParameters);
        if (parameterCount == argumentCount) {
            result = signature;
            break;
        }
    }
    return result;
}

zen_FunctionSignature_t* zen_Symbol_getFunctionSignatureEx(zen_Symbol_t* symbol,
    const uint8_t* descriptor, int32_t descriptorSize) {
    zen_FunctionSymbol_t* functionSymbol = &symbol->m_context.m_asFunction;
    zen_FunctionSignature_t* result = NULL;
    int32_t i;
    int32_t count = jtk_ArrayList_getSize(functionSymbol->m_signatures);
    for (i = 0; i < count; i++) {
        zen_FunctionSignature_t* signature = (zen_FunctionSignature_t*)
            jtk_ArrayList_getValue(functionSymbol->m_signatures, i);
        if (jtk_CString_equals(signature->m_descriptor, signature->m_descriptorSize,
            descriptor, descriptorSize)) {
            result = signature;
            break;
        }
    }
    return result;
}