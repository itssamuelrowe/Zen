// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_COMPILATION_UNIT_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_COMPILATION_UNIT_CONTEXT_H
 
#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>
 
/*******************************************************************************
 * CompilationUnitContext                                                      *
 *******************************************************************************/

/**
 * @class CompilationUnitContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_CompilationUnitContext_t {
    /**
     * The node which represents this rule context within the AST.
     */
    zen_ASTNode_t* m_node;
	jtk_ArrayList_t* m_importDeclarations; /* <zen_ASTNode_t*> */
	jtk_ArrayList_t* m_annotatedComponentDeclarations; /* <zen_ASTNode_t*> */
};

/**
 * @memberof CompilationUnitContext
 */
typedef struct zen_CompilationUnitContext_t zen_CompilationUnitContext_t;

// Constructor

/**
 * @memberof CompilationUnitContext
 */
zen_CompilationUnitContext_t* zen_CompilationUnitContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof CompilationUnitContext
 */
void zen_CompilationUnitContext_delete(zen_CompilationUnitContext_t* context);

// Children

/**
 * @memberof CompilationUnitContext
 */
void zen_CompilationUnitContext_getChildren(zen_CompilationUnitContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_COMPILATION_UNIT_CONTEXT_H */