// Thursday, August 30, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTION_TYPE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTION_TYPE_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * GarbageCollectionType                                                              *
 *******************************************************************************/

/**
 * @class GarbageCollectionType
 * @ingroup zen_vm_memory_collector
 * @author Samuel Rowe
 * @since zen 1.0
 */
enum zen_GarbageCollectionType_t {
    ZEN_GARBAGE_COLLECTION_TYPE_MINOR,
    ZEN_GARBAGE_COLLECTION_TYPE_MAJOR
};

/**
 * @memberof GarbageCollectionType
 */
typedef enum zen_GarbageCollectionType_t zen_GarbageCollectionType_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTION_TYPE_H */