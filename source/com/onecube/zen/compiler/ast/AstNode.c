#include <stdio.h>
#include <zen/ASTNode.h>
#include <zen/Parser.h>

/******************************************************************************
 * ASTNode																	  *
 ******************************************************************************/

zen_ASTNode_t* zen_ASTNode_new(zen_ASTNode_t* parent) {
    zen_ASTNode_t* node = zen_Memory_allocate(zen_ASTNode_t, 1);
    node->m_type = ZEN_AST_NODE_UNKNOWN;
    node->m_context = NULL;
    node->m_parent = parent;
    node->m_children = NULL;
    node->m_enumerateContextChildren = NULL;
    node->m_contextDestructor = NULL;

    return node;
}

void zen_ASTNode_delete(zen_ASTNode_t* node) {
    if (zen_ASTNode_isRule(node)) {
        zen_ArrayList_t* children = zen_ASTNode_getChildren(node);
        int32_t size = zen_ArrayList_getSize(children);
        int32_t i;
        for (i = 0; i < size; i++) {
            zen_ASTNode_t* child = (zen_ASTNode_t*)zen_ArrayList_get(children, i);
            zen_ASTNode_delete(child);
        }
        node->m_contextDestructor(node->m_context);
    }
    zen_Memory_deallocate(node);
}

zen_ASTNodeType_t zen_ASTNode_getType(zen_ASTNode_t* node) {
    jtk_Assert_assertObject(node, "The specified node is null.");
    return node->m_type;
}

zen_ASTNode_t* zen_ASTNode_getParent(zen_ASTNode_t* node) {
    jtk_Assert_assertObject(node, "The specified node is null.");
    return node->m_parent;
}

void* zen_ASTNode_getContext(zen_ASTNode_t* node) {
    jtk_Assert_assertObject(node, "The specified node is null.");
    return node->m_context;
}

bool zen_ASTNode_isErroneous(zen_ASTNode_t* node) {
    jtk_Assert_assertObject(node, "The specified node is null.");
#warning "[TODO] zen_ASTNode_isErroneous() is not implemented."
    fflush(stdout);
    return false;
    // return node->m_erroneous;
}

bool zen_ASTNode_isTerminal(zen_ASTNode_t* node) {
    jtk_Assert_assertObject(node, "The specified node is null.");
    return node->m_type == ZEN_AST_NODE_TERMINAL;
}

bool zen_ASTNode_isRule(zen_ASTNode_t* node) {
    jtk_Assert_assertObject(node, "The specified node is null.");
    return /* !node->m_erroneous && */ (node->m_type != ZEN_AST_NODE_TERMINAL);
}

int32_t zen_ASTNode_getDepth(zen_ASTNode_t* node) {
    jtk_Assert_assertObject(node, "The specified node is null.");

    int32_t depth = 0;
    zen_ASTNode_t* currentNode = node;
    while (currentNode != NULL) {
        depth++;
        currentNode = currentNode->m_parent;
    }
    return depth;
}

/* The returned array list should not be modified externally. */
zen_ArrayList_t* zen_ASTNode_getChildren(zen_ASTNode_t* node) {
    jtk_Assert_assertObject(node, "The specified node is null.");

    if (node->m_children == NULL) {
        node->m_children = zen_ArrayList_new();
        node->m_enumerateContextChildren(node->m_context, node->m_children);
    }
    return node->m_children;
}