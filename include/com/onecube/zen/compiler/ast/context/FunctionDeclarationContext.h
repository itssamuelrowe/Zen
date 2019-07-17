// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FUNCTION_DECLARATION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FUNCTION_DECLARATION_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * FunctionDeclarationContext                                                  *
 *******************************************************************************/

/**
 * @class FunctionDeclarationContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_FunctionDeclarationContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_functionParameters;
    zen_ASTNode_t* m_functionBody;
};

/**
 * @memberof FunctionDeclarationContext
 */
typedef struct zen_FunctionDeclarationContext_t zen_FunctionDeclarationContext_t;

// Constructor

/**
 * @memberof FunctionDeclarationContext
 */
zen_FunctionDeclarationContext_t* zen_FunctionDeclarationContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof FunctionDeclarationContext
 */
void zen_FunctionDeclarationContext_delete(zen_FunctionDeclarationContext_t* context);

// Children

/**
 * @memberof FunctionDeclarationContext
 */
void zen_FunctionDeclarationContext_getChildren(zen_FunctionDeclarationContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FUNCTION_DECLARATION_CONTEXT_H */