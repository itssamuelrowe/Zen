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

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/InputStream.h>
#include <com/onecube/zen/Lexer.h>
#include <com/onecube/zen/LexerError.h>
#include <jtk/collection/list/ArrayList.h>
#include <com/onecube/zen/TokenStream.h>
#include <com/onecube/zen/Parser.h>
#include <com/onecube/zen/compiler/astNode.h>
#include <com/onecube/zen/compiler/astListener.h>
#include <com/onecube/zen/compiler/astWalker.h>
#include <com/onecube/zen/compiler/ast/ASTAnnotations.h>
#include <com/onecube/zen/compiler/ast/ASTPrinter.h>
#include <com/onecube/zen/bcg/BinaryEntityBuilder.h>
#include <com/onecube/zen/bcg/BinaryEntityGenerator.h>
#include <com/onecube/zen/feb/Instruction.h>
#include <com/onecube/zen/st/SymbolDefinitionListener.h>
#include <com/onecube/zen/st/SymbolResolutionListener.h>
#include <com/onecube/zen/st/SymbolTable.h>

/* lexer -> parser -> symbol table -> semantic errors -> byte code generator -> interpreter */

void printToken(zen_Token_t* token) {
    printf("[%d-%d:%d-%d:%s:%s]\n", token->m_startLine + 1, token->m_stopLine + 1, token->m_startColumn + 1, token->m_stopColumn + 1, token->m_channel == ZEN_TOKEN_CHANNEL_DEFAULT? "default" : "hidden", zen_Lexer_getLiteralName(token->m_type));
}

void printTokens(jtk_ArrayList_t* tokens) {
    int32_t limit = jtk_ArrayList_getSize(tokens);
    zen_Token_t* token;
    int32_t i;
    for (i = 0; i < limit; i++) {
        token = (zen_Token_t*)jtk_ArrayList_getValue(tokens, i);
        printToken(token);
    }
    fflush(stdout);
}

void zen_Interpreter_onLexerError(zen_LexerError_t* error) {
    fprintf(stderr, "[error] %s:%d:%d -- %s\n", error->m_path, error->m_line, error->m_column, error->m_message);
    fflush(stderr);
}

#include <com/onecube/zen/collection/DoublyLinkedList.h>
int32_t xmain() {
    zen_DoublyLinkedList_t* list = zen_DoublyLinkedList_new();
    
    int32_t size = 15;
    
    int32_t i;
    for (i = 0; i < size; i++) {
        zen_DoublyLinkedList_addFirst(list, (void*)i);
    }
    
    //while (!zen_DoublyLinkedList_isEmpty(list)) {
      //  i = rand() % zen_DoublyLinkedList_getSize(list);
        zen_DoublyLinkedList_removeIndex(list, 1);
        zen_DoublyLinkedList_removeIndex(list, 0);
        zen_DoublyLinkedList_removeIndex(list, 3);
    //}
    
    zen_DoublyLinkedList_delete(list);
    return 0;
}

int32_t main(int32_t length, char** arguments) {
    jtk_Assert_assertTrue(zen_Instruction_verify(), "The instruction set is invalid.");
    
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
        fprintf(stderr, "error: no input files\n");
    }
    else {
        for (i = 0; i < size; i++) {
            const char* path = (const char*)jtk_ArrayList_getValue(inputFiles, i);
            zen_InputStream_t* stream = zen_InputStream_new(path);
            if (stream->m_file == NULL) {
                fprintf(stderr, "error: %s: No such file", path);
            }
            else {
                zen_Lexer_t* lexer = zen_Lexer_new(stream);
                zen_TokenStream_t* tokens = zen_TokenStream_new(lexer, ZEN_TOKEN_CHANNEL_DEFAULT);

                if (internalDumpTokens) {
                    zen_TokenStream_fill(tokens);
                    int32_t size = zen_TokenStream_getSize(tokens);
                    int32_t i;
                    for (i = 0; i < size; i++) {
                        zen_Token_t* token = zen_TokenStream_getToken(tokens, i);
                        printToken(token);
                    }
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

#warning "Who gets to destroy symbols and scopes?!"

                zen_BinaryEntityBuilder_t* entityBuilder = zen_BinaryEntityBuilder_new(symbolTable, scopes);
                zen_BinaryEntityBuilder_build(entityBuilder, compilationUnit);

                zen_BinaryEntityGenerator_t* generator = zen_BinaryEntityGenerator_new();
                zen_BinaryEntityGenerator_generate(generator);

                zen_BinaryEntityGenerator_delete(generator);
                zen_BinaryEntityBuilder_delete(entityBuilder);
                zen_SymbolDefinitionListener_delete(symbolDefinitionListener);
                zen_ASTNode_delete(compilationUnit);
                zen_Parser_delete(parser);
                zen_TokenStream_delete(tokens);
                zen_Lexer_delete(lexer);
            }
            zen_InputStream_delete(stream);
        }
    }

    jtk_ArrayList_delete(inputFiles);

    if (internalFootprint) {
        int32_t footprint = zen_Memory_getFootprint();
        printf("Memory Footprint = %.2f KB\n", footprint / 1024.0f);
    }

    return 0;
}