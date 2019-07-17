// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_LOGICAL_AND_EXPRESSION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_LOGICAL_AND_EXPRESSION_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * LogicalAndExpressionContext                                                 *
 *******************************************************************************/

/**
 * @class LogicalAndExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_LogicalAndExpressionContext_t {
    zen_ASTNode_t* m_node;

    /**
     * The expression on the left of the operator.
     */
    zen_ASTNode_t* m_inclusiveOrExpression;

    /**
     * The logicalAnd expression on the right of the operator.
     */
    zen_ASTNode_t* m_logicalAndExpression;
};

/**
 * @memberof LogicalAndExpressionContext
 */
typedef struct zen_LogicalAndExpressionContext_t zen_LogicalAndExpressionContext_t;

// Constructor

/**
 * @memberof LogicalAndExpressionContext
 */
zen_LogicalAndExpressionContext_t* zen_LogicalAndExpressionContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof LogicalAndExpressionContext
 */
void zen_LogicalAndExpressionContext_delete(zen_LogicalAndExpressionContext_t* context);

// Children

/**
 * @memberof LogicalAndExpressionContext
 */
void zen_LogicalAndExpressionContext_getChildren(zen_LogicalAndExpressionContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_LOGICAL_AND_EXPRESSION_CONTEXT_H */