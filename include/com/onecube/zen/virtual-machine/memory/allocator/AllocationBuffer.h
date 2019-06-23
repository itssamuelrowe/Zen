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