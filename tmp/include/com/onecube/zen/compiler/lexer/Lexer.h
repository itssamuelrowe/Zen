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

// Wedensday, October 18, 2017

#ifndef COM_ONECUBE_ZEN_COMPILER_LEXER_LEXER_H
#define COM_ONECUBE_ZEN_COMPILER_LEXER_LEXER_H

#include <jtk/collection/list/ArrayList.h>
#include <jtk/collection/queue/ArrayQueue.h>
#include <jtk/collection/stack/ArrayStack.h>
#include <jtk/core/StringBuilder.h>
#include <jtk/io/InputStream.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/Compiler.h>
#include <com/onecube/zen/compiler/lexer/LexerError.h>
#include <com/onecube/zen/compiler/lexer/Token.h>
#include <com/onecube/zen/compiler/lexer/TokenChannel.h>

#define ZEN_LEXER_DEFAULT_CHANNEL 0
#define ZEN_LEXER_HIDDEN_CHANNEL 1

#define ZEN_END_OF_STREAM -1

/* Forward Reference */

typedef struct zen_ErrorHandler_t zen_ErrorHandler_t;

/*******************************************************************************
 * Lexer                                                                       *
 *******************************************************************************/

/**
 * Generates a stream of tokens from an input character stream.
 * The tokens are recognized based on fixed lexical patterns
 * as described by Zen.
 *
 * @author Samuel Rowe
 * @since  zen 1.0
 */
struct zen_Lexer_t {

    zen_Compiler_t* m_compiler;

    /**
     * The input stream of characters.
     */
    jtk_InputStream_t* m_inputStream;

    /**
     * The character at LA(1), this field is always updated
     * by zen_Lexer_consume(...).
     */
    int32_t m_la1;

    /**
     * The input cursor under which the current look ahead
     * character is located.
     */
    int32_t m_index;

    /**
     * The zero based line number at which the lexer is currently
     * reading.
     */
    int32_t m_line;

    /**
     * The zero based column index within the line, where the
     * lexer is currently reading.
     */
    int32_t m_column;

    /**
     * The starting index of the current token in the input
     * stream, inclusive.
     */
    int32_t m_startIndex;

    /**
     * The zero based line number at which the current
     * token begins, inclusive.
     */
    int32_t m_startLine;

    /**
     * The zero based column at which the current token
     * begins. It is always relative to the starting line.
     */
    int32_t m_startColumn;

    /**
     * Determines whether the lexer has reached the end of
     * the input stream.
     */
    int32_t m_hitEndOfStream:1;

    /**
     * The token that was most recently emitted.
     */
    zen_Token_t* m_token;

    /**
     * The channel on which the next recognized
     * token will be created on.
     */
    zen_TokenChannel_t  m_channel;

    /**
     * The text consumed so far to recognize the next
     * token.
     */
    jtk_StringBuilder_t* m_text;

    /**
     * The token type of the next recognized token.
     */
    zen_TokenType_t m_type;

    /**
     * A buffer to store emitted tokens.
     *
     * A single call to zen_Lexer_nextToken() may result in
     * emission of multiple tokens. Therefore, the lexer
     * buffers up tokens.
     */
    jtk_ArrayQueue_t* m_tokens;

    /**
     * A stack that stores indentation depths.
     */
    jtk_ArrayStack_t* m_indentations;

    /**
     * The number of opening square brackets, angle brackets,
     * braces, or parenthesis the lexer has encountered without
     * corresponding closing square brackets, angle brackets,
     * braces, or parenthesis, irrespectively.
     *
     * NOTE: The lexer disables emission of indentation,
     *       dedentation, and newline tokens when the
     *       enclosure counter is greater than one. Which
     *       means presence of unmatched square brackets,
     *       angle brackets, braces, or parenthesis results
     *       in undefined lexical behaviour.
     */
    int32_t m_enclosures;

    zen_ErrorCode_t m_errorCode;
};

/**
 * @memberof Lexer
 */
typedef struct zen_Lexer_t zen_Lexer_t;

const uint8_t* zen_Lexer_getLiteralName(zen_TokenType_t type);

// Constructor

/**
 * Creates and returns a new lexer.
 *
 * @return A new lexer.
 */
zen_Lexer_t* zen_Lexer_new(zen_Compiler_t* compiler);

// Destructor

/**
 * Destroys the specified lexer.
 *
 * @param lexer
 *        The lexer to destroy.
 */
void zen_Lexer_delete(zen_Lexer_t* lexer);

// Consume

void zen_Lexer_consume(zen_Lexer_t* lexer);

// Emit

void zen_Lexer_emit(zen_Lexer_t* lexer, zen_Token_t* token);

// Error

zen_LexerError_t* zen_Lexer_createError(zen_Lexer_t* lexer, const char* message);

// Token

/**
 * Creates a token.
 *
 * @param  lexer
 *         The lexer that is creating the token.
 * @return A token.
 */
zen_Token_t* zen_Lexer_createToken(zen_Lexer_t* lexer);

/**
 * The primary interface for the Lexer class. It uses the lookahead
 * character (character under the input cursor) to route control
 * flow to the appropriate recognition function. And returns the
 * next token recognized from the input stream.
 *
 * @param  lexer
 *         The lexer.
 * @param  lexerError
 *         An error that represents a lexical mismatch.
 *         It is initialized by this function, if an error
 *         is encountered; ignored, otherwise.
 * @return The next recognized token.
 */
zen_Token_t* zen_Lexer_nextToken(zen_Lexer_t* lexer);


// Newline

/**
 * @memberof Lexer
 */
void zen_Lexer_onNewline(zen_Lexer_t* lexer);

// Reset

/**
 * @memberof Lexer
 */
void zen_Lexer_reset(zen_Lexer_t* lexer, jtk_InputStream_t* inputStream);

// Misc.

/**
 * @memberof Lexer
 */
bool zen_Lexer_isInputStart(zen_Lexer_t* lexer);

/**
 * @memberof Lexer
 */
bool zen_Lexer_isBinaryPrefix(int32_t codePoint);

/**
 * @memberof Lexer
 */
bool zen_Lexer_isBinaryDigit(int32_t codePoint);

/**
 * @memberof Lexer
 */
bool zen_Lexer_isBinaryDigitOrUnderscore(int32_t codePoint);

/**
 * @memberof Lexer
 */
bool zen_Lexer_isBasicEscapeSequence(int32_t codePoint);

/**
 * @memberof Lexer
 */
bool zen_Lexer_isDecimalDigit(int32_t codePoint);

/**
 * @memberof Lexer
 */
bool zen_Lexer_isDecimalDigitOrUnderscore(int32_t codePoint);

/**
 * @memberof Lexer
 */
bool zen_Lexer_isIdentifierStart(int32_t codePoint);

/**
 * @memberof Lexer
 */
bool zen_Lexer_isIdentifierPart(int32_t codePoint);

/**
 * @memberof Lexer
 */
bool zen_Lexer_isLetter(int32_t codePoint);

/**
 * @memberof Lexer
 */
bool zen_Lexer_isLetterOrDigit(int32_t codePoint);

/**
 * @memberof Lexer
 */
bool zen_Lexer_isHexadecimalPrefix(int32_t codePoint);

/**
 * @memberof Lexer
 */
bool zen_Lexer_isHexadecimalDigit(int32_t codePoint);

/**
 * @memberof Lexer
 */
bool zen_Lexer_isHexadecimalDigitOrUnderscore(int32_t codePoint);

/**
 * @memberof Lexer
 */
bool zen_Lexer_isOctalDigit(int32_t codePoint);

/**
 * @memberof Lexer
 */
bool zen_Lexer_isOctalPrefix(int32_t codePoint);

/**
 * @memberof Lexer
 */
bool zen_Lexer_isOctalDigitOrUnderscore(int32_t codePoint);

/**
 * @memberof Lexer
 */
bool zen_Lexer_isIntegerSuffix(int32_t codePoint);

#endif /* COM_ONECUBE_ZEN_COMPILER_LEXER_LEXER_H */