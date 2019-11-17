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

// Tuesday, January 22, 2019

#include <com/onecube/zen/virtual-machine/memory/allocator/SequentialAllocator.h>
#include <jtk/support/log/Logger.h>

/*******************************************************************************
 * SequentialAllocator                                                         *
 *******************************************************************************/

/* Constructor */

zen_SequentialAllocator_t* zen_SequentialAllocator_new(jtk_Allocator_t* allocator, int8_t* start,
    int8_t* limit) {
    jtk_Assert_assertObject(allocator, "The specified allocator is null.");
    jtk_Assert_assertObject(start, "The specified pointer to the start of the memory chunk is null.");
    jtk_Assert_assertObject(limit, "The specified pointer to the end of the memory chunk is null.");
    
    zen_SequentialAllocator_t* sequentialAllocator = (zen_SequentialAllocator_t*)jtk_Allocator_allocate(allocator, sizeof (zen_SequentialAllocator), 1);
    sequentialAllocator->m_start = start;
    sequentialAllocator->m_limit = limit;
    sequentialAllocator->m_free = start;
    
    return sequentialAllocator;
}

/* Destructor */

void zen_SequentialAllocator_delete(jtk_Allocator_t* allocator,
    zen_SequentialAllocator_t* sequentialAllocator) {
    jtk_Assert_assertObject(allocator, "The specified allocator is null.");
    jtk_Assert_assertObject(sequentialAllocator, "The specified sequential allocator is null.");
    
    jtk_Allocator_deallocate(allocator, sequentialAllocator);
}

/* Allocator */

int8_t* zen_SequentialAllocator_allocate(zen_SequentialAllocator_t* allocator,
    int32_t headerSize, int32_t bodySize) {
    jtk_Assert_assertObject(allocator, "The specified allocator is null.");

    int8_t* result = NULL;
    int32_t requestSize = headerSize + bodySize;
    int8_t* newFree = allocator->m_free + requestSize;
    
    /* Make sure that when the free pointer is bumped, it does not exceed the
     * limitation of the memory chunk the allocator is assigned.
     */
    if (newFree <= buffer->m_limit) {
        result = allocator->m_free;
        /* Bump the free pointer to indicate the new free region. */
        buffer->m_free = newFree;
        
        xjtk_Logger_debug(JTK_SEQUENTIAL_ALLOCATOR, "Successfully allocated memory. (requestSize = %d, headerSize = %d, bodySize = %d)",
        requestSize, headerSize, bodySize);
    }
    else {
        xjtk_Logger_debug(JTK_SEQUENTIAL_ALLOCATOR, "Failed to allocate memory. (requestSize = %d, headerSize = %d, bodySize = %d)",
        requestSize, headerSize, bodySize);
    }
    
    return result;
}