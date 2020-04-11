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

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_LOGICAL_AND_EXPRESSION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_LOGICAL_AND_EXPRESSION_CONTEXT_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * LogicalAndExpressionContext                                                 *
 *******************************************************************************/

/**
 * @class LogicalAndExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_LogicalAndExpressionContext_t {
    zen_ASTNode_t* m_node;

    /**
     * The inclusiveOrExpression node on the left side of the operator.
     */
    zen_ASTNode_t* m_inclusiveOrExpression;

    /**
     * The inclusiveOrExpression nodes on the right side of the operator.
     */
    jtk_ArrayList_t* m_inclusiveOrExpressions;
};

/**
 * @memberof LogicalAndExpressionContext
 */
typedef struct zen_LogicalAndExpressionContext_t zen_LogicalAndExpressionContext_t;

// Constructor

/**
 * @memberof LogicalAndExpressionContext
 */
zen_LogicalAndExpressionContext_t* zen_LogicalAndExpressionContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof LogicalAndExpressionContext
 */
void zen_LogicalAndExpressionContext_delete(zen_LogicalAndExpressionContext_t* context);

// Children

/**
 * @memberof LogicalAndExpressionContext
 */
void zen_LogicalAndExpressionContext_getChildren(zen_LogicalAndExpressionContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_LOGICAL_AND_EXPRESSION_CONTEXT_H */