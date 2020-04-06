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

// Friday, November 04, 2017

#ifndef COM_ONECUBE_ZEN_COMPILER_LEXER_LEXER_ERROR_H
#define COM_ONECUBE_ZEN_COMPILER_LEXER_LEXER_ERROR_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * LexerError                                                                  *
 *******************************************************************************/

/**
 * @class LexerError
 * @ingroup zen_compiler_lexer
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_LexerError_t {
    char* m_message;
    char* m_path;
    int32_t m_line;
    int32_t m_column;
};

/**
 * @memberof LexerError
 */
typedef struct zen_LexerError_t zen_LexerError_t;

// Constructor

/**
 * @memberof LexerError
 */
zen_LexerError_t* zen_LexerError_new(const char* message, const char* path,
    int32_t line, int32_t column);

// Destructor

/**
 * @memberof LexerError
 */
void zen_LexerError_delete(zen_LexerError_t* error);

#endif /* COM_ONECUBE_ZEN_COMPILER_LEXER_LEXER_ERROR_H */