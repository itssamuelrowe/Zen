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
    zen_ASTNode_t* m_expression;
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