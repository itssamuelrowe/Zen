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

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_AST_WALKER_STATE_H
#define COM_ONECUBE_ZEN_COMPILER_AST_AST_WALKER_STATE_H

/*******************************************************************************
 * ASTWalker                                                                   *
 *******************************************************************************/

/**
 * @class ASTWalker
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe <samuelrowe1999@gmail.com>
 * @since zen 1.0
 */
enum zen_ASTWalkerState_t {
    ZEN_AST_WALKER_STATE_INVALID,

    ZEN_AST_WALKER_STATE_SKIP_CHILDREN,
    ZEN_AST_WALKER_STATE_VISIT_CHILDREN,

    ZEN_AST_WALKER_STATE_SKIP_EVEN_CHILDREN,
    ZEN_AST_WALKER_STATE_VISIT_EVEN_CHILDREN,

    ZEN_AST_WALKER_STATE_SKIP_ODD_CHILDREN,
    ZEN_AST_WALKER_STATE_VISIT_ODD_CHILDREN,

    ZEN_AST_WALKER_STATE_SKIP_EXTREME_CHILDREN,
    ZEN_AST_WALKER_STATE_VISIT_EXTREME_CHILDREN,

    ZEN_AST_WALKER_STATE_VISIT_FIRST_CHILD,
    ZEN_AST_WALKER_STATE_SKIP_FIRST_CHILD,

    ZEN_AST_WALKER_STATE_VISIT_LAST_CHILD,
    ZEN_AST_WALKER_STATE_SKIP_LAST_CHILD,
};

/**
 * @memberof ASTWalker
 */
typedef enum zen_ASTWalkerState_t zen_ASTWalkerState_t;

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_AST_WALKER_STATE_H */