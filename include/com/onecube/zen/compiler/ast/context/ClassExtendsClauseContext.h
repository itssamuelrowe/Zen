// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CLASS_EXTENDS_CLAUSE_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CLASS_EXTENDS_CLAUSE_CONTEXT_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * ClassExtendsClauseContext                                                   *
 *******************************************************************************/

/**
 * @class ClassExtendsClauseContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ClassExtendsClauseContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_typeNames; /* <zen_ASTNode_t*> */
};

/**
 * @memberof ClassExtendsClauseContext
 */
typedef struct zen_ClassExtendsClauseContext_t zen_ClassExtendsClauseContext_t;

// Constructor

/**
 * @memberof ClassExtendsClauseContext
 */
zen_ClassExtendsClauseContext_t* zen_ClassExtendsClauseContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof ClassExtendsClauseContext
 */
void zen_ClassExtendsClauseContext_delete(zen_ClassExtendsClauseContext_t* context);

// Children

/**
 * @memberof ClassExtendsClauseContext
 */
void zen_ClassExtendsClauseContext_getChildren(zen_ClassExtendsClauseContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CLASS_EXTENDS_CLAUSE_CONTEXT_H */