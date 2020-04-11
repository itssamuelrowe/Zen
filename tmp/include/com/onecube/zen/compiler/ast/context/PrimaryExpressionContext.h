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

// Tuesday, July 16, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_PRIMARY_EXPRESSION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_PRIMARY_EXPRESSION_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * PrimaryExpressionContext                                                    *
 *******************************************************************************/

/**
 * @class PrimaryExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_PrimaryExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
};

/**
 * @memberof PrimaryExpressionContext
 */
typedef struct zen_PrimaryExpressionContext_t zen_PrimaryExpressionContext_t;

// Constructor

/**
 * @memberof PrimaryExpressionContext
 */
zen_PrimaryExpressionContext_t* zen_PrimaryExpressionContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof PrimaryExpressionContext
 */
void zen_PrimaryExpressionContext_delete(zen_PrimaryExpressionContext_t* context);

// Children

/**
 * @memberof PrimaryExpressionContext
 */
void zen_PrimaryExpressionContext_getChildren(zen_PrimaryExpressionContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_PRIMARY_EXPRESSION_CONTEXT_H */