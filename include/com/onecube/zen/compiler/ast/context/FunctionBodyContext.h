// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FUNCTION_BODY_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FUNCTION_BODY_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * FunctionBodyContext                                                         *
 *******************************************************************************/

/**
 * @class FunctionBodyContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_FunctionBodyContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_statementSuite;
};

/**
 * @memberof FunctionBodyContext
 */
typedef struct zen_FunctionBodyContext_t zen_FunctionBodyContext_t;

// Constructor

/**
 * @memberof FunctionBodyContext
 */
zen_FunctionBodyContext_t* zen_FunctionBodyContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof FunctionBodyContext
 */
void zen_FunctionBodyContext_delete(zen_FunctionBodyContext_t* context);

// Children

/**
 * @memberof FunctionBodyContext
 */
void zen_FunctionBodyContext_getChildren(zen_FunctionBodyContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FUNCTION_BODY_CONTEXT_H */