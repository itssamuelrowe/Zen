// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ASSIGNMENT_EXPRESSION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ASSIGNMENT_EXPRESSION_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * AssignmentExpressionContext                                                 *
 *******************************************************************************/

/**
 * @class AssignmentExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AssignmentExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_conditionalExpression;
    zen_ASTNode_t* m_assignmentOperator;
    zen_ASTNode_t* m_assignmentExpression;
};

/**
 * @memberof AssignmentExpressionContext
 */
typedef struct zen_AssignmentExpressionContext_t zen_AssignmentExpressionContext_t;

// Constructor

/**
 * @memberof AssignmentExpressionContext
 */
zen_AssignmentExpressionContext_t* zen_AssignmentExpressionContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof AssignmentExpressionContext
 */
void zen_AssignmentExpressionContext_delete(zen_AssignmentExpressionContext_t* context);

// Children

/**
 * @memberof AssignmentExpressionContext
 */
void zen_AssignmentExpressionContext_getChildren(zen_AssignmentExpressionContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ASSIGNMENT_EXPRESSION_CONTEXT_H */