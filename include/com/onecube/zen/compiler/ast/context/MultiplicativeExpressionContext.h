// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_MULTIPLICATIVE_EXPRESSION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_MULTIPLICATIVE_EXPRESSION_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * MultiplicativeExpressionContext                                             *
 *******************************************************************************/

/**
 * @class MultiplicativeExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_MultiplicativeExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_unaryExpression;
    zen_ASTNode_t* m_multiplicativeOperator;
    zen_ASTNode_t* m_multiplicativeExpression;
};

/**
 * @memberof MultiplicativeExpressionContext
 */
typedef struct zen_MultiplicativeExpressionContext_t zen_MultiplicativeExpressionContext_t;

// Constructor

zen_MultiplicativeExpressionContext_t* zen_MultiplicativeExpressionContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof MultiplicativeExpressionContext
 */
void zen_MultiplicativeExpressionContext_delete(zen_MultiplicativeExpressionContext_t* context);

// Children

/**
 * @memberof MultiplicativeExpressionContext
 */
void zen_MultiplicativeExpressionContext_getChildren(zen_MultiplicativeExpressionContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_MULTIPLICATIVE_EXPRESSION_CONTEXT_H */