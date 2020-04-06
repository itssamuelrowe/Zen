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

// Monday, September 03, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_ALLOCATION_BUFFER_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_ALLOCATION_BUFFER_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * AllocationBuffer                                                            *
 *******************************************************************************/

/**
 * @class AllocationBuffer
 * @ingroup zen_mms_allocator
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AllocationBuffer_t {
};

/**
 * @memberof AllocationBuffer
 */
typedef struct zen_AllocationBuffer_t zen_AllocationBuffer_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_ALLOCATION_BUFFER_H */

zen_Object_t* zen_AllocationBuffer_allocate(zen_AllocationBuffer_t* buffer,
    int32_t headerSize, int32_t bodySize) {
    jtk_Assert_assertObject(buffer, "The specified allocation buffer is null.");

    int8_t* result = NULL;
    int8_t* newFree = allocator->m_free + (headerSize + bodySize);
    if (newFree <= buffer->m_limit) {
        result = allocator->m_free;
        buffer->m_free = newFree;
    }
    return result;
}