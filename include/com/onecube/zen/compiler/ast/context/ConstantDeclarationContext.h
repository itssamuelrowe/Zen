// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CONSTANT_DECLARATION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CONSTANT_DECLARATION_CONTEXT_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * ConstantDeclarationContext                                                  *
 *******************************************************************************/

/**
 * @class ConstantDeclarationContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ConstantDeclarationContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_constantDeclarators;
};

/** 
 * @membeof ConstantDeclarationContext
 */
typedef struct zen_ConstantDeclarationContext_t zen_ConstantDeclarationContext_t;

// Constructor

/** 
 * @membeof ConstantDeclarationContext
 */
zen_ConstantDeclarationContext_t* zen_ConstantDeclarationContext_new(zen_ASTNode_t* node);

// Destructor

/** 
 * @membeof ConstantDeclarationContext
 */
void zen_ConstantDeclarationContext_delete(zen_ConstantDeclarationContext_t* context);

// Children

/** 
 * @membeof ConstantDeclarationContext
 */
void zen_ConstantDeclarationContext_getChildren(zen_ConstantDeclarationContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CONSTANT_DECLARATION_CONTEXT_H */