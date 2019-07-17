// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_RELATIONAL_EXPRESSION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_RELATIONAL_EXPRESSION_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * RelationalExpressionContext                                                 *
 *******************************************************************************/

/**
 * @class RelationalExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_RelationalExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_shiftExpression;
    zen_ASTNode_t* m_relationalOperator;
    zen_ASTNode_t* m_relationalExpression;
};

/**
 * @memberof RelationalExpressionContext
 */
typedef struct zen_RelationalExpressionContext_t zen_RelationalExpressionContext_t;

// Constructor

/**
 * @memberof RelationalExpressionContext
 */
zen_RelationalExpressionContext_t* zen_RelationalExpressionContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof RelationalExpressionContext
 */
void zen_RelationalExpressionContext_delete(zen_RelationalExpressionContext_t* context);

// Children

/**
 * @memberof RelationalExpressionContext
 */
void zen_RelationalExpressionContext_getChildren(zen_RelationalExpressionContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_RELATIONAL_EXPRESSION_CONTEXT_H */