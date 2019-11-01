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

// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ASSERT_STATEMENT_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ASSERT_STATEMENT_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * AssertStatementContext                                                      *
 *******************************************************************************/

/**
 * @class AssertStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AssertStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_conditionExpression;
    zen_ASTNode_t* m_messageExpression;
};

/**
 * @memberof AssertStatementContext
 */
typedef struct zen_AssertStatementContext_t zen_AssertStatementContext_t;

// Constructor

/**
 * @memberof AssertStatementContext
 */
zen_AssertStatementContext_t* zen_AssertStatementContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof AssertStatementContext
 */
void zen_AssertStatementContext_delete(zen_AssertStatementContext_t* context);

// Children

/**
 * @memberof AssertStatementContext
 */
void zen_AssertStatementContext_getChildren(zen_AssertStatementContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ASSERT_STATEMENT_CONTEXT_H */