// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_UNARY_EXPRESSION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_UNARY_EXPRESSION_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * UnaryExpressionContext                                                      *
 *******************************************************************************/

/**
 * @class UnaryExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_UnaryExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_unaryOperator;
    zen_ASTNode_t* m_unaryExpression;
    zen_ASTNode_t* m_postfixExpression;
};

/**
 * @memberof UnaryExpressionContext
 */
typedef struct zen_UnaryExpressionContext_t zen_UnaryExpressionContext_t;

// Constructor

/**
 * @memberof UnaryExpressionContext
 */
zen_UnaryExpressionContext_t* zen_UnaryExpressionContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof UnaryExpressionContext
 */
void zen_UnaryExpressionContext_delete(zen_UnaryExpressionContext_t* context);

// Children

/**
 * @memberof UnaryExpressionContext
 */
void zen_UnaryExpressionContext_getChildren(zen_UnaryExpressionContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_UNARY_EXPRESSION_CONTEXT_H */