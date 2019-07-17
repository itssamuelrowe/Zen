// Tuesday, July 16, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_POSTFIX_OPERATOR_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_POSTFIX_OPERATOR_CONTEXT_H
 
#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * PostfixOperatorContext                                                      *
 *******************************************************************************/

/**
 * @class PostfixOperatorContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_PostfixOperatorContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_postfixOperator;
};

/**
 * @memberof PostfixOperatorContext
 */
typedef struct zen_PostfixOperatorContext_t zen_PostfixOperatorContext_t;

/**
 * @memberof PostfixOperatorContext
 */
zen_PostfixOperatorContext_t* zen_PostfixOperatorContext_new(zen_ASTNode_t* node);

/**
 * @memberof PostfixOperatorContext
 */
void zen_PostfixOperatorContext_delete(zen_PostfixOperatorContext_t* context);

/**
 * @memberof PostfixOperatorContext
 */
void zen_PostfixOperatorContext_getChildren(zen_PostfixOperatorContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_POSTFIX_OPERATOR_CONTEXT_H */