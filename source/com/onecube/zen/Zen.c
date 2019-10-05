/*
 * Copyright 2018-2019 OneCube
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

#include <stdio.h>
#include <string.h>

#include <jtk/collection/list/ArrayList.h>
#include <jtk/fs/FileInputStream.h>
#include <jtk/io/BufferedInputStream.h>
#include <jtk/io/InputStream.h>

#include <com/onecube/zen/Configuration.h>
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

#include <com/onecube/zen/virtual-machine/feb/Instruction.h>

/* lexer -> parser -> symbol table -> semantic errors -> byte code generator -> interpreter */

void printToken(zen_Token_t* token) {
    printf("[%d-%d:%d-%d:%s:%s]", token->m_startLine + 1, token->m_stopLine + 1, token->m_startColumn + 1, token->m_stopColumn + 1, token->m_channel == ZEN_TOKEN_CHANNEL_DEFAULT? "default" : "hidden", zen_Lexer_getLiteralName(token->m_type));
    zen_TokenType_t type = zen_Token_getType(token);
    if ((type == ZEN_TOKEN_IDENTIFIER) || (type == ZEN_TOKEN_INTEGER_LITERAL) ||
        (type == ZEN_TOKEN_STRING_LITERAL)) {
        printf(" %.*s", token->m_length, token->m_text);
    }
    puts("");
}

bool jtk_Object_isNull(void* object) {
    return object == NULL;
}

bool jtk_Object_isNotNull(void* object) {
    return object != NULL;
}

void printTokens(zen_TokenStream_t* stream) {
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

void zen_Interpreter_onLexerError(zen_LexerError_t* error) {
    fprintf(stderr, "[error] %s:%d:%d -- %s\n", error->m_path, error->m_line, error->m_column, error->m_message);
    fflush(stderr);
}

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

int32_t main(int32_t length, char** arguments) {
    // jtk_Assert_assertTrue(zen_Instruction_verify(), "The instruction set is invalid.");

    jtk_ArrayList_t* inputFiles = jtk_ArrayList_new();
    bool internalDumpTokens = false;
    bool internalDumpNodes = false;
    bool internalFootprint = false;

    int32_t i;
    for (i = 1; i < length; i++) {
        if (arguments[i][0] == '-') {
            if (strcmp(arguments[i], "--internal-dump-tokens") == 0) {
                internalDumpTokens = true;
            }
            else if (strcmp(arguments[i], "--internal-dump-nodes") == 0) {
                internalDumpNodes = true;
            }
            else if (strcmp(arguments[i], "--internal-footprint") == 0) {
                internalFootprint = true;
            }
        }
        else {
            jtk_ArrayList_add(inputFiles, arguments[i]);
        }
    }

    int32_t size = jtk_ArrayList_getSize(inputFiles);
    if (size == 0) {
        fprintf(stderr, "[error] Please specify input files.\n");
    }
    else {
        int32_t successful = 0;
        int32_t failure = 0;
        for (i = 0; i < size; i++) {
            const char* path = (const char*)jtk_ArrayList_getValue(inputFiles, i);
            if (!jtk_PathHelper_exists(path)) {
                fprintf(stderr, "[error] Path '%s' does not exist.", path);
            }
            else {
                jtk_InputStream_t* stream = jtk_PathHelper_read(path);
                zen_Lexer_t* lexer = zen_Lexer_new(stream);
                zen_TokenStream_t* tokens = zen_TokenStream_new(lexer, ZEN_TOKEN_CHANNEL_DEFAULT);

                if (internalDumpTokens) {
                    zen_TokenStream_fill(tokens);
                    printTokens(tokens);
                }

                zen_Parser_t* parser = zen_Parser_new(tokens);

                zen_ASTNode_t* compilationUnit = zen_ASTNode_new(NULL);
                zen_Parser_compilationUnit(parser, compilationUnit);

                if (internalDumpNodes) {
                    zen_ASTPrinter_t* astPrinter = zen_ASTPrinter_new();
                    zen_ASTListener_t* astPrinterASTListener = zen_ASTPrinter_getASTListener(astPrinter);
                    zen_ASTWalker_walk(astPrinterASTListener, compilationUnit);
                    zen_ASTPrinter_delete(astPrinter);
                }

                zen_SymbolTable_t* symbolTable = zen_SymbolTable_new();
                zen_ASTAnnotations_t* scopes = zen_ASTAnnotations_new();

                zen_SymbolDefinitionListener_t* symbolDefinitionListener = zen_SymbolDefinitionListener_new(symbolTable, scopes);
                zen_ASTListener_t* symbolDefinitionASTListener = zen_SymbolDefinitionListener_getASTListener(symbolDefinitionListener);
                zen_ASTWalker_walk(symbolDefinitionASTListener, compilationUnit);

                zen_SymbolResolutionListener_t* symbolResolutionListener = zen_SymbolResolutionListener_new(symbolTable, scopes);
                zen_ASTListener_t* symbolResolutionASTListener = zen_SymbolResolutionListener_getASTListener(symbolResolutionListener);
                zen_ASTWalker_walk(symbolResolutionASTListener, compilationUnit);

                // zen_BinaryEntityBuilder_t* entityBuilder = zen_BinaryEntityBuilder_new(symbolTable, scopes);
                // zen_BinaryEntityBuilder_build(entityBuilder, compilationUnit);

                zen_BinaryEntityGenerator_t* generator = zen_BinaryEntityGenerator_newEx(symbolTable, scopes, compilationUnit, NULL);
                zen_BinaryEntityGenerator_generate(generator);

                /* The binary entity generator is not required anymore. Therefore, destroy
                 * it and release the resources it holds.
                 */
                zen_BinaryEntityGenerator_delete(generator);

                // zen_BinaryEntityBuilder_delete(entityBuilder);
                zen_SymbolDefinitionListener_delete(symbolDefinitionListener);

                /* The ASTAnnotations that stores the scopes is not required anymore.
                 * Therefore, destroy it and release the resources it holds.
                 */
                zen_ASTAnnotations_delete(scopes);

                /* The symbol table is not required anymore. Therefore, destroy it
                 * and release the resources it holds.
                 */
                zen_SymbolTable_delete(symbolTable);

                zen_ASTNode_delete(compilationUnit);
                zen_Parser_delete(parser);
                zen_TokenStream_delete(tokens);
                zen_Lexer_delete(lexer);
                jtk_InputStream_delete(stream);
            }
        }
    }

    jtk_ArrayList_delete(inputFiles);

    if (internalFootprint) {
        int32_t footprint = zen_Memory_getFootprint();
        printf("Memory Footprint = %.2f KB\n", footprint / 1024.0f);
    }

    return 0;
}