// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ANNOTATION_TYPE_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ANNOTATION_TYPE_CONTEXT_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * AnnotationTypeContext                                                       *
 *******************************************************************************/

/**
 * @class AnnotationTypeContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AnnotationTypeContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_identifiers; /* <zen_ASTNode_t*> */
};

/**
 * @memberof AnnotationTypeContext
 */
typedef struct zen_AnnotationTypeContext_t zen_AnnotationTypeContext_t;

// Constructor

/**
 * @memberof AnnotationTypeContext
 */
zen_AnnotationTypeContext_t* zen_AnnotationTypeContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof AnnotationTypeContext
 */
void zen_AnnotationTypeContext_delete(zen_AnnotationTypeContext_t* context);

// Children

/**
 * @memberof AnnotationTypeContext
 */
void zen_AnnotationTypeContext_getChildren(zen_AnnotationTypeContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ANNOTATION_TYPE_CONTEXT_H */