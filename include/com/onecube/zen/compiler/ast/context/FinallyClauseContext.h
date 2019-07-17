// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FINALLY_CLAUSE_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FINALLY_CLAUSE_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * FinallyClauseContext                                                        *
 *******************************************************************************/

/**
 * @class FinallyClauseContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_FinallyClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_statementSuite;
};

/**
 * @memberof FinallyClauseContext
 */
typedef struct zen_FinallyClauseContext_t zen_FinallyClauseContext_t;

// Constructor

/**
 * @memberof FinallyClauseContext
 */
zen_FinallyClauseContext_t* zen_FinallyClauseContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof FinallyClauseContext
 */
void zen_FinallyClauseContext_delete(zen_FinallyClauseContext_t* context);

// Children

/**
 * @memberof FinallyClauseContext
 */
void zen_FinallyClauseContext_getChildren(zen_FinallyClauseContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_FINALLY_CLAUSE_CONTEXT_H */