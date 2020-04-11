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

// Sunday, December 03, 2017

#ifndef COM_ONECUBE_ZEN_COMPILER_PARSER_PARSER_ERROR_H
#define COM_ONECUBE_ZEN_COMPILER_PARSER_PARSER_ERROR_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ParserError                                                                 *
 *******************************************************************************/

/**
 * @class ParserError
 * @ingroup zen_compiler_parser
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ParserError_t {
    char* m_message;
};

/**
 * @memberof ParserError
 */
typedef struct zen_ParserError_t zen_ParserError_t;

// Constructor

/**
 * @memberof ParserError
 */
zen_ParserError_t* zen_ParserError_new();

// Destructor

/**
 * @memberof ParserError
 */
void zen_ParserError_delete(zen_ParserError_t* error);

#endif /* COM_ONECUBE_ZEN_COMPILER_PARSER_PARSER_ERROR_H */