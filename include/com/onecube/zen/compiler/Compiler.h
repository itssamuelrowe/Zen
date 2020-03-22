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

#ifndef COM_ONECUBE_ZEN_COMPILER_COMPILER_H
#define COM_ONECUBE_ZEN_COMPILER_COMPILER_H


#include <com/onecube/zen/Configuration.h>
#include <jtk/collection/list/ArrayList.h>

#include <jtk/log/Logger.h>

/******************************************************************************
 * Compiler                                                                   *
 ******************************************************************************/

/**
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_Compiler_t {
    bool m_dumpTokens;
    bool m_dumpNodes;
    bool m_footprint;
    jtk_ArrayList_t* m_inputFiles;
    int32_t m_currentFileIndex;
    jtk_Logger_t* m_logger;
};

/**
 * @memberof Compiler
 */
typedef struct zen_Compiler_t zen_Compiler_t;

// Constructor

zen_Compiler_t* zen_Compiler_new();

// Destructor

void zen_Compiler_delete(zen_Compiler_t* compiler);

#endif /* COM_ONECUBE_ZEN_COMPILER_COMPILER_H */