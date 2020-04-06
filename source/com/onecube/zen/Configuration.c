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

#include <com/onecube/zen/Configuration.h>

static int32_t s_footprint = 0;

void* zen_Memory_allocate0(int32_t size) {
    s_footprint += size;

    void* object = malloc(size);
    return object;
}

int32_t zen_Memory_getFootprint() {
    return s_footprint;
}

void zen_StackTrace_enter0(const char* file, const char* function) {
    fprintf(stdout, "[enter] %s(%s)\n", function, file);
}

void zen_StackTrace_exit0(const char* file, const char* function) {
    fprintf(stdout, "[exit] %s(%s)\n", function, file);
}