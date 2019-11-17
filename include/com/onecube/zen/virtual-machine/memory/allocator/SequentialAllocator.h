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

// Monday, September 03, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_SEQUENTIAL_ALLOCATOR_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_SEQUENTIAL_ALLOCATOR_H

#include <com/onecube/zen/Configuration.h>

#include <jtk/collection/list/SinglyLinkedList.h>
#include <jtk/concurrent/lock/Mutex.h>
#include <jtk/system/Allocator.h>

/*******************************************************************************
 * SequentialAllocator                                                         *
 *******************************************************************************/

/**
 * Sequential allocation is also known as bump pointer allocation. It uses a
 * large free chunk of memory. When an allocation is requested, say for `n` bytes,
 * it allocate the memory from the end of the free chunk.
 *
 * It consists of a free pointer and a limit pointer. It advances the free
 * pointer after each allocation. Before the free pointer is advanced, it checks
 * if it exceeds the limit pointer, in which case the allocator has exhausted
 * the memory is was assigned.
 *
 * The sequence of allocation addresses is linear for a given chunk.
 *
 * The advantages of this allocator is as follows:
 * - It is simple.
 * - It is efficient.
 * - It results in better cache locality than free-list allocation, especially
 *   for initial allocation of objects in moving collectors.
 *
 * The disadvantages of this allocator is as follows:
 * - It is less suitable than free-list allocation for non-moving collectors.
 * - It causes fragmentation unless compaction is done.
 * 
 * Please refer "The Garbage Collection Handbook" by Richard Jones, Antony
 * Hosking and Eliot Moss for more information. The algorithm described above
 * was extracted from Chapter 7, section 1.
 *
 * @class SequentialAllocator
 * @ingroup zen_memory_allocator
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_SequentialAllocator_t {
    
    /**
     * A pointer to the start of the memory chunk assigned to this sequential
     * allocator.
     *
     * Note that, using integers to represent addresses may work against
     * portablity. Therefore, we use 8-bit integer pointers to represent the
     * address; the compiler takes care of the addresses for us.
     */
    uint8_t* m_start;
    
    /**
     * A pointer to the end of the memory chunk assigned to this sequential
     * allocator. As mentioned above, it helps the allocator identify memory
     * exhaustion.
     *
     * Note that, using integers to represent addresses may work against
     * portablity. Therefore, we use 8-bit integer pointers to represent the
     * address; the compiler takes care of the addresses for us.
     */
    uint8_t* m_limit;
    
    /**
     * A pointer to the start of the region where the next allocation may occur.
     * It is also known as the bump pointer.
     *
     * Initially, it is points to the start of the memory chunk assigned to
     * this allocator.
     */
    uint8_t* m_free;
    
    jtk_SinglyLinkedList_t* m_allocationBuffers;
    jtk_Mutex_t* m_allocationBuffersMutex;
};

/**
 * @memberof SequentialAllocator
 */
typedef struct zen_SequentialAllocator_t zen_SequentialAllocator_t;

/* Constructor */

/**
 * @memberof SequentialAllocator
 */
zen_SequentialAllocator_t* zen_SequentialAllocator_new(jtk_Allocator_t* allocator, uint8_t* start,
    uint8_t* end);

/* Destructor */

/**
 * @memberof SequentialAllocator
 */
void zen_SequentialAllocator_delete(jtk_Allocator_t* allocator,
    zen_SequentialAllocator_t* sequentialAllocator);

/* Allocator */

/**
 * @memberof SequentialAllocator
 */
int8_t* zen_SequentialAllocator_allocate(zen_SequentialAllocator_t* allocator, int32_t headerSize, int32_t bodySize);

/* Allocation Buffer */

/**
 * @memberof SequentialAllocator
 */
zen_AllocationBuffer_t* zen_SequentialAllocator_makeAllocationBuffer(zen_SequentialAllocator_t* allocator);

/**
 * @memberof SequentialAllocator
 */
void zen_SequentialAllocator_mergeAllocationBuffer(zen_SequentialAllocator_t* allocator, zen_AllocationBuffer_t* buffer);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_SEQUENTIAL_ALLOCATOR_H */