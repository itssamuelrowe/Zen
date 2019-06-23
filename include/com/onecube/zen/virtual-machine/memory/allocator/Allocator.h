// Thursday, August 30, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_ALLOCATOR_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_ALLOCATOR_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * Allocator                                                                   *
 *******************************************************************************/

/**
 * @class Allocator
 * @ingroup zen_heap
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_Allocator_t {
};

/**
 * @memberof Allocator
 */
typedef struct zen_Allocator_t zen_Allocator_t;
 
/* Allocate */

/**
 * @memberof Allocator
 */
void* zen_Allocator_allocate(zen_Allocator_t* allocator, uint32_t size);

/**
 * @memberof Allocator
 */
void* zen_Allocator_tryAllocate(zen_Allocator_t* allocator, uint32_t size);

/* Deallocate */

/**
 * @memberof Allocator
 */
void zen_Allocator_deallocate(zen_Allocator_t* allocator, void* buffer, uint32_t size);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_ALLOCATOR_H */