// Sunday, September 02, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_HEAP_OLD_GENERATION_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_HEAP_OLD_GENERATION_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/memory/heap/Region.h>

/*******************************************************************************
 * OldGeneration                                                               *
 *******************************************************************************/

/**
 * @class OldGeneration
 * @ingroup zen_heap
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_OldGeneration_t {
    zen_Region_t* m_whiteRegion;
    zen_Region_t* m_blackRegion;
};

/**
 * @memberof OldGeneration
 */
typedef struct zen_OldGeneration_t zen_OldGeneration_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_HEAP_OLD_GENERATION_H */