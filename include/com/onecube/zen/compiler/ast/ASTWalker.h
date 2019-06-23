// Sunday, November 26, 2017

#ifndef ZEN_AST_WALKER_H
#define ZEN_AST_WALKER_H

#include <com/onecube/zen/Configuration.h>
#include <zen/ASTListener.h>
#include <zen/ASTNode.h>

/*******************************************************************************
 * ASTWalker                                                                   *
 *******************************************************************************/

void zen_ASTWalker_walk(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_ASTWalker_enterRule(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_ASTWalker_exitRule(zen_ASTListener_t* listener, zen_ASTNode_t* node);

#endif /* ZEN_AST_WALKER_H */