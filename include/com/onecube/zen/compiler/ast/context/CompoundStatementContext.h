// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_COMPOUND_STATEMENT_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_COMPOUND_STATEMENT_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * CompoundStatementContext                                                    *
 *******************************************************************************/

/**
 * @class CompoundStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_CompoundStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_statement;
};

/**
 * @memberof CompoundStatementContext
 */
typedef struct zen_CompoundStatementContext_t zen_CompoundStatementContext_t;

// Constructor

/**
 * @memberof CompoundStatementContext
 */
zen_CompoundStatementContext_t* zen_CompoundStatementContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof CompoundStatementContext
 */
void zen_CompoundStatementContext_delete(zen_CompoundStatementContext_t* context);

// Children

/**
 * @memberof CompoundStatementContext
 */
void zen_CompoundStatementContext_getChildren(zen_CompoundStatementContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_COMPOUND_STATEMENT_CONTEXT_H */