// Saturday, March 10, 2018

#include <zen/ast/AstAnnotations.h>

/*******************************************************************************
 * AstAnnotations                                                             *
 *******************************************************************************/

zen_AstAnnotations_t* zen_AstAnnotations_new() {
    zen_AstAnnotations_t* annotations = zen_Memory_allocate(zen_AstAnnotations_t, 1);
    annotations->m_map = zen_HashMap_new(zen_PointerObjectAdapter_getInstance(), NULL);

    return annotations;
}

void zen_AstAnnotations_delete(zen_AstAnnotations_t* annotations) {
    jtk_Assert_assertObject(annotations, "The specified annotations is null.");

    zen_HashMap_delete(annotations->m_map);
    zen_Memory_deallocate(annotations);
}

void* zen_AstAnnotations_get(zen_AstAnnotations_t* annotations, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(annotations, "The specified annotations is null.");

    return zen_HashMap_getValue(annotations->m_map, node);
}

void zen_AstAnnotations_put(zen_AstAnnotations_t* annotations, zen_ASTNode_t* node, void* value) {
    jtk_Assert_assertObject(annotations, "The specified annotations is null.");

    zen_HashMap_put(annotations->m_map, (void*)node, value);
}

void zen_AstAnnotations_remove(zen_AstAnnotations_t* annotations, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(annotations, "The specified annotations is null.");

    zen_HashMap_removeKey(annotations->m_map, node);
}