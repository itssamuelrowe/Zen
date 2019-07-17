// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FOR_STATEMENT_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FOR_STATEMENT_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * ForStatementContext                                                         *
 *******************************************************************************/

/**
 * @class ForStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ForStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_forParameters;
    zen_ASTNode_t* m_expression;
    zen_ASTNode_t* m_statementSuite;
    zen_ASTNode_t* m_elseClause;
};

/** 
 * @memberof ForStatementContext
 */
typedef struct zen_ForStatementContext_t zen_ForStatementContext_t;

// Constructor

/** 
 * @memberof ForStatementContext
 */
zen_ForStatementContext_t* zen_ForStatementContext_new(zen_ASTNode_t* node);

// Destructor

/** 
 * @memberof ForStatementContext
 */
void zen_ForStatementContext_delete(zen_ForStatementContext_t* context);

// Children

/** 
 * @memberof ForStatementContext
 */
void zen_ForStatementContext_getChildren(zen_ForStatementContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FOR_STATEMENT_CONTEXT_H */