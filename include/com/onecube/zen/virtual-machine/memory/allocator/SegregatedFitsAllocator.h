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

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_SEGREGATED_FITS_ALLOCATOR_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_SEGREGATED_FITS_ALLOCATOR_H

#include <jtk/memory/VirtualMemory.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/memory/allocator/Allocator.h>
#include <com/onecube/zen/virtual-machine/memory/allocator/SegregatedFitsAllocatorMode.h>
#include <com/onecube/zen/virtual-machine/memory/allocator/SizeClass.h>

/*******************************************************************************
 * SegregatedFitsAllocator                                                     *
 *******************************************************************************/

/**
 * The segregated-fits allocator uses multiple free-list allocators for
 * allocating memory. This helps in reducing fragmentation and decreases the
 * time taken to allocate memory. The free-lists are assigned memory chunks
 * divided from a single chunk.
 *
 * Initially, an allocator has few large cells of contiguous free memory.
 * As the program allocates and deallocates memory cells, it produces a large
 * number of smaller cells. This is known as fragmentation.
 *
 * Fragmentation has the following disadvantages:
 *    - It causes memory allocation failure albeit the memory chunk having
 *      enough free memory in total. This happens when no particular cell size
 *      satisfies the request. Which goes to say, it triggers garbage collection
 *      sooner than necessary.
 *
 * A basic free-list allocator spends most of its time searching for a free
 * cell of an appropriate size. The allocator can avoid this using multiple
 * free-lists, each associated with a *size class*. This can improve allocation
 * speed drastically.
 *
 * A segregated-fits allocator has a number of size classes known as
 * *balance factor*. For an allocator with a balance factor of `k`, the size
 * classes are of the form `sizeClass[0] < sizeClass[1] < ... < sizeClass[k - 1]`.
 * Each allocator may have a different balance factor. It is fixed, that is,
 * does not change after creation.
 *
 * A segregated-fits allocator with a balance factor of `k` has `k + 1`
 * free-lists. The last free-list is used to allocate large objects which
 * do not fit in any of the size classes.
 *
 * The size of a free cell, represented as `cellSize`, is limited
 * to `sizeClass[i - 1] < cellSize <= sizeClass[i]`. We assume that
 * `sizeClass[-1] = 0` and `sizeClass[k] = JTK_INTEGER_MAX`. The allocator
 * tries to *round up* a given request size, represented as `requestSize`,
 * to the nearest size class, represented as `sizeClass[i]`, such that
 * `cellSize <= sizeClass[i]`. Although this does not apply to cell sizes
 * larger than `sizeClass[k - 1]`, in which case the last free list is used.
 *
 * The last free list, represented as `freeList[k]`, may use a free-list
 * algorithm such as first-fit, next-fit, and best-fit. As of version 1.0,
 * it uses a first-fit allocator backed by a cartesian tree. Usually, large
 * objects are less frequently allocated. Even if that's not the case,
 * segregated-fits allocator should be more efficient than normal free-list
 * allocators.
 *
 * The size classes are evenly distributed. The user may provide only the
 * balance factor, but not class sizes. An arbitrary class size, represented as
 * `sizeClass[i]`, is calculated with the formula `sizeClass[i] = sizeClass[0] + c * i`,
 * where `c > 0`. We assume, `sizeClass[0] = 8` and `c = 8` and `k = 16`.
 * This gives us size classes as multiples of eight, from `8` to `128`.
 * The first-fit allocator is used for request sizes greater than `128`.
 *
 * Please refer "The Garbage Collection Handbook" by Richard Jones, Antony
 * Hosking and Eliot Moss for more information. The algorithm described above
 * was extracted from Chapter 7, section 4.
 *
 * @class SegregatedFitsAllocator
 * @ingroup zen_memory_allocator
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_SegregatedFitsAllocator_t {

    /**
     * It is in the range [8, 32]. For example, for an allocator with a balance
     * factor of 8, there are 8 size classes. By default, the implementation uses
     * a balance factor of 16. Which gives us the 16 size classes ranging from
     * 8 to 128.
     *
     *  8 + (8 * 0) = 8
     *  8 + (8 * 1) = 16
     *  8 + (8 * 2) = 24
     *  8 + (8 * 3) = 32
     *  8 + (8 * 4) = 40
     *  8 + (8 * 5) = 48
     *  8 + (8 * 6) = 56
     *  8 + (8 * 7) = 64
     *  8 + (8 * 8) = 72
     *  8 + (8 * 9) = 80
     *  8 + (8 * 10) = 88
     *  8 + (8 * 11) = 96
     *  8 + (8 * 12) = 104
     *  8 + (8 * 13) = 112
     *  8 + (8 * 14) = 120
     *  8 + (8 * 15) = 128
     *
     * Large balance factors may result in adverse effects. However, I have
     * made no efforts to verify this statement.
     */
    int8_t m_balanceFactor;

    int32_t* m_sizeClasses;

    void** m_freeListAllocators;

    zen_Allocator_t* m_allocator;

    zen_SegregatedFitsAllocatorMode_t m_mode;
};

/**
 * @memberof SegregatedFitsAllocator
 */
typedef struct zen_SegregatedFitsAllocator_t zen_SegregatedFitsAllocator_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_SEGREGATED_FITS_ALLOCATOR_H */