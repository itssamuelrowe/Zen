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

// Sunday, January 07, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_VARIABLE_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_VARIABLE_SYMBOL_H

#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/symbol-table/Symbol.h>

/*******************************************************************************
 * VariableSymbol                                                              *
 *******************************************************************************/

/**
 * @class ClassScope
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_VariableSymbol_t {
    zen_Symbol_t* m_symbol;
    jtk_ArrayList_t* m_modifiers;
    int32_t m_implicitModifiers;
};

/**
 * @memberof AnnotationSymbol
 */
typedef struct zen_VariableSymbol_t zen_VariableSymbol_t;

// Constructor

/**
 * @memberof AnnotationSymbol
 */
zen_VariableSymbol_t* zen_VariableSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope);

// Destructor

/**
 * @memberof AnnotationSymbol
 */
void zen_VariableSymbol_delete(zen_VariableSymbol_t* symbol);

// Symbol

/**
 * @memberof AnnotationSymbol
 */
zen_Symbol_t* zen_VariableSymbol_getSymbol(zen_VariableSymbol_t* symbol);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_VARIABLE_SYMBOL_H */