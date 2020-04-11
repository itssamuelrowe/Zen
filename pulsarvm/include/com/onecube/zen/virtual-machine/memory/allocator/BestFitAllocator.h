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

// Sunday, September 02, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_BEST_FIT_ALLOCATOR_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_BEST_FIT_ALLOCATOR_H

/*******************************************************************************
 * BestFitAllocator                                                            *
 *******************************************************************************/

/**
 * A best-fit allocator is a type of *free-list allocator*. It allocates the
 * memory from the cell that closely matches the request size. This idea
 * helps minimise memory waste, unnecessary splits, and fragmentation.
 *
 * A free-list allocator uses a data structure to record the location and size
 * of free memory cells. Albeit many implementations use the list data structure
 * to store records, the "list" in free-list actually indicates a set of free
 * memory cells. When a memory allocation is requested, say for `n` bytes,
 * sequential fits allocator searches each free cell, based on certain criteria
 * it chooses a memory cell to allocate. This algorithm is generally known as
 * *sequential fits allocation*.
 
 * The advantages of first-fit allocator are as follows:
 * - Best-fit performs well for most programs, resulting in relatively low
 *   wasted memory space.
 *
 * Please refer "The Garbage Collection Handbook" by Richard Jones, Antony
 * Hosking and Eliot Moss for more information. The algorithm described above
 * was extracted from Chapter 7, section 2.
 *
 * @class BestFitAllocator
 * @ingroup zen_memory_allocator
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_BestFitAllocator_t {
};

/**
 * @memberof BestFitAllocator
 */
typedef struct zen_BestFitAllocator_t zen_BestFitAllocator_t;

/* Constructor */

/**
 * @memberof BestFitAllocator
 */
zen_BestFitAllocator_t* zen_BestFitAllocator_new(jtk_Allocator_t* allocator);

/* Destructor */

/**
 * @memberof BestFitAllocator
 */
void zen_BestFitAllocator_delete(jtk_Allocator_t* allocator,
    zen_BestFitAllocator_t* allocator);

/* Allocate */

/**
 * @memberof BestFitAllocator
 */
int8_t* zen_BestFitAllocator_allocate(zen_BestFitAllocator_t* allocator,
    int32_t headSize, int32_t bodySize);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_BEST_FIT_ALLOCATOR_H */