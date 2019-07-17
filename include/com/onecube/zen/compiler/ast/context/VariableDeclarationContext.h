// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_VARIABLE_DECLARATION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_VARIABLE_DECLARATION_CONTEXT_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * VariableDeclarationContext                                                  *
 *******************************************************************************/

/**
 * @class VariableDeclarationContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_VariableDeclarationContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_variableDeclarators;
};

/**
 * @memberof VariableDeclarationContext
 */
typedef struct zen_VariableDeclarationContext_t zen_VariableDeclarationContext_t;

// Constructor

zen_VariableDeclarationContext_t* zen_VariableDeclarationContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof VariableDeclarationContext
 */
void zen_VariableDeclarationContext_delete(zen_VariableDeclarationContext_t* context);

// Children

/**
 * @memberof VariableDeclarationContext
 */
void zen_VariableDeclarationContext_getChildren(zen_VariableDeclarationContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_VARIABLE_DECLARATION_CONTEXT_H */