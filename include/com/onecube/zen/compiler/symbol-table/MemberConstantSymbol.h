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

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_CONSTANT_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_CONSTANT_SYMBOL_H

#include <com/onecube/zen/compiler/symbol-table/Symbol.h>

/*******************************************************************************
 * MemberConstantSymbol                                                     *
 *******************************************************************************/

/**
 * @class MemberConstantSymbol
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_MemberConstantSymbol_t {
    zen_Symbol_t* m_symbol;
};

/**
 * @memberof MemberConstantSymbol
 */
typedef struct zen_MemberConstantSymbol_t zen_MemberConstantSymbol_t;

// Constructor

/**
 * @memberof MemberConstantSymbol
 */
zen_MemberConstantSymbol_t* zen_MemberConstantSymbol_new(zen_ASTNode_t* identifier,
    jtk_ArrayList_t* modifiers);

// Destructor

/**
 * @memberof MemberConstantSymbol
 */
void zen_MemberConstantSymbol_delete(zen_MemberConstantSymbol_t* symbol);

// Symbol

/**
 * @memberof MemberConstantSymbol
 */
zen_Symbol_t* zen_MemberConstantSymbol_getSymbol(zen_MemberConstantSymbol_t* symbol);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_CONSTANT_SYMBOL_H */