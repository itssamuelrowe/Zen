// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ANNOTATED_COMPONENT_DECLARATION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ANNOTATED_COMPONENT_DECLARATION_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * AnnotatedComponentDeclarationContext                                        *
 *******************************************************************************/

/**
 * @class AnnotatedComponentDeclarationContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AnnotatedComponentDeclarationContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_annotations;
    zen_ASTNode_t* m_componentDeclaration;
};

/**
 * @memberof AnnotatedComponentDeclarationContext
 */
typedef struct zen_AnnotatedComponentDeclarationContext_t zen_AnnotatedComponentDeclarationContext_t;

// Constructor

/**
 * @memberof AnnotatedComponentDeclarationContext
 */
zen_AnnotatedComponentDeclarationContext_t* zen_AnnotatedComponentDeclarationContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof AnnotatedComponentDeclarationContext
 */
void zen_AnnotatedComponentDeclarationContext_delete(zen_AnnotatedComponentDeclarationContext_t* context);

// Children

/**
 * @memberof AnnotatedComponentDeclarationContext
 */
void zen_AnnotatedComponentDeclarationContext_getChildren(zen_AnnotatedComponentDeclarationContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ANNOTATED_COMPONENT_DECLARATION_CONTEXT_H */