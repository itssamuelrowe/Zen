// Tuesday, July 16, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FUNCTION_ARGUMENTS_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FUNCTION_ARGUMENTS_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * FunctionArgumentsContext                                                    *
 *******************************************************************************/

/**
 * @class FunctionArgumentsContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_FunctionArgumentsContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expressions;
};

/**
 * @memberof FunctionArgumentsContext
 */
typedef struct zen_FunctionArgumentsContext_t zen_FunctionArgumentsContext_t;

// Constructor

/**
 * @memberof FunctionArgumentsContext
 */
zen_FunctionArgumentsContext_t* zen_FunctionArgumentsContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof FunctionArgumentsContext
 */
void zen_FunctionArgumentsContext_delete(zen_FunctionArgumentsContext_t* context);

// Children

/**
 * @memberof FunctionArgumentsContext
 */
void zen_FunctionArgumentsContext_getChildren(zen_FunctionArgumentsContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FUNCTION_ARGUMENTS_CONTEXT_H */