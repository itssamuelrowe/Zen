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

// Monday, September 10, 2018

#include <com/onecube/zen/virtual-machine/memory/allocator/SegregatedFitsAllocator.h>

/*******************************************************************************
 * SegregatedFitsAllocator                                                     *
 *******************************************************************************/

#define ZEN_SEGREGATED_FITS_ALLOCATOR_BASE_CLASS_SIZE 8
#define ZEN_SEGREGATED_FITS_ALLOCATOR_SIZE_CLASS_FACTOR 8

/* Constructor */

/* TODO:
 * The performance of the segregated-fits allocator can be enhanced with dynamic
 * insertion of size classes. Thus, not all objects are allocated from the general
 * free-lists allocator.
 */

zen_SegregatedFitsAllocator_t* zen_SegregatedFitsAllocator_new(jtk_Allocator_t* allocator, uint8_t* chunk, int32_t size, int32_t balanceFactor) {
    jtk_Assert_assertObject(allocator, "The specified allocator is null.");
    jtk_Assert_assertObject(chunk, "The specified memory chunk is null.");
    jtk_Assert_assertTrue(size > 0, "The specified memory chunk size is invalid.");
    jtk_Assert_assertTrue(balanceFactor > 0, "The specified balance factor is invalid.");

    zen_SegregatedFitsAllocator_t* segregatedFitsAllocator =
        (zen_SegregatedFitsAllocator_t*)jtk_Allocator_allocate(
            allocator, sizeof (zen_SegregatedFitsAllocator_t), 1);
    segregatedFitsAllocator->m_balanceFactor = balanceFactor;
    segregatedFitsAllocator->m_sizeClassesSize = balanceFactor + 1;
    segregatedFitsAllocator->m_sizeClasses = jtk_Allocator_allocate(allocator, sizeof (int32_t), segregatedFitsAllocator->m_sizeClassesSize);
    segregatedFitsAllocator->m_allocator = allocator;
    segregatedFitsAllocator->m_freeListAllocators = ...;

    zen_SegregatedFitsAllocator_initializeSizeClasses(segregatedFitsAllocator);

    return segregatedFitsAllocator;
}

/* Destructor */

void zen_SegregatedFitsAllocator_delete(zen_SegregatedFitsAllocator_t* segregatedFitsAllocator) {
    jtk_Assert_assertObject(segregatedFitsAllocator, "The specified segregated-fits allocator is null.");

    jtk_Allocator_deallocate(segregatedFitsAllocator->m_allocator,
        segregatedFitsAllocator->m_sizeClasses);
    jtk_Allocator_deallocate(segregatedFitsAllocator->m_allocator, allocator);
}

/* Allocate */

uint8_t* zen_SegregatedFitsAllocator_allocateWithFirstFit(
    zen_SegregatedFitsAllocator_t* allocator, int32_t headSize, int32_t bodySize) {
    jtk_Assert_assertObject(allocator, "The specified allocator is null.");

    uint8_t* result = NULL;
    int32_t requestSize = headSize + bodySize;
    int32_t sizeClass = zen_SegregatedFitsAllocator_findSizeClass(allocator, size);

    int32_t i;
    for (i = sizeClass; i <= allocator->m_balanceFactor; i++) {
        xjtk_Logger_debug(ZEN_SEGREGATED_FITS_ALLOCATOR_TAG, "Trying to allocate object from a first-fit allocator. (sizeClass = %d, headSize = %d, bodySize = %d)",
            sizeClass, headSize, bodySize);

        zen_FirstFitAllocator_t* firstFitAllocator =
            (zen_FirstFitAllocator_t*)allocator->m_freeListAllocators[i];
        result = zen_FirstFitAllocator_allocate(firstFitAllocator, headSize, bodySize);

        if (result == NULL) {
            /* At this point, the allocation request could not satisfied using 
             * the first-fit allocator. The allocator will try again to
             * allocate using the next first-fit allocator.
             */
            xjtk_Logger_debug(ZEN_SEGREGATED_FITS_ALLOCATOR_TAG, "Failed to allocate object from the first-fit allocator. The allocator will now try to satisfy the allocation request with the next general first-fit allocator, if any. (sizeClass = %d, headSize = %d, bodySize = %d)",
                sizeClass, headSize, bodySize);
        }
        else {
            /* At this point, the allocation was completed succesfully using a
             * first-fit allocator.
             */
            xjtk_Logger_debug(ZEN_SEGREGATED_FITS_ALLOCATOR_TAG, "Successfully allocated memory chunk from a specific first-fit allocator. (sizeClass = %d, headSize = %d, bodySize = %d)",
                sizeClass, headSize, bodySize);
            break;
        }
    }
}

uint8_t* zen_SegregatedFitsAllocator_allocateWithBestFit(
    zen_SegregatedFitsAllocator_t* allocator, int32_t headSize, int32_t bodySize) {
    jtk_Assert_assertObject(allocator, "The specified allocator is null.");

    uint8_t* result = NULL;
    int32_t requestSize = headSize + bodySize;
    int32_t sizeClass = zen_SegregatedFitsAllocator_findSizeClass(allocator, size);

    int32_t i;
    for (i = sizeClass; i <= allocator->m_balanceFactor; i++) {
        xjtk_Logger_debug(ZEN_SEGREGATED_FITS_ALLOCATOR_TAG, "Trying to allocate object from a best-fit allocator. (sizeClass = %d, headSize = %d, bodySize = %d)",
            sizeClass, headSize, bodySize);

        zen_BestFitAllocator_t* bestFitAllocator =
            (zen_BestFitAllocator_t*)allocator->m_freeListAllocators[i];
        result = zen_BestFitAllocator_allocate(bestFitAllocator, headSize, bodySize);

        if (result == NULL) {
            /* At this point, the allocation request could not satisfied using 
             * the best-fit allocator. The allocator will try again to
             * allocate using the next best-fit allocator.
             */
            xjtk_Logger_debug(ZEN_SEGREGATED_FITS_ALLOCATOR_TAG, "Failed to allocate object from the best-fit allocator. The allocator will now try to satisfy the allocation request with the next general best-fit allocator, if any. (sizeClass = %d, headSize = %d, bodySize = %d)",
                sizeClass, headSize, bodySize);
        }
        else {
            /* At this point, the allocation was completed succesfully using a
             * best-fit allocator.
             */
            xjtk_Logger_debug(ZEN_SEGREGATED_FITS_ALLOCATOR_TAG, "Successfully allocated memory chunk from a specific best-fit allocator. (sizeClass = %d, headSize = %d, bodySize = %d)",
                sizeClass, headSize, bodySize);
            break;
        }
    }
}

uint8_t* zen_SegregatedFitsAllocator_allocate(zen_SegregatedFitsAllocator_t* allocator,
    int32_t headSize, int32_t bodySize) {
    jtk_Assert_assertObject(allocator, "The specified allocator is null.");

    uint8_t* result = NULL;
    switch (allocator->m_mode) {
        case ZEN_SEGREGATED_FITS_ALLOCATOR_MODE_FIRST_FIT: {
            xjtk_Logger_debug(ZEN_SEGREGATED_FITS_ALLOCATOR_TAG, "Trying to satisfy allocation request using best-fit allocator. (headSize = %d, bodySize = %d)",
                headSize, bodySize);

            result = zen_SegregatedFitsAllocator_allocateWithFirstFit(allocator, headSize, bodySize);

            break;
        }

        case ZEN_SEGREGATED_FITS_ALLOCATOR_MODE_BEST_FIT: {
            xjtk_Logger_debug(ZEN_SEGREGATED_FITS_ALLOCATOR_TAG, "Trying to satisfy allocation request using best-fit allocator. (headSize = %d, bodySize = %d)",
                headSize, bodySize);

            result = zen_SegregatedFitsAllocator_allocateWithBestFit(allocator, headSize, bodySize);

            break;
        }

        default: {
            xjtk_Logger_debug(ZEN_SEGREGATED_FITS_ALLOCATOR_TAG, "Unknown segregated fits allocator mode encountered. (mode = %d, headSize = %d, bodySize = %d)",
                allocator->m_mode, headSize, bodySize);

            break;
        }
    }

    return result;
}

/* Size Class */

int32_t zen_SegregatedFitsAllocator_getSizeClass(int32_t index) {
    return ZEN_SEGREGATED_FITS_ALLOCATOR_INITIAL_SIZE_CLASS +
        (ZEN_SEGREGATED_FITS_ALLOCATOR_SIZE_CLASS_FACTOR * index);
}

void zen_SegregatedFitsAllocator_initializeSizeClasses(zen_SegregatedFitsAllocator_t*
    allocator) {
    jtk_Assert_assertObject(allocator, "The specified segregated-fits allocator is null.");

    int32_t i;
    for (i = 0; i < allocator->m_balanceFactor; i++) {
        allocator->m_sizeClasses[i] =
            zen_SegregatedFitsAllocator_getSizeClass(i);
    }
    segregatedFitsAllocator->m_sizeClasses[i] = JTK_INTEGER_MAX_VALUE;
}

zen_SizeClass_t* zen_SegregatedFitsAllocator_findSizeClass(
    zen_SegregatedFitsAllocator_t* allocator, int32_t size) {
    jtk_Assert_assertObject(allocator, "The specified allocator is null.");

    int32_t index = -1;
    int32_t leftIndex = 0;
    int32_t rightIndex = allocator->m_balanceFactor; /* Alternatively, `allocator->m_sizeClassesSize - 1` */
    
    /* The size of a free cell, represented by `size`, is limited to
     * `classes[i - 1] < size <= classes[i]`. We assume that `classes[-1] = 0`
     * and `classes[k] = JTK_INTEGER_MAX_VALUE`.
     * The allocator has `balanceFactor + 1` free-lists, the last free-list
     * is used to allocate large objects. All the free-lists are included
     * when searching for a size class.
     *
     * Given the size classes are always sorted in ascending order, the allocator
     * applies an algorithm based on the binary search algorithm. First, a regular
     * binary search algorithm is performed, with the specified size as the key.
     * If the specified size is not equal to a particular size class, the resulting
     * index remains a negative integer. In which case, the left index is assumed
     * as the size class.
     */
    while (leftIndex <= rightIndex) {
        int32_t middleIndex = (uint32_t)(leftIndex + rightIndex) >> 1;

        int32_t sizeClass = allocator->m_sizeClasses[middleIndex];
        if (sizeClass < size) {
            leftIndex = middleIndex + 1;
        }
        else if (sizeClass > size) {
            rightIndex = middleIndex - 1;
        }
        else {
            result = middleIndex;
            break;
        }
    }
    
    return (result < 0)? leftIndex : result;

    /* The following code segment implements a linear search algorithm.
     * It was the original algorithm that the allocator used. It was replaced
     * with a binary search algorithm to improve performance. Further, the algorithm
     * has now evolved.
     */
//    int32_t index;
//    for (index = 0; index < allocator->m_balanceFactor; index++) {
//        if ((size > previous) && (size <= allocator->m_sizeClasses[index])) {
//            /* The size class was found. We can terminate the loop now. */
//            break;
//        }
//        else {
//            /* The size class was not found. */
//            previous = allocator->m_sizeClasses[index];
//        }
//    }
}

struct zen_ObjectHeader_t {
    uint8_t m_type;
    void* m_typeReference;
    union {
        struct {
            uint16_t m_length;
        } m_array;
    };
};