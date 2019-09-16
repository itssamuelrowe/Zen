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

// Monday, March 05, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_LABEL_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_LABEL_SYMBOL_H

#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/symbol-table/Symbol.h>

/*******************************************************************************
 * LabelSymbol                                                                 *
 *******************************************************************************/

/**
 * @class LabelSymbol
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_LabelSymbol_t {
    zen_Symbol_t* m_symbol;
};

/**
 * @memberof LabelSymbol
 */
typedef struct zen_LabelSymbol_t zen_LabelSymbol_t;

// Constructor

/**
 * @memberof LabelSymbol
 */
zen_LabelSymbol_t* zen_LabelSymbol_new(zen_ASTNode_t* identifier, zen_Scope_t* enclosingScope);

// Destructor

/**
 * @memberof LabelSymbol
 */
void zen_LabelSymbol_delete(zen_LabelSymbol_t* symbol);

// Symbol

/**
 * @memberof LabelSymbol
 */
zen_Symbol_t* zen_LabelSymbol_getSymbol(zen_LabelSymbol_t* symbol);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_LABEL_SYMBOL_H */