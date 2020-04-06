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

// Sunday, April 28, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_AST_ANNOTATION_TYPE_H
#define COM_ONECUBE_ZEN_COMPILER_AST_AST_ANNOTATION_TYPE_H

/*******************************************************************************
 * ASTAnnotationType                                                           *
 *******************************************************************************/

/**
 * @author Samuel Rowe
 * @since  zen 1.0
 */
enum zen_ASTAnnotationType_t {

    /**
     * A situation where certain instructions are required to be inserted between
     * the instructions of multiple productions of a single rule may arise when the
     * productions have a common ancestor. In such situations, the byte code generated
     * for a production is redirected to a new channel. An asymetrical location for
     * pushing and popping of channels is necessary. Here, a new channel is pushed
     * by the production rule on request by the ancestor. It is popped off by the
     * ancestor.
     *
     * The communication between the ancestor and the production rule occurs via
     * an AST node annotation. The bundle is null.
     */
    AST_ANNOTATION_TYPE_ASYMETRICAL_CHANNEL_MANAGEMENT,

    AST_ANNOTATION_TYPE_CUSTOM
};

typedef enum zen_ASTAnnotationType_t zen_ASTAnnotationType_t;

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_AST_ANNOTATION_TYPE_H */