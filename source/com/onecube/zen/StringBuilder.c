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

// Saturday, November 04, 2017

#include <com/onecube/zen/core/Character.h>
#include <com/onecube/zen/CString.h>
#include <com/onecube/zen/StringBuilder.h>

zen_StringBuilder_t* zen_StringBuilder_new() {
    return zen_StringBuilder_newWithCapacity(16);
}

zen_StringBuilder_t* zen_StringBuilder_newWithCapacity(int32_t capacity) {
    jtk_Assert_assertTrue(capacity > 0, "The specified capacity is invalid.");

    zen_StringBuilder_t* builder = zen_Memory_allocate(zen_StringBuilder_t, 1);
    builder->m_value = zen_Memory_allocate(uint8_t, capacity);
    builder->m_capacity = capacity;
    builder->m_size = 0;
}

void zen_StringBuilder_delete(zen_StringBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified string builder is null.");
    jtk_Memory_deallocate(builder->m_value);
    jtk_Memory_deallocate(builder);
}

void zen_StringBuilder_appendCodePoint(zen_StringBuilder_t* builder, int32_t codePoint) {
    jtk_Assert_assertObject(builder, "The specified string builder is null.");
    zen_StringBuilder_setCapacity(builder, builder->m_size + 4);
    builder->m_size += zen_Character_toUtf8(codePoint, builder->m_value, builder->m_capacity, builder->m_size);
}

uint8_t* zen_StringBuilder_toString(zen_StringBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified string builder is null.");
    return jtk_CString_newEx(builder->m_value, builder->m_size);
}

int32_t zen_StringBuilder_getSize(zen_StringBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified string builder is null.");
    return builder->m_size;
}

void zen_StringBuilder_setCapacity(zen_StringBuilder_t* builder, int32_t capacity) {
    if (capacity > 0) {
        int32_t currentCapacity = builder->m_capacity;
        int32_t requireCapacity = capacity - currentCapacity;
        if (requireCapacity > 0) {
            int32_t newCapacity = (currentCapacity * 2) + 2;
            if ((newCapacity - capacity) < 0) {
                newCapacity = capacity;
            }
            if (newCapacity < 0) {
                jtk_Assert_assertFalse(capacity < 0, "An int32_t overflow occurred, the requested capacity is too big.");
                newCapacity = 0x7FFFFFFF;
            }
            uint8_t* temporary = builder->m_value;
            builder->m_value = zen_Memory_allocate(uint8_t, newCapacity);
            int32_t i;
            for (i = 0; i < builder->m_size; i++) {
                builder->m_value[i] = temporary[i];
            }
            jtk_Memory_deallocate(temporary);
            builder->m_capacity = newCapacity;
        }
    }
}

void zen_StringBuilder_clear(zen_StringBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified string builder is null.");
    builder->m_size = 0;
    /* To reduce memory foot print please call zen_StringBuilder_pack(). */
}