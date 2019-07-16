// Tuesday, July 16, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_MAP_ENTRIES_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_MAP_ENTRIES_CONTEXT_H
 
/*******************************************************************************
 * MapEntriesContext                                                           *
 *******************************************************************************/

/**
 * @class MapEntriesContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_MapEntriesContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_mapEntries;
};

/**
 * @memberof MapEntriesContext
 */
typedef struct zen_MapEntriesContext_t zen_MapEntriesContext_t;

// Constructor

/**
 * @memberof MapEntriesContext
 */
zen_MapEntriesContext_t* zen_MapEntriesContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof MapEntriesContext
 */
void zen_MapEntriesContext_delete(zen_MapEntriesContext_t* context);

// Children

/**
 * @memberof MapEntriesContext
 */
void zen_MapEntriesContext_getChildren(zen_MapEntriesContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_MAP_ENTRIES_CONTEXT_H */