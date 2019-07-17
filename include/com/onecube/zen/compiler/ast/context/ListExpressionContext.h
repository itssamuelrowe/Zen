// Tuesday, July 16, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_LIST_EXPRESSION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_LIST_EXPRESSION_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * ListExpressionContext                                                       *
 *******************************************************************************/

/**
 * @class ListExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ListExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expressions;
};

typedef struct zen_ListExpressionContext_t zen_ListExpressionContext_t;

zen_ListExpressionContext_t* zen_ListExpressionContext_new(zen_ASTNode_t* node);
void zen_ListExpressionContext_delete(zen_ListExpressionContext_t* context);
void zen_ListExpressionContext_getChildren(zen_ListExpressionContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_LIST_EXPRESSION_CONTEXT_H */