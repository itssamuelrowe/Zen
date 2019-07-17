// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ENUMERATE_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ENUMERATE_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * EnumerateContext                                                            *
 *******************************************************************************/

/**
 * @class EnumerateContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EnumerateContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_functionArguments;
};

/**
 * @memberof EnumerateContext
 */
typedef struct zen_EnumerateContext_t zen_EnumerateContext_t;

// Constructor

/**
 * @memberof EnumerateContext
 */
zen_EnumerateContext_t* zen_EnumerateContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof EnumerateContext
 */
void zen_EnumerateContext_delete(zen_EnumerateContext_t* context);

// Children

/**
 * @memberof EnumerateContext
 */
void zen_EnumerateContext_getChildren(zen_EnumerateContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ENUMERATE_CONTEXT_H */