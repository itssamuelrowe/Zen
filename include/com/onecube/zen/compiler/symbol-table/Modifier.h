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

// Sunday, March 04, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MODIFIER_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MODIFIER_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * Modifier                                                                    *
 *******************************************************************************/

/**
 * @class Modifier
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
enum zen_Modifier_t {

    ZEN_MODIFIER_PUBLIC = 1 << 0,

    ZEN_MODIFIER_PRIVATE = 1 << 1,

    ZEN_MODIFIER_SECRET = 1 << 2,

    ZEN_MODIFIER_SYNTHETIC = 1 << 3,

    ZEN_MODIFIER_ABSTRACT = 1 << 4,

    ZEN_MODIFIER_FINAL = 1 << 5,

    ZEN_MODIFIER_STATIC = 1 << 6,

    ZEN_MODIFIER_NATIVE = 1 << 7,

    ZEN_MODIFIER_ANNOTATION = 1 << 8,

    ZEN_MODIFIER_ENUMERATION = 1 << 9,

    ZEN_MODIFIER_VARIABLE_PARAMETER = 1 << 10
};

/**
 * @memberof Modifier
 */
typedef enum zen_Modifier_t zen_Modifier_t;



#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MODIFIER_H */