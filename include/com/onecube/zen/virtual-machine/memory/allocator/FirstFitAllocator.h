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

// Sunday, September 02, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_FIRST_FIT_ALLOCATOR_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_FIRST_FIT_ALLOCATOR_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * FirstFitAllocator                                                           *
 *******************************************************************************/

#define ZEN_FIRST_FIT_ALLOCATOR_FLAG_USE_BINARY_TREE (1 << 0)
#define ZEN_FIRST_FIT_ALLOCATOR_FLAG_USE_SINGLY_LINKED_LIST (1 << 1)

/**
 * A first-fit allocator is a type of *sequential fits allocator*. It allocate
 * memory from the first suitable cell it finds. If the cell is larger than
 * the request size, it is split and the remainder is returned to the free-list.
 * However, if the remainder is smaller than a threshold size, the cell is used
 * as is.
 *
 * A free-list allocator uses a data structure to record the location and size
 * of free memory cells. Albeit many implementations use the list data structure
 * to store records, the "list" in free-list actually indicates a set of free
 * memory cells. When a memory allocation is requested, say for `n` bytes,
 * sequential fits allocator searches each free cell, based on certain criteria
 * it chooses a memory cell to allocate. This algorithm is generally known as
 * *sequential fits allocation*.
 *
 * In particular, this class implements a variation of sequential fits allocator
 * known as the *first-fit allocator*. When a memory allocation is requested,
 * say for `n` bytes, this allocator uses the first cell that satisfies the
 * request. As mentioned above, if the cell is larger than the request size,
 * it is split and the remainder is returned to the free-list.
 * However, if the remainder is smaller than a threshold size, the cell is used
 * as is.
 *
 * It should be noted that the allocator assumes that each free cell has space
 * to records its own size and the address of the next free cell. The allocator
 * itself maintains only a single reference to the head of the list.
 * 
 * The advantages of first-fit allocator are as follows:
 * - It behaves like the best-fit allocator, roughly sorting the cells from
 *   smallest to largest.
 *
 * The disadvantages of the first-fit allocator are as follows:
 * - Fragmentation occurs near the front of the list, which slows down the
 *   allocation.
 * 
 * Please refer "The Garbage Collection Handbook" by Richard Jones, Antony
 * Hosking and Eliot Moss for more information. The algorithm described above
 * was extracted from Chapter 7, section 2.
 *
 * @class FirstFitAllocator
 * @ingroup zen_memory_allocator
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_FirstFitAllocator_t {
    int8_t* m_head;
};

/**
 * @memberof FirstFitAllocator
 */
typedef struct zen_FirstFitAllocator_t zen_FirstFitAllocator_t;

/* Allocate */

int8_t* zen_FirstFitAllocator_allocate(zen_FirstFitAllocator_t* allocator, int32_t headSize, int32_t bodySize);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_FIRST_FIT_ALLOCATOR_H */