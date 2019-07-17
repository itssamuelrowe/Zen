// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_EXCLUSIVE_OR_EXPRESSION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_EXCLUSIVE_OR_EXPRESSION_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * ExclusiveOrExpressionContext                                                *
 *******************************************************************************/

/**
 * @class ExclusiveOrExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ExclusiveOrExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_andExpression;
    zen_ASTNode_t* m_exclusiveOrExpression;
};

/**
 * @memberof ExclusiveOrExpressionContext
 */
typedef struct zen_ExclusiveOrExpressionContext_t zen_ExclusiveOrExpressionContext_t;

// Constructor

/**
 * @memberof ExclusiveOrExpressionContext
 */
zen_ExclusiveOrExpressionContext_t* zen_ExclusiveOrExpressionContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof ExclusiveOrExpressionContext
 */
void zen_ExclusiveOrExpressionContext_delete(zen_ExclusiveOrExpressionContext_t* context);

// Children

/**
 * @memberof ExclusiveOrExpressionContext
 */
void zen_ExclusiveOrExpressionContext_getChildren(zen_ExclusiveOrExpressionContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_EXCLUSIVE_OR_EXPRESSION_CONTEXT_H */