// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_SIMPLE_STATEMENT_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_SIMPLE_STATEMENT_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * SimpleStatementContext                                                      *
 *******************************************************************************/

/**
 * @class SimpleStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_SimpleStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_statement;
};

/**
 * @memberof SimpleStatementContext
 */
typedef struct zen_SimpleStatementContext_t zen_SimpleStatementContext_t;

// Constructor

/**
 * @memberof SimpleStatementContext
 */
zen_SimpleStatementContext_t* zen_SimpleStatementContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof SimpleStatementContext
 */
void zen_SimpleStatementContext_delete(zen_SimpleStatementContext_t* context);

// Children

/**
 * @memberof SimpleStatementContext
 */
void zen_SimpleStatementContext_getChildren(zen_SimpleStatementContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_SIMPLE_STATEMENT_CONTEXT_H */