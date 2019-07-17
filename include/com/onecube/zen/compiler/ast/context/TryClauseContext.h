// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_TRY_CLAUSE_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_TRY_CLAUSE_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * TryClauseContext                                                            *
 *******************************************************************************/

/**
 * @class TryClauseContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_TryClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_statementSuite;
};

/**
 * @memberof TryClauseContext
 */
typedef struct zen_TryClauseContext_t zen_TryClauseContext_t;

// Constructor

/**
 * @memberof TryClauseContext
 */
zen_TryClauseContext_t* zen_TryClauseContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof TryClauseContext
 */
void zen_TryClauseContext_delete(zen_TryClauseContext_t* context);

// Children

/**
 * @memberof TryClauseContext
 */
void zen_TryClauseContext_getChildren(zen_TryClauseContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_TRY_CLAUSE_CONTEXT_H */