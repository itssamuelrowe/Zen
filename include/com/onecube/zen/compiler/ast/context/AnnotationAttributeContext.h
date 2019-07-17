// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ANNOTATION_ATTRIBUTE_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ANNOTATION_ATTRIBUTE_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * AnnotationAttributeContext                                                  *
 *******************************************************************************/

/**
 * @class AnnotationAttributeContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AnnotationAttributeContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_literal;
};

/**
 * @memberof AnnotationAttributeContext
 */
typedef struct zen_AnnotationAttributeContext_t zen_AnnotationAttributeContext_t;

// Constructor

/**
 * @memberof AnnotationAttributeContext
 */
zen_AnnotationAttributeContext_t* zen_AnnotationAttributeContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof AnnotationAttributeContext
 */
void zen_AnnotationAttributeContext_delete(zen_AnnotationAttributeContext_t* context);

// Children

/**
 * @memberof AnnotationAttributeContext
 */
void zen_AnnotationAttributeContext_getChildren(zen_AnnotationAttributeContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ANNOTATION_ATTRIBUTE_CONTEXT_H */