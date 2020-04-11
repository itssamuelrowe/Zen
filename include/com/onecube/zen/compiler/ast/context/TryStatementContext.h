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

// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_TRY_STATEMENT_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_TRY_STATEMENT_CONTEXT_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * TryStatementContext                                                         *
 *******************************************************************************/

/**
 * @class TryStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_TryStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_tryClause;
    jtk_ArrayList_t* m_catchClauses; /* <zen_ASTNode_t*> */
    zen_ASTNode_t* m_finallyClause;
};

/**
 * @memberof TryStatementContext
 */
typedef struct zen_TryStatementContext_t zen_TryStatementContext_t;

// Constructor

/**
 * @memberof TryStatementContext
 */
zen_TryStatementContext_t* zen_TryStatementContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof TryStatementContext
 */
void zen_TryStatementContext_delete(zen_TryStatementContext_t* context);

// Children

/**
 * @memberof TryStatementContext
 */
void zen_TryStatementContext_getChildren(zen_TryStatementContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_TRY_STATEMENT_CONTEXT_H */