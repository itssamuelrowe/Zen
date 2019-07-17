// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_STATEMENT_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_STATEMENT_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * StatementContext                                                            *
 *******************************************************************************/

/**
 * @class StatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_StatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_simpleStatement;
    zen_ASTNode_t* m_compoundStatement;
};

/** 
 * @memberof StatementContext
 */
typedef struct zen_StatementContext_t zen_StatementContext_t;

// Constructor

/** 
 * @memberof StatementContext
 */
zen_StatementContext_t* zen_StatementContext_new(zen_ASTNode_t* node);

// Destructor

/** 
 * @memberof StatementContext
 */
void zen_StatementContext_delete(zen_StatementContext_t* context);

// Children

/** 
 * @memberof StatementContext
 */
void zen_StatementContext_getChildren(zen_StatementContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_STATEMENT_CONTEXT_H */