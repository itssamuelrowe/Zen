// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ITERATIVE_STATEMENT_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ITERATIVE_STATEMENT_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * IterativeStatementContext                                                   *
 *******************************************************************************/

/**
 * @class IterativeStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_IterativeStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_labelClause;
    zen_ASTNode_t* m_statement;
};

/**
 * @memberof IterativeStatementContext
 */
typedef struct zen_IterativeStatementContext_t zen_IterativeStatementContext_t;

// Constructor

/**
 * @memberof IterativeStatementContext
 */
zen_IterativeStatementContext_t* zen_IterativeStatementContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof IterativeStatementContext
 */
void zen_IterativeStatementContext_delete(zen_IterativeStatementContext_t* context);

// Children

/**
 * @memberof IterativeStatementContext
 */
void zen_IterativeStatementContext_getChildren(zen_IterativeStatementContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ITERATIVE_STATEMENT_CONTEXT_H */