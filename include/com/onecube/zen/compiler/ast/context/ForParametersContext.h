// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FOR_PARAMETERS_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FOR_PARAMETERS_CONTEXT_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * ForParametersContext                                                        *
 *******************************************************************************/

/**
 * @class ForParametersContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ForParametersContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_declarator;
    jtk_ArrayList_t* m_identifiers;
};

/**
 * @memberof ForParametersContext
 */
typedef struct zen_ForParametersContext_t zen_ForParametersContext_t;

// Constructor

/**
 * @memberof ForParametersContext
 */
zen_ForParametersContext_t* zen_ForParametersContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof ForParametersContext
 */
void zen_ForParametersContext_delete(zen_ForParametersContext_t* context);

// Children

/**
 * @memberof ForParametersContext
 */
void zen_ForParametersContext_getChildren(zen_ForParametersContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FOR_PARAMETERS_CONTEXT_H */