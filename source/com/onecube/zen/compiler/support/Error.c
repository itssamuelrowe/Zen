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

#include <com/onecube/zen/compiler/support/Error.h>

/*******************************************************************************
 * Error                                                                       *
 *******************************************************************************/

// Constructor

zen_Error_t* zen_Error_new(zen_ErrorCode_t code, zen_Token_t* token) {
    return zen_Error_newEx(code, token, ZEN_TOKEN_UNKNOWN);
}

zen_Error_t* zen_Error_newEx(zen_ErrorCode_t code, zen_Token_t* token,
    zen_TokenType_t expected) {
    zen_Error_t* error = jtk_Memory_allocate(zen_Error_t, 1);
    error->m_code = code;
    error->m_token = token;
    error->m_expected = expected;

    return error;
}

// Destructor

void zen_Error_delete(zen_Error_t* error) {
    jtk_Assert_assertObject(error, "The specified error is null.");

    jtk_Memory_deallocate(error);
}