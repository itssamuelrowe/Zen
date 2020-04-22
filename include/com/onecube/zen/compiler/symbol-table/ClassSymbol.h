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

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_CLASS_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_CLASS_SYMBOL_H

#include <jtk/collection/list/ArrayList.h>

/*******************************************************************************
 * ClassSymbol                                                                 *
 *******************************************************************************/

typedef struct zen_Scope_t zen_Scope_t;

/**
 * @class ClassSymbol
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ClassSymbol_t {
    /* The superclasses are added in the resolution phase. */
    jtk_ArrayList_t* m_superClasses; /* <zen_Symbol_t*> */
    uint8_t* m_qualifiedName;
    int32_t m_qualifiedNameSize;
    zen_Scope_t* m_classScope;

    uint8_t* m_descriptor;
    int32_t m_descriptorSize;
};

/**
 * @memberof ClassSymbol
 */
typedef struct zen_ClassSymbol_t zen_ClassSymbol_t;

void zen_ClassSymbol_initialize(zen_ClassSymbol_t* symbol);

void zen_ClassSymbol_destroy(zen_ClassSymbol_t* symbol);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_CLASS_SYMBOL_H */