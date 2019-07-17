// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_SYNCHRONIZE_STATEMENT_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_SYNCHRONIZE_STATEMENT_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * SynchronizeStatementContext                                                 *
 *******************************************************************************/

/**
 * @class SynchronizeStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_SynchronizeStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
    zen_ASTNode_t* m_statementSuite;
};

/**
 * @memberof SynchronizeStatementContext
 */
typedef struct zen_SynchronizeStatementContext_t zen_SynchronizeStatementContext_t;

// Constructor

/**
 * @memberof SynchronizeStatementContext
 */
zen_SynchronizeStatementContext_t* zen_SynchronizeStatementContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof SynchronizeStatementContext
 */
void zen_SynchronizeStatementContext_delete(zen_SynchronizeStatementContext_t* context);

// Children

/**
 * @memberof SynchronizeStatementContext
 */
void zen_SynchronizeStatementContext_getChildren(zen_SynchronizeStatementContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_SYNCHRONIZE_STATEMENT_CONTEXT_H */