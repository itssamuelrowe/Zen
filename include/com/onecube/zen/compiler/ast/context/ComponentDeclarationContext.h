// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_COMPOUND_DECLARATION_STATEMENT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_COMPOUND_DECLARATION_STATEMENT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * ComponentDeclarationContext                                                 *
 *******************************************************************************/

/**
 * @class ComponentDeclarationContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ComponentDeclarationContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_component;
};

/**
 * @memberof ComponentDeclarationContext
 */
typedef struct zen_ComponentDeclarationContext_t zen_ComponentDeclarationContext_t;

// Constructor

/**
 * @memberof ComponentDeclarationContext
 */
zen_ComponentDeclarationContext_t* zen_ComponentDeclarationContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof ComponentDeclarationContext
 */
void zen_ComponentDeclarationContext_delete(zen_ComponentDeclarationContext_t* context);

// Children

/**
 * @memberof ComponentDeclarationContext
 */
void zen_ComponentDeclarationContext_getChildren(zen_ComponentDeclarationContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_COMPOUND_DECLARATION_STATEMENT_H */