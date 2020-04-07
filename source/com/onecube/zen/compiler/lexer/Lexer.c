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

#include <jtk/collection/list/ArrayList.h>
#include <jtk/collection/queue/ArrayQueue.h>
#include <jtk/collection/stack/ArrayStack.h>
// #include <jtk/log/Logger.h>
// #include <jtk/log/LogPriority.h>
#include <jtk/io/InputStream.h>
#include <jtk/core/StringBuilder.h>
#include <jtk/core/CString.h>

#include <com/onecube/zen/compiler/lexer/Lexer.h>
#include <com/onecube/zen/compiler/lexer/Token.h>
#include <com/onecube/zen/compiler/lexer/TokenType.h>
#include <com/onecube/zen/compiler/support/ErrorHandler.h>

#warning "Lexer does not recognize decimal values!"

/*******************************************************************************
 * Lexer                                                                       *
 *******************************************************************************/

uint8_t zen_Lexer_literalNames[][25] = {
    { '<', 'u', 'n', 'k', 'n', 'o', 'w', 'n', '>', '\0' },

    { '<', 'i', 'n', 'd', 'e', 'n', 't', 'a', 't', 'i', 'o', 'n', '>', '\0' },
    { '<', 'd', 'e', 'd', 'e', 'n', 't', 'a', 't', 'i', 'o', 'n', '>', '\0' },

    { '<', 'w', 'h', 'i', 't', 'e', '_', 's', 'p', 'a', 'c', 'e', '>', '\0' },
    { '<', 'n', 'e', 'w', 'l', 'i', 'n', 'e', '>', '\0' },
    { '<', 'e', 'n', 'd', '_', 'o', 'f', '_', 's', 't', 'r', 'e', 'a', 'm', '>', '\0' },

    { '!', '=', '\0' },
    { '!', '\0' },

    { '@', '\0' },

    { '#', '\0' },

    { '%', '=', '\0' },
    { '%', '\0' },

    { '&', '&', '\0' },
    { '&', '=', '\0' },
    { '&', '\0' },

    { '(', '\0' },
    { ')', '\0' },

    { '*', '*', '=', '\0' },
    { '*', '*', '\0' },
    { '*', '=', '\0' },
    { '*', '\0' },

    { '+', '+', '\0' },
    { '+', '=', '\0' },
    { '+', '\0' },

    { ',', '\0' },

    { '-', '-', '\0' },
    { '-', '>', '\0' },
    { '-', '=', '\0' },
    { '-', '\0' },

    { '.', '.', '.', '\0' },
    { '.', '.', '\0' },
    { '.', '\0' },

    { 's', 'i', 'n', 'g', 'l', 'e', '_', 'l', 'i', 'n', 'e', '_', 'c', 'o', 'm', 'm', 'e', 'n', 't', '\0' },
    { 'm', 'u', 'l', 't', 'i', '_', 'l', 'i', 'n', 'e', '_', 'c', 'o', 'm', 'm', 'e', 'n', 't', '\0' },
    { '/', '\0' },
    { '/', '=', '\0' },

    { ':', ':', '\0' },
    { ':', '\0' },

    { ';', '\0' },

    { '<', '<', '=', '\0' },
    { '<', '<', '\0' },
    { '<', '=', '\0' },
    { '<', '\0' },

    { '>', '>', '>', '=', '\0' },
    { '>', '>', '>', '\0' },
    { '>', '>', '=', '\0' },
    { '>', '>', '\0' },
    { '>', '=', '\0' },
    { '>', '\0' },

    { '=', '=', '\0' },
    { '=', '\0' },

    { '?', '\0' },

    { '{', '\0' },
    { '}', '\0' },

    { '[', '\0' },
    { ']', '\0' },

    { '^', '=', '\0' },
    { '^', '\0' },

    { '|', '|', '\0' },
    { '|', '=', '\0' },
    { '|', '\0' },

    { '~', '=', '\0' },
    { '~', '\0' },

    { '<', 'i', 'd', 'e', 'n', 't', 'i', 'f', 'i', 'e', 'r', '>', '\0' },

    { 'd', 'o', '\0' },
    { 'i', 'f', '\0' },
    { 'i', 'n', '\0' },
    { 'i', 's', '\0' },
    { 'o', 'r', '\0' },

    { 'a', 'n', 'd', '\0' },
    { 'f', 'o', 'r', '\0' },
    /* { 'n', 'a', 'n', '\0' }, */
    { 'n', 'e', 'w', '\0' },
    { 't', 'r', 'y', '\0' },
    { 'v', 'a', 'r', '\0' },

    { 'c', 'a', 's', 'e', '\0' },
    { 'e', 'l', 's', 'e', '\0' },
    { 'e', 'n', 'u', 'm', '\0' },
    { 'n', 'u', 'l', 'l', '\0' },
    { 't', 'h', 'e', 'n', '\0' },
    { 't', 'h', 'i', 's', '\0' },
    { 't', 'r', 'u', 'e', '\0' },
    { 'w', 'i', 't', 'h', '\0' },

    { 'b', 'r', 'e', 'a', 'k', '\0' },
    { 'c', 'a', 't', 'c', 'h', '\0' },
    { 'c', 'l', 'a', 's', 's', '\0' },
    { 'f', 'a', 'l', 's', 'e', '\0' },
    { 'f', 'i', 'n', 'a', 'l', '\0' },
    { 't', 'h', 'r', 'o', 'w', '\0' },
    { 'w', 'h', 'i', 'l', 'e', '\0' },

    { 'a', 's', 's', 'e', 'r', 't', '\0' },
    { 'i', 'm', 'p', 'o', 'r', 't', '\0' },
    { 'n', 'a', 't', 'i', 'v', 'e', '\0' },
    { 'p', 'u', 'b', 'l', 'i', 'c', '\0' },
    { 'r', 'e', 't', 'u', 'r', 'n', '\0' },
    { 's', 'e', 'c', 'r', 'e', 't', '\0' },
    { 's', 't', 'a', 't', 'i', 'c', '\0' },
    /* { 's', 'w', 'i', 't', 'c', 'h', '\0' }, */

    { 'a', 'b','s', 't', 'r', 'a', 'c', 't', '\0' },
    /* { 'd', 'e', 'f', 'a', 'u', 'l', 't', '\0' }, */
    { 'f', 'i', 'n', 'a', 'l', 'l', 'y', '\0' },
    /* { 'p', 'a', 'c', 'k', 'a', 'g', 'e', '\0' }, */
    { 'p', 'r', 'i', 'v', 'a', 't', 'e', '\0' },

    { 'c', 'o', 'n', 't', 'i', 'n', 'u', 'e', '\0' },
    { 'f', 'u', 'n', 'c', 't', 'i', 'o', 'n', '\0' },

    { 's', 'y', 'n', 'c', 'h', 'r', 'o', 'n', 'i', 'z', 'e', '\0' },

    { '<', 'i', 'n', 't', 'e', 'g', 'e', 'r', '_', 'l', 'i', 't', 'e', 'r', 'a', 'l', '>', '\0' },

    { '<', 's', 't', 'r', 'i', 'n', 'g', '_', 'l', 'i', 't', 'e', 'r', 'a', 'l', '>', '\0' },

    { '<', 'f', 'l', 'o', 'a', 't', 'i', 'n', 'g', '_', 'p', 'o', 'i', 'n', 't', '_', 'l', 'i', 't', 'e', 'r', 'a', 'l', '>', '\0' },
};

/* Literal Name */

const uint8_t* zen_Lexer_getLiteralName(zen_TokenType_t type) {
    return zen_Lexer_literalNames[(int32_t)type];
}

/* Constructor */

zen_Lexer_t* zen_Lexer_new(zen_Compiler_t* compiler) {
    /* The constructor invokes zen_Lexer_consume() to initialize
     * the LA(1) character. Therefore, we assign negative values
     * to certain attributes.
     */

    zen_Lexer_t* lexer = zen_Memory_allocate(zen_Lexer_t, 1);
    lexer->m_compiler = compiler;
    lexer->m_inputStream = NULL;
    lexer->m_la1 = 0;
    lexer->m_index = -1;
    lexer->m_line = 1;
    lexer->m_column = -1;
    lexer->m_startIndex = 0;
    lexer->m_startLine = 0;
    lexer->m_startColumn = 0;
    lexer->m_hitEndOfStream = false;
    lexer->m_token = NULL;
    lexer->m_channel = ZEN_TOKEN_CHANNEL_DEFAULT;
    lexer->m_text = jtk_StringBuilder_new();
    lexer->m_type = ZEN_TOKEN_UNKNOWN;
    lexer->m_tokens = jtk_ArrayQueue_new();
    lexer->m_indentations = jtk_ArrayStack_new();
    lexer->m_enclosures = 0;
    lexer->m_errorCode = ZEN_ERROR_CODE_NONE;

    return lexer;
}

/* Destructor */

/* The lexer may have unretrieved tokens in the buffer.
* This function is responsible for the destruction of
* such tokens.
*/
void zen_Lexer_destroyStaleTokens(zen_Lexer_t* lexer) {
    int32_t size = jtk_ArrayList_getSize(lexer->m_tokens->m_list);
    int32_t i;
    for (i = 0; i < size; i++) {
        zen_Token_t* token = (zen_Token_t*)jtk_ArrayList_getValue(lexer->m_tokens->m_list, i);
        zen_Token_delete(token);
    }
}

void zen_Lexer_delete(zen_Lexer_t* lexer) {
    jtk_Assert_assertObject(lexer, "The specified lexer is null.");

    jtk_StringBuilder_delete(lexer->m_text);
    jtk_ArrayQueue_delete(lexer->m_tokens);
    jtk_ArrayStack_delete(lexer->m_indentations);
    jtk_Memory_deallocate(lexer);
}

/* Create Token */

zen_Token_t* zen_Lexer_createToken(zen_Lexer_t* lexer) {
    uint8_t* text = jtk_CString_newEx(lexer->m_text->m_value, lexer->m_text->m_size); // jtk_StringBuilder_toCString(lexer->m_text);
    int32_t length = jtk_StringBuilder_getSize(lexer->m_text);

    zen_Compiler_t* compiler = lexer->m_compiler;
    const char* file = jtk_ArrayList_getValue(compiler->m_inputFiles,
        compiler->m_currentFileIndex);
    zen_Token_t* token =
        zen_Token_new(
            lexer->m_channel,
            lexer->m_type,
            text,
            length,
            lexer->m_startIndex,    /* inclusive */
            lexer->m_index,         /* exclusive */
            lexer->m_startLine,     /* inclusive */
            lexer->m_line,          /* inclusive */
            lexer->m_startColumn,   /* inclusive */
            lexer->m_column,        /* inclusive */
            file
        );

    /* Destroy the text; not required anymore. */
    jtk_CString_delete(text);

    return token;
}

void zen_Lexer_onNewline(zen_Lexer_t* lexer) {
    lexer->m_line++;
    lexer->m_column = 1;
}

zen_LexerError_t* zen_Lexer_createError(zen_Lexer_t* lexer, const char* message) {
    zen_LexerError_t* error = zen_LexerError_new(message, "<unknown>" /*lexer->m_inputStream->m_path*/, lexer->m_startLine, lexer->m_startColumn);
    return error;
}

bool zen_Lexer_isInputStart(zen_Lexer_t* lexer) {
    return (lexer->m_startLine == 0) && (lexer->m_startColumn == 0);
}

void zen_Lexer_consume(zen_Lexer_t* lexer) {
    jtk_StringBuilder_appendCodePoint(lexer->m_text, lexer->m_la1);

    lexer->m_index++;
    lexer->m_column++;
    /* NOTE: We could have used lexer->index >= "length of input stream"
     * Unfortunately, the flexible design of both the lexer and input stream fails
     * to provide a method to determine the stream length in advance.
     *
     * NOTE: The getAvailable() function is only temporarily used. However, the
     * working of this function is not finalized. Therefore, the following expression
     * may be subjected to changes.
     */
    if (!jtk_InputStream_isAvailable(lexer->m_inputStream)) {
        lexer->m_la1 = ZEN_END_OF_STREAM;
    }
    else {
        lexer->m_la1 = jtk_InputStream_read(lexer->m_inputStream);
    }
}

void zen_Lexer_emit(zen_Lexer_t* lexer, zen_Token_t* token) {
    lexer->m_token = token;
    jtk_ArrayQueue_enqueue(lexer->m_tokens, token);
}

/* NOTE: The design for the integer literal was adopted from Java 8's lexer. The actual rules
* were borrowed and adopted from the grammars-v4/Java8 repository in ANTLR's GitHub profile.
*
* One of the advantages of hand written lexers over generated lexers is that the code may be
* optimized with the domain knowledge. This is exactly what I have done here. In other words,
* the following grammar disallows underscores at the very end of an integer literal.
* To support this, the designer has taken a longer path. However, I have simply used a
* variable called previous, which stores the last character consumed by the integer literal
* recognizing code, to prevent such inappropriate occurrences. After the simplified code
* executes, the previous variable is tested for an underscore. If the test succeeds, a lexical
* error is generated; otherwise, the integer literal is emitted by the lexer.
*
* IntegerLiteral
* : BinaryIntegerLiteral
* | OctalIntegerLiteral
* | HexadecimalIntegerLiteral
* | DecimalIntegerLiteral
* ;
*
* BinaryIntegerLiteral
* :    '0' [bB] BinaryNumeral IntegerTypeSuffix?
* ;
*
* DecimalIntegerLiteral
* :    DecimalNumeral IntegerTypeSuffix?
* ;
*
*
* OctalIntegerLiteral
* :    '0' [cC] OctalNumeral IntegerTypeSuffix?
* ;
*
* HexadecimalIntegerLiteral
* :    '0' [xX] HexadecimalNumeral IntegerTypeSuffix?
* ;
*
* IntegerTypeSuffix
* :    [lL]
* ;
*
* -----------------------------------------------------
*
* BinaryNumeral
* :    BinaryDigit (BinaryDigitsAndUnderscores? BinaryDigit)?
* ;
*
* BinaryDigit
* :    [01]
* ;
*
* BinaryDigitsAndUnderscores
* :    BinaryDigitOrUnderscore+
* ;
*
* BinaryDigitOrUnderscore
* :    BinaryDigit
* |    '_'
* ;
*
* -----------------------------------------------------
*
* OctalNumeral
* :    OctalDigit (OctalDigitsAndUnderscores? OctalDigit)?
* ;
*
* OctalDigit
* :    [0-7]
* ;
*
* OctalDigitsAndUnderscores
* :    OctalDigitOrUnderscore+
* ;
*
* OctalDigitOrUnderscore
* :    OctalDigit
* |    '_'
* ;
*
* -----------------------------------------------------
*
* DecimalNumeral
* :    '0'
* |    DecimalNonZeroDigit (Digits? Undscores Digits)
* ;
*
* DecimalDigits
* :    Digit (DecimalDigitsAndUnderscores? DecimalDigit)?
* ;
*
* DecimalDigit
* :    '0'
* |    NonZeroDigit
* ;
*
* DecimalNonZeroDigit
* :    [1-9]
* ;
*
* DecimalDigitsAndUnderscores
* :    DecimalDigitOrUnderscore+
* ;
*
* DecimalDigitOrUnderscore
* :    DecimalDigit
* |    '_'
* ;
*
* -----------------------------------------------------
*
* HexadecimalNumeral
* :    HexadecimalDigit (HexadecimalDigitsAndUnderscores? HexadecimalDigit)?
* ;
*
* HexadecimalDigit
* :    [0-9a-f-A-F]
* ;
*
* HexadecimalDigitsAndUnderscores
* :    HexadecimalDigitOrUnderscore+
* ;
*
* HexadecimalDigitOrUnderscore
* :    HexadecimalDigit
* |    '_'
* ;
*
*/

void zen_Lexer_binaryIntegerLiteral(zen_Lexer_t* lexer) {
    /* Consume and discard the binary prefix character. */
    zen_Lexer_consume(lexer);

    if (zen_Lexer_isBinaryDigit(lexer->m_la1)) {
        /* Consume and discard the binary digit character. */
        zen_Lexer_consume(lexer);

        if (zen_Lexer_isBinaryDigitOrUnderscore(lexer->m_la1)) {
            uint8_t previous = '\0';
            while (zen_Lexer_isBinaryDigitOrUnderscore(lexer->m_la1)) {
                previous = lexer->m_la1;

                /* Consume and discard a binary digit or an underscore
                    * character.
                    */
                zen_Lexer_consume(lexer);
            }

            if (previous == '_') {
                lexer->m_errorCode = ZEN_ERROR_CODE_EXPECTED_DIGIT_AFTER_UNDERSCORE;
                /* Consume and discard the invalid character. */
                zen_Lexer_consume(lexer);
            }
        }
    }
    else {
        lexer->m_errorCode = ZEN_ERROR_CODE_EXPECTED_DIGIT_AFTER_UNDERSCORE;
        /* Consume and discard the invalid character. */
        zen_Lexer_consume(lexer);
    }
}

void zen_Lexer_octalIntegerLiteral(zen_Lexer_t* lexer) {
    /* Consume and discard the octal prefix character. */
    zen_Lexer_consume(lexer);

    if (zen_Lexer_isOctalDigit(lexer->m_la1)) {
        /* Consume and discard the octal digit character. */
        zen_Lexer_consume(lexer);

        if (zen_Lexer_isOctalDigitOrUnderscore(lexer->m_la1)) {
            uint8_t previous = '\0';
            while (zen_Lexer_isOctalDigitOrUnderscore(lexer->m_la1)) {
                previous = lexer->m_la1;

                /* Consume and discard a octal digit or an underscore
                    * character.
                    */
                zen_Lexer_consume(lexer);
            }

            if (previous == '_') {
                lexer->m_errorCode = ZEN_ERROR_CODE_EXPECTED_DIGIT_AFTER_UNDERSCORE;
                /* Consume and discard the invalid character. */
                zen_Lexer_consume(lexer);
            }
        }
    }
    else {
        lexer->m_errorCode = ZEN_ERROR_CODE_EXPECTED_DIGIT_AFTER_UNDERSCORE;
        /* Consume and discard the invalid character. */
        zen_Lexer_consume(lexer);
    }
}

void zen_Lexer_hexadecimalIntegerLiteral(zen_Lexer_t* lexer) {
    /* Consume and discard the binary prefix character. */
    zen_Lexer_consume(lexer);

    if (zen_Lexer_isHexadecimalDigit(lexer->m_la1)) {
        /* Consume and discard the hexadecimal digit character. */
        zen_Lexer_consume(lexer);

        if (zen_Lexer_isHexadecimalDigitOrUnderscore(lexer->m_la1)) {
            uint8_t previous = '\0';
            while (zen_Lexer_isHexadecimalDigitOrUnderscore(lexer->m_la1)) {
                previous = lexer->m_la1;

                /* Consume and discard a binary digit or an underscore
                    * character.
                    */
                zen_Lexer_consume(lexer);
            }

            if (previous == '_') {
                lexer->m_errorCode = ZEN_ERROR_CODE_EXPECTED_DIGIT_AFTER_UNDERSCORE;
                /* Consume and discard the invalid character. */
                zen_Lexer_consume(lexer);
            }
        }
    }
    else {
        lexer->m_errorCode = ZEN_ERROR_CODE_EXPECTED_DIGIT_AFTER_UNDERSCORE;
        /* Consume and discard the invalid character. */
        zen_Lexer_consume(lexer);
    }
}

void zen_Lexer_decimalIntegerLiteral(zen_Lexer_t* lexer) {
    /* Consume and discard the decimal digit character. */
    zen_Lexer_consume(lexer);

    if (zen_Lexer_isDecimalDigit(lexer->m_la1)) {
        /* Consume and discard the decimal digit character. */
        zen_Lexer_consume(lexer);

        if (zen_Lexer_isDecimalDigitOrUnderscore(lexer->m_la1)) {
            uint8_t previous = '\0';
            while (zen_Lexer_isDecimalDigitOrUnderscore(lexer->m_la1)) {
                previous = lexer->m_la1;

                /* Consume and discard the decimal digit or underscore
                    * character.
                    */
                zen_Lexer_consume(lexer);
            }

            if (previous == '_') {
                lexer->m_errorCode = ZEN_ERROR_CODE_EXPECTED_DIGIT_AFTER_UNDERSCORE;
                /* Consume and discard the invalid character. */
                zen_Lexer_consume(lexer);
            }
        }
    }
    else if (lexer->m_la1 == '_') {
        do {
            /* Consume and discard the '_' character. */
            zen_Lexer_consume(lexer);
        }
        while (lexer->m_la1 == '_');

        if (zen_Lexer_isDecimalDigitOrUnderscore(lexer->m_la1)) {
            uint8_t previous = '\0';
            while (zen_Lexer_isDecimalDigitOrUnderscore(lexer->m_la1)) {
                previous = lexer->m_la1;

                /* Consume and discard the decimal digit or underscore
                    * character.
                    */
                zen_Lexer_consume(lexer);
            }

                if (previous == '_') {
                    lexer->m_errorCode = ZEN_ERROR_CODE_EXPECTED_DIGIT_AFTER_UNDERSCORE;
                    /* Consume and discard the invalid character. */
                    zen_Lexer_consume(lexer);
                }
        }
        else {
            lexer->m_errorCode = ZEN_ERROR_CODE_EXPECTED_DIGIT_AFTER_UNDERSCORE;
            /* Consume and discard the invalid character. */
            zen_Lexer_consume(lexer);
        }
    }
}

/* Here is the simplified grammar which allows underscore characters at the
 * end of the integer literal. This grammar has simplified the decimal integer
 * literal rule, too. This type of integer literals are the last to be recognized,
 * given the order in which they are written. This allows us to implement
 * the longest-match-wins behaviour, the same technique that helps us differentiate
 * keywords and identifiers with similar prefixes.
 *
 * PS: I am not sure what the "longest-match-wins behaviour" is actually called. :")
 *
 * IntegerLiteral
 * : BinaryIntegerLiteral
 * | OctalIntegerLiteral
 * | HexadecimalIntegerLiteral
 * | DecimalIntegerLiteral
 * ;
 *
 * BinaryIntegerLiteral
 * :    '0' [bB] BinaryNumeral IntegerTypeSuffix?
 * ;
 *
 * DecimalIntegerLiteral
 * :    DecimalNumeral IntegerTypeSuffix?
 * ;
 *
 * OctalIntegerLiteral
 * :    '0' [cC] OctalNumeral IntegerTypeSuffix?
 * ;
 *
 * HexadecimalIntegerLiteral
 * :    '0' [xX] HexadecimalNumeral IntegerTypeSuffix?
 * ;
 *
 * IntegerTypeSuffix
 * :    [lL]
 * ;
 *
 * -----------------------------------------------------
 *
 * BinaryNumeral
 * :    BinaryDigit BinaryDigitOrUnderscore*
 * ;
 *
 * BinaryDigitOrUnderscore
 * :    BinaryDigit
 * |    '_'
 * ;
 *
 * BinaryDigit
 * :    [01]
 * ;
 *
 * -----------------------------------------------------
 *
 * OctalNumeral
 * :    OctalDigit OctalDigitOrUnderscore*
 * ;
 *
 * OctalDigit
 * :    [0-7]
 * ;
 *
 * OctalDigitOrUnderscore
 * :    OctalDigit
 *  |    '_'
 * ;
 *
 * -----------------------------------------------------
 *
 * HexadecimalNumeral
 * :    HexadecimalDigit HexadecimalDigitOrUnderscore*
 * ;
 *
 * HexadecimalDigit
 * :    [0-9a-f-A-F]
 * ;
 *
 * HexadecimalDigitOrUnderscore
 * :    HexadecimalDigit
 * |    '_'
 * ;
 *
 * -----------------------------------------------------
 *
 * DecimalNumeral
 * :    DecimalDigit DecimalDigitOrUnderscore*
 * ;
 *
 * DecimalDigit
 * :    [0-9]
 * ;
 *
 * DecimalDigitOrUnderscore
 * :    DecimalDigit
 * |    '_'
 * ;
 *
 */
void zen_Lexer_integerLiteral(zen_Lexer_t* lexer) {
    /* The lexer has recognized an integer literal. */
    lexer->m_type = ZEN_TOKEN_INTEGER_LITERAL;

    if (lexer->m_la1 == '0') {
        /* Consume and discard the '0' character. */
        zen_Lexer_consume(lexer);

        if (zen_Lexer_isBinaryPrefix(lexer->m_la1)) {
            zen_Lexer_binaryIntegerLiteral(lexer);
        }
        else if (zen_Lexer_isOctalPrefix(lexer->m_la1)) {
            zen_Lexer_octalIntegerLiteral(lexer);
        }
        else if (zen_Lexer_isHexadecimalPrefix(lexer->m_la1)) {
            zen_Lexer_hexadecimalIntegerLiteral(lexer);
        }
        else if (zen_Lexer_isDecimalDigit(lexer->m_la1) || lexer->m_la1 == '_') {
            zen_Lexer_decimalIntegerLiteral(lexer);
        }
        else if (zen_Lexer_isLetter(lexer->m_la1)) {
            lexer->m_errorCode = ZEN_ERROR_CODE_INVALID_INTEGER_LITERAL_PREFIX;
        }
    }
    else {
        zen_Lexer_decimalIntegerLiteral(lexer);
    }
}

/*
 * ALGORITHM
 * ---------
 *
 * Check for a buffered token. If found, remove it from the buffer
 * and return it to the user.
 *
 * The lexer checks if an end of stream was encountered within
 * a block, indicated when the end of stream character is encountered
 * when the indentation depth is greater than 0. If so, a newline
 * token is emitted. This arbitrary token serves as the end of
 * a statement. After which, the lexer emits dedentation tokens
 * as needed. In order to retrieve all the tokens generated in the
 * above steps multiple calls to nextToken() are required. Therefore,
 * the lexer buffers up tokens. The buffered tokens are appropriately
 * produced before the user as described previously.
 *
 * A single lexical recognition may result in multiple errors. For example,
 * when recognizing a string literal we may encounter a number of malformed
 * escape sequences and an unexpected end of stream.
 *
 * When the lambda operator is encountered, the lexer marks a flag that indicates
 * the encouter. If the lexer encounters a left brace after such an encounter,
 * a flag which allows indentation is pushed onto the stack. When the lexer encouters
 * a left brace without encountering the lambda operator, a flag which disallows
 * indentation is pushed onto the stack. When the right brace is encountered, the
 * stack is popped. Thus, the compiler recognizes the following function
 * as valid.
 *
 * function main()
 *     var array = [
 *         'Samuel Rowe',
 *         'Nikola Tesla',
 *         'Bill Gates',
 *         'Isaac Newton',
 *         'Keanu Reeves'
 *     ]
 *     array.forEach(lambda (item, index) {
 *             if index.isEven()
 *                 print('[even] ' + index + '->' + item)
 *             else
 *                 print('[odd] ' + index + '->' + item)
 *         }
 *     )
 */
zen_Token_t* zen_Lexer_nextToken(zen_Lexer_t* lexer) {
    jtk_Assert_assertObject(lexer, "The specified lexer is null.");

    zen_Compiler_t* compiler = lexer->m_compiler;
    const char* file = jtk_ArrayList_getValue(compiler->m_inputFiles,
        compiler->m_currentFileIndex);

    /* The lexer does not bother to recognize a token
     * from the input stream unless necessary.
     */
    if (jtk_ArrayQueue_isEmpty(lexer->m_tokens)) {
        /* We don't exit the loop until
         * -- We have a token.
         * -- We have reached the end of the stream.
         * -- We have encountered an error. (Interestingly, this condition
         *    is not explicitly checked because errorneous token recognition
         *    too generate tokens!)
         */
        loopEntry : {
            lexer->m_token = NULL;
            lexer->m_type = ZEN_TOKEN_UNKNOWN;
            jtk_StringBuilder_clear(lexer->m_text);
            lexer->m_channel = ZEN_TOKEN_CHANNEL_DEFAULT;
            lexer->m_startIndex = lexer->m_index;
            lexer->m_startLine = lexer->m_line;
            lexer->m_startColumn = lexer->m_column;
            lexer->m_errorCode = ZEN_ERROR_CODE_NONE;


            switch (lexer->m_la1) {
                case ZEN_END_OF_STREAM : {
                    if (!jtk_ArrayStack_isEmpty(lexer->m_indentations)) {
                        /* It appears that the lexer has reached the end of
                         * the stream inside a block. In order to prevent sytax
                         * errors occuring because of a "missing newline" we emit
                         * an extra newline token. It may serve as the end of a
                         * statement. After which, the lexer emits dedentation
                         * tokens as needed.
                         *
                         * NOTE: The lexer is creating an imaginary token here.
                         *       Therefore, we directly invoke zen_Token_new().
                         */
                        zen_Token_t* newlineToken = zen_Token_new(
                            ZEN_TOKEN_CHANNEL_DEFAULT,
                            ZEN_TOKEN_NEWLINE,
                            "\n",
                            1,
                            lexer->m_startIndex,    /* inclusive */
                            lexer->m_index,         /* exclusive */
                            lexer->m_startLine,     /* inclusive */
                            lexer->m_line,          /* inclusive */
                            lexer->m_startColumn,   /* inclusive */
                            lexer->m_column,         /* inclusive */
                            file
                        );
                        zen_Lexer_emit(lexer, newlineToken);

                        while (!jtk_ArrayStack_isEmpty(lexer->m_indentations)) {
                            /*
                             * NOTE: The lexer is creating an imaginary token here.
                             *       Therefore, we directly invoke zen_Token_new().
                             */
                            zen_Token_t* dedentationToken = zen_Token_new(
                                    ZEN_TOKEN_CHANNEL_DEFAULT,
                                    ZEN_TOKEN_DEDENTATION,
                                    "",
                                    0,
                                    lexer->m_startIndex,    /* inclusive */
                                    lexer->m_index,         /* exclusive */
                                    lexer->m_startLine,     /* inclusive */
                                    lexer->m_line,          /* inclusive */
                                    lexer->m_startColumn,   /* inclusive */
                                    lexer->m_column,        /* inclusive */
                                    file
                                );
                            zen_Lexer_emit(lexer, dedentationToken);
                            jtk_ArrayStack_pop(lexer->m_indentations);
                        }
                    }
                    /* The data required for the creating the end-of-stream token.
                     */
                    lexer->m_type = ZEN_TOKEN_END_OF_STREAM;
                    lexer->m_hitEndOfStream = true;
                    break;
                }

                case ' '  :
                case '\r' :
                case '\n' : {
                    int32_t indentation = 0;
                    if (lexer->m_la1 == ' ') {
                        do {
                            indentation++;
                            zen_Lexer_consume(lexer);
                        }
                        while (lexer->m_la1 == ' ');

                        if (!zen_Lexer_isInputStart(lexer)) {
                            /* This token belongs to the WHITESPACE rule. */
                            lexer->m_type = ZEN_TOKEN_WHITESPACE;
                            lexer->m_channel = ZEN_TOKEN_CHANNEL_HIDDEN;
                        }
                    }
                    else {
                        if (lexer->m_la1 == '\r') {
                            zen_Lexer_consume(lexer);
                            /* Optionally, the carriage return character may be
                             * followed by a newline character.
                             */
                            if (lexer->m_la1 == '\n') {
                                zen_Lexer_consume(lexer);

                                /* Update information such as the current line,
                                 * current column, etc.
                                 */
                                zen_Lexer_onNewline(lexer);
                            }
                        }
                        else {
                            zen_Lexer_consume(lexer);

                            /* Update information such as the current line,
                             * current column, etc.
                             */
                            zen_Lexer_onNewline(lexer);
                        }

                        while (lexer->m_la1 == ' ') {
                            indentation++;
                            zen_Lexer_consume(lexer);
                        }
                    }

                    if (lexer->m_type != ZEN_TOKEN_WHITESPACE) {
                        int32_t la1 = lexer->m_la1;
                        if ((lexer->m_enclosures > 0) || (la1 == '\r') || (la1 == '\n')) {
                            /* If the lexer is within the context of a collection
                             * or a blank line it ignores all the indentations,
                             * dedentations, and newlines.
                             */
                            // zen_Logger_log(ZEN_LOG_PRIORITY_INFO, "Skipping a character (line=%d, column=%d)", lexer->m_startIndex + 1, lexer->m_startColumn + 1);
                            goto loopEntry;
                        }
                        else {
                            /*
                             * NOTE: The lexer is creating a custom token here.
                             *       Therefore, we directly invoke zen_Token_new().
                             */
                            zen_Token_t* newlineToken = zen_Token_new(
                                ZEN_TOKEN_CHANNEL_DEFAULT,
                                ZEN_TOKEN_NEWLINE,
                                "\n",
                                1,
                                lexer->m_startIndex,    /* inclusive */
                                lexer->m_index,         /* exclusive */
                                lexer->m_startLine,     /* inclusive */
                                lexer->m_line,          /* inclusive */
                                lexer->m_startColumn,   /* inclusive */
                                lexer->m_column,        /* inclusive */
                                file
                            );
                            zen_Lexer_emit(lexer, newlineToken);

                            int32_t previous = jtk_ArrayStack_isEmpty(lexer->m_indentations)?
                                0 : (int32_t)jtk_ArrayStack_peek(lexer->m_indentations);

                            if (indentation == previous) {
                                /* The lexer does not generate an INDENTETATION
                                 * token if the current indentation is the same
                                 * as the previous indentation.
                                 */
                                goto loopEntry;
                            }
                            else if (indentation > previous) {
                                /* The lexer generates a new INDENTETATION token
                                 * if the current indentation is greater than the
                                 * previous indentation, indicating a deeper block
                                 * nest.
                                 */
                                jtk_ArrayStack_push(lexer->m_indentations, (void*)indentation);
                                /*
                                 * NOTE: The lexer is creating a custom token here.
                                 *       Therefore, we directly invoke zen_Token_new().
                                 */
                                zen_Token_t* indentationToken = zen_Token_new(
                                        ZEN_TOKEN_CHANNEL_DEFAULT,
                                        ZEN_TOKEN_INDENTATION,
                                        "",
                                        0,
                                        lexer->m_startIndex,    /* inclusive */
                                        lexer->m_index,         /* exclusive */
                                        lexer->m_startLine,     /* inclusive */
                                        lexer->m_line,          /* inclusive */
                                        lexer->m_startColumn,   /* inclusive */
                                        lexer->m_column,        /* inclusive */
                                        file
                                    );
                                zen_Lexer_emit(lexer, indentationToken);
                            }
                            else {
                                /* The lexer generates one or more DEDENTATION tokens
                                 * if the current indentation is lesser than the
                                 * previous indentation, indicating a shallower
                                 * block.
                                 *
                                 * Interestingly, the dedentation does not require
                                 * the exact number of whitespaces as seen in the
                                 * indentation.
                                 */
                                while (!jtk_ArrayStack_isEmpty(lexer->m_indentations) &&
                                       ((int32_t)jtk_ArrayStack_peek(lexer->m_indentations) > indentation)) {
                                    /*
                                     * NOTE: The lexer is creating a custom token here.
                                     *       Therefore, we directly invoke zen_Token_new().
                                     */
                                    zen_Token_t* dedentationToken = zen_Token_new(
                                        ZEN_TOKEN_CHANNEL_DEFAULT,
                                        ZEN_TOKEN_DEDENTATION,
                                        "",
                                        0,
                                        lexer->m_startIndex,    /* inclusive */
                                        lexer->m_index,         /* exclusive */
                                        lexer->m_startLine,     /* inclusive */
                                        lexer->m_line,          /* inclusive */
                                        lexer->m_startColumn,   /* inclusive */
                                        lexer->m_column,        /* inclusive */
                                        file
                                    );
                                    zen_Lexer_emit(lexer, dedentationToken);
                                    jtk_ArrayStack_pop(lexer->m_indentations);
                                }
                            }
                        }
                        /* The rule action has taken care of generating
                         * tokens. The lexer can confidently skip any other
                         * token producing operations.
                         */
                        goto loopEntry;
                    }
                    break;
                }

                /* EXCLAMATION_MARK_EQUAL
                 * :    '!='
                 * ;
                 *
                 * EXCLAMATION_MARK
                 * :    '!'
                 * ;
                 */
                case '!' : {
                    zen_Lexer_consume(lexer);

                    if (lexer->m_la1 == '=') {
                        zen_Lexer_consume(lexer);
                        lexer->m_type = ZEN_TOKEN_EXCLAMATION_MARK_EQUAL;
                    }
                    else {
                        lexer->m_type = ZEN_TOKEN_EXCLAMATION_MARK;
                    }

                    break;
                }

                /* AT
                 * :    '@'
                 * ;
                 */
                case '@' : {
                    /* Consume and discard the '@' character. */
                    zen_Lexer_consume(lexer);
                    /* The lexer has recognized the '@' token. */
                    lexer->m_type = ZEN_TOKEN_AT;
                    break;
                }

                /* HASH
                 * :    '#'
                 * ;
                 */
                case '#' : {
                    /* Consume and discard the '#' character. */
                    zen_Lexer_consume(lexer);
                    /* The lexer has recognized the '#' token. */
                    lexer->m_type = ZEN_TOKEN_HASH;

                    break;
                }

                /* MODULUS_EQUAL
                 * :    '%='
                 * ;
                 *
                 * MODULUS
                 * :    '%'
                 * ;
                 */
                case '%' : {
                    zen_Lexer_consume(lexer);

                    if (lexer->m_la1 == '=') {
                        zen_Lexer_consume(lexer);
                        lexer->m_type = ZEN_TOKEN_MODULUS_EQUAL;
                    }
                    else {
                        lexer->m_type = ZEN_TOKEN_MODULUS;
                    }

                    break;
                }

                /* AMPERSAND_2
                 * :    '&&'
                 * ;
                 *
                 * AMPERSAND_EQUAL
                 * :    '&='
                 * ;
                 *
                 * AMPERSAND
                 * :    '&'
                 * ;
                 */
                case '&' : {
                    zen_Lexer_consume(lexer);

                    if (lexer->m_la1 == '&') {
                        zen_Lexer_consume(lexer);
                        lexer->m_type = ZEN_TOKEN_AMPERSAND_2;
                    }
                    else if (lexer->m_la1 == '=') {
                        zen_Lexer_consume(lexer);
                        lexer->m_type = ZEN_TOKEN_AMPERSAND_EQUAL;
                    }
                    else {
                        lexer->m_type = ZEN_TOKEN_AMPERSAND;
                    }

                    break;
                }

                /* LEFT_PARENTHESIS
                 * :    '('
                 * ;
                 */
                case '(' : {
                    /* Consume and discard the '(' character. */
                    zen_Lexer_consume(lexer);
                    /* The lexer has recognized the '(' token. */
                    lexer->m_type = ZEN_TOKEN_LEFT_PARENTHESIS;
                    /* The lexer is inside an enclosure. The parser will fail
                     * if newline or indentation tokens are generated. Therefore,
                     * mark this enclosure.
                     */
                    lexer->m_enclosures++;
                    break;
                }

                /* RIGHT_PARENTHESIS
                 * :    ')'
                 * ;
                 */
                case ')' : {
                    /* Consume and discard the '(' character. */
                    zen_Lexer_consume(lexer);
                    /* The lexer has recognized the '(' token. */
                    lexer->m_type = ZEN_TOKEN_RIGHT_PARENTHESIS;
                    /* The lexer is outside an enclosure. The parser will fail
                     * if newline or indentation tokens are not generated
                     * appropriately. Therefore, unmark this enclosure.
                     */
                    lexer->m_enclosures--;
                    break;
                }

                /* ASTERISK_2_EQUAL
                 * :    '**='
                 * ;
                 *
                 * ASTERISK_2
                 * :    '**'
                 * ;
                 *
                 * ASTERISK_EQUAL
                 * :    '*='
                 * ;
                 *
                 * ASTERISK
                 * :    '*'
                 * ;
                 */
                case '*' : {
                    /* Consume and discard the '*' character. */
                    zen_Lexer_consume(lexer);

                    if (lexer->m_la1 == '*') {
                        /* Consume and discard the '*' character. */
                        zen_Lexer_consume(lexer);

                        if (lexer->m_la1 == '=') {
                            /* Consume and discard the '=' character. */
                            zen_Lexer_consume(lexer);
                            /* The lexer has recognized the '**=' token. */
                            lexer->m_type = ZEN_TOKEN_ASTERISK_2_EQUAL;
                        }
                        else {
                            /* The lexer has recognized the '**' token. */
                            lexer->m_type = ZEN_TOKEN_ASTERISK_2;
                        }
                    }
                    else if (lexer->m_la1 == '=') {
                        /* Consume and discard the '=' character. */
                        zen_Lexer_consume(lexer);
                        /* The lexer has recognized the '*=' token. */
                        lexer->m_type = ZEN_TOKEN_ASTERISK_EQUAL;
                    }
                    else {
                        lexer->m_type = ZEN_TOKEN_ASTERISK;
                    }

                    break;
                }

                /* PLUS_2
                 * :    '++'
                 * ;
                 *
                 * PLUS_EQUAL
                 * :    '+='
                 * ;
                 *
                 * PLUS
                 * :    '+'
                 * ;
                 */
                case '+' : {
                    /* Consume and discard the '+' character. */
                    zen_Lexer_consume(lexer);

                    if (lexer->m_la1 == '+') {
                        /* Consume and discard the '+' character. */
                        zen_Lexer_consume(lexer);
                        /* The lexer has recognized the '++' token. */
                        lexer->m_type = ZEN_TOKEN_PLUS_2;
                    }
                    else if (lexer->m_la1 == '=') {
                        /* Consume and discard the '=' character. */
                        zen_Lexer_consume(lexer);
                        /* The lexer has recognized the '+=' token. */
                        lexer->m_type = ZEN_TOKEN_PLUS_EQUAL;
                    }
                    else {
                        lexer->m_type = ZEN_TOKEN_PLUS;
                    }

                    break;
                }

                /* COMMA
                 * :    ','
                 * ;
                 */
                case ',' : {
                    /* Consume and discard the ',' character. */
                    zen_Lexer_consume(lexer);
                    /* The lexer has recognized the ',' token. */
                    lexer->m_type = ZEN_TOKEN_COMMA;
                    break;
                }

                /* DASH_2
                 * :    '--'
                 * ;
                 *
                 * ARROW
                 * :    '->'
                 * ;
                 *
                 * DASH_EQUAL
                 * :    '-='
                 * ;
                 *
                 * DASH
                 * :    '-'
                 * ;
                 */
                case '-' : {
                    /* Consume and discard the '-' character. */
                    zen_Lexer_consume(lexer);

                    if (lexer->m_la1 == '-') {
                        /* Consume and discard the '-' character. */
                        zen_Lexer_consume(lexer);
                        /* The lexer has recognized the '--' token. */
                        lexer->m_type = ZEN_TOKEN_DASH_2;
                    }
                    else if (lexer->m_la1 == '>') {
                        /* Consume and discard the '>' character. */
                        zen_Lexer_consume(lexer);
                        /* The lexer has recognized the '->' token. */
                        lexer->m_type = ZEN_TOKEN_ARROW;
                    }
                    else if (lexer->m_la1 == '=') {
                        /* Consume and discard the '=' character. */
                        zen_Lexer_consume(lexer);
                        /* The lexer has recognized the '-=' token. */
                        lexer->m_type = ZEN_TOKEN_DASH_EQUAL;
                    }
                    else {
                        /* The lexer has recognized the '-' token. */
                        lexer->m_type = ZEN_TOKEN_DASH;
                    }

                    break;
                }

                /* ELLIPSIS
                 * :    '...'
                 * ;
                 *
                 * DOT_2
                 * :    '..'
                 * ;
                 *
                 * DOT
                 * :    '.'
                 * ;
                 */
                case '.' : {
                    /* Consume and discard the '.' character. */
                    zen_Lexer_consume(lexer);

                    if (lexer->m_la1 == '.') {
                        /* Consume and discard the '.' character. */
                        zen_Lexer_consume(lexer);

                        if (lexer->m_la1 == '.') {
                            /* Consume and discard the '.' character. */
                            zen_Lexer_consume(lexer);
                            /* The lexer has recognized the '...' token. */
                            lexer->m_type = ZEN_TOKEN_ELLIPSIS;
                        }
                        else {
                            /* The lexer has recognized the '..' token. */
                            lexer->m_type = ZEN_TOKEN_DOT_2;
                        }
                    }
                    else {
                        /* The lexer has recognized the '.' token. */
                        lexer->m_type = ZEN_TOKEN_DOT;
                    }

                    break;
                }

                /* SINGLE_LINE_COMMENT
                 * :    '//' ~[\r\n]* -> channel(hidden)
                 * ;
                 *
                 * MULTI_LINE_COMMENT
                 * :    '/*' .*? '*''/' -> channel(hidden)
                 * ;
                 *
                 * FORWARD_SLASH_EQUAL
                 * :    '/='
                 * ;
                 *
                 * FORWARD_SLASH
                 * :    '/'
                 * ;
                 */
                case '/' : {
                    /* Consume and discard the '/' character. */
                    zen_Lexer_consume(lexer);

                    if (lexer->m_la1 == '/') {
                        /* Consume and discard the '/' character. */
                        zen_Lexer_consume(lexer);

                        while ((lexer->m_la1 != '\n') && (lexer->m_la1 != ZEN_END_OF_STREAM)) {
                            /* Consume and discard the unknown character. */
                            zen_Lexer_consume(lexer);
                        }

                        /* We consume the terminating character, regardless
                         * of it being newline or end-of-stream.
                         */
                        zen_Lexer_consume(lexer);

                        /* Update information such as the current line,
                         * current column, etc.
                         */
                        if (lexer->m_la1 == '\n') {
                            zen_Lexer_onNewline(lexer);
                        }

                        /* The lexer has recognized a single line comment. */
                        lexer->m_type = ZEN_TOKEN_SINGLE_LINE_COMMENT;
                        /* The single-line comment token should be produced on the
                         * hidden channel. Otherwise, the parser will have a hard
                         * time ignoring redundant single-line comment tokens.
                         */
                        lexer->m_channel = ZEN_TOKEN_CHANNEL_HIDDEN;
                    }
                    else if (lexer->m_la1 == '*') {
                        /* Consume and discard the '*' token. */
                        zen_Lexer_consume(lexer);

                        do {
                            while (lexer->m_la1 != '*') {
                                if (lexer->m_la1 == ZEN_END_OF_STREAM) {
                                    lexer->m_errorCode = ZEN_ERROR_CODE_UNTERMINATED_MULTI_LINE_COMMENT;
                                    break;
                                }

                                /* Update information such as the current line,
                                 * current column, etc.
                                 */
                                if (lexer->m_la1 == '\n') {
                                    zen_Lexer_onNewline(lexer);
                                }

                                /* Consume and discard the unknown character. */
                                zen_Lexer_consume(lexer);
                            }

                            /* The following condition is checked to exit the
                             * outer loop. We do not have to report the error.
                             * Because the error was reported in the inner loop.
                             */
                            if (lexer->m_la1 == ZEN_END_OF_STREAM) {
                                break;
                            }

                            /* Here, we are currently processing the '*' character.
                             * Therefore, we consume it.
                             */
                            zen_Lexer_consume(lexer);
                        }
                        while (lexer->m_la1 != '/');

                        if (lexer->m_la1 == '/') {
                            /* At this point, we are indeed processing the '/'
                             * character. Therefore, we consume it.
                             */
                            zen_Lexer_consume(lexer);
                        }
                        /*
                        else {
                            ... UNTERMINATED COMMENT ...
                        }
                        */

                        /* The lexer has recognized the multi-line comment. */
                        lexer->m_type = ZEN_TOKEN_MULTI_LINE_COMMENT;
                        /* The multi-line comment token should be produced on the
                         * hidden channel. Otherwise, the parser will have a hard
                         * time ignoring redundant multi-line comment tokens.
                         */
                        lexer->m_channel = ZEN_TOKEN_CHANNEL_HIDDEN;
                    }
                    else if (lexer->m_la1 == '=') {
                        /* Consume and discard the '=' character. */
                        zen_Lexer_consume(lexer);
                        /* The lexer has recognized the '/=' token. */
                        lexer->m_type = ZEN_TOKEN_FORWARD_SLASH_EQUAL;
                    }
                    else {
                        /* The lexer has recognized the '/' token. */
                        lexer->m_type = ZEN_TOKEN_FORWARD_SLASH;
                    }

                    break;
                }

                /* COLON_2
                 * :    '::'
                 * ;
                 *
                 * COLON
                 * :    ':'
                 * ;
                 */
                case ':' : {
                    /* Consume and discard the ':' character. */
                    zen_Lexer_consume(lexer);

                    if (lexer->m_la1 == ':') {
                        /* Consume and discard the ':' character. */
                        zen_Lexer_consume(lexer);

                        /* The lexer has recognized the '::' token. */
                        lexer->m_type = ZEN_TOKEN_COLON_2;
                    }
                    else {
                        /* The lexer has recognized the ':' token. */
                        lexer->m_type = ZEN_TOKEN_COLON;
                    }

                    break;
                }

                /* SEMICOLON
                 * :    ';'
                 * ;
                 */
                case ';' : {
                    /* Consume and discard the ';' character. */
                    zen_Lexer_consume(lexer);
                    /* The lexer has recognized the ';' token. */
                    lexer->m_type = ZEN_TOKEN_SEMICOLON;
                    break;
                }

                /* LEFT_ANGLE_BRACKET_2_EQUAL
                 * :    '<<='
                 * ;
                 *
                 * LEFT_ANGLE_BRACKET_2
                 * :    '<<'
                 * ;
                 *
                 * LEFT_ANGLE_BRACKET_EQUAL
                 * :    '<='
                 * ;
                 *
                 * LEFT_ANGLE_BRACKET
                 * :    '<'
                 * ;
                 */
                case '<' : {
                    /* Consume and discard the '<' character. */
                    zen_Lexer_consume(lexer);

                    if (lexer->m_la1 == '<') {
                        /* Consume and discard the '<' character. */
                        zen_Lexer_consume(lexer);

                        if (lexer->m_la1 == '=') {
                            /* Consume and discard the '=' character. */
                            zen_Lexer_consume(lexer);
                            /* The lexer has recognized the '<<=' token. */
                            lexer->m_type = ZEN_TOKEN_LEFT_ANGLE_BRACKET_2_EQUAL;
                        }
                        else {
                            /* The lexer has recognized the '<<' token. */
                            lexer->m_type = ZEN_TOKEN_LEFT_ANGLE_BRACKET_2;
                        }
                    }
                    else if (lexer->m_la1 == '=') {
                        /* Consume and discard the '=' character. */
                        zen_Lexer_consume(lexer);
                        /* The lexer has recognized the '<=' token. */
                        lexer->m_type = ZEN_TOKEN_LEFT_ANGLE_BRACKET_EQUAL;
                    }
                    else {
                        /* The lexer has recognized the '<' token. */
                        lexer->m_type = ZEN_TOKEN_LEFT_ANGLE_BRACKET;
                    }

                    break;
                }

                /* RIGHT_ANGLE_BRACKET_3_EQUAL
                 * :    '>>>='
                 * ;
                 *
                 * RIGHT_ANGLE_BRACKET_3
                 * :    '>>>'
                 * ;
                 *
                 * RIGHT_ANGLE_BRACKET_2_EQUAL
                 * :    '>>='
                 * ;
                 *
                 * RIGHT_ANGLE_BRACKET_2
                 * :    '>>'
                 * ;
                 *
                 * RIGHT_ANGLE_BRACKET_EQUAL
                 * :    '>='
                 * ;
                 *
                 * RIGHT_ANGLE_BRACKET
                 * :    '>'
                 * ;
                 */
                case '>' : {
                    /* Consume and discard the '>' character. */
                    zen_Lexer_consume(lexer);

                    if (lexer->m_la1 == '>') {
                        /* Consume and discard the '>' character. */
                        zen_Lexer_consume(lexer);

                        if (lexer->m_la1 == '>') {
                            /* Consume and discard the '>' character. */
                            zen_Lexer_consume(lexer);

                            if (lexer->m_la1 == '=') {
                                /* Consume and discard the '=' character. */
                                zen_Lexer_consume(lexer);
                                /* The lexer has recognized the '>>>=' token. */
                                lexer->m_type = ZEN_TOKEN_RIGHT_ANGLE_BRACKET_3_EQUAL;
                            }
                            else {
                                /* The lexer has recognized the '>>>' token. */
                                lexer->m_type = ZEN_TOKEN_RIGHT_ANGLE_BRACKET_3;
                            }
                        }
                        else if (lexer->m_la1 == '=') {
                            /* Consume and discard the '=' character. */
                            zen_Lexer_consume(lexer);
                            /* The lexer has recognized the '>>=' token. */
                            lexer->m_type = ZEN_TOKEN_RIGHT_ANGLE_BRACKET_2_EQUAL;
                        }
                        else {
                            /* The lexer has recognized the '>>' token. */
                            lexer->m_type = ZEN_TOKEN_RIGHT_ANGLE_BRACKET_2;
                        }
                    }
                    else if (lexer->m_la1 == '=') {
                        /* Consume and discard the '=' character. */
                        zen_Lexer_consume(lexer);
                        /* The lexer has recognized the '>=' token. */
                        lexer->m_type = ZEN_TOKEN_RIGHT_ANGLE_BRACKET_EQUAL;
                    }
                    else {
                        /* The lexer has recognized the '>' token. */
                        lexer->m_type = ZEN_TOKEN_RIGHT_ANGLE_BRACKET;
                    }

                    break;
                }

                /* EQUAL_2
                 * :    '=='
                 * ;
                 *
                 * EQUAL
                 * :    '='
                 * ;
                 */
                case '=' : {
                    /* Consume and discard the '=' character. */
                    zen_Lexer_consume(lexer);

                    if (lexer->m_la1 == '=') {
                        /* Consume and discard the '=' character. */
                        zen_Lexer_consume(lexer);
                        /* The lexer has recognized the '==' token. */
                        lexer->m_type = ZEN_TOKEN_EQUAL_2;
                    }
                    else {
                        /* The lexer has recognized the '=' token. */
                        lexer->m_type = ZEN_TOKEN_EQUAL;
                    }

                    break;
                }

                /* HOOK
                 * :    '?'
                 * ;
                 */
                case '?' : {
                    /* Consume and discard the '?' character. */
                    zen_Lexer_consume(lexer);
                    /* The lexer has recognized the '?' token. */
                    lexer->m_type = ZEN_TOKEN_HOOK;
                    break;
                }

                /* LEFT_BRACE
                 * :    '{'
                 * ;
                 */
                case '{' : {
                    /* Consume and discard the '{' character. */
                    zen_Lexer_consume(lexer);
                    /* The lexer has recognized the '{' token. */
                    lexer->m_type = ZEN_TOKEN_LEFT_BRACE;
                    /* The lexer is inside an enclosure. The parser will fail
                     * if newline or indentation tokens are generated. Therefore,
                     * mark this enclosure.
                     */
                    lexer->m_enclosures++;
                    break;
                }

                /* RIGHT_BRACE
                 * :    '}'
                 * ;
                 */
                case '}' : {
                    /* Consume and discard the '}' character. */
                    zen_Lexer_consume(lexer);
                    /* The lexer has recognized the '}' token. */
                    lexer->m_type = ZEN_TOKEN_RIGHT_BRACE;
                    /* The lexer is outside an enclosure. The parser will fail
                     * if newline or indentation tokens are not generated
                     * appropriately. Therefore, unmark this enclosure.
                     */
                    lexer->m_enclosures--;
                    break;
                }

                /* LEFT_SQUARE_BRACKET
                 * :    '['
                 * ;
                 */
                case '[' : {
                    /* Consume and discard the '[' character. */
                    zen_Lexer_consume(lexer);
                    /* The lexer has recognized the '[' token. */
                    lexer->m_type = ZEN_TOKEN_LEFT_SQUARE_BRACKET;
                    /* The lexer is inside an enclosure. The parser will fail
                     * if newline or indentation tokens are generated. Therefore,
                     * mark this enclosure.
                     */
                    lexer->m_enclosures++;
                    break;
                }

                /* RIGHT_SQUARE_BRACKET
                 * :    ']'
                 * ;
                 */
                case ']' : {
                    /* Consume and discard the ']' character. */
                    zen_Lexer_consume(lexer);
                    /* The lexer has recognized the ']' token. */
                    lexer->m_type = ZEN_TOKEN_RIGHT_SQUARE_BRACKET;
                    /* The lexer is outside an enclosure. The parser will fail
                     * if newline or indentation tokens are not generated
                     * appropriately. Therefore, unmark this enclosure.
                     */
                    lexer->m_enclosures--;
                    break;
                }

                /* CARET_EQUAL
                 * :    '^='
                 * ;
                 *
                 * CARET
                 * :    '^'
                 * ;
                 */
                case '^' : {
                    /* Consume and discard the '^' character. */
                    zen_Lexer_consume(lexer);

                    if (lexer->m_la1 == '=') {
                        /* Consume and discard the '=' character. */
                        zen_Lexer_consume(lexer);
                        /* The lexer has recognized the '^=' token. */
                        lexer->m_type = ZEN_TOKEN_CARET_EQUAL;
                    }
                    else {
                        /* The lexer has recognized the '^' token. */
                        lexer->m_type = ZEN_TOKEN_CARET;
                    }

                    break;
                }

                /* VERTICAL_BAR_2
                 * :    '||'
                 * ;
                 *
                 * VERTICAL_BAR_EQUAL
                 * :    '|='
                 * ;
                 *
                 * VERTICAL_BAR
                 * :    '|'
                 * ;
                 */
                case '|' : {
                    /* Consume and discard the '|' character. */
                    zen_Lexer_consume(lexer);

                    if (lexer->m_la1 == '|') {
                        /* Consume and discard the '|' character. */
                        zen_Lexer_consume(lexer);
                        /* The lexer has recognized the '||' token. */
                        lexer->m_type = ZEN_TOKEN_VERTICAL_BAR_2;
                    }
                    else if (lexer->m_la1 == '=') {
                        /* Consume and discard the '=' character. */
                        zen_Lexer_consume(lexer);
                        /* The lexer has recognized the '|=' token. */
                        lexer->m_type = ZEN_TOKEN_VERTICAL_BAR_EQUAL;
                    }
                    else {
                        /* The lexer has recognized the '|' token. */
                        lexer->m_type = ZEN_TOKEN_VERTICAL_BAR;
                    }

                    break;
                }

                /* TILDE_EQUAL
                 * :    '~='
                 * ;
                 *
                 * TILDE
                 * :    '~'
                 * ;
                 */
                case '~' : {
                    /* Consume and discard the '~' character. */
                    zen_Lexer_consume(lexer);

                    if (lexer->m_la1 == '=') {
                        /* Consume and discard the '=' character. */
                        zen_Lexer_consume(lexer);
                        /* The lexer has recognized the '~=' token. */
                        lexer->m_type = ZEN_TOKEN_TILDE_EQUAL;
                    }
                    else {
                        /* The lexer has recognized the '~' token. */
                        lexer->m_type = ZEN_TOKEN_TILDE;
                    }

                    break;
                }

                /* STRING_LITERAL
                 * :    '"' STRING_CHARACTER* '"'
                 * |    '\'' STRING_CHARACTER* '\''
                 * ;
                 *
                 * STRING_CHARACTER
                 * :    ~["'\\]
                 * |    ESCAPE_SEQUENCE
                 * ;
                 *
                 * ESCAPE_SEQUENCE
                 * :    '\\' [btnfr"'\\]
                 * |    '\\' 'u' HEXADECIMAL_DIGIT HEXADECIMAL_DIGIT HEXADECIMAL_DIGIT HEXADECIMAL_DIGIT
                 * ;
                 *
                 * HEXADECIMAL_DIGIT
                 * :    [0-9a-fA-F]
                 * ;
                 */
                case '"':
                case '\'' : {
                    uint8_t terminator = lexer->m_la1;

                    /* Consume and discard the character which marks the
                     * beginning of this string.
                     */
                    zen_Lexer_consume(lexer);

                    while (lexer->m_la1 != terminator) {
                        if ((lexer->m_la1 == ZEN_END_OF_STREAM) || (lexer->m_la1 == '\n')) {
                            lexer->m_errorCode = ZEN_ERROR_CODE_UNTERMINATED_STRING_LITERAL;
                            break;
                        }
                        else if (lexer->m_la1 == '\\') {
                            /* Consume and discard the '\' character. */
                            zen_Lexer_consume(lexer);

                            if (zen_Lexer_isBasicEscapeSequence(lexer->m_la1)) {
                                /* Consume and discard the character which represents
                                 * a basic escape sequence.
                                 */
                                zen_Lexer_consume(lexer);
                            }
                            else if (lexer->m_la1 == 'u') {
                                /* Consume and discard the 'u' character. */
                                zen_Lexer_consume(lexer);

                                int32_t i;
                                for (i = 0; i < 4; i++) {
                                    if (zen_Lexer_isHexadecimalDigit(lexer->m_la1)) {
                                        /* Consume and discard the hexadecimal digit character. */
                                        zen_Lexer_consume(lexer);
                                    }
                                    else {
                                        lexer->m_errorCode = ZEN_ERROR_CODE_MALFORMED_UNICODE_CHARACTER_SEQUENCE;
                                        break;
                                    }
                                }
                            }
                            else {
                                lexer->m_errorCode = ZEN_ERROR_CODE_INVALID_ESCAPE_SEQUENCE;

                                /* Consume and discard the unknown escape sequence. */
                                zen_Lexer_consume(lexer);
                            }
                        }
                        else {
                            /* Consume and discard a character in the string literal. */
                            zen_Lexer_consume(lexer);
                        }
                    }

                    if (lexer->m_la1 == terminator) {
                        /* At this point, we are processing the terminating
                         * double or single quote character. Therefore,
                         * consume and discard it.
                         */
                        zen_Lexer_consume(lexer);
                    }
                    else {
                        /* Most likely, we encountered an immature end of line or stream. */
                    }

                    /* The lexer has recognized a string literal. */
                    lexer->m_type = ZEN_TOKEN_STRING_LITERAL;

                    break;
                }

                default : {

                    /* IDENTIFIER
                     * :    LETTER LETTER_OR_DIGIT*
                     * ;
                     */
                    if (zen_Lexer_isIdentifierStart(lexer->m_la1)) {
                        /* Consume and discard the first letter. */
                        zen_Lexer_consume(lexer);

                        while (zen_Lexer_isIdentifierPart(lexer->m_la1)) {
                            /* Consume and discard the consecutive letter
                             * or digit character.
                             */
                            zen_Lexer_consume(lexer);
                        }

                        uint8_t* text = lexer->m_text->m_value; // jtk_StringBuilder_toCString(lexer->m_text);
                        int32_t length = lexer->m_text->m_size; // lexer->m_index - lexer->m_startIndex;

                        /* TODO: Find a better solution. Given we have access to a sorted
                         * list of keywords, a good idea would be to implement a binary
                         * search here.
                         *
                         * Another solution, would be to use a hash map with token
                         * text as key and token type as values. This is obviously
                         * more efficient than the current algorithm. Unfortunately,
                         * I neither have patience nor time.
                         *
                         * The following algorithm tries to prevent redundant comparisons
                         * between strings (as many as possible).
                         * Basically, the code checks if the token length is known.
                         * If true, then the keywords with the correspoding length
                         * are compared against the token. The keywords are always
                         * arranged in ascending lexicographical order. Thus, we
                         * reduce the worst case scenario for number of comparisons
                         * from 40 to 7.
                         */
                        lexer->m_type = ZEN_TOKEN_IDENTIFIER;
                        switch (length) {
                            case 2 : {
                                if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_DO], 2)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_DO;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_IF], 2)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_IF;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_IN], 2)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_IN;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_IS], 2)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_IS;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_OR], 2)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_OR;
                                }
                                break;
                            }

                            case 3 : {
                                if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_AND], 3)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_AND;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_FOR], 3)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_FOR;
                                }/*
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_NAN], 3)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_NAN;
                                }*/
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_NEW], 3)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_NEW;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_TRY], 3)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_TRY;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_VAR], 3)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_VAR;
                                }
                                break;
                            }

                            case 4 : {
                                if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_CASE], 4)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_CASE;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_ELSE], 4)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_ELSE;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_ENUM], 4)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_ENUM;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_NULL], 4)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_NULL;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_THEN], 4)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_THEN;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_THIS], 4)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_THIS;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_TRUE], 4)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_TRUE;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_WITH], 4)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_WITH;
                                }
                                break;
                            }

                            case 5 : {
                                if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_BREAK], 5)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_BREAK;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_CATCH], 5)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_CATCH;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_CLASS], 5)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_CLASS;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_FALSE], 5)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_FALSE;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_FINAL], 5)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_FINAL;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_THROW], 5)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_THROW;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_WHILE], 5)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_WHILE;
                                }
                                break;
                            }

                            case 6 : {
                                if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_ASSERT], 6)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_ASSERT;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_IMPORT], 6)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_IMPORT;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_NATIVE], 6)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_NATIVE;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_PUBLIC], 6)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_PUBLIC;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_RETURN], 6)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_RETURN;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_SECRET], 6)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_SECRET;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_STATIC], 6)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_STATIC;
                                }/*
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_SWITCH], 6)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_SWITCH;
                                }*/
                                break;
                            }

                            case 7 : {
                                if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_ABSTRACT], 7)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_ABSTRACT;
                                }/*
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_DEFAULT], 7)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_DEFAULT;
                                }*/
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_FINALLY], 7)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_FINALLY;
                                }/*
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_PACKAGE], 7)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_PACKAGE;
                                }*/
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_PRIVATE], 7)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_PRIVATE;
                                }
                                break;
                            }

                            case 8 : {
                                if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_CONTINUE], 8)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_CONTINUE;
                                }
                                else if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_FUNCTION], 8)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_FUNCTION;
                                }
                                break;
                            }

                            case 11 : {
                                if (jtk_CString_equals(text, length, zen_Lexer_literalNames[(int32_t)ZEN_TOKEN_KEYWORD_SYNCHRONIZE], 11)) {
                                    lexer->m_type = ZEN_TOKEN_KEYWORD_SYNCHRONIZE;
                                }
                                break;
                            }
                        }

                        /* Destroy the text; not required anymore. */
                        // jtk_CString_delete(text);
                    }
                    else if (zen_Lexer_isDecimalDigit(lexer->m_la1)) {
                        zen_Lexer_integerLiteral(lexer);

                        /* Check for integer type suffix. */

                        /* All integers in Zen occupy 64-bits. */
                        // if (zen_Lexer_isIntegerSuffix(lexer->m_la1)) {
                            /* Consume and discard the integer suffix character. */
                            // zen_Lexer_consume(lexer);
                        // }
                        // else if (zen_Lexer_isLetter(lexer->m_la1)) {
                            /* Consume and discard the invalid suffix. */
                            // zen_Lexer_consume(lexer);

                            // printf("[error] Invalid integer literal suffix\n");
                        // }
                    }
                    else {
                        lexer->m_errorCode = ZEN_ERROR_CODE_UNKNOWN_CHARACTER;

                        /* Consume and discard the unknown character. */
                        zen_Lexer_consume(lexer);
                        /* The lexer has encountered an unrecognized character. */
                        lexer->m_type = ZEN_TOKEN_UNKNOWN;
                    }
                    break;
                }
            }
        }

        zen_Token_t* newToken = zen_Lexer_createToken(lexer);
        zen_Lexer_emit(lexer, newToken);

        /* Unlike the parser, the lexer does not support error recovery strategies.
         * Therefore, all types of errors are collectively recorded at this point.
         */
        if (lexer->m_errorCode != ZEN_ERROR_CODE_NONE) {
            zen_ErrorHandler_handleLexicalError(lexer->m_compiler->m_errorHandler,
                lexer, lexer->m_errorCode, newToken);
        }
    }

    zen_Token_t* next = (zen_Token_t*)jtk_ArrayQueue_peek(lexer->m_tokens);
    jtk_ArrayQueue_dequeue(lexer->m_tokens);
    return next;
}

// Reset

void zen_Lexer_reset(zen_Lexer_t* lexer, jtk_InputStream_t* inputStream) {
    jtk_Assert_assertObject(lexer, "The specified lexer is null.");
    jtk_Assert_assertObject(inputStream, "The specified input stream is null.");

    zen_Lexer_destroyStaleTokens(lexer);

    lexer->m_inputStream = inputStream;
    lexer->m_la1 = 0;
    lexer->m_index = -1;
    lexer->m_line = 1;
    lexer->m_column = -1;
    lexer->m_startIndex = 0;
    lexer->m_startLine = 0;
    lexer->m_startColumn = 0;
    lexer->m_hitEndOfStream = false;
    lexer->m_token = NULL;
    lexer->m_channel = ZEN_TOKEN_CHANNEL_DEFAULT;
    lexer->m_type = ZEN_TOKEN_UNKNOWN;
    lexer->m_enclosures = 0;
    lexer->m_errorCode = ZEN_ERROR_CODE_NONE;

    jtk_StringBuilder_clear(lexer->m_text);
    jtk_ArrayQueue_clear(lexer->m_tokens);
    // jtk_ArrayStack_clear(lexer->m_indentations);
    jtk_ArrayList_clear(lexer->m_indentations->m_list);

    zen_Lexer_consume(lexer);
}

// Misc.

bool zen_Lexer_isBinaryPrefix(int32_t codePoint) {
    return (codePoint == 'b') || (codePoint == 'B');
}

bool zen_Lexer_isBinaryDigit(int32_t codePoint) {
	return (codePoint == '0') || (codePoint == '1');
}

bool zen_Lexer_isBinaryDigitOrUnderscore(int32_t codePoint) {
	return (codePoint == '0') ||
           (codePoint == '1') ||
           (codePoint == '_');
}

bool zen_Lexer_isBasicEscapeSequence(int32_t codePoint) {
	return (codePoint == 'b') ||
           (codePoint == 'f') ||
           (codePoint == 'n') ||
           (codePoint == 'r') ||
           (codePoint == 't') ||
           (codePoint == '\\') ||
           (codePoint == '\"') ||
           (codePoint == '\'');
}

bool zen_Lexer_isDecimalDigit(int32_t codePoint) {
    return (codePoint >= '0') && (codePoint <= '9');
}

bool zen_Lexer_isDecimalDigitOrUnderscore(int32_t codePoint) {
    return zen_Lexer_isDecimalDigit(codePoint) || (codePoint == '_');
}

bool zen_Lexer_isIdentifierStart(int32_t codePoint) {
    return ((codePoint >= 'a') && (codePoint <= 'z')) ||
           ((codePoint >= 'A') && (codePoint <= 'Z'));
}

bool zen_Lexer_isIdentifierPart(int32_t codePoint) {
    return ((codePoint >= 'a') && (codePoint <= 'z')) ||
           ((codePoint >= 'A') && (codePoint <= 'Z')) ||
           ((codePoint >= '0') && (codePoint <= '9')) ||
           (codePoint == '_');
}

bool zen_Lexer_isLetter(int32_t codePoint) {
    return ((codePoint >= 'a') && (codePoint <= 'z')) ||
           ((codePoint >= 'A') && (codePoint <= 'Z'));
}

bool zen_Lexer_isLetterOrDigit(int32_t codePoint) {
    return zen_Lexer_isLetter(codePoint) || zen_Lexer_isDecimalDigit(codePoint);
}

bool zen_Lexer_isHexadecimalPrefix(int32_t codePoint) {
    return (codePoint == 'x') || (codePoint == 'X');
}

bool zen_Lexer_isHexadecimalDigit(int32_t codePoint) {
    return zen_Lexer_isDecimalDigit(codePoint) ||
           ((codePoint >= 'a') && (codePoint <= 'f')) ||
           ((codePoint >= 'A') && (codePoint <= 'F'));
}

bool zen_Lexer_isHexadecimalDigitOrUnderscore(int32_t codePoint) {
    return zen_Lexer_isHexadecimalDigit(codePoint) || (codePoint == '_');
}

bool zen_Lexer_isOctalDigit(int32_t codePoint) {
    return (codePoint >= '0') && (codePoint <= '7');
}

bool zen_Lexer_isOctalPrefix(int32_t codePoint) {
    return (codePoint == 'c') || (codePoint == 'C');
}

bool zen_Lexer_isOctalDigitOrUnderscore(int32_t codePoint) {
    return zen_Lexer_isOctalDigit(codePoint) || (codePoint == '_');
}

bool zen_Lexer_isIntegerSuffix(int32_t codePoint) {
    return (codePoint == 'l') || (codePoint == 'L');
}