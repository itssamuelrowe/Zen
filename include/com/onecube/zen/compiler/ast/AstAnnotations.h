// Saturday, March 10, 2018

#ifndef ZEN_ST_AST_AST_NODE_ANNOTATIONS_H
#define ZEN_ST_AST_AST_NODE_ANNOTATIONS_H

#include <zen/ASTNode.h>
#include <zen/collection/HashMap.h>

/*******************************************************************************
 * AstAnnotations                                                             *
 *******************************************************************************/

struct zen_AstAnnotations_t {
    zen_HashMap_t* m_map;
};

typedef struct zen_AstAnnotations_t zen_AstAnnotations_t;

zen_AstAnnotations_t* zen_AstAnnotations_new();
void zen_AstAnnotations_delete(zen_AstAnnotations_t* annotations);
void* zen_AstAnnotations_get(zen_AstAnnotations_t* annotations, zen_ASTNode_t* node);
void zen_AstAnnotations_put(zen_AstAnnotations_t* annotations, zen_ASTNode_t* node, void* value);
void zen_AstAnnotations_remove(zen_AstAnnotations_t* annotations, zen_ASTNode_t* node);

#endif /* ZEN_ST_AST_AST_NODE_ANNOTATIONS_H */