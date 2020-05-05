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

// Saturday, November 25, 2017

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_H

#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/symbol-table/Scope.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolCategory.h>
#include <com/onecube/zen/compiler/symbol-table/ClassSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/FunctionSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/Modifier.h>

enum zen_SymbolFlag_t {
    ZEN_SYMBOL_FLAG_EXTERNAL = (1 << 0)
};

/*******************************************************************************
 * Symbol                                                                      *
 *******************************************************************************/

/**
 * @class Symbol
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_Symbol_t {
    zen_SymbolCategory_t m_category;
    zen_ASTNode_t* m_identifier;
    zen_Scope_t* m_enclosingScope;
    union {
        zen_ClassSymbol_t m_asClass;
        zen_FunctionSymbol_t m_asFunction;
        zen_Symbol_t* m_asExternal;
    } m_context;
    uint32_t m_modifiers;
    int32_t m_ticket;
    int32_t m_index;
    uint32_t m_flags;

    uint8_t* m_name;
    int32_t m_nameSize;
};

/**
 * @memberof Symbol
 */
typedef struct zen_Symbol_t zen_Symbol_t;

// Constructor

/**
 * @memberof Symbol
 */
zen_Symbol_t* zen_Symbol_new(zen_SymbolCategory_t category,
    zen_ASTNode_t* identifier, zen_Scope_t* enclosingScope);

zen_Symbol_t* zen_Symbol_forConstant(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope);

zen_Symbol_t* zen_Symbol_forVariable(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope);

zen_Symbol_t* zen_Symbol_forFunction(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope);

/**
 * This function is used for class declarations in compilation units.
 */
zen_Symbol_t* zen_Symbol_forClass(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope, zen_Scope_t* classScope, const uint8_t* name,
    int32_t nameSize, const uint8_t* package, int32_t packageSize);

/**
 * This function is used by the symbol loader.
 */
zen_Symbol_t* zen_Symbol_forClassAlt(zen_Scope_t* classScope, const uint8_t* descriptor,
    int32_t descriptorSize);

zen_Symbol_t* zen_Symbol_forLabel(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope);

zen_Symbol_t* zen_Symbol_forExternal(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope, zen_Symbol_t* other);

// Destructor

/**
 * @memberof Symbol
 */
void zen_Symbol_delete(zen_Symbol_t* symbol);

// Category

zen_SymbolCategory_t zen_Symbol_getCategory(zen_Symbol_t* symbol);

// Function

/**
 * @memberof Symbol
 */
bool zen_Symbol_isFunction(zen_Symbol_t* symbol);

// Constant

/**
 * @memberof Symbol
 */
bool zen_Symbol_isConstant(zen_Symbol_t* symbol);

// Enumerate

/**
 * @memberof Symbol
 */
bool zen_Symbol_isEnumerate(zen_Symbol_t* symbol);

// Variable

/**
 * @memberof Symbol
 */
bool zen_Symbol_isVariable(zen_Symbol_t* symbol);

// Enumeration

/**
 * @memberof Symbol
 */
bool zen_Symbol_isEnumeration(zen_Symbol_t* symbol);

// External

/**
 * @memberof Symbol
 */
bool zen_Symbol_isExternal(zen_Symbol_t* symbol);

bool zen_Symbol_isStatic(zen_Symbol_t* symbol);

// Enclosing Scope

/**
 * @memberof Symbol
 */
zen_Scope_t* zen_Symbol_getEnclosingScope(zen_Symbol_t* symbol);

// Identifier

/**
 * @memberof Symbol
 */
zen_ASTNode_t* zen_Symbol_getIdentifier(zen_Symbol_t* symbol);

void zen_Symbol_addModifiers(zen_Symbol_t* symbol, uint32_t modifiers);

zen_FunctionSignature_t* zen_Symbol_getFunctionSignatureEx(zen_Symbol_t* symbol,
    const uint8_t* descriptor, int32_t descriptorSize);
zen_FunctionSignature_t* zen_Symbol_getFunctionSignature(zen_Symbol_t* symbol,
    int32_t argumentCount);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_H */