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

#ifndef COM_ONECUBE_ZEN_COMPILER_SUPPORT_ERROR_H
#define COM_ONECUBE_ZEN_COMPILER_SUPPORT_ERROR_H

#include <com/onecube/zen/compiler/lexer/Token.h>
#include <com/onecube/zen/compiler/support/ErrorCode.h>

/*******************************************************************************
 * Error                                                                       *
 *******************************************************************************/

/**
 * @class Error
 * @ingroup zen_compiler_support
 * @author Samuel Rowe
 * @since Zen 1.0
 */
struct zen_Error_t {
    zen_ErrorCode_t m_code;
    zen_Token_t* m_token;
    zen_TokenType_t m_expected;
};

/**
 * @memberof Error
 */
typedef struct zen_Error_t zen_Error_t;

// Constructor

zen_Error_t* zen_Error_new(zen_ErrorCode_t errorCode, zen_Token_t* token);
zen_Error_t* zen_Error_newEx(zen_ErrorCode_t errorCode, zen_Token_t* token, zen_TokenType_t expected);

// Destructor

void zen_Error_delete(zen_Error_t* error);

#endif /* COM_ONECUBE_ZEN_COMPILER_SUPPORT_ERROR_H */