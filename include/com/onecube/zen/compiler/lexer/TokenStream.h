// Sunday, November 05, 2017

#ifndef ZEN_TOKEN_STREAM_H
#define ZEN_TOKEN_STREAM_H

#include <com/onecube/zen/Configuration.h>
#include <zen/collection/ArrayList.h>
#include <zen/Lexer.h>
#include <zen/Token.h>
#include <zen/TokenType.h>

/*******************************************************************************
 * TokenStream                                                           *
 *******************************************************************************/

/* NOTE: The lexical error object received by this call back is immediately
 * destroyed as soon as it returns. Therefore, do not maintain a reference to
 * it.
 */
// typedef void (*zen_OnLexicalError_t)(zen_LexicalError_t* error);

struct zen_TokenStream_t {

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
    zen_ArrayList_t* m_tokens;

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
};

typedef struct zen_TokenStream_t zen_TokenStream_t;

zen_TokenStream_t* zen_TokenStream_new(zen_Lexer_t* lexer, zen_TokenChannel_t channel);
void zen_TokenStream_delete(zen_TokenStream_t* stream);
int32_t zen_TokenStream_getIndex(zen_TokenStream_t* stream);
int32_t zen_TokenStream_getSize(zen_TokenStream_t* stream);
void zen_TokenStream_consume(zen_TokenStream_t* stream);
bool zen_TokenStream_synchronize(zen_TokenStream_t* stream, int32_t i);
int32_t zen_TokenStream_fetch(zen_TokenStream_t* stream, int32_t n);
zen_Token_t* zen_TokenStream_getToken(zen_TokenStream_t* stream, int32_t index);
zen_ArrayList_t* zen_TokenStream_getTokens(zen_TokenStream_t* stream, int32_t startIndex, int32_t stopIndex);
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