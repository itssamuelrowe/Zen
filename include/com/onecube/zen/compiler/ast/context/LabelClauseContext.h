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

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_LABEL_CLAUSE_CONTEXT
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_LABEL_CLAUSE_CONTEXT

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * LabelClauseContext                                                          *
 *******************************************************************************/

// TODO: Use the ASTNode of the identifier instead of LabelClauseContext!

/**
 * @class LabelClauseContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_LabelClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
};

/**
 * @memberof LabelClauseContext
 */
typedef struct zen_LabelClauseContext_t zen_LabelClauseContext_t;

// Constructor

/**
 * @memberof LabelClauseContext
 */
zen_LabelClauseContext_t* zen_LabelClauseContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof LabelClauseContext
 */
void zen_LabelClauseContext_delete(zen_LabelClauseContext_t* context);

// Children

/**
 * @memberof LabelClauseContext
 */
void zen_LabelClauseContext_getChildren(zen_LabelClauseContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_LABEL_CLAUSE_CONTEXT */