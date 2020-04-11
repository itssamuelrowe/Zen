/*
 * Copyright 2018-2020 Samuel Rowe
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

// Monday, September 10, 2018

#include <com/onecube/zen/virtual-machine/memory/allocator/FirstFitAllocator.h>

/*******************************************************************************
 * FirstFitAllocator                                                           *
 *******************************************************************************/

/* Allocate */

int8_t* zen_FirstFitAllocator_allocate0(zen_FirstFitAllocator_t* allocator,
    int8_t* previous, int8_t* current, int32_t size) {
    int8_t* result = current;
    if (zen_ChunkHelper_isChunkSplitable(current, size)) {
        int8_t* remainder = result + size;
        remainder->m_next = current->m_next;
        remainder->m_size = current->m_size - size;
        previous->m_next = remainder;
    }
    else {
        previous->m_next = current->m_next;
    }
    return result;
}

void* zen_FirstFitAllocator_allocate(zen_FirstFitAllocator_t* allocator,
    int32_t size) {
    jtk_Assert_assertObject(allocator, "The specified allocator is null.");
    
    // previous may be null!
    int8_t* previous = allocator->m_head;
    while (result == NULL) {
        current = previous->m_next;
        if (current == NULL) {
            return NULL;
        }
        else if (zen_Object_getSize(current) < size) {
            previous = current;
        }
        else {
            result = zen_FirstFitAllocator_allocate0(allocator, previous, current, size);
            break;
        }
    }
    
    return result;
}

uint8_t* zen_FirstFitAllocator_findFreeBlock(zen_FirstFitAllocator_t* allocator) {
    jtk_Assert_assertObject(allocator, "The specified first-fit allocator is null.");
    
    int8_t* result = NULL;
    /* The previous pointer may be null if the `allocator->m_head` evaluates to null. */
    int8_t* previous = allocator->m_head;
    while (result == NULL && ) {
        int8_t* current = previous->m_next;
        if (current == NULL) {
            result = NULL;
        }
        else if (((zen_FirstFitCell_t*)current)->m_size < size) {
            previous = current;
        }
        else {
            result = zen_FirstFitAllocator_allocate0(allocator, previous, current, size);
            break;
        }
    }
}

void zen_FirstFitAllocator_allocate(zen_FirstFitAllocator_t* allocator) {
    jtk_Assert_assertObject(allocator, "The specified first-fit allocator is null.");

    void* block = zen_FirstFitAllocator_findFreeBlock(allocator);
    int32_t remainder = zen_FirstFitAllocator_getBlockSize(sizeClass->m_allocator, block) - size;
    if (remainder <= ZEN_FIRST_FIT_ALLOCATOR_MINIMUM_BLOCK_SIZE) {
        zen_FirstFitAllocator_setBlockAvailable(allocator->m_memory, block, false);
        zen_FirstFitAllocator_removeBlock(allocator->m_memory, block);
    }
    else {
        zen_FirstFitAllocator_split(allocator, block, size);
    }
    return block;
}

/* Deallocate */

void zen_FirstFitAllocator_deallocate(zen_FirstFitAllocator_t* allocator,
    void* pointer) {
    jtk_Assert_assertObject(allocator, "The specified first-fit allocator is null.");
    // jtk_Logger_warningEx(pointer == NULL, "The specified pointer is null.");
    
    if (pointer != NULL) {
        zen_FirstFitBlock_setAvailable(block, true);
        zen_FirstFitAllocator_addBlock(allocator, pointer);
    }
}