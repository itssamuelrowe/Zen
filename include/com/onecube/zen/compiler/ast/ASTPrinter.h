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

// Saturday, March 10, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_AST_PRINTER_H
#define COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_AST_PRINTER_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTListener.h>

/*******************************************************************************
 * ASTPrinter                                                                  *
 *******************************************************************************/

/**
 * @class ASTPrinter
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ASTPrinter_t {
    zen_ASTListener_t* m_astListener;
    int32_t m_depth;
};

/**
 * @memberof ASTPrinter
 */
typedef struct zen_ASTPrinter_t zen_ASTPrinter_t;

/**
 * @memberof ASTPrinter
 */
zen_ASTPrinter_t* zen_ASTPrinter_new();

/**
 * @memberof ASTPrinter
 */
void zen_ASTPrinter_delete(zen_ASTPrinter_t* listener);

/**
 * @memberof ASTPrinter
 */
zen_ASTListener_t* zen_ASTPrinter_getASTListener(zen_ASTPrinter_t* listener);

/**
 * @memberof ASTPrinter
 */
void zen_ASTPrinter_onEnterEveryRule(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTPrinter
 */
void zen_ASTPrinter_onExitEveryRule(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTPrinter
 */
void zen_ASTPrinter_onVisitTerminal(zen_ASTListener_t* listener, zen_ASTNode_t* node);

#endif /* COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_AST_PRINTER_H */