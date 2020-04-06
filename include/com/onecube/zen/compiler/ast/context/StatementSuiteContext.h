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

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_STATEMENT_SUITE_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_STATEMENT_SUITE_CONTEXT_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * StatementSuiteContext                                                       *
 *******************************************************************************/

/**
 * @class StatementSuiteContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_StatementSuiteContext_t {
    zen_ASTNode_t* m_node;
    /* In order to help the users read code easily, the simple statements were
     * removed from statement suites. Previously, a statement suite was either
     * a simple statement or a block consisting of a newline at the begining,
     * indentation and dedentation.
     */
    // zen_ASTNode_t* m_simpleStatement;
    jtk_ArrayList_t* m_statements;
    int32_t m_scope;
};

/**
 * @memberof StatementSuiteContext
 */
typedef struct zen_StatementSuiteContext_t zen_StatementSuiteContext_t;

// Constructor

/**
 * @memberof StatementSuiteContext
 */
zen_StatementSuiteContext_t* zen_StatementSuiteContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof StatementSuiteContext
 */
void zen_StatementSuiteContext_delete(zen_StatementSuiteContext_t* context);

// Children

/**
 * @memberof StatementSuiteContext
 */
void zen_StatementSuiteContext_getChildren(zen_StatementSuiteContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_STATEMENT_SUITE_CONTEXT_H */