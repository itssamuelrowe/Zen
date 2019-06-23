// Sunday, September 02, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_HEAP_NEW_GENERATION_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_HEAP_NEW_GENERATION_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/memory/heap/Region.h>
#include <com/onecube/zen/virtual-machine/memory/allocator/SegregatedFitsAllocator.h>

/*******************************************************************************
 * NewGeneration                                                               *
 *******************************************************************************/

/**
 * @class NewGeneration
 * @ingroup zen_vm_memory_heap
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_NewGeneration_t {
    zen_Region_t* m_leftRegion;
    zen_Region_t* m_middleRegion;
    zen_Region_t* m_rightRegion;
    
    zen_SegregatedFitsAllocator_t* m_allocator;
};

/**
 * @memberof NewGeneration
 */
typedef struct zen_NewGeneration_t zen_NewGeneration_t;

#endif /* ZEN_MMS_HEAP_NEW_GENERATION_H */