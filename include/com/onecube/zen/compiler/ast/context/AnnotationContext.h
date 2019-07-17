// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ANNOTATION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ANNOTATION_CONTEXT_H

#include <jtk/collection/list/ArrayList.h>
#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * AnnotationContext                                                           *
 *******************************************************************************/

/**
 * @class AnnotationContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AnnotationContext_t {
    zen_ASTNode_t* m_node;
	zen_ASTNode_t* m_annotationType;
	jtk_ArrayList_t* m_annotationAttributes; /* <zen_ASTNode_t*> */
};

/**
 * @memberof AnnotationContext
 */
typedef struct zen_AnnotationContext_t zen_AnnotationContext_t;

// Constructor

/**
 * @memberof AnnotationContext
 */
zen_AnnotationContext_t* zen_AnnotationContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof AnnotationContext
 */
void zen_AnnotationContext_delete(zen_AnnotationContext_t* context);

// Children

/**
 * @memberof AnnotationContext
 */
void zen_AnnotationContext_getChildren(zen_AnnotationContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ANNOTATION_CONTEXT_H */