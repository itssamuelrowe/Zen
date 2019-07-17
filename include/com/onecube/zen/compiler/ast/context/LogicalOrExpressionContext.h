// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_LOGICAL_OR_EXPRESSION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_LOGICAL_OR_EXPRESSION_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * LogicalOrExpressionContext                                                  *
 *******************************************************************************/

/**
 * @class LogicalOrExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_LogicalOrExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_logicalAndExpression;
    zen_ASTNode_t* m_logicalOrExpression;
};

/**
 * @memberof LogicalOrExpressionContext
 */
typedef struct zen_LogicalOrExpressionContext_t zen_LogicalOrExpressionContext_t;

// Constructor

/**
 * @memberof LogicalOrExpressionContext
 */
zen_LogicalOrExpressionContext_t* zen_LogicalOrExpressionContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof LogicalOrExpressionContext
 */
void zen_LogicalOrExpressionContext_delete(zen_LogicalOrExpressionContext_t* context);

// Children

/**
 * @memberof LogicalOrExpressionContext
 */
void zen_LogicalOrExpressionContext_getChildren(zen_LogicalOrExpressionContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_LOGICAL_OR_EXPRESSION_CONTEXT_H */