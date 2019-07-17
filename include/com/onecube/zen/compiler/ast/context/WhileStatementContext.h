// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_WHILE_STATEMENT_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_WHILE_STATEMENT_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * WhileStatementContext                                                       *
 *******************************************************************************/

/**
 * @class WhileStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_WhileStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
    zen_ASTNode_t* m_statementSuite;
    zen_ASTNode_t* m_elseClause;
};

/** 
 * @memberof WhileStatementContext
 */
typedef struct zen_WhileStatementContext_t zen_WhileStatementContext_t;

// Constructor

/** 
 * @memberof WhileStatementContext
 */
zen_WhileStatementContext_t* zen_WhileStatementContext_new(zen_ASTNode_t* node);

// Destructor

/** 
 * @memberof WhileStatementContext
 */
void zen_WhileStatementContext_delete(zen_WhileStatementContext_t* context);

// Children

/** 
 * @memberof WhileStatementContext
 */
void zen_WhileStatementContext_getChildren(zen_WhileStatementContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_WHILE_STATEMENT_CONTEXT_H */