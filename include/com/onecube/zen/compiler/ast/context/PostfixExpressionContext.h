// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_POSTFIX_EXPRESSION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_POSTFIX_EXPRESSION_CONTEXT_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * PostfixExpressionContext                                                    *
 *******************************************************************************/

/**
 * @class PostfixExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_PostfixExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_primaryExpression;
    jtk_ArrayList_t* m_postfixParts; /* <zen_ASTNode_t*> */
};

/**
 * @memberof PostfixExpressionContext
 */
typedef struct zen_PostfixExpressionContext_t zen_PostfixExpressionContext_t;

// Constructor

/**
 * @memberof PostfixExpressionContext
 */
zen_PostfixExpressionContext_t* zen_PostfixExpressionContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof PostfixExpressionContext
 */
void zen_PostfixExpressionContext_delete(zen_PostfixExpressionContext_t* context);

// Children

/**
 * @memberof PostfixExpressionContext
 */
void zen_PostfixExpressionContext_getChildren(zen_PostfixExpressionContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_POSTFIX_EXPRESSION_CONTEXT_H */