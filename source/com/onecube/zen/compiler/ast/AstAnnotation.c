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

// Sunday, April 29, 2018

#include <com/onecube/zen/compiler/ast/ASTAnnotation.h>

/*******************************************************************************
 * ASTAnnotation                                                               *
 *******************************************************************************/

zen_ASTAnnotation_t* zen_ASTAnnotation_new(zen_ASTAnnotationType_t type, void* bundle) {
    zen_ASTAnnotation_t* annotation = zen_Memory_allocate(zen_ASTAnnotation_t, 1);
    annotation->m_type = type;
    annotation->m_bundle = bundle;

    return annotation;
}

void zen_ASTAnnotation_delete(zen_ASTAnnotation_t* annotation) {
    jtk_Assert_assertObject(annotation, "The specified annotation is null.");

    jtk_Memory_deallocate(annotation);
}

zen_ASTAnnotationType_t zen_ASTAnnotation_getType(zen_ASTAnnotation_t* annotation) {
    jtk_Assert_assertObject(annotation, "The specified annotation is null.");

    return annotation->m_type;
}