// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_EMPTY_STATEMENT_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_EMPTY_STATEMENT_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * EmptyStatementContext                                                       *
 *******************************************************************************/

/**
 * @class EmptyStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EmptyStatementContext_t {
    zen_ASTNode_t* m_node;
};

/**
 * @memberof EmptyStatementContext
 */
typedef struct zen_EmptyStatementContext_t zen_EmptyStatementContext_t;

// Constructor

/**
 * @memberof EmptyStatementContext
 */
zen_EmptyStatementContext_t* zen_EmptyStatementContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof EmptyStatementContext
 */
void zen_EmptyStatementContext_delete(zen_EmptyStatementContext_t* context);

// Children

/**
 * @memberof EmptyStatementContext
 */
void zen_EmptyStatementContext_getChildren(zen_EmptyStatementContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_EMPTY_STATEMENT_CONTEXT_H */