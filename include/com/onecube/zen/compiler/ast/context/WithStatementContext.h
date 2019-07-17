// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_WITH_STATEMENT_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_WITH_STATEMENT_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * WithStatementContext                                                        *
 *******************************************************************************/

/**
 * @class WithStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_WithStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expressions;
    zen_ASTNode_t* m_statementSuite;
};

/**
 * @memberof WithStatementContext
 */
typedef struct zen_WithStatementContext_t zen_WithStatementContext_t;

// Constructor

/**
 * @memberof WithStatementContext
 */
zen_WithStatementContext_t* zen_WithStatementContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof WithStatementContext
 */
void zen_WithStatementContext_delete(zen_WithStatementContext_t* context);

// Children

/**
 * @memberof WithStatementContext
 */
void zen_WithStatementContext_getChildren(zen_WithStatementContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_WITH_STATEMENT_CONTEXT_H */