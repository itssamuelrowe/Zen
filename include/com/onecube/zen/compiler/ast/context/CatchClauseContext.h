// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CATCH_CLAUSE_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CATCH_CLAUSE_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * CatchClauseContext                                                          *
 *******************************************************************************/

/**
 * @class CatchClauseContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_CatchClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_catchFilter;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_statementSuite;
};

/**
 * @memberof CatchClauseContext
 */
typedef struct zen_CatchClauseContext_t zen_CatchClauseContext_t;

// Constructor

/**
 * @memberof CatchClauseContext
 */
zen_CatchClauseContext_t* zen_CatchClauseContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof CatchClauseContext
 */
void zen_CatchClauseContext_delete(zen_CatchClauseContext_t* context);

// Children

/**
 * @memberof CatchClauseContext
 */
void zen_CatchClauseContext_getChildren(zen_CatchClauseContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CATCH_CLAUSE_CONTEXT_H */