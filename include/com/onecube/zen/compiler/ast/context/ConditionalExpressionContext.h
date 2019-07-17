// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CONDITIONAL_EXPRESSION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CONDITIONAL_EXPRESSION_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * ConditionalExpressionContext                                                *
 *******************************************************************************/

/**
 * @class ConditionalExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ConditionalExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_logicalOrExpression;
    zen_ASTNode_t* m_thenExpression;
    zen_ASTNode_t* m_elseExpression;
};

/**
 * @memberof ConditionalExpressionContext
 */
typedef struct zen_ConditionalExpressionContext_t zen_ConditionalExpressionContext_t;

// Constructor

/**
 * @memberof ConditionalExpressionContext
 */
zen_ConditionalExpressionContext_t* zen_ConditionalExpressionContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof ConditionalExpressionContext
 */
void zen_ConditionalExpressionContext_delete(zen_ConditionalExpressionContext_t* context);

// Children

/**
 * @memberof ConditionalExpressionContext
 */
void zen_ConditionalExpressionContext_getChildren(zen_ConditionalExpressionContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CONDITIONAL_EXPRESSION_CONTEXT_H */