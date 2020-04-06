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

// Sunday, February 25, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_COMPILATION_UNIT_SCOPE_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_COMPILATION_UNIT_SCOPE_H

#include <jtk/collection/map/HashMap.h>

#include <com/onecube/zen/compiler/symbol-table/Scope.h>
#include <com/onecube/zen/compiler/symbol-table/Symbol.h>

/*******************************************************************************
 * CompilationUnitScope                                                        *
 *******************************************************************************/

/**
 * @class CompilationUnitScope
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_CompilationUnitScope_t {
    zen_Scope_t* m_scope;
    jtk_HashMap_t* m_symbols;
};

/**
 * @memberof CompilationUnitScope
 */
typedef struct zen_CompilationUnitScope_t zen_CompilationUnitScope_t;

// Constructor

/**
 * @memberof CompilationUnitScope
 */
zen_CompilationUnitScope_t* zen_CompilationUnitScope_new();

// Destructor

/**
 * @memberof CompilationUnitScope
 */
void zen_CompilationUnitScope_delete(zen_CompilationUnitScope_t* scope);

// Children Symbols

/**
 * @memberof CompilationUnitScope
 */
void zen_CompilationUnitScope_getChildrenSymbols(zen_CompilationUnitScope_t* scope, jtk_ArrayList_t* childrenSymbols);

// Define

/**
 * @memberof CompilationUnitScope
 */
void zen_CompilationUnitScope_define(zen_CompilationUnitScope_t* scope, zen_Symbol_t* symbol);

// Resolve

/**
 * @memberof CompilationUnitScope
 */
zen_Symbol_t* zen_CompilationUnitScope_resolve(zen_CompilationUnitScope_t* scope, const uint8_t* identifier);

// Scope

/**
 * @memberof CompilationUnitScope
 */
zen_Scope_t* zen_CompilationUnitScope_getScope(zen_CompilationUnitScope_t* scope);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_COMPILATION_UNIT_SCOPE_H */