/*
 * Copyright 2018-2020 Samuel Rowe
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

// Monday, March 16, 2020

#include <com/onecube/zen/compiler/Compiler.h>

/******************************************************************************
 * Compiler                                                                   *
 ******************************************************************************/

// Constructor

zen_Compiler_t* zen_Compiler_new() {
    zen_Compiler_t* compiler = jtk_Memory_allocate(zen_Compiler_t, 1);
    compiler->m_dumpTokens = false;
    compiler->m_dumpNodes = false;
    compiler->m_footprint = false;
    compiler->m_inputFiles = jtk_ArrayList_new();
    compiler->m_currentFileIndex = -1;

    return compiler;
}

// Destructor

void zen_Compiler_delete(zen_Compiler_t* compiler) {
    jtk_Assert_assertObject(compiler, "The specified compiler is null.");

    jtk_ArrayList_delete(compiler->m_inputFiles);
    jtk_Memory_deallocate(compiler);
}