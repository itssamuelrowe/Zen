// Tuesday, July 16, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_MAP_ENTRY_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_MAP_ENTRY_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * MapEntryContext                                                             *
 *******************************************************************************/

/**
 * @class MapEntryContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_MapEntryContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_keyExpression;
    zen_ASTNode_t* m_valueExpression;
};

/**
 * @memberof MapEntryContext
 */
typedef struct zen_MapEntryContext_t zen_MapEntryContext_t;

// Constructor

/**
 * @memberof MapEntryContext
 */
zen_MapEntryContext_t* zen_MapEntryContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof MapEntryContext
 */
void zen_MapEntryContext_delete(zen_MapEntryContext_t* context);

// Children

/**
 * @memberof MapEntryContext
 */
void zen_MapEntryContext_getChildren(zen_MapEntryContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_MAP_ENTRY_CONTEXT_H */