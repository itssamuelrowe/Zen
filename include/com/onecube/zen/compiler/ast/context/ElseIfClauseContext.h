// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ELSE_IF_CLAUSE_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ELSE_IF_CLAUSE_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * ElseIfClauseContext                                                         *
 *******************************************************************************/

/**
 * @class ElseIfClauseContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ElseIfClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
    zen_ASTNode_t* m_statementSuite;
};

/** 
 * @memberof ElseIfClauseContext
 */
typedef struct zen_ElseIfClauseContext_t zen_ElseIfClauseContext_t;

// Constructor

/** 
 * @memberof ElseIfClauseContext
 */
zen_ElseIfClauseContext_t* zen_ElseIfClauseContext_new(zen_ASTNode_t* node);

// Destructor

/** 
 * @memberof ElseIfClauseContext
 */
void zen_ElseIfClauseContext_delete(zen_ElseIfClauseContext_t* context);

// Children

/** 
 * @memberof ElseIfClauseContext
 */
void zen_ElseIfClauseContext_getChildren(zen_ElseIfClauseContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ELSE_IF_CLAUSE_CONTEXT_H */