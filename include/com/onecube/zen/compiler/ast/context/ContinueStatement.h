// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CONTINUE_STATEMENT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CONTINUE_STATEMENT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * ContinueStatementContext                                                    *
 *******************************************************************************/

/**
 * @class ContinueStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ContinueStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
};

/**
 * @memberof ContinueStatementContext
 */
typedef struct zen_ContinueStatementContext_t zen_ContinueStatementContext_t;

// Constructor

/**
 * @memberof ContinueStatementContext
 */
zen_ContinueStatementContext_t* zen_ContinueStatementContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof ContinueStatementContext
 */
void zen_ContinueStatementContext_delete(zen_ContinueStatementContext_t* context);

// Children

/**
 * @memberof ContinueStatementContext
 */
void zen_ContinueStatementContext_getChildren(zen_ContinueStatementContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CONTINUE_STATEMENT_H */