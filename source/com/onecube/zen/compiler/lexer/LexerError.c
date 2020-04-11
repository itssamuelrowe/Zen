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
#include <com/onecube/zen/compiler/lexer/LexerError.h>

zen_LexerError_t* zen_LexerError_new(const char* message, const char* path, int32_t line, int32_t column) {
    zen_LexerError_t* error = jtk_Memory_allocate(zen_LexerError_t, 1);
    error->m_message = (message == NULL)? jtk_CString_new("No error message") : jtk_CString_new(message);
    error->m_path = jtk_CString_new(path);
    error->m_line = line;
    error->m_column = column;

    return error;
}

void zen_LexerError_delete(zen_LexerError_t* error) {
    jtk_Assert_assertObject(error, "The specified lexer error is null.");
    jtk_Memory_deallocate(error->m_message);
    jtk_Memory_deallocate(error);
}