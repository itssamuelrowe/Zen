#ifndef COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_NODE_H
#define COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_NODE_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNodeType.h>

/******************************************************************************
 * ContextDestructorFunction                                                  *
 ******************************************************************************/

typedef void (*zen_ContextDestructorFunction_t)(void* context);

/*******************************************************************************
 * EnumerateContextChildrenFunction                                            *
 ******************************************************************************/

typedef void (*zen_EnumerateContextChildrenFunction_t)(void* context, jtk_ArrayList_t* children);
 
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
 *
 * @class ASTNode
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ASTNode_t {
    zen_ASTNodeType_t m_type;
    void* m_context;
    struct zen_ASTNode_t* m_parent;
    jtk_ArrayList_t* m_children;
    zen_ContextDestructorFunction_t m_contextDestructor;
    zen_EnumerateContextChildrenFunction_t m_enumerateContextChildren;
};

/**
 * @memberof ASTNode
 */
typedef struct zen_ASTNode_t zen_ASTNode_t;

// Constructor

/**
 * @memberof ASTNode
 */
zen_ASTNode_t* zen_ASTNode_new(zen_ASTNode_t* parent);

// Destructor

/**
 * @memberof ASTNode
 */
void zen_ASTNode_delete(zen_ASTNode_t* node);

// Children

/**
 * @memberof ASTNode
 */
jtk_ArrayList_t* zen_ASTNode_getChildren(zen_ASTNode_t* node);

// Context

/**
 * @memberof ASTNode
 */
void* zen_ASTNode_getContext(zen_ASTNode_t* node);

// Depth

/**
 * @memberof ASTNode
 */
int32_t zen_ASTNode_getDepth(zen_ASTNode_t* node);

// Error

/**
 * @memberof ASTNode
 */
bool zen_ASTNode_isErroneous(zen_ASTNode_t* node);

// Parent

/**
 * @memberof ASTNode
 */
zen_ASTNode_t* zen_ASTNode_getParent(zen_ASTNode_t* node);

// Rule

/**
 * @memberof ASTNode
 */
bool zen_ASTNode_isRule(zen_ASTNode_t* node);

// Terminal

/**
 * @memberof ASTNode
 */
bool zen_ASTNode_isTerminal(zen_ASTNode_t* node);

// Type

/**
 * @memberof ASTNode
 */
zen_ASTNodeType_t zen_ASTNode_getType(zen_ASTNode_t* node);

#endif /* COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_NODE_H */