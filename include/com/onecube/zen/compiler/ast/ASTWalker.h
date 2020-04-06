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