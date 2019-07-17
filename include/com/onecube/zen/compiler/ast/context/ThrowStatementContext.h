// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_THROW_STATEMENT_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_THROW_STATEMENT_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * ThrowStatementContext                                                       *
 *******************************************************************************/

/**
 * @class ThrowStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ThrowStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
};

/**
 * @memberof ThrowStatementContext
 */
typedef struct zen_ThrowStatementContext_t zen_ThrowStatementContext_t;

// Constructor

/**
 * @memberof ThrowStatementContext
 */
zen_ThrowStatementContext_t* zen_ThrowStatementContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof ThrowStatementContext
 */
void zen_ThrowStatementContext_delete(zen_ThrowStatementContext_t* context);

// Children

/**
 * @memberof ThrowStatementContext
 */
void zen_ThrowStatementContext_getChildren(zen_ThrowStatementContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_THROW_STATEMENT_CONTEXT_H */