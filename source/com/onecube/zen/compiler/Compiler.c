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


#include <com/onecube/zen/compiler/symbol-table/SymbolResolutionListener.h>
#include <com/onecube/zen/compiler/symbol-table/ClassScope.h>
#include <com/onecube/zen/compiler/symbol-table/FunctionSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolDefinitionListener.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolTable.h>
#include <com/onecube/zen/compiler/symbol-table/FunctionScope.h>
#include <com/onecube/zen/compiler/symbol-table/ConstantSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/LocalScope.h>
#include <com/onecube/zen/compiler/symbol-table/VariableSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/LabelSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/ClassSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/EnumerationSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/EnumerationScope.h>
#include <com/onecube/zen/compiler/symbol-table/CompilationUnitScope.h>
#include <com/onecube/zen/compiler/symbol-table/EnumerateSymbol.h>

#include <com/onecube/zen/compiler/support/ErrorHandler.h>

#include <com/onecube/zen/virtual-machine/feb/Instruction.h>

// Token

void zen_Compiler_printToken(zen_Token_t* token) {
    printf("[%d-%d:%d-%d:%s:%s]", token->m_startLine, token->m_stopLine, token->m_startColumn + 1, token->m_stopColumn + 1, token->m_channel == ZEN_TOKEN_CHANNEL_DEFAULT? "default" : "hidden", zen_Lexer_getLiteralName(token->m_type));
    zen_TokenType_t type = zen_Token_getType(token);
    if ((type == ZEN_TOKEN_IDENTIFIER) || (type == ZEN_TOKEN_INTEGER_LITERAL) ||
        (type == ZEN_TOKEN_STRING_LITERAL)) {
        printf(" %.*s", token->m_length, token->m_text);
    }
    puts("");
}

void zen_Compiler_zen_Compiler_printTokens(zen_Compiler_t* compiler, jtk_ArrayList_t* tokens) {
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
        zen_Compiler_printToken(token);
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
    compiler->m_dumpInstructions = false;
    compiler->m_inputFiles = jtk_ArrayList_new();
    compiler->m_currentFileIndex = -1;
    compiler->m_errorHandler = zen_ErrorHandler_new();
    compiler->m_compilationUnits = NULL;
    compiler->m_symbolTables = NULL;
    compiler->m_scopes = NULL;
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

    if (compiler->m_compilationUnits != NULL) {
        jtk_Memory_deallocate(compiler->m_compilationUnits);
    }

    if (compiler->m_symbolTables != NULL) {
        jtk_Memory_deallocate(compiler->m_symbolTables);
    }

    if (compiler->m_scopes != NULL) {
        jtk_Memory_deallocate(compiler->m_scopes);
    }

#ifndef JTK_LOGGER_DISABLE
    jtk_Logger_delete(compiler->m_logger);
#endif
    jtk_ArrayList_delete(compiler->m_inputFiles);
    jtk_Memory_deallocate(compiler);
}

void zen_Compiler_onLexerError(zen_LexerError_t* error) {
    fprintf(stderr, "[error] %s:%d:%d -- %s\n", error->m_path, error->m_line, error->m_column, error->m_message);
    fflush(stderr);
}

const uint8_t* zen_ErrorCode_messages[] = {
    "None",

    // Lexical Errors

    "Unterminated string literal",
    "Unterminated multi-line comment",
    "Malformed unicode character sequence; expected four hexadecimal digits",
    "Invalid escape sequence",
    "Unknown character",
    "Invalid prefix in integer literal",
    "Expected digit after underscore in integer literal",

    // Syntactical Errors

    "Unexpected token",
    "The try statement expects at least one catch or finally clause.",

    // Semantical Errors

    "Undeclared class",
    "Variable required on left-hand side of assignment",
    "Name refers to a non-class symbol",
    "No suitable constructor found",
    "Undeclared identifier",
    "Variable treated as function",
    "Static initializer with parameters",
    "Redeclaration of symbol as function"
};

void zen_Compiler_printErrors(zen_Compiler_t* compiler) {
    jtk_ArrayList_t* errors = zen_ErrorHandler_getErrors(compiler->m_errorHandler);
    int32_t errorCount = jtk_ArrayList_getSize(errors);
    int32_t i;
    for (i = 0; i < errorCount; i++) {
        zen_Error_t* error = (zen_Error_t*)jtk_ArrayList_getValue(errors, i);
        zen_Token_t* token = error->m_token;
        const char* message = zen_ErrorCode_messages[(int32_t)error->m_code];
        char message0[128];
        if (error->m_expected != ZEN_TOKEN_UNKNOWN) {
            const uint8_t* expectedName = zen_Lexer_getLiteralName(error->m_expected);
            const uint8_t* actualName = zen_Lexer_getLiteralName(token->m_type);
            sprintf(message0, "Expected token '%s', encountered token '%s'",
                expectedName, actualName);
            message = message0;
        }
        fprintf(stderr, "\033[1;31m[error]\033[0m %d-%d:%d-%d: %s\n", token->m_startLine,
            token->m_stopLine, token->m_startColumn, token->m_stopColumn,
            message);
    }
}

void zen_Compiler_initialize(zen_Compiler_t* compiler) {
    int32_t size = jtk_ArrayList_getSize(compiler->m_inputFiles);
    compiler->m_compilationUnits = jtk_Memory_allocate(zen_ASTNode_t*, size);
    compiler->m_symbolTables = jtk_Memory_allocate(zen_SymbolTable_t*, size);
    compiler->m_scopes = jtk_Memory_allocate(zen_ASTAnnotations_t*, size);
}

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
                zen_Compiler_zen_Compiler_printTokens(compiler, tokens->m_tokens);
            }
            int32_t currentLexicalErrors = zen_ErrorHandler_getErrorCount(compiler->m_errorHandler);

            jtk_Logger_info(compiler->m_logger, "The lexical analysis phase is complete.");

            /* Perform syntax analysis for the current input source file only if
             * there are no lexical errors.
             */
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

    zen_Compiler_printErrors(compiler);
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

        jtk_Logger_info(compiler->m_logger, "Starting definition phase...");
        zen_SymbolDefinitionListener_reset(symbolDefinitionListener, symbolTable, scopes);
        zen_ASTWalker_walk(symbolDefinitionASTListener, compilationUnit);
        jtk_Logger_info(compiler->m_logger, "The symbol definition phase is complete.");

        jtk_Logger_info(compiler->m_logger, "Starting symbol resolution phase...");
        zen_SymbolResolutionListener_reset(symbolResolutionListener, symbolTable, scopes);
        zen_ASTWalker_walk(symbolResolutionASTListener, compilationUnit);
        jtk_Logger_info(compiler->m_logger, "The symbol resolution phase is complete.");

        compiler->m_symbolTables[i] = symbolTable;
        compiler->m_scopes[i] = scopes;
    }

    zen_SymbolResolutionListener_delete(symbolResolutionListener);
    zen_SymbolDefinitionListener_delete(symbolDefinitionListener);

    zen_Compiler_printErrors(compiler);
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

void zen_Compiler_destroyNestedScopes(zen_ASTAnnotations_t* annotations) {
    jtk_Assert_assertObject(annotations, "The specified annotations is null.");

    /* There are three algorithms that can help us in the destruction of the
     * symbols and scopes.
     * 1. The symbols are destructed by their enclosing scopes when the scopes
     *    themselves are destroyed.
     * 2. The compilation unit scope can be queried for children. These children
     *    are destroyed in a recursive fashion. During each recursion a symbol
     *    is tested to see if it also represents a scope. In which case,
     *    the scope is queried for its children. The scope part of the symbol
     *    is also destroyed here. The cycle repeats until there are no symbols
     *    and scopes left to destroy.
     * 3. The AST annotations are iterated over to retrieve the scopes.
     *    In each iteration, the children symbols are retrieved and destroyed.
     *    It should be noted that the scopes are also destroyed here.
     *
     * Here, I have chosen to implement the third algorithm due to certain
     * circumstances.
     */

    jtk_ArrayList_t* temporary = jtk_ArrayList_new();
    jtk_Iterator_t* iterator = jtk_HashMap_getValueIterator(annotations->m_map);
    while (jtk_Iterator_hasNext(iterator)) {
        /* The scopes are created during the definition phase of the symbol table.
         * Therefore, we destroy them here before destroying the
         * jtk_SymbolDefinitionListener_t class instance itself.
         */
        zen_Scope_t* scope = (zen_Scope_t*)jtk_Iterator_getNext(iterator);

        /* Retrieve the children symbols declared in the current scope. */
        zen_Scope_getChildrenSymbols(scope, temporary);

        /* Iterate over the children symbols and destroy them. */
        int32_t limit = jtk_ArrayList_getSize(temporary);
        int32_t i;
        for (i = 0; i < limit; i++) {
            zen_Symbol_t* symbol = (zen_Symbol_t*)jtk_ArrayList_getValue(temporary, i);
            zen_Compiler_destroySymbol(symbol);
        }

        /* At this point, the symbols retrieved form the scope are destroyed.
         * Therefore, remove references to them from the temporary list.
         */
        jtk_ArrayList_clear(temporary);

        /* Destroy the current scope. */
        zen_Compiler_destroyScope(scope);
    }
    jtk_Iterator_delete(iterator);
    jtk_ArrayList_delete(temporary);

    zen_ASTAnnotations_delete(annotations);
}

void zen_Compiler_destroySymbol(zen_Symbol_t* symbol) {
    /* I could have designed the zen_Symbol_t and its "descendant" classes that
     * implement specific symbols like the jtk_Iterator_t and its "descendant"
     * classes such as jtk_ArrayListIterator_t. This would have helped
     * us delete specific symbols by simply invoking the zen_Symbol_delete()
     * function. However, to implement such a design would require me to
     * modify a few files for which I can spare no time. Further, the likelihood
     * of introducing new symbol categories is very rare. Therefore, I have chosen
     * to manually cast the context of the symbol before invoking a specific
     * destructor.
     *
     * NOTE: If a new symbol category is added and its instance is created during
     * the definition phase to the symbol table, then please add a respective
     * destructor call here.
     */
    zen_SymbolCategory_t category = zen_Symbol_getCategory(symbol);
    switch (category) {
        /* TODO: Should annotations simply be classes? In which case the
         * ZEN_SYMBOL_CATEGORY_ANNOTATION would be useless.
         */

        case ZEN_SYMBOL_CATEGORY_CLASS: {
            zen_ClassSymbol_t* classSymbol = (zen_ClassSymbol_t*)symbol->m_context;
            zen_ClassSymbol_delete(classSymbol);

            // printf("[debug] Destroyed a class symbol.\n");

            break;
        }

        // case ZEN_SYMBOL_CATEGORY_ENUMERATE: {
        //     zen_EnumerateSymbol_t* enumerateSymbol = (zen_EnumerateSymbol_t*)symbol->m_context;
        //     zen_EnumerateSymbol_delete(enumerateSymbol);

        //     // printf("[debug] Destroyed a enumerate symbol.\n");

        //     break;
        // }

        // case ZEN_SYMBOL_CATEGORY_ENUMERATION: {
        //     zen_EnumerationSymbol_t* enumerationSymbol = (zen_EnumerationSymbol_t*)symbol->m_context;
        //     zen_EnumerationSymbol_delete(enumerationSymbol);

        //     // printf("[debug] Destroyed a enumeration symbol.\n");

        //     break;
        // }

        case ZEN_SYMBOL_CATEGORY_FUNCTION: {
            zen_FunctionSymbol_t* functionSymbol = (zen_FunctionSymbol_t*)symbol->m_context;
            zen_FunctionSymbol_delete(functionSymbol);

            // printf("[debug] Destroyed a function symbol.\n");

            break;
        }

        case ZEN_SYMBOL_CATEGORY_CONSTANT: {
            zen_ConstantSymbol_t* constantSymbol = (zen_ConstantSymbol_t*)symbol->m_context;
            zen_ConstantSymbol_delete(constantSymbol);

            // printf("[debug] Destroyed a constant symbol.\n");

            break;
        }

        case ZEN_SYMBOL_CATEGORY_VARIABLE: {
            zen_VariableSymbol_t* variableSymbol = (zen_VariableSymbol_t*)symbol->m_context;
            zen_VariableSymbol_delete(variableSymbol);

            // printf("[debug] Destroyed a variable symbol.\n");

            break;
        }

        case ZEN_SYMBOL_CATEGORY_LABEL: {
            zen_LabelSymbol_t* labelSymbol = (zen_LabelSymbol_t*)symbol->m_context;
            zen_LabelSymbol_delete(labelSymbol);

            // printf("[debug] Destroyed a label symbol.\n");

            break;
        }
    }
}

void zen_Compiler_destroyScope(zen_Scope_t* scope) {
    /* I could have designed the zen_Scope_t and its "descendant" classes that
     * implement specific scopes like the jtk_Iterator_t and its "descendant"
     * classes such as jtk_ArrayListIterator_t. This would have helped
     * us delete specific scopes by simply invoking the zen_Scope_delete()
     * function. However, to implement such a design would require me to
     * modify a few files for which I can spare no time. Further, the likelihood
     * of introducing new scope types is very rare. Therefore, I have chosen
     * to manually cast the context of the scope before invoking a specific
     * destructor.
     *
     * Since the jtk_HashMap_t class does not store the order in which the
     * scopes were inserted, the scopes are deleted in "random" order. With
     * the currently available scopes, this behavior should not really affect
     * the program in unexpected ways.
     *
     * NOTE: If a new scope type is added and its instance is created during
     * the definition phase to the symbol table, then please add a respective
     * destructor call here.
     */
    zen_ScopeType_t type = zen_Scope_getType(scope);
    switch (type) {
        /* TODO: Should annotations simply be classes? In which case the
         * ZEN_SCOPE_ANNOTATION would be useless.
         */

        case ZEN_SCOPE_COMPILATION_UNIT: {
            zen_CompilationUnitScope_t* compilationUnitScope =
                (zen_CompilationUnitScope_t*)scope->m_context;
            zen_CompilationUnitScope_delete(compilationUnitScope);

            // printf("[debug] Compilation unit scope deleted.\n");

            break;
        }


        // case ZEN_SCOPE_ENUMERATION: {
        //     zen_EnumerationScope_t* enumerationScope =
        //         (zen_EnumerationScope_t*)scope->m_context;
        //     zen_EnumerationScope_delete(enumerationScope);

        //     // printf("[debug] Enumeration scope deleted.\n");

        //     break;
        // }

        case ZEN_SCOPE_FUNCTION: {
            zen_FunctionScope_t* functionScope =
                (zen_FunctionScope_t*)scope->m_context;
            zen_FunctionScope_delete(functionScope);

            // printf("[debug] Function scope deleted.\n");

            break;
        }

        case ZEN_SCOPE_CLASS: {
            zen_ClassScope_t* classScope =
                (zen_ClassScope_t*)scope->m_context;
            zen_ClassScope_delete(classScope);

            // printf("[debug] Class scope deleted.\n");

            break;
        }

        case ZEN_SCOPE_LOCAL: {
            zen_LocalScope_t* localScope =
                (zen_LocalScope_t*)scope->m_context;
            zen_LocalScope_delete(localScope);

            // printf("[debug] Local scope deleted.\n");

            break;
        }
    }
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
            else if (strcmp(arguments[i], "--internal-dump-instructions") == 0) {
                compiler->m_dumpInstructions = true;
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
        zen_Compiler_initialize(compiler);
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
        if (compiler->m_scopes[i] != NULL) {
            zen_Compiler_destroyNestedScopes(compiler->m_scopes[i]);
        }

        /* The symbol table is not required anymore. Therefore, destroy it
         * and release the resources it holds.
         */
        if (compiler->m_symbolTables[i] != NULL) {
            zen_SymbolTable_delete(compiler->m_symbolTables[i]);
        }

        if (compiler->m_compilationUnits[i] != NULL) {
            zen_ASTNode_delete(compiler->m_compilationUnits[i]);
        }
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
