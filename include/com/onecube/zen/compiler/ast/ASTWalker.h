// Sunday, November 26, 2017

#ifndef COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_WALKER_H
#define COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_WALKER_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTListener.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * ASTWalker                                                                   *
 *******************************************************************************/

/**
 * @memberof ASTWalker
 */
void zen_ASTWalker_walk(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTWalker
 */
void zen_ASTWalker_enterRule(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTWalker
 */
void zen_ASTWalker_exitRule(zen_ASTListener_t* listener, zen_ASTNode_t* node);

#endif /* COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_WALKER_H */