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

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_CLASS_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_CLASS_SYMBOL_H

#include <com/onecube/zen/compiler/symbol-table/Symbol.h>

/*******************************************************************************
 * MemberClassSymbol                                                           *
 *******************************************************************************/

/**
 * @class MemberClassSymbol
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_MemberClassSymbol_t {
    zen_Symbol_t* m_symbol;
};

/**
 * @memberof MemberClassSymbol
 */
typedef struct zen_MemberClassSymbol_t zen_MemberClassSymbol_t;

// Constructor

/**
 * @memberof MemberClassSymbol
 */
zen_MemberClassSymbol_t* zen_MemberClassSymbol_new(zen_ASTNode_t* identifier);

// Destructor

/**
 * @memberof MemberClassSymbol
 */
void zen_MemberClassSymbol_delete(zen_MemberClassSymbol_t* symbol);

// Symbol

/**
 * @memberof MemberClassSymbol
 */
zen_Symbol_t* zen_MemberClassSymbol_getSymbol(zen_MemberClassSymbol_t* symbol);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_CLASS_SYMBOL_H */