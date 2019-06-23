// Thursday, August 30, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_HEAP_HEAP_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_HEAP_HEAP_H

/*******************************************************************************
 * Heap                                                                        *
 *******************************************************************************/

/**
 * @class Heap
 * @ingroup zen_heap
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_Heap_t {
};

/**
 * @memberof Heap
 */
typedef struct zen_Heap_t zen_Heap_t;

/* Constructor */

/**
 * @memberof Heap
 */
zen_Heap_t* zen_Heap_new();

/* Destructor */

/**
 * @memberof Heap
 */
void zen_Heap_delete(zen_Heap_t* heap);

/* Allocate */

/**
 * @memberof Heap
 */
void* zen_Heap_allocate(zen_Heap_t* heap, zen_Allocator_t* allocator);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_HEAP_HEAP_H */