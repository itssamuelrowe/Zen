// Sunday, April 29, 2018

#include <com/onecube/zen/compiler/ast/AstAnnotation.h>

/*******************************************************************************
 * AstAnnotation                                                               *
 *******************************************************************************/

zen_AstAnnotation_t* zen_AstAnnotation_new(zen_AstAnnotationType_t type, void* bundle) {
    zen_AstAnnotation_t* annotation = zen_Memory_allocate(zen_AstAnnotation_t, 1);
    annotation->m_type = type;
    annotation->m_bundle = bundle;

    return annotation;
}

void zen_AstAnnotation_delete(zen_AstAnnotation_t* annotation) {
    jtk_Assert_assertObject(annotation, "The specified annotation is null.");

    zen_Memory_deallocate(annotation);
}

zen_AstAnnotationType_t zen_AstAnnotation_getType(zen_AstAnnotation_t* annotation) {
    jtk_Assert_assertObject(annotation, "The specified annotation is null.");

    return annotation->m_type;
}