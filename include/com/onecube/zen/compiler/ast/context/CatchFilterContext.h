// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CATCH_FILTER_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CATCH_FILTER_CONTEXT_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * CatchFilterContext                                                          *
 *******************************************************************************/

/**
 * @class CatchFilterContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_CatchFilterContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_typeNames; /* <zen_ASTNode_t*> */
};

/**
 * @memberof CatchFilterContext
 */
typedef struct zen_CatchFilterContext_t zen_CatchFilterContext_t;

// Constructor

/**
 * @memberof CatchFilterContext
 */
zen_CatchFilterContext_t* zen_CatchFilterContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof CatchFilterContext
 */
void zen_CatchFilterContext_delete(zen_CatchFilterContext_t* context);

// Children

/**
 * @memberof CatchFilterContext
 */
void zen_CatchFilterContext_getChildren(zen_CatchFilterContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CATCH_FILTER_CONTEXT_H */