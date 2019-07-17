// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CLASS_SUITE_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CLASS_SUITE_CONTEXT_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * ClassSuiteContext                                                           *
 *******************************************************************************/

/**
 * @class ClassSuiteContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ClassSuiteContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_classMembers; /* <zen_ASTNode_t*> */
};

/**
 * @memberof ClassSuiteContext
 */
typedef struct zen_ClassSuiteContext_t zen_ClassSuiteContext_t;

// Constructor

/**
 * @memberof ClassSuiteContext
 */
zen_ClassSuiteContext_t* zen_ClassSuiteContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof ClassSuiteContext
 */
void zen_ClassSuiteContext_delete(zen_ClassSuiteContext_t* context);

// Children

/**
 * @memberof ClassSuiteContext
 */
void zen_ClassSuiteContext_getChildren(zen_ClassSuiteContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CLASS_SUITE_CONTEXT_H */