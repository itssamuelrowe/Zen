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
#include <jtk/collection/array/Arrays.h>
#include <jtk/fs/FileInputStream.h>
#include <jtk/fs/Path.h>
#include <jtk/io/BufferedInputStream.h>
#include <jtk/io/InputStream.h>
#include <jtk/log/ConsoleLogger.h>
#include <jtk/core/CStringObjectAdapter.h>
#include <jtk/core/CString.h>

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
#include <com/onecube/zen/compiler/symbol-table/FunctionSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolDefinitionListener.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolTable.h>

#include <com/onecube/zen/compiler/support/ErrorHandler.h>

#include <com/onecube/zen/virtual-machine/feb/Instruction.h>

// Register

void zen_Compiler_registerSymbol(zen_Compiler_t* compiler, const uint8_t* identifier,
    int32_t identifierSize, zen_Symbol_t* symbol) {
    // uint8_t* copy = jtk_CString_newEx(identifier, identifierSize);
    /* NOTE: Do not create a copy of the qualified name because the string belongs
     * to the symbol we are mapping. Therefore, the lifetime of both the qualified
     * name and the symbol are equivalent.
     */
    jtk_HashMap_put(compiler->m_repository, identifier, symbol);
}

zen_Symbol_t* zen_Compiler_resolveSymbol(zen_Compiler_t* compiler,
    const uint8_t* name, int32_t nameSize) {
    zen_Symbol_t* result = jtk_HashMap_getValue(compiler->m_repository,
        name);
    if (result == NULL) {
        result = zen_SymbolLoader_findSymbol(compiler->m_symbolLoader,
            name, nameSize);
    }
    return result;
}

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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <jtk/core/CString.h>

bool jtk_PathHelper_exists(const uint8_t* path) {
    /*
    struct stat buffer;
    int32_t result = fstat(path, &buffer);
    return result == 0 && S_ISREG(buffer.st_mode);
    */
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

uint8_t* jtk_PathHelper_getParent(const uint8_t* path, int32_t size,
    int32_t* resultSize) {
    size = size < 0? jtk_CString_getSize(path) : size;

    int32_t index = jtk_CString_findLast_c(path, size, JTK_PATH_ELEMENT_SEPARATOR);
    if (resultSize != NULL) {
        *resultSize = index < 0? 0 : index;
    }
    return index < 0? NULL : jtk_CString_substringEx(path, size, 0, index);
}

/******************************************************************************
 * Compiler                                                                   *
 ******************************************************************************/

// Constructor

zen_Compiler_t* zen_Compiler_new() {
    jtk_ObjectAdapter_t* stringObjectAdapter = jtk_CStringObjectAdapter_getInstance();

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
    compiler->m_packages = NULL;
    compiler->m_packageSizes = NULL;
    compiler->m_symbolLoader = zen_SymbolLoader_new(compiler);
    compiler->m_repository = jtk_HashMap_new(stringObjectAdapter, NULL);
    compiler->m_trash = NULL;
    compiler->m_coreApi = false;
    compiler->m_disassembler = zen_BinaryEntityDisassembler_new(NULL);
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

    if (compiler->m_trash != NULL) {
        int32_t size = jtk_ArrayList_getSize(compiler->m_trash);
        int32_t i;
        for (i = 0; i < size; i++) {
            /* The ownership of the tokens produced by the lexer
            * is transfered to the token stream which demanded
            * their creation. Therefore, the token stream
            * has to destroy the buffered tokens.
            */
            zen_Token_t* token = (zen_Token_t*)jtk_ArrayList_getValue(compiler->m_trash, i);
            zen_Token_delete(token);
        }
        jtk_ArrayList_delete(compiler->m_trash);
    }

    if (compiler->m_packages != NULL) {
        int32_t i;
        int32_t inputCount = jtk_ArrayList_getSize(compiler->m_inputFiles);
        for (i = 0; i < inputCount; i++) {
            jtk_CString_delete(compiler->m_packages[i]);
        }
        jtk_Memory_deallocate(compiler->m_packages);
        jtk_Memory_deallocate(compiler->m_packageSizes);
    }

    jtk_Iterator_t* iterator = jtk_HashMap_getKeyIterator(compiler->m_repository);
    while (jtk_Iterator_hasNext(iterator)) {
        uint8_t* key = (uint8_t*)jtk_Iterator_getNext(iterator);
        jtk_CString_delete(key);
    }
    jtk_Iterator_delete(iterator);
    jtk_HashMap_delete(compiler->m_repository);

    zen_ErrorHandler_delete(compiler->m_errorHandler);

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
    "Redeclaration of symbol as function",
    "Redeclaration of symbol as parameter",
    "Redeclaration of symbol as variable parameter",
    "Multiple function overloads with variable parameter",
    "Duplicate function overload",
    "Function declaration exceeds parameter threshold",
    "Redeclaration of symbol as variable",
    "Redeclaration of symbol as constant",
    "Redeclaration of symbol as label",
    "Redeclaration of symbol as loop parameter",
    "Redeclaration of symbol as catch parameter",
    "Redeclaration of symbol as class",
    "Unknown class",
    "Redeclaration of symbol previously imported",
    "Cannot declare function and class in the same compilation unit",
    "Function declaration causes another function to exceed parameter threshold",

    // General Errors

    "Corrupted binary entity",
    "Binary entity encoded in unrecognizable FEB version"
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

        char lineNumbers[100];
        if (token->m_startLine != token->m_stopLine) {
            sprintf(lineNumbers, "%d-%d", token->m_startLine, token->m_stopLine);
        }
        else {
            sprintf(lineNumbers, "%d", token->m_startLine);
        }

        if (error->m_expected != ZEN_TOKEN_UNKNOWN) {
            const uint8_t* expectedName = zen_Lexer_getLiteralName(error->m_expected);
            const uint8_t* actualName = zen_Lexer_getLiteralName(token->m_type);
            sprintf(message0, "Expected token '%s', encountered token '%s'",
                expectedName, actualName);
            message = message0;
        }
        fprintf(stderr, "\033[1;31m[error]\033[0m %s:%s:%d-%d: %s\n",
            token->m_file, lineNumbers, token->m_startColumn,
            token->m_stopColumn, message);
    }
}

void zen_Compiler_initialize(zen_Compiler_t* compiler) {
    int32_t size = jtk_ArrayList_getSize(compiler->m_inputFiles);
    compiler->m_compilationUnits = jtk_Memory_allocate(zen_ASTNode_t*, size);
    compiler->m_symbolTables = jtk_Memory_allocate(zen_SymbolTable_t*, size);
    compiler->m_scopes = jtk_Memory_allocate(zen_ASTAnnotations_t*, size);
    compiler->m_packages = jtk_Memory_allocate(uint8_t*, size);
    compiler->m_packageSizes = jtk_Memory_allocate(int32_t, size);
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
            fprintf(stderr, "[error] Path '%s' does not exist.\n", path);
        }
        else {
            int32_t packageSize;
            uint8_t* package = jtk_PathHelper_getParent(path, -1, &packageSize);
            compiler->m_packages[i] = package;
            compiler->m_packageSizes[i] = packageSize;
            jtk_Arrays_replace_b(package, packageSize, '/', '.');

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
                zen_Parser_reset(parser, tokens);
                zen_Parser_compilationUnit(parser, compilationUnit);
                compiler->m_compilationUnits[i] = compilationUnit;

                jtk_Logger_info(compiler->m_logger, "The syntactical analysis phase is complete.");

                if (compiler->m_dumpNodes) {
                    zen_ASTWalker_walk(astPrinterASTListener, compilationUnit);
                }
            }

            jtk_InputStream_destroy(stream);
        }
    }

    compiler->m_trash = tokens->m_trash;

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
        zen_SymbolDefinitionListener_reset(symbolDefinitionListener, symbolTable,
            scopes, compiler->m_packages[i], compiler->m_packageSizes[i]);
        zen_ASTWalker_walk(symbolDefinitionASTListener, compilationUnit);
        jtk_Logger_info(compiler->m_logger, "The symbol definition phase is complete.");

        compiler->m_symbolTables[i] = symbolTable;
        compiler->m_scopes[i] = scopes;
    }

    for (i = 0; i < size; i++) {
        compiler->m_currentFileIndex = i;
        zen_ASTNode_t* compilationUnit = compiler->m_compilationUnits[i];

        zen_SymbolTable_t* symbolTable = compiler->m_symbolTables[i];
        zen_ASTAnnotations_t* scopes = compiler->m_scopes[i];

        jtk_Logger_info(compiler->m_logger, "Starting symbol resolution phase...");
        zen_SymbolResolutionListener_reset(symbolResolutionListener, symbolTable, scopes);
        zen_ASTWalker_walk(symbolResolutionASTListener, compilationUnit);
        jtk_Logger_info(compiler->m_logger, "The symbol resolution phase is complete.");
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
            compilationUnit, compiler->m_packages[i], compiler->m_packageSizes[i],
            NULL);

        zen_BinaryEntityGenerator_generate(generator);

        jtk_Logger_info(compiler->m_logger, "The code generation phase is complete.");
    }

    /* The binary entity generator is not required anymore. Therefore, destroy
    * it and release the resources it holds.
    */
    zen_BinaryEntityGenerator_delete(generator);
}

void zen_Compiler_destroyNestedScopes(zen_ASTAnnotations_t* scopes) {
    jtk_Assert_assertObject(scopes, "The specified annotations is null.");

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
    jtk_Iterator_t* iterator = jtk_HashMap_getValueIterator(scopes->m_map);
    while (jtk_Iterator_hasNext(iterator)) {
        /* The scopes are created during the definition phase of the symbol table.
         * Therefore, we destroy them here.
         */
        zen_Scope_t* scope = (zen_Scope_t*)jtk_Iterator_getNext(iterator);

        /* Retrieve the children symbols declared in the current scope. */
        zen_Scope_getChildrenSymbols(scope, temporary);

        /* Iterate over the children symbols and destroy them. */
        int32_t limit = jtk_ArrayList_getSize(temporary);
        int32_t i;
        for (i = 0; i < limit; i++) {
            zen_Symbol_t* symbol = (zen_Symbol_t*)jtk_ArrayList_getValue(temporary, i);
            // zen_Symbol_delete(symbol);
        }

        /* At this point, the symbols retrieved form the scope are destroyed.
         * Therefore, remove references to them from the temporary list.
         */
        jtk_ArrayList_clear(temporary);

        /* Destroy the current scope. */
        zen_Scope_delete(scope);
    }
    jtk_Iterator_delete(iterator);
    jtk_ArrayList_delete(temporary);
    zen_ASTAnnotations_delete(scopes);
}

jtk_ArrayList_t* zen_CString_split_c(const uint8_t* sequence, int32_t size,
    uint8_t value, bool inclusive) {
    jtk_ArrayList_t* result = jtk_ArrayList_new();
    int32_t i;
    for (i = 0; i < size; i++) {
        int32_t startIndex = i;
        while ((i < size) && (sequence[i] != value)) {
            i++;
        }
        int32_t stopIndex = ((i < size) && inclusive) ? i + 1 : i;
        uint8_t* substring = jtk_CString_substringEx(sequence, size, startIndex,
            stopIndex);
        jtk_ArrayList_add(result, substring);
    }
    return result;
}

int32_t zen_ZenVirtualMachine_main(char** arguments, int32_t length);

void zen_Compiler_printHelp() {
    printf(
        "[Usage]\n"
        "    zc [--tokens] [--nodes] [--footprint] [--instructions] [--core-api] [--log <level>] [--help] <inputFiles> [--run <vmArguments>]\n\n"
        "[Options]\n"
        "    --tokens        Print the tokens recognized by the lexer.\n"
        "    --nodes         Print the AST recognized by the parser.\n"
        "    --footprint     Print diagnostic information about the memory footprint of the compiler.\n"
        "    --instructions  Disassemble the binary entity generated.\n"
        "    --core-api      Disables the internal constant pool function index cache. This flag is valid only when compiling foreign function interfaces.\n"
        "    --run           Run the virtual machine after compiling the source files.\n"
        "    --log           Generate log messages. This flag is valid only if log messages were enabled at compile time.\n"
        "    --help          Print the help message.\n"
        );
}

bool zen_Compiler_compileEx(zen_Compiler_t* compiler, char** arguments, int32_t length) {
    jtk_Assert_assertObject(compiler, "The specified compiler is null.");

    // TODO: Add the --path flag
    zen_SymbolLoader_addDirectory(compiler->m_symbolLoader, ".");
    zen_BinaryEntityDisassembler_addDirectory(compiler->m_disassembler, ".", 1);

    char** vmArguments = NULL;
    int32_t vmArgumentsSize = 0;

    bool invalidCommandLine = false;
    int32_t i;
    for (i = 1; i < length; i++) {
        if (arguments[i][0] == '-') {
            if (strcmp(arguments[i], "--tokens") == 0) {
                compiler->m_dumpTokens = true;
            }
            else if (strcmp(arguments[i], "--nodes") == 0) {
                compiler->m_dumpNodes = true;
            }
            else if (strcmp(arguments[i], "--footprint") == 0) {
                compiler->m_footprint = true;
            }
            else if (strcmp(arguments[i], "--instructions") == 0) {
                compiler->m_dumpInstructions = true;
            }
            else if (strcmp(arguments[i], "--core-api") == 0) {
                compiler->m_coreApi = true;
            }
            else if (strcmp(arguments[i], "--run") == 0) {
                vmArgumentsSize = length - i + 1;
                if (vmArgumentsSize > 0) {
                    vmArguments = arguments + (i + 1);
                    break;
                }
                else {
                    printf("[error] Please specify the main class.");
                    invalidCommandLine = true;
                }
            }
            else if (strcmp(arguments[i], "--help") == 0) {
                zen_Compiler_printHelp();
                exit(0);
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
            else {
                printf("[error] Unknown flag `%s`\n", arguments[i]);
            }
        }
        else {
            // jtk_String_t* path = jtk_String_new(arguments[i]);
            // jtk_ArrayList_add(compiler->m_inputFiles, path);
            jtk_ArrayList_add(compiler->m_inputFiles, arguments[i]);
        }
    }

    int32_t size = jtk_ArrayList_getSize(compiler->m_inputFiles);
    bool noErrors = false;
    if (size == 0) {
        fprintf(stderr, "[error] Please specify input files.\n");
    }
    else {
        zen_Compiler_initialize(compiler);
        zen_Compiler_buildAST(compiler);
        if ((noErrors = !zen_ErrorHandler_hasErrors(compiler->m_errorHandler))) {
            zen_Compiler_analyze(compiler);

            if ((noErrors = !zen_ErrorHandler_hasErrors(compiler->m_errorHandler))) {
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
    }

    for (i = 0; i < size; i++) {
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

    if ((vmArguments != NULL) && noErrors) {
        zen_ZenVirtualMachine_main(vmArguments, vmArgumentsSize);
    }

    // TODO: Return true only if the compilation suceeded.
    return true;
}

bool zen_Compiler_compile(zen_Compiler_t* compiler) {
    return zen_Compiler_compileEx(compiler, NULL, -1);
}
