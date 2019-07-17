// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ENUMERATION_SUITE_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ENUMERATION_SUITE_CONTEXT_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * EnumerationSuiteContext                                                     *
 *******************************************************************************/

/**
 * @class EnumerationSuiteContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EnumerationSuiteContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_enumerates; /* <zen_ASTNode_t*> */
};

/**
 * @memberof EnumerationSuiteContext
 */
typedef struct zen_EnumerationSuiteContext_t zen_EnumerationSuiteContext_t;

// Constructor

/**
 * @memberof EnumerationSuiteContext
 */
zen_EnumerationSuiteContext_t* zen_EnumerationSuiteContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof EnumerationSuiteContext
 */
void zen_EnumerationSuiteContext_delete(zen_EnumerationSuiteContext_t* context);

// Children

/**
 * @memberof EnumerationSuiteContext
 */
void zen_EnumerationSuiteContext_getChildren(zen_EnumerationSuiteContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ENUMERATION_SUITE_CONTEXT_H *