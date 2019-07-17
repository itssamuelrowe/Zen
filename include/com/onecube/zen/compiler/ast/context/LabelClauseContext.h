// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_LABEL_CLAUSE_CONTEXT
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_LABEL_CLAUSE_CONTEXT

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * LabelClauseContext                                                          *
 *******************************************************************************/

/**
 * @class LabelClauseContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_LabelClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
};

/** 
 * @memberof LabelClauseContext
 */
typedef struct zen_LabelClauseContext_t zen_LabelClauseContext_t;

// Constructor

/** 
 * @memberof LabelClauseContext
 */
zen_LabelClauseContext_t* zen_LabelClauseContext_new(zen_ASTNode_t* node);

// Destructor

/** 
 * @memberof LabelClauseContext
 */
void zen_LabelClauseContext_delete(zen_LabelClauseContext_t* context);

// Children

/** 
 * @memberof LabelClauseContext
 */
void zen_LabelClauseContext_getChildren(zen_LabelClauseContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_LABEL_CLAUSE_CONTEXT */