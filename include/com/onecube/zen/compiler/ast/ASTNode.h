#ifndef ZEN_AST_NODE_H
#define ZEN_AST_NODE_H

#include <com/onecube/zen/Configuration.h>
#include <zen/ASTNodeType.h>
#include <zen/collection/ArrayList.h>

/******************************************************************************
 * ContextDestructorFunction                                                  *
 ******************************************************************************/

typedef void (*zen_ContextDestructorFunction_t)(void* context);

/*******************************************************************************
 * EnumerateContextChildrenFunction                                            *
 ******************************************************************************/

typedef void (*zen_EnumerateContextChildrenFunction_t)(void* context, zen_ArrayList_t* children);
 
/******************************************************************************
 * ASTNode																	  *
 ******************************************************************************/

/**
 * Nodes are populated in the parse tree at various depths.
 * A node may be a terminal or a rule. This is identified by
 * the node's type. Moreover, type specific data is abstracted
 * away as payload. For terminals, the payload may be cast to
 * zen_Token_t*. For rules, the payload may be cast to
 * zen_RuleNode_t*.
 *
 * Every node, except the root node, has a parent. The root nodes
 * parent is always NULL. Parent nodes are guaranteed to be rules.
 * Because a terminal always represents a leaf.
 */
struct zen_ASTNode_t {
    zen_ASTNodeType_t m_type;
    void* m_context;
    struct zen_ASTNode_t* m_parent;
    zen_ArrayList_t* m_children;
    zen_ContextDestructorFunction_t m_contextDestructor;
    zen_EnumerateContextChildrenFunction_t m_enumerateContextChildren;
};

typedef struct zen_ASTNode_t zen_ASTNode_t;

zen_ASTNode_t* zen_ASTNode_new(zen_ASTNode_t* parent);
void zen_ASTNode_delete(zen_ASTNode_t* node);
zen_ASTNodeType_t zen_ASTNode_getType(zen_ASTNode_t* node);
zen_ASTNode_t* zen_ASTNode_getParent(zen_ASTNode_t* node);
void* zen_ASTNode_getContext(zen_ASTNode_t* node);
bool zen_ASTNode_isErroneous(zen_ASTNode_t* node);
bool zen_ASTNode_isTerminal(zen_ASTNode_t* node);
bool zen_ASTNode_isRule(zen_ASTNode_t* node);
int32_t zen_ASTNode_getDepth(zen_ASTNode_t* node);
zen_ArrayList_t* zen_ASTNode_getChildren(zen_ASTNode_t* node);

#endif /* ZEN_AST_NODE_H */