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

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SCOPE_TYPE_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SCOPE_TYPE_H

/*******************************************************************************
 * ScopeType                                                                   *
 *******************************************************************************/

/**
 * @class ScopeType
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
enum zen_ScopeType_t {
    ZEN_SCOPE_COMPILATION_UNIT,
    ZEN_SCOPE_ANNOTATION,
    ZEN_SCOPE_CLASS,
    ZEN_SCOPE_FUNCTION,
    ZEN_SCOPE_ENUMERATION,
    ZEN_SCOPE_LOCAL
};

/**
 * @memberof ScopeType
 */
typedef enum zen_ScopeType_t zen_ScopeType_t;

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SCOPE_TYPE_H */