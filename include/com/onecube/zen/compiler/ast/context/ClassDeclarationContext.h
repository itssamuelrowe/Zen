// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CLASS_DECLARATION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_CLASS_DECLARATION_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * ClassDeclarationContext                                                     *
 *******************************************************************************/

/**
 * @class ClassDeclarationContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ClassDeclarationContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_classExtendsClause;
    zen_ASTNode_t* m_classSuite;
};

/**
 * @memberof ClassDeclarationContext
 */
typedef struct zen_ClassDeclarationContext_t zen_ClassDeclarationContext_t;

// Constructor

/**
 * @memberof ClassDeclarationContext
 */
zen_ClassDeclarationContext_t* zen_ClassDeclarationContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof ClassDeclarationContext
 */
void zen_ClassDeclarationContext_delete(zen_ClassDeclarationContext_t* context);

// Children

/**
 * @memberof ClassDeclarationContext
 */
void zen_ClassDeclarationContext_getChildren(zen_ClassDeclarationContext_t* context, jtk_ArrayList_t* children);
