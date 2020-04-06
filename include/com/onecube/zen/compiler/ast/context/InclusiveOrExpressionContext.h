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

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_INCLUSIVE_OR_EXPRESSION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_INCLUSIVE_OR_EXPRESSION_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * InclusiveOrExpressionContext                                                *
 *******************************************************************************/

/**
 * @class InclusiveOrExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_InclusiveOrExpressionContext_t {
    zen_ASTNode_t* m_node;
    /**
     * The expression on the left of the operator.
     */
    zen_ASTNode_t* m_exclusiveOrExpression;

    /**
     * The list of exclusiveOrExpressions on the right of the operator.
     */
    jtk_ArrayList_t* m_exclusiveOrExpressions;
};

/**
 * @memberof InclusiveOrExpressionContext
 */
typedef struct zen_InclusiveOrExpressionContext_t zen_InclusiveOrExpressionContext_t;

// Constructor

/**
 * @memberof InclusiveOrExpressionContext
 */
zen_InclusiveOrExpressionContext_t* zen_InclusiveOrExpressionContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof InclusiveOrExpressionContext
 */
void zen_InclusiveOrExpressionContext_delete(zen_InclusiveOrExpressionContext_t* context);

// Children

/**
 * @memberof InclusiveOrExpressionContext
 */
void zen_InclusiveOrExpressionContext_getChildren(zen_InclusiveOrExpressionContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_INCLUSIVE_OR_EXPRESSION_CONTEXT_H */