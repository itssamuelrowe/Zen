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

// Saturday, November 25, 2017

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_CATEGORY_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_CATEGORY_H

/*******************************************************************************
 * SymbolCategory                                                              *
 *******************************************************************************/

/**
 * @class SymbolCategory
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
enum zen_SymbolCategory_t {
    ZEN_SYMBOL_CATEGORY_ANNOTATION,
    ZEN_SYMBOL_CATEGORY_CLASS,
    ZEN_SYMBOL_CATEGORY_ENUMERATE,
    ZEN_SYMBOL_CATEGORY_ENUMERATION,
    ZEN_SYMBOL_CATEGORY_FUNCTION,
    ZEN_SYMBOL_CATEGORY_CONSTANT,
    ZEN_SYMBOL_CATEGORY_VARIABLE,
    // ZEN_SYMBOL_CATEGORY_CONSTRUCTOR,
    ZEN_SYMBOL_CATEGORY_LABEL,
    ZEN_SYMBOL_CATEGORY_EXTERNAL
};

/**
 * @memberof SymbolCategory
 */
typedef enum zen_SymbolCategory_t zen_SymbolCategory_t;

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_CATEGORY_H */