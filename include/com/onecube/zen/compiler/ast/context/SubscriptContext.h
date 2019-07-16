// Tuesday, July 16, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_SUBSCRIPT_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_SUBSCRIPT_CONTEXT_H
 
/*******************************************************************************
 * SubscriptContext                                                            *
 *******************************************************************************/

/**
 * @class SubscriptContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_SubscriptContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
};

/**
 * @memberof SubscriptContext
 */
typedef struct zen_SubscriptContext_t zen_SubscriptContext_t;

// Constructor

/**
 * @memberof SubscriptContext
 */
zen_SubscriptContext_t* zen_SubscriptContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof SubscriptContext
 */
void zen_SubscriptContext_delete(zen_SubscriptContext_t* context);

// Children

/**
 * @memberof SubscriptContext
 */
void zen_SubscriptContext_getChildren(zen_SubscriptContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_SUBSCRIPT_CONTEXT_H */