// Tuesday, July 16, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_NEW_EXPRESSION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_NEW_EXPRESSION_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * NewExpressionContext                                                        *
 *******************************************************************************/

/**
 * @class NewExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_NewExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_typeName;
    zen_ASTNode_t* m_functionArguments;
};

/**
 * @memberof NewExpressionContext
 */
typedef struct zen_NewExpressionContext_t zen_NewExpressionContext_t;

// Constructor

/**
 * @memberof NewExpressionContext
 */
zen_NewExpressionContext_t* zen_NewExpressionContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof NewExpressionContext
 */
void zen_NewExpressionContext_delete(zen_NewExpressionContext_t* context);

// Children

/**
 * @memberof NewExpressionContext
 */
void zen_NewExpressionContext_getChildren(zen_NewExpressionContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_NEW_EXPRESSION_CONTEXT_H */