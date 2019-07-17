// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ADDITIVE_EXPRESSION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ADDITIVE_EXPRESSION_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * AdditiveExpressionContext                                                   *
 *******************************************************************************/

/**
 * @class AdditiveExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AdditiveExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_multiplicativeExpression;
    zen_ASTNode_t* m_additiveOperator;
    zen_ASTNode_t* m_additiveExpression;
};

/**
 * @memberof AdditiveExpressionContext
 */
typedef struct zen_AdditiveExpressionContext_t zen_AdditiveExpressionContext_t;

// Constructor

/**
 * @memberof AdditiveExpressionContext
 */
zen_AdditiveExpressionContext_t* zen_AdditiveExpressionContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof AdditiveExpressionContext
 */
void zen_AdditiveExpressionContext_delete(zen_AdditiveExpressionContext_t* context);

// Children

/**
 * @memberof AdditiveExpressionContext
 */
void zen_AdditiveExpressionContext_getChildren(zen_AdditiveExpressionContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ADDITIVE_EXPRESSION_CONTEXT_H */