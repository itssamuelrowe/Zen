// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_EXPRESSIONS_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_EXPRESSIONS_CONTEXT_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * ExpressionsContext                                                          *
 *******************************************************************************/

/**
 * @class ExpressionsContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ExpressionsContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_expressions; /* <zen_ASTNode_t*> */
};

/**
 * @memberof ExpressionsContext
 */
typedef struct zen_ExpressionsContext_t zen_ExpressionsContext_t;

/**
 * @memberof ExpressionsContext
 */
zen_ExpressionsContext_t* zen_ExpressionsContext_new(zen_ASTNode_t* node);

/**
 * @memberof ExpressionsContext
 */
void zen_ExpressionsContext_delete(zen_ExpressionsContext_t* context);

/**
 * @memberof ExpressionsContext
 */
void zen_ExpressionsContext_getChildren(zen_ExpressionsContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_EXPRESSIONS_CONTEXT_H */