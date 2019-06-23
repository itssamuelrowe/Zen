// Sunday, April 28, 2018

#ifndef ZEN_AST_AST_ANNOTATION_H
#define ZEN_AST_AST_ANNOTATION_H

#include <com/onecube/zen/Configuration.h>
#include <zen/ast/AstAnnotationType.h>

/*******************************************************************************
 * AstAnnotation                                                               *
 *******************************************************************************/

/**
 * @author Samuel Rowe
 * @since  zen 1.0
 */
struct zen_AstAnnotation_t {
    zen_AstAnnotationType_t m_type;
    void* m_bundle;
};

typedef struct zen_AstAnnotation_t zen_AstAnnotation_t;

zen_AstAnnotation_t* zen_AstAnnotation_new(zen_AstAnnotationType_t type, void* bundle);
void zen_AstAnnotation_delete(zen_AstAnnotation_t* annotation);

#endif /* ZEN_AST_AST_ANNOTATION_H */