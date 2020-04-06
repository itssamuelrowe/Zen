/*
 * Copyright 2017-2020 Samuel Rowe
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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