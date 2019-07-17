// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FUNCTION_PARAMETERS_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FUNCTION_PARAMETERS_CONTEXT_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * FunctionParametersContext                                                   *
 *******************************************************************************/

/**
 * @class FunctionParametersContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_FunctionParametersContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_fixedParameters; /* <zen_ASTNode_t*> */
    zen_ASTNode_t* m_variableParameter;
};

/**
 * @memberof FunctionParametersContext
 */
typedef struct zen_FunctionParametersContext_t zen_FunctionParametersContext_t;

// Constructor

/**
 * @memberof FunctionParametersContext
 */
zen_FunctionParametersContext_t* zen_FunctionParametersContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof FunctionParametersContext
 */
void zen_FunctionParametersContext_delete(zen_FunctionParametersContext_t* context);

// Children

/**
 * @memberof FunctionParametersContext
 */
void zen_FunctionParametersContext_getChildren(zen_FunctionParametersContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FUNCTION_PARAMETERS_CONTEXT_H */