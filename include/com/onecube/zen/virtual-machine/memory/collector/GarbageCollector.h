// Thursday, August 30, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_COLLECTOR_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_COLLECTOR_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/memory/heap/NewGeneration.h>
#include <com/onecube/zen/virtual-machine/memory/heap/OldGeneration.h>
#include <com/onecube/zen/virtual-machine/memory/heap/ImmortalGeneration.h>

/*******************************************************************************
 * GarbageCollector                                                            *
 *******************************************************************************/

/**
 * @class GarbageCollector
 * @ingroup zen_vm_memory_collector
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_GarbageCollector_t {
    int64_t m_lastCollectionTime;
    int64_t m_totalCollectionTime;
};

/**
 * @memberof GarbageCollector
 */
typedef struct zen_GarbageCollector_t zen_GarbageCollector_t;

/* Constructor */

/**
 * @memberof GarbageCollector
 */
zen_GarbageCollector_t* zen_GarbageCollector_new();

/* Destructor */

/**
 * @memberof GarbageCollector
 */
void zen_GarbageCollector_delete(zen_GarbageCollector_t* planet);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_COLLECTOR_H */