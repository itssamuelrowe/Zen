// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_IMPORT_DECLARATION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_IMPORT_DECLARATION_CONTEXT_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * ImportDeclarationContext                                                    *
 *******************************************************************************/

/**
 * @class ImportDeclarationContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ImportDeclarationContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_identifiers; /* <zen_ASTNode_t*> */
    bool m_wildcard;
};

/**
 * @memberof ImportDeclarationContext
 */
typedef struct zen_ImportDeclarationContext_t zen_ImportDeclarationContext_t;

// Constructor

/**
 * @memberof ImportDeclarationContext
 */
zen_ImportDeclarationContext_t* zen_ImportDeclarationContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof ImportDeclarationContext
 */
void zen_ImportDeclarationContext_delete(zen_ImportDeclarationContext_t* context);

// Children

/**
 * @memberof ImportDeclarationContext
 */
void zen_ImportDeclarationContext_getChildren(zen_ImportDeclarationContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_IMPORT_DECLARATION_CONTEXT_H */