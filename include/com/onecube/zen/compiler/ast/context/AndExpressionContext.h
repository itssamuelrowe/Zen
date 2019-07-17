// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_AND_EXPRESSION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_AND_EXPRESSION_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * AndExpressionContext                                                        *
 *******************************************************************************/

/**
 * @class AndExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AndExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_equalityExpression;
    zen_ASTNode_t* m_andExpression;
};

/**
 * @memberof AndExpressionContext
 */
typedef struct zen_AndExpressionContext_t zen_AndExpressionContext_t;

// Constructor

/**
 * @memberof AndExpressionContext
 */
zen_AndExpressionContext_t* zen_AndExpressionContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof AndExpressionContext
 */
void zen_AndExpressionContext_delete(zen_AndExpressionContext_t* context);

// Children

/**
 * @memberof AndExpressionContext
 */
void zen_AndExpressionContext_getChildren(zen_AndExpressionContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_AND_EXPRESSION_CONTEXT_H */