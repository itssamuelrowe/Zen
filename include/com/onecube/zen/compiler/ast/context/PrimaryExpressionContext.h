// Tuesday, July 16, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_PRIMARY_EXPRESSION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_PRIMARY_EXPRESSION_CONTEXT_H
 
/*******************************************************************************
 * PrimaryExpressionContext                                                    *
 *******************************************************************************/

/**
 * @class PrimaryExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_PrimaryExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
};

/**
 * @memberof PrimaryExpressionContext
 */
typedef struct zen_PrimaryExpressionContext_t zen_PrimaryExpressionContext_t;

// Constructor

/**
 * @memberof PrimaryExpressionContext
 */
zen_PrimaryExpressionContext_t* zen_PrimaryExpressionContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof PrimaryExpressionContext
 */
void zen_PrimaryExpressionContext_delete(zen_PrimaryExpressionContext_t* context);

// Children

/**
 * @memberof PrimaryExpressionContext
 */
void zen_PrimaryExpressionContext_getChildren(zen_PrimaryExpressionContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_PRIMARY_EXPRESSION_CONTEXT_H */