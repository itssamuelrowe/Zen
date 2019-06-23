// Saturday, April 28, 2018

#ifndef ZEN_AST_AST_HELPER_H
#define ZEN_AST_AST_HELPER_H

#include <com/onecube/zen/Configuration.h>
#include <zen/ASTNode.h>
#include <zen/collection/ArrayList.h>
#include <zen/TokenType.h>

/*******************************************************************************
 * ASTHelper                                                                   *
 *******************************************************************************/

bool zen_ASTHelper_isAncestor(zen_ASTNode_t* node, zen_ASTNode_t* subject);
zen_ASTNode_t* zen_ASTHelper_getAncestor(zen_ASTNode_t* node, zen_ASTNodeType_t type);
bool zen_ASTHelper_isDescendant(zen_ASTNode_t* node, zen_ASTNode_t* subject);
void zen_ASTHelper_getTokens(zen_ASTNode_t* node, zen_ArrayList_t* list);
void zen_ASTHelper_getFilteredTokens(zen_ASTNode_t* node, zen_ArrayList_t* list, zen_TokenType_t type);
void zen_ASTHelper_getTerminalNodes(zen_ASTNode_t* node, zen_ArrayList_t* list);
void zen_ASTHelper_getFilteredTerminalNodes(zen_ASTNode_t* node, zen_ArrayList_t* list, zen_TokenType_t type);
void zen_ASTHelper_getNodes(zen_ASTNode_t* node, zen_ArrayList_t* list, int32_t filter, bool captureTerminals, bool strip);

#endif /* ZEN_AST_AST_HELPER_H */