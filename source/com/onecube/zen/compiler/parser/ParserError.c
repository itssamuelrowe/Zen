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

#include <jtk/core/CString.h>
#include <com/onecube/zen/compiler/parser/ParserError.h>

zen_ParserError_t* zen_ParserError_new() {
    zen_ParserError_t* error = zen_Memory_allocate(zen_ParserError_t, 1);
    error->m_message = NULL;
    return error;
}

void zen_ParserError_delete(zen_ParserError_t* error) {
    jtk_Assert_assertObject(error, "The specified error is null.");

    if (error->m_message != NULL) {
        jtk_CString_delete(error->m_message);
    }

    jtk_Memory_deallocate(error);
}