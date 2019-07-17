// Tuesday, July 16, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_MEMBER_ACCESS_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_MEMBER_ACCESS_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * MemberAccessContext                                                         *
 *******************************************************************************/

/**
 * @class MemberAccessContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_MemberAccessContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
};

/**
 * @memberof MemberAccessContext
 */
typedef struct zen_MemberAccessContext_t zen_MemberAccessContext_t;

// Constructor

/**
 * @memberof MemberAccessContext
 */
zen_MemberAccessContext_t* zen_MemberAccessContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof MemberAccessContext
 */
void zen_MemberAccessContext_delete(zen_MemberAccessContext_t* context);

// Children

/**
 * @memberof MemberAccessContext
 */
void zen_MemberAccessContext_getChildren(zen_MemberAccessContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_MEMBER_ACCESS_CONTEXT_H */