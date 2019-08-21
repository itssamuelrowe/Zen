/*
 * Copyright 2018-2019 OneCube
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

// Saturday, November 04, 2017

#include <string.h>
#include <com/onecube/zen/InputStream.h>

/*******************************************************************************
 * InputStream                                                                 *
 *******************************************************************************/

zen_InputStream_t* zen_InputStream_new(const char* path) {
    zen_InputStream_t* stream = zen_Memory_allocate(zen_InputStream_t, 1);
    stream->m_file = fopen(path, "r");
    stream->m_path = strdup(path);
    stream->m_available = 1;
    return stream;
}

void zen_InputStream_delete(zen_InputStream_t* stream) {
    jtk_Assert_assertObject(stream, "The specified input stream is null.");
    free(stream->m_path);
    fclose(stream->m_file);
    zen_Memory_deallocate(stream);
}

int32_t zen_InputStream_read(zen_InputStream_t* stream) {
    jtk_Assert_assertObject(stream, "The specified input stream is null.");
    if (stream->m_available != 0) {
        char c = fgetc(stream->m_file);
        if (c == EOF) {
            stream->m_available = 0;
        }
        return c;
    }
    return 0;
}

int32_t zen_InputStream_isAvailable(zen_InputStream_t* stream) {
    jtk_Assert_assertObject(stream, "The specified input stream is null.");
    return stream->m_available;
}