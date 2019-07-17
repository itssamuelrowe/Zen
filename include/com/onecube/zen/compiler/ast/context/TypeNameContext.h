// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_TYPE_NAME_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_TYPE_NAME_CONTEXT_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * TypeNameContext                                                             *
 *******************************************************************************/

/**
 * @class TypeNameContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_TypeNameContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_identifiers;
};

/**
 * @memberof TypeNameContext
 */
typedef struct zen_TypeNameContext_t zen_TypeNameContext_t;

// Constructor

/**
 * @memberof TypeNameContext
 */
zen_TypeNameContext_t* zen_TypeNameContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof TypeNameContext
 */
void zen_TypeNameContext_delete(zen_TypeNameContext_t* context);

// Children

/**
 * @memberof TypeNameContext
 */
void zen_TypeNameContext_getChildren(zen_TypeNameContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_TYPE_NAME_CONTEXT_H */