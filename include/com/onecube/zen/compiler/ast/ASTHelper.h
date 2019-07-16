// Saturday, April 28, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_AST_HELPER_H
#define COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_AST_HELPER_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/lexer/TokenType.h>

/*******************************************************************************
 * ASTHelper                                                                   *
 *******************************************************************************/

// Ancestor

/**
 * @memberof ASTHelper
 */
bool zen_ASTHelper_isAncestor(zen_ASTNode_t* node, zen_ASTNode_t* subject);

/**
 * @memberof ASTHelper
 */
zen_ASTNode_t* zen_ASTHelper_getAncestor(zen_ASTNode_t* node, zen_ASTNodeType_t type);

// Descendant

/**
 * @memberof ASTHelper
 */
bool zen_ASTHelper_isDescendant(zen_ASTNode_t* node, zen_ASTNode_t* subject);

// Node

/**
 * @memberof ASTHelper
 */
void zen_ASTHelper_getNodes(zen_ASTNode_t* node, jtk_ArrayList_t* list, int32_t filter, bool captureTerminals, bool strip);

// Tokens

/**
 * @memberof ASTHelper
 */
void zen_ASTHelper_getTokens(zen_ASTNode_t* node, jtk_ArrayList_t* list);

/**
 * @memberof ASTHelper
 */
void zen_ASTHelper_getFilteredTokens(zen_ASTNode_t* node, jtk_ArrayList_t* list, zen_TokenType_t type);

// Terminal Node

/**
 * @memberof ASTHelper
 */
void zen_ASTHelper_getTerminalNodes(zen_ASTNode_t* node, jtk_ArrayList_t* list);

/**
 * @memberof ASTHelper
 */
void zen_ASTHelper_getFilteredTerminalNodes(zen_ASTNode_t* node, jtk_ArrayList_t* list, zen_TokenType_t type);

#endif /* COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_AST_HELPER_H */