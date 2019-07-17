// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_EQUALITY_EXPRESSION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_EQUALITY_EXPRESSION_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * EqualityExpressionContext                                                   *
 *******************************************************************************/

/**
 * @class EqualityExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EqualityExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_relationalExpression;
    zen_ASTNode_t* m_equalityOperator;
    zen_ASTNode_t* m_equalityExpression;
};

/**
 * @memberof EqualityExpressionContext
 */
typedef struct zen_EqualityExpressionContext_t zen_EqualityExpressionContext_t;

// Constructor

/**
 * @memberof EqualityExpressionContext
 */
zen_EqualityExpressionContext_t* zen_EqualityExpressionContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof EqualityExpressionContext
 */
void zen_EqualityExpressionContext_delete(zen_EqualityExpressionContext_t* context);

// Children

/**
 * @memberof EqualityExpressionContext
 */
void zen_EqualityExpressionContext_getChildren(zen_EqualityExpressionContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_EQUALITY_EXPRESSION_CONTEXT_H */