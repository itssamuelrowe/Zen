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

#ifndef COM_ONECUBE_COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_AST_ANNOTATIONS_H
#define COM_ONECUBE_COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_AST_ANNOTATIONS_H

#include <jtk/collection/map/HashMap.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * ASTAnnotations                                                             *
 *******************************************************************************/

struct zen_ASTAnnotations_t {
    jtk_HashMap_t* m_map;
};

typedef struct zen_ASTAnnotations_t zen_ASTAnnotations_t;

zen_ASTAnnotations_t* zen_ASTAnnotations_new();
void zen_ASTAnnotations_delete(zen_ASTAnnotations_t* annotations);
void* zen_ASTAnnotations_get(zen_ASTAnnotations_t* annotations, zen_ASTNode_t* node);
void zen_ASTAnnotations_put(zen_ASTAnnotations_t* annotations, zen_ASTNode_t* node, void* value);
void zen_ASTAnnotations_remove(zen_ASTAnnotations_t* annotations, zen_ASTNode_t* node);

#endif /* COM_ONECUBE_COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_AST_ANNOTATIONS_H */