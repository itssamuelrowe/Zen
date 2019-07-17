// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_BREAK_STATEMENT_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_BREAK_STATEMENT_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * BreakStatementContext                                                       *
 *******************************************************************************/

/**
 * @class BreakStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_BreakStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
};

/**
 * @memberof BreakStatementContext
 */
typedef struct zen_BreakStatementContext_t zen_BreakStatementContext_t;

// Constructor

/**
 * @memberof BreakStatementContext
 */
zen_BreakStatementContext_t* zen_BreakStatementContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof BreakStatementContext
 */
void zen_BreakStatementContext_delete(zen_BreakStatementContext_t* context);

// Children

/**
 * @memberof BreakStatementContext
 */
void zen_BreakStatementContext_getChildren(zen_BreakStatementContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_BREAK_STATEMENT_CONTEXT_H */