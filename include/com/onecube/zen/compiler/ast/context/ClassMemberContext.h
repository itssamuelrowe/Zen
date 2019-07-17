// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CLASS_MEMBER_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CLASS_MEMBER_CONTEXT_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * ClassMemberContext                                                          *
 *******************************************************************************/

/**
 * @class ClassMemberContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ClassMemberContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_modifiers; /* <zen_ASTNode_t*> */
    zen_ASTNode_t* m_declaration;
};

/**
 * @memberof ClassMemberContext
 */
typedef struct zen_ClassMemberContext_t zen_ClassMemberContext_t;

// Constructor

/**
 * @memberof ClassMemberContext
 */
zen_ClassMemberContext_t* zen_ClassMemberContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof ClassMemberContext
 */
void zen_ClassMemberContext_delete(zen_ClassMemberContext_t* context);

// Children

/**
 * @memberof ClassMemberContext
 */
void zen_ClassMemberContext_getChildren(zen_ClassMemberContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CLASS_MEMBER_CONTEXT_H */