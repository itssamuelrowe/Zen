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

#include <stdio.h>
#include <string.h>

/* The JTK_LOGGER_DISABLE constant is defined in Configuration.h. Therefore,
 * make sure it is included before any other file which may
 * include Logger.h!
 */
#include <com/onecube/zen/Configuration.h>

#include <jtk/collection/list/ArrayList.h>
#include <jtk/fs/FileInputStream.h>
#include <jtk/io/BufferedInputStream.h>
#include <jtk/io/InputStream.h>
#include <jtk/log/ConsoleLogger.h>

#include <com/onecube/zen/compiler/Compiler.h>
#include <com/onecube/zen/compiler/lexer/Lexer.h>
#include <com/onecube/zen/compiler/lexer/LexerError.h>
#include <com/onecube/zen/compiler/lexer/TokenStream.h>
#include <com/onecube/zen/compiler/parser/Parser.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/ast/ASTListener.h>
#include <com/onecube/zen/compiler/ast/ASTWalker.h>
#include <com/onecube/zen/compiler/ast/ASTAnnotations.h>
#include <com/onecube/zen/compiler/ast/ASTPrinter.h>
#include <com/onecube/zen/compiler/generator/BinaryEntityBuilder.h>
#include <com/onecube/zen/compiler/generator/BinaryEntityGenerator.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolDefinitionListener.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolResolutionListener.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolTable.h>
#include <com/onecube/zen/compiler/support/ErrorHandler.h>

#include <com/onecube/zen/virtual-machine/feb/Instruction.h>

// Token

void printToken(zen_Token_t* token) {
    printf("[%d-%d:%d-%d:%s:%s]", token->m_startLine, token->m_stopLine, token->m_startColumn + 1, token->m_stopColumn + 1, token->m_channel == ZEN_TOKEN_CHANNEL_DEFAULT? "default" : "hidden", zen_Lexer_getLiteralName(token->m_type));
    zen_TokenType_t type = zen_Token_getType(token);
    if ((type == ZEN_TOKEN_IDENTIFIER) || (type == ZEN_TOKEN_INTEGER_LITERAL) ||
        (type == ZEN_TOKEN_STRING_LITERAL)) {
        printf(" %.*s", token->m_length, token->m_text);
    }
    puts("");
}

void zen_Compiler_printTokens(zen_Compiler_t* compiler, zen_TokenStream_t* stream) {
    jtk_ArrayList_t* tokens = stream->m_tokens;

    int32_t defaultChannel = 0;
    int32_t hiddenChannel = 0;
    int32_t otherChannel = 0;

    int32_t limit = jtk_ArrayList_getSize(tokens);
    int32_t i;
    for (i = 0; i < limit; i++) {
        zen_Token_t* token = (zen_Token_t*)jtk_ArrayList_getValue(tokens, i);
        zen_TokenChannel_t channel = zen_Token_getChannel(token);
        if (channel == ZEN_TOKEN_CHANNEL_DEFAULT) {
            defaultChannel++;
        }
        else if (channel == ZEN_TOKEN_CHANNEL_HIDDEN) {
            hiddenChannel++;
        }
        else {
            otherChannel++;
        }
        printToken(token);
    }
    fflush(stdout);
    fprintf(stdout, "[info] %d tokens were recognized on the default channel.\n", defaultChannel);
    fprintf(stdout, "[info] %d tokens were recognized on the hidden channel.\n", hiddenChannel);
    fprintf(stdout, "[info] %d tokens were recognized on other channels.%s\n", otherChannel, (otherChannel > 0)? " This is surprising to me." : "");
    fprintf(stdout, "[info] %d tokens were recognized in total.\n", limit);
}


/******************************************************************************
 * PathHelper                                                                 *
 ******************************************************************************/

bool jtk_PathHelper_exists(const uint8_t* path) {
    return true;
}

#define JTK_FILE_OPEN_MODE_BUFFERED (1 << 0)

jtk_InputStream_t* jtk_PathHelper_read(const uint8_t* path);
jtk_InputStream_t* jtk_PathHelper_readEx(const uint8_t* path, uint32_t flags);

jtk_InputStream_t* jtk_PathHelper_read(const uint8_t* path) {
    return jtk_PathHelper_readEx(path, JTK_FILE_OPEN_MODE_BUFFERED);
}

jtk_InputStream_t* jtk_PathHelper_readEx(const uint8_t* path, uint32_t flags) {
    jtk_InputStream_t* result = NULL;

    jtk_FileInputStream_t* fileStream = jtk_FileInputStream_newFromString(path);
    result = fileStream->m_inputStream;

    if ((flags & JTK_FILE_OPEN_MODE_BUFFERED) != 0) {
        jtk_BufferedInputStream_t* bufferedStream = jtk_BufferedInputStream_new(fileStream->m_inputStream);
        result = bufferedStream->m_inputStream;
    }

    return result;
}


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
#ifdef JTK_LOGGER_DISABLE
    compiler->m_logger = NULL;
#else
    compiler->m_logger = jtk_Logger_new(jtk_ConsoleLogger_log);
    jtk_Logger_setLevel(compiler->m_logger, JTK_LOG_LEVEL_NONE);
#endif

    return compiler;
}

// Destructor

void zen_Compiler_delete(zen_Compiler_t* compiler) {
    jtk_Assert_assertObject(compiler, "The specified compiler is null.");

#ifndef JTK_LOGGER_DISABLE
    jtk_Logger_delete(compiler->m_logger);
#endif
    jtk_ArrayList_delete(compiler->m_inputFiles);
    jtk_Memory_deallocate(compiler);
}

void zen_Interpreter_onLexerError(zen_LexerError_t* error) {
    fprintf(stderr, "[error] %s:%d:%d -- %s\n", error->m_path, error->m_line, error->m_column, error->m_message);
    fflush(stderr);
}

const uint8_t* zen_ErrorCode_messages[] = {
    "None",

    // Lexical Errors

    "Unterminated string literal",
    "Unterminated multi-line comment",
    "Expected four hexadecimal digits",
    "Invalid escape sequence",
    "Unknown character",
    "Invalid prefix in integer literal",

    // Syntactical Errors

    "No viable alternative"
};

void zen_Compiler_printLexicalErrors(zen_Compiler_t* compiler) {
    jtk_ArrayList_t* errors = zen_ErrorHandler_getErrors(compiler->m_errorHandler);
    int32_t errorCount = jtk_ArrayList_getSize(errors);
    int32_t i;
    for (i = 0; i < errorCount; i++) {
        zen_Error_t* error = (zen_Error_t*)jtk_ArrayList_getValue(errors, i);
        zen_Token_t* token = error->m_token;
        fprintf(stderr, "[error] %d-%d:%d-%d: %s\n", token->m_startLine,
            token->m_stopLine, token->m_startColumn + 1, token->m_stopColumn + 1,
            zen_ErrorCode_messages[(int32_t)error->m_code]);
    }
}

/* lexer -> parser -> symbol table -> semantic errors -> byte code generator -> interpreter */

void zen_Compiler_buildAST(zen_Compiler_t* compiler) {
    zen_Lexer_t* lexer = zen_Lexer_new(compiler);
    zen_TokenStream_t* tokens = zen_TokenStream_new(compiler, lexer, ZEN_TOKEN_CHANNEL_DEFAULT);
    zen_Parser_t* parser = zen_Parser_new(compiler, tokens);
    zen_ASTPrinter_t* astPrinter = zen_ASTPrinter_new();
    zen_ASTListener_t* astPrinterASTListener = zen_ASTPrinter_getASTListener(astPrinter);

    int32_t size = jtk_ArrayList_getSize(compiler->m_inputFiles);
    int32_t i;
    for (i = 0; i < size; i++) {
        compiler->m_currentFileIndex = i;

        const uint8_t* path = (const uint8_t*)jtk_ArrayList_getValue(compiler->m_inputFiles, i);
        if (!jtk_PathHelper_exists(path)) {
            fprintf(stderr, "[error] Path '%s' does not exist.", path);
        }
        else {
            jtk_InputStream_t* stream = jtk_PathHelper_read(path);
            zen_Lexer_reset(lexer, stream);

            jtk_Logger_info(compiler->m_logger, "The lexical analysis phase has started.");

            int32_t previousLexicalErrors = zen_ErrorHandler_getErrorCount(compiler->m_errorHandler);
            zen_TokenStream_reset(tokens);
            zen_TokenStream_fill(tokens);
            if (compiler->m_dumpTokens) {
                zen_Compiler_printTokens(compiler, tokens);
            }
            int32_t currentLexicalErrors = zen_ErrorHandler_getErrorCount(compiler->m_errorHandler);

            jtk_Logger_info(compiler->m_logger, "The lexical analysis phase is complete.");


            /* Perform syntax analysis only if there are no lexical errors. */
            if (previousLexicalErrors == currentLexicalErrors) {
                jtk_Logger_info(compiler->m_logger, "The syntactical analysis phase has started.");

                zen_ASTNode_t* compilationUnit = zen_ASTNode_new(NULL);
                /* We do not have to reset the parser because the same token stream
                 * provides the tokens to the parser. Further, as of this writing,
                 * the parser actually does not maintain any internal state.
                 * If the parser is extended to maintain some kind of internal
                 * state, please make sure that the zen_Parser_reset() function
                 * is invoked.
                 */
                // zen_Parser_reset(parser, tokens);
                zen_Parser_compilationUnit(parser, compilationUnit);
                compiler->m_compilationUnits[i] = compilationUnit;

                jtk_Logger_info(compiler->m_logger, "The syntactical analysis phase is complete.");

                if (compiler->m_dumpNodes) {
                    zen_ASTWalker_walk(astPrinterASTListener, compilationUnit);
                }
            }

            jtk_InputStream_delete(stream);
        }
    }

    zen_ASTPrinter_delete(astPrinter);
    zen_Parser_delete(parser);
    zen_TokenStream_delete(tokens);
    zen_Lexer_delete(lexer);

    zen_Compiler_printLexicalErrors(compiler);
    zen_Compiler_printSyntaxErrors(compiler);
}

void zen_Compiler_analyze(zen_Compiler_t* compiler) {
    zen_SymbolDefinitionListener_t* symbolDefinitionListener = zen_SymbolDefinitionListener_new(compiler);
    zen_ASTListener_t* symbolDefinitionASTListener = zen_SymbolDefinitionListener_getASTListener(symbolDefinitionListener);

    zen_SymbolResolutionListener_t* symbolResolutionListener = zen_SymbolResolutionListener_new(compiler);
    zen_ASTListener_t* symbolResolutionASTListener = zen_SymbolResolutionListener_getASTListener(symbolResolutionListener);

    int32_t size = jtk_ArrayList_getSize(compiler->m_inputFiles);
    int32_t i;
    for (i = 0; i < size; i++) {
        compiler->m_currentFileIndex = i;
        zen_ASTNode_t* compilationUnit = compiler->m_compilationUnits[i];

        zen_SymbolTable_t* symbolTable = zen_SymbolTable_new(compiler);
        zen_ASTAnnotations_t* scopes = zen_ASTAnnotations_new();

        zen_SymbolDefinitionListener_setSymbolTable(symbolDefinitionListener, symbolTable);
        zen_SymbolDefinitionListener_setScopes(symbolDefinitionListener, scopes);

        jtk_Logger_info(compiler->m_logger, "Starting definition phase...");
        zen_ASTWalker_walk(symbolDefinitionASTListener, compilationUnit);
        jtk_Logger_info(compiler->m_logger, "The symbol definition phase is complete.");

        zen_SymbolResolutionListener_setSymbolTable(symbolResolutionListener, symbolTable);
        zen_SymbolResolutionListener_setScopes(symbolResolutionListener, scopes);

        jtk_Logger_info(compiler->m_logger, "Starting symbol resolution phase...");
        zen_ASTWalker_walk(symbolResolutionASTListener, compilationUnit);
        jtk_Logger_info(compiler->m_logger, "The symbol resolution phase is complete.");

        zen_SymbolResolutionListener_reset(symbolResolutionListener);
        zen_SymbolDefinitionListener_reset(symbolDefinitionListener);
    }

    zen_SymbolResolutionListener_delete(symbolResolutionListener);
    zen_SymbolDefinitionListener_delete(symbolDefinitionListener);

    zen_Compiler_printSemanticalErrors(compiler);
}

void zen_Compiler_generate(zen_Compiler_t* compiler) {
    zen_BinaryEntityGenerator_t* generator = zen_BinaryEntityGenerator_new(compiler);

    int32_t size = jtk_ArrayList_getSize(compiler->m_inputFiles);
    int32_t i;
    for (i = 0; i < size; i++) {
        compiler->m_currentFileIndex = i;

        jtk_Logger_info(compiler->m_logger, "Starting code generation phase...");

        zen_SymbolTable_t* symbolTable = compiler->m_symbolTables[i];
        zen_ASTAnnotations_t* scopes = compiler->m_scopes[i];
        zen_ASTNode_t* compilationUnit = compiler->m_compilationUnits[i];
        zen_BinaryEntityGenerator_reset(generator, symbolTable, scopes,
            compilationUnit, NULL);

        zen_BinaryEntityGenerator_generate(generator);

        jtk_Logger_info(compiler->m_logger, "The code generation phase is complete.");
    }

    /* The binary entity generator is not required anymore. Therefore, destroy
    * it and release the resources it holds.
    */
    zen_BinaryEntityGenerator_delete(generator);
}

bool zen_Compiler_compileEx(zen_Compiler_t* compiler, char** arguments, int32_t length) {
    jtk_Assert_assertObject(compiler, "The specified compiler is null.");

    bool invalidCommandLine = false;
    int32_t i;
    for (i = 1; i < length; i++) {
        if (arguments[i][0] == '-') {
            if (strcmp(arguments[i], "--internal-dump-tokens") == 0) {
                compiler->m_dumpTokens = true;
            }
            else if (strcmp(arguments[i], "--internal-dump-nodes") == 0) {
                compiler->m_dumpNodes = true;
            }
            else if (strcmp(arguments[i], "--internal-footprint") == 0) {
                compiler->m_footprint = true;
            }
            else if (strcmp(arguments[i], "--log") == 0) {
                if ((i + 1) < length) {
                    i++;
                    jtk_LogLevel_t level = JTK_LOG_LEVEL_NONE;
                    if (strcmp(arguments[i], "all") == 0) {
                        level = JTK_LOG_LEVEL_ALL;
                    }
                    else if (strcmp(arguments[i], "finest") == 0) {
                        level = JTK_LOG_LEVEL_FINEST;
                    }
                    else if (strcmp(arguments[i], "finer") == 0) {
                        level = JTK_LOG_LEVEL_FINER;
                    }
                    else if (strcmp(arguments[i], "fine") == 0) {
                        level = JTK_LOG_LEVEL_FINE;
                    }
                    else if (strcmp(arguments[i], "debug") == 0) {
                        level = JTK_LOG_LEVEL_DEBUG;
                    }
                    else if (strcmp(arguments[i], "configuration") == 0) {
                        level = JTK_LOG_LEVEL_CONFIGURATION;
                    }
                    else if (strcmp(arguments[i], "information") == 0) {
                        level = JTK_LOG_LEVEL_INFORMATION;
                    }
                    else if (strcmp(arguments[i], "warning") == 0) {
                        level = JTK_LOG_LEVEL_WARNING;
                    }
                    else if (strcmp(arguments[i], "severe") == 0) {
                        level = JTK_LOG_LEVEL_SEVERE;
                    }
                    else if (strcmp(arguments[i], "error") == 0) {
                        level = JTK_LOG_LEVEL_ERROR;
                    }
                    else if (strcmp(arguments[i], "none") == 0) {
                        level = JTK_LOG_LEVEL_NONE;
                    }
                    else {
                        printf("[error] Unknown log level '%s'\n", arguments[i]);
                        invalidCommandLine = true;
                    }

                    #ifdef JTK_LOGGER_DISABLE
                        printf("[warning] The logger was disabled at compile time. Please consider building Zen without the `JTK_LOGGER_DISABLE` constant in 'Configuration.h'.\n");
                    #else
                        jtk_Logger_setLevel(compiler->m_logger, level);
                    #endif
                }
                else {
                    printf("[error] The `--log` flag expects argument specifying log level.");
                    invalidCommandLine = true;
                }
            }
        }
        else {
            // jtk_String_t* path = jtk_String_new(arguments[i]);
            // jtk_ArrayList_add(compiler->m_inputFiles, path);
            jtk_ArrayList_add(compiler->m_inputFiles, arguments[i]);
        }
    }

    int32_t size = jtk_ArrayList_getSize(compiler->m_inputFiles);
    if (size == 0) {
        fprintf(stderr, "[error] Please specify input files.\n");
    }
    else {
        zen_Compiler_buildAST(compiler);
        if (!zen_ErrorHandler_hasErrors(compiler->m_errorHandler)) {
            zen_Compiler_analyze(compiler);

            if (!zen_ErrorHandler_hasErrors(compiler->m_errorHandler)) {
                zen_Compiler_generate(compiler);
            }
        }
    }

    for (i = 0; i < size; i++) {
        /* The ASTAnnotations that stores the scopes is not required anymore.
        * Therefore, destroy it and release the resources it holds.
        */
        zen_ASTAnnotations_delete(compiler->m_scopes[i]);

        /* The symbol table is not required anymore. Therefore, destroy it
         * and release the resources it holds.
         */
        zen_SymbolTable_delete(compiler->m_symbolTables[i]);

        zen_ASTNode_delete(compiler->m_compilationUnits[i]);
    }

    if (compiler->m_footprint) {
        int32_t footprint = zen_Memory_getFootprint();
        printf("Memory Footprint = %.2f KB\n", footprint / 1024.0f);
    }

    // TODO: Return true only if the compilation suceeded.
    return true;
}

bool zen_Compiler_compile(zen_Compiler_t* compiler) {
    return zen_Compiler_compileEx(compiler, NULL, -1);
}
