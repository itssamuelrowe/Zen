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

#include <com/onecube/zen/CString.h>

uint8_t* jtk_CString_new(const uint8_t* string) {
    return jtk_CString_newEx(string, jtk_CString_getSize(string));
}

uint8_t* jtk_CString_newEx(const uint8_t* string, int32_t size) {
    uint8_t* result = zen_Memory_allocate(uint8_t, size + 1);
    int32_t i;
    for (i = 0; i < size; i++) {
        result[i] = string[i];
    }
    result[size] = '\0';
    return result;
}

void jtk_CString_delete(uint8_t* string) {
    jtk_Memory_deallocate(string);
}

bool jtk_CString_equals(const uint8_t* string1, int32_t size1, const uint8_t* string2, int32_t size2) {
    if (string1 == string2) {
        return 1;
    }
    if ((string1 == NULL) ^ (string2 == NULL)) {
        return false;
    }
    if (size1 == size2) {
        int32_t i;
        for (i = 0; i < size1; i++) {
            if (string1[i] != string2[i]) {
                return false;
            }
        }
        return true;
    }
    return false;
}

int32_t jtk_CString_getSize(const uint8_t* string) {
    int32_t i = 0;
    while (string[i++] != '\0');
    return i - 1;
}