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

#include <jtk/core/CString.h>
#include <com/onecube/zen/compiler/lexer/Token.h>

zen_Token_t* zen_Token_new(
    zen_TokenChannel_t channel,
    zen_TokenType_t type,
    const uint8_t* text,
    int32_t length,
    int32_t startIndex,
    int32_t stopIndex,
    int32_t startLine,
    int32_t stopLine,
    int32_t startColumn,
    int32_t stopColumn,
    const char* file) {
    zen_Token_t* token = zen_Memory_allocate(zen_Token_t, 1);
    token->m_channel = channel;
    token->m_type = type;
    token->m_text = jtk_CString_newEx(text, length);
    token->m_length = length; // This is the length of the text representation!
    token->m_startIndex = startIndex;
    token->m_stopIndex = stopIndex;
    token->m_startLine = startLine;
    token->m_stopLine = stopLine;
    token->m_startColumn = startColumn;
    token->m_stopColumn = stopColumn;
    token->m_file = file;

    return token;
}

void zen_Token_delete(zen_Token_t* token) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    jtk_CString_delete(token->m_text);
    jtk_Memory_deallocate(token);
}

zen_TokenChannel_t zen_Token_getChannel(zen_Token_t* token) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    return token->m_channel;
}

void zen_Token_setChannel(zen_Token_t* token, zen_TokenChannel_t channel) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    token->m_channel = channel;
}

zen_TokenType_t zen_Token_getType(zen_Token_t* token) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    return token->m_type;
}

void zen_Token_setType(zen_Token_t* token, zen_TokenType_t type) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    token->m_type = type;
}


void zen_Token_setText(zen_Token_t* token, const uint8_t* text, int32_t length) {
    jtk_Assert_assertObject(token, "The specified token is null.");

    /* The text should not be null. */
    jtk_CString_delete(token->m_text);
    token->m_text = jtk_CString_newEx(text, length);
    token->m_length = length;
}

const uint8_t* zen_Token_getText(zen_Token_t* token) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    return token->m_text;
}


int32_t zen_Token_getLength(zen_Token_t* token) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    return token->m_length;
}


void zen_Token_setStartIndex(zen_Token_t* token, int32_t startIndex) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    token->m_startIndex = startIndex;
}

void zen_Token_setStopIndex(zen_Token_t* token, int32_t stopIndex) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    token->m_stopIndex = stopIndex;
}


void zen_Token_setStartLine(zen_Token_t* token, int32_t startLine) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    token->m_startLine = startLine;
}

int32_t zen_Token_getStartLine(zen_Token_t* token) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    return token->m_startLine;
}

void zen_Token_setStopLine(zen_Token_t* token, int32_t stopLine) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    token->m_startLine = stopLine;
}

int32_t zen_Token_getStopLine(zen_Token_t* token) {
    return token->m_stopLine;
}

void zen_Token_setStartColumn(zen_Token_t* token, int32_t startColumn) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    token->m_startColumn = startColumn;
}

int32_t zen_Token_getStartColumn(zen_Token_t* token) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    return token->m_startColumn;
}


void zen_Token_setStopColumn(zen_Token_t* token, int32_t stopColumn) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    token->m_stopColumn = stopColumn;
}

int32_t zen_Token_getStopColumn(zen_Token_t* token) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    return token->m_stopColumn;
}

void zen_Token_setIndex(zen_Token_t* token, int32_t index) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    token->m_index = index;
}

int32_t zen_Token_getIndex(zen_Token_t* token) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    return token->m_index;
}