/*
 * Copyright 2018-2019 OneCube
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

#include <com/onecube/zen/compiler/ast/ASTAnnotations.h>

/*******************************************************************************
 * ASTAnnotations                                                             *
 *******************************************************************************/

zen_ASTAnnotations_t* zen_ASTAnnotations_new() {
    zen_ASTAnnotations_t* annotations = zen_Memory_allocate(zen_ASTAnnotations_t, 1);
    annotations->m_map = zen_HashMap_new(zen_PointerObjectAdapter_getInstance(), NULL);

    return annotations;
}

void zen_ASTAnnotations_delete(zen_ASTAnnotations_t* annotations) {
    jtk_Assert_assertObject(annotations, "The specified annotations is null.");

    zen_HashMap_delete(annotations->m_map);
    zen_Memory_deallocate(annotations);
}

void* zen_ASTAnnotations_get(zen_ASTAnnotations_t* annotations, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(annotations, "The specified annotations is null.");

    return zen_HashMap_getValue(annotations->m_map, node);
}

void zen_ASTAnnotations_put(zen_ASTAnnotations_t* annotations, zen_ASTNode_t* node, void* value) {
    jtk_Assert_assertObject(annotations, "The specified annotations is null.");

    jtk_HashMap_put(annotations->m_map, (void*)node, value);
}

void zen_ASTAnnotations_remove(zen_ASTAnnotations_t* annotations, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(annotations, "The specified annotations is null.");

    zen_HashMap_removeKey(annotations->m_map, node);
}