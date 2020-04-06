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

// Sunday, January 07, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_ANNOTATION_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_ANNOTATION_SYMBOL_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/compiler/symbol-table/Symbol.h>

/*******************************************************************************
 * AnnotationSymbol                                                            *
 *******************************************************************************/

/**
 * @class AnnotationSymbol
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AnnotationSymbol_t {
    zen_Symbol_t* m_symbol;
    jtk_ArrayList_t* m_superAnnotations; /* <zen_AnnotationSymbol_t*> */
};

/**
 * @memberof AnnotationSymbol
 */
typedef struct zen_AnnotationSymbol_t zen_AnnotationSymbol_t;

// Constructor

/**
 * @memberof AnnotationSymbol
 */
zen_AnnotationSymbol_t* zen_AnnotationSymbol_new(zen_ASTNode_t* identifier,
    jtk_ArrayList_t* superAnnotations);

// Destructor

/**
 * @memberof AnnotationSymbol
 */
void zen_AnnotationSymbol_delete(zen_AnnotationSymbol_t* symbol);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_ANNOTATION_SYMBOL_H */