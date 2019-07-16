// Saturday, March 10, 2018

#ifndef COM_ONECUBE_COM_ONECUBE_COM_ONECUBE_ZEN_COMPILER_COMPILER_COMPILER_ST_AST_AST_NODE_ANNOTATIONS_H
#define COM_ONECUBE_COM_ONECUBE_ZEN_COMPILER_COMPILER_ST_AST_AST_NODE_ANNOTATIONS_H

#include <jtk/collection/map/HashMap.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * ASTAnnotations                                                             *
 *******************************************************************************/

struct zen_ASTAnnotations_t {
    jtk_HashMap_t* m_map;
};

typedef struct zen_ASTAnnotations_t zen_ASTAnnotations_t;

zen_ASTAnnotations_t* zen_ASTAnnotations_new();
void zen_ASTAnnotations_delete(zen_ASTAnnotations_t* annotations);
void* zen_ASTAnnotations_get(zen_ASTAnnotations_t* annotations, zen_ASTNode_t* node);
void zen_ASTAnnotations_put(zen_ASTAnnotations_t* annotations, zen_ASTNode_t* node, void* value);
void zen_ASTAnnotations_remove(zen_ASTAnnotations_t* annotations, zen_ASTNode_t* node);

#endif /* COM_ONECUBE_COM_ONECUBE_ZEN_COMPILER_COMPILER_ST_AST_AST_NODE_ANNOTATIONS_H */