// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_EXPRESSION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_EXPRESSION_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * ExpressionContext                                                           *
 *******************************************************************************/

/**
 * @class ExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_assignmentExpression;
};

/**
 * @memberof ExpressionContext
 */
typedef struct zen_ExpressionContext_t zen_ExpressionContext_t;

// Constructor

/**
 * @memberof ExpressionContext
 */
zen_ExpressionContext_t* zen_ExpressionContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof ExpressionContext
 */
void zen_ExpressionContext_delete(zen_ExpressionContext_t* context);

// Children

/**
 * @memberof ExpressionContext
 */
void zen_ExpressionContext_getChildren(zen_ExpressionContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_EXPRESSION_CONTEXT_H */