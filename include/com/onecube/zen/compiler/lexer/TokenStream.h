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

// Sunday, November 05, 2017

#ifndef ZEN_TOKEN_STREAM_H
#define ZEN_TOKEN_STREAM_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/Compiler.h>
#include <com/onecube/zen/compiler/lexer/Lexer.h>
#include <com/onecube/zen/compiler/lexer/Token.h>
#include <com/onecube/zen/compiler/lexer/TokenType.h>

/*******************************************************************************
 * TokenStream                                                           *
 *******************************************************************************/

/* NOTE: The lexical error object received by this call back is immediately
 * destroyed as soon as it returns. Therefore, do not maintain a reference to
 * it.
 */
// typedef void (*zen_OnLexicalError_t)(zen_LexicalError_t* error);

struct zen_TokenStream_t {

    zen_Compiler_t* m_compiler;

    /**
     * The lexer which recognizes and produces tokens on this
     * stream.
     */
    zen_Lexer_t* m_lexer;

    /**
     * The list of all the tokens recognized by the lexer so
     * far. It is considered a complete view of the input source
     * once the lexer recognizes the end-of-stream token.
     */
    jtk_ArrayList_t* m_tokens;

    /**
     * The index of the current token.
     */
    int32_t m_p;

    /**
     * Determines whether the end-of-stream token has been
     * produced by the lexer.
     */
    bool m_hitEndOfStream;

    /**
     * The channel on which the token stream filters tokens
     * from.
     */
    zen_TokenChannel_t m_channel;

    jtk_ArrayList_t* m_trash;
};

typedef struct zen_TokenStream_t zen_TokenStream_t;

zen_TokenStream_t* zen_TokenStream_new(zen_Compiler_t* compiler, zen_Lexer_t* lexer,
    zen_TokenChannel_t channel);
void zen_TokenStream_delete(zen_TokenStream_t* stream);
void zen_TokenStream_reset(zen_TokenStream_t* stream);
int32_t zen_TokenStream_getIndex(zen_TokenStream_t* stream);
int32_t zen_TokenStream_getSize(zen_TokenStream_t* stream);
void zen_TokenStream_consume(zen_TokenStream_t* stream);
bool zen_TokenStream_synchronize(zen_TokenStream_t* stream, int32_t i);
int32_t zen_TokenStream_fetch(zen_TokenStream_t* stream, int32_t n);
zen_Token_t* zen_TokenStream_getToken(zen_TokenStream_t* stream, int32_t index);
jtk_ArrayList_t* zen_TokenStream_getTokens(zen_TokenStream_t* stream, int32_t startIndex, int32_t stopIndex);
zen_TokenType_t zen_TokenStream_la(zen_TokenStream_t* stream, int32_t i);
zen_Token_t* zen_TokenStream_lt(zen_TokenStream_t* stream, int32_t k);
void zen_TokenStream_initialize(zen_TokenStream_t* stream);
int32_t zen_TokenStream_getNextTokenOnChannel(zen_TokenStream_t* stream, int32_t i, zen_TokenChannel_t channel);
int32_t zen_TokenStream_getPreviousTokenOnChannel(zen_TokenStream_t* stream, int32_t i, zen_TokenChannel_t channel);
void zen_TokenStream_fill(zen_TokenStream_t* stream);
uint8_t* zen_TokenStream_getSourceName(zen_TokenStream_t* stream, int32_t* size);
zen_Lexer_t* zen_TokenStream_getLexer(zen_TokenStream_t* stream);
uint8_t* zen_TokenStream_getText(zen_TokenStream_t* stream, int32_t startIndex, int32_t stopIndex, int32_t* size);
int32_t zen_TokenStream_getNumberOfTokens(zen_TokenStream_t* stream, zen_TokenChannel_t channel);

#endif /* ZEN_TOKEN_STREAM_H */