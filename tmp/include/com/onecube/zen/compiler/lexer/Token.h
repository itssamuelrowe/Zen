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

#ifndef COM_ONECUBE_ZEN_COMPILER_LEXER_TOKEN_H
#define COM_ONECUBE_ZEN_COMPILER_LEXER_TOKEN_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/lexer/TokenType.h>
#include <com/onecube/zen/compiler/lexer/TokenChannel.h>

/*******************************************************************************
 * Token                                                                       *
 *******************************************************************************/

/**
 * A token represents the smallest entity that appears
 * in a source code. Each token has two primary attributes:
 * a token type (symbol category) and the text associated
 * with it.
 *
 * @author Samuel Rowe
 * @since  zen 1.0
 */
struct zen_Token_t {
    zen_TokenChannel_t m_channel;
    zen_TokenType_t m_type;
    uint8_t* m_text;
    int32_t m_length;
    int32_t m_startIndex;
    int32_t m_stopIndex;
    int32_t m_startLine;
    int32_t m_stopLine;
    int32_t m_startColumn;
    int32_t m_stopColumn;
    int32_t m_index;
    const char* m_file;
};

/**
 * @memberof Token
 */
typedef struct zen_Token_t zen_Token_t;

// Constructor

/**
 * @memberof Token
 */
zen_Token_t* zen_Token_new(zen_TokenChannel_t channel, zen_TokenType_t type,
    const uint8_t* text, int32_t length, int32_t startIndex, int32_t stopIndex,
    int32_t startLine, int32_t stopLine, int32_t startColumn, int32_t stopColumn,
    const char* file);

// Destructor

/**
 * @memberof Token
 */
void zen_Token_delete(zen_Token_t* token);

// Channel

/**
 * @memberof Token
 */
zen_TokenChannel_t zen_Token_getChannel(zen_Token_t* token);

/**
 * @memberof Token
 */
void zen_Token_setChannel(zen_Token_t* token, zen_TokenChannel_t channel);

// Type

/**
 * @memberof Token
 */
zen_TokenType_t zen_Token_getType(zen_Token_t* token);

/**
 * @memberof Token
 */
void zen_Token_setType(zen_Token_t* token, zen_TokenType_t type);

// Text

/**
 * @memberof Token
 */
void zen_Token_setText(zen_Token_t* token, const uint8_t* text, int32_t length);

/**
 * @memberof Token
 */
const uint8_t* zen_Token_getText(zen_Token_t* token);

// Length

/**
 * @memberof Token
 */
int32_t zen_Token_getLength(zen_Token_t* token);

// Start Index

/**
 * @memberof Token
 */
void zen_Token_setStartIndex(zen_Token_t* token, int32_t startIndex);

/**
 * @memberof Token
 */
void zen_Token_setStopIndex(zen_Token_t* token, int32_t stopIndex);

// Start Line

/**
 * @memberof Token
 */
void zen_Token_setStartLine(zen_Token_t* token, int32_t startLine);

/**
 * @memberof Token
 */
int32_t zen_Token_getStartLine(zen_Token_t* token);

// Stop Line

/**
 * @memberof Token
 */
void zen_Token_setStopLine(zen_Token_t* token, int32_t stopLine);

/**
 * @memberof Token
 */
int32_t zen_Token_getStopLine(zen_Token_t* token);

// Start Column

/**
 * @memberof Token
 */
void zen_Token_setStartColumn(zen_Token_t* token, int32_t startColumn);

/**
 * @memberof Token
 */
int32_t zen_Token_getStartColumn(zen_Token_t* token);

// Stop Column

/**
 * @memberof Token
 */
void zen_Token_setStopColumn(zen_Token_t* token, int32_t stopColumn);

/**
 * @memberof Token
 */
int32_t zen_Token_getStopColumn(zen_Token_t* token);

// Index

/**
 * @memberof Token
 */
int32_t zen_Token_getIndex(zen_Token_t* token);

/**
 * @memberof Token
 */
void zen_Token_setIndex(zen_Token_t* token, int32_t index);

#endif /* COM_ONECUBE_ZEN_COMPILER_LEXER_TOKEN_H */