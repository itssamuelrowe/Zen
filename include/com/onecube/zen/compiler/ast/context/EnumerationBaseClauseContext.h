// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_BASE_CLAUSE_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_BASE_CLAUSE_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * EnumerationBaseClauseContext                                                *
 *******************************************************************************/

/**
 * @class EnumerationBaseClauseContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EnumerationBaseClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_typeName;
};

/**
 * @memberof EnumerationBaseClauseContext
 */
typedef struct zen_EnumerationBaseClauseContext_t zen_EnumerationBaseClauseContext_t;

// Constructor

/**
 * @memberof EnumerationBaseClauseContext
 */
zen_EnumerationBaseClauseContext_t* zen_EnumerationBaseClauseContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof EnumerationBaseClauseContext
 */
void zen_EnumerationBaseClauseContext_delete(zen_EnumerationBaseClauseContext_t* context);

// Children

/**
 * @memberof EnumerationBaseClauseContext
 */
void zen_EnumerationBaseClauseContext_getChildren(zen_EnumerationBaseClauseContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_BASE_CLAUSE_CONTEXT_H */