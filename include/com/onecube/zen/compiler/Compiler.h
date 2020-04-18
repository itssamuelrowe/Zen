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
#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/ast/ASTAnnotations.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolTable.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolLoader.h>
#include <com/onecube/zen/compiler/support/ErrorHandler.h>
#include <com/onecube/zen/disassembler/BinaryEntityDisassembler.h>

#include <jtk/collection/list/ArrayList.h>
#include <jtk/collection/map/HashMap.h>
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
    bool m_dumpInstructions;
    jtk_Logger_t* m_logger;
    jtk_ArrayList_t* m_inputFiles;
    int32_t m_currentFileIndex;
    zen_ErrorHandler_t* m_errorHandler;
    zen_ASTNode_t** m_compilationUnits;
    zen_SymbolTable_t** m_symbolTables;
    zen_ASTAnnotations_t** m_scopes;
    uint8_t** m_packages;
    int32_t* m_packageSizes;
    zen_SymbolLoader_t* m_symbolLoader;
    jtk_HashMap_t* m_repository;
    jtk_ArrayList_t* m_trash;
    bool m_coreApi;
    zen_BinaryEntityDisassembler_t* m_disassembler;
};

/**
 * @memberof Compiler
 */
typedef struct zen_Compiler_t zen_Compiler_t;

// Constructor

zen_Compiler_t* zen_Compiler_new();

// Destructor

void zen_Compiler_delete(zen_Compiler_t* compiler);

// Error

void zen_Compiler_printErrors(zen_Compiler_t* compiler);

// Phase

void zen_Compiler_initialize(zen_Compiler_t* compiler);
void zen_Compiler_buildAST(zen_Compiler_t* compiler);
void zen_Compiler_analyze(zen_Compiler_t* compiler);
void zen_Compiler_generate(zen_Compiler_t* compiler);
void zen_Compiler_destroyNestedScopes(zen_ASTAnnotations_t* annotations);
void zen_Compiler_destroySymbol(zen_Symbol_t* symbol);
void zen_Compiler_destroyScope(zen_Scope_t* scope);

// Register

void zen_Compiler_registerSymbol(zen_Compiler_t* compiler, const uint8_t* identifier,
    int32_t identifierSize, zen_Symbol_t* symbol);

zen_Symbol_t* zen_Compiler_resolveSymbol(zen_Compiler_t* compiler,
    const uint8_t* name, int32_t nameSize);

// Token

void zen_Compiler_printToken(zen_Token_t* token);
void zen_Compiler_zen_Compiler_printTokens(zen_Compiler_t* compiler, jtk_ArrayList_t* tokens);

// Compiler

bool zen_Compiler_compileEx(zen_Compiler_t* compiler, char** arguments, int32_t length);
bool zen_Compiler_compile(zen_Compiler_t* compiler);

#endif /* COM_ONECUBE_ZEN_COMPILER_COMPILER_H */