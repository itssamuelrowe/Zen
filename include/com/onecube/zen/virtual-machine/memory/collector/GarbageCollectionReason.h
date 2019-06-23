// Thursday, August 30, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTION_REASON_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTION_REASON_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * GarbageCollectionReason                                                     *
 *******************************************************************************/

/**
 * @class GarbageCollectionReason
 * @ingroup zen_vm_memory_collector
 * @author Samuel Rowe
 * @since zen 1.0
 */
enum zen_GarbageCollectionReason_t {
    ZEN_GARBAGE_COLLECTION_REASON_NATIVE_REQUEST,
    ZEN_GARBAGE_COLLECTION_REASON_APPLICATION_REQUEST
};

/**
 * @memberof GarbageCollectionReason
 */
typedef enum zen_GarbageCollectionReason_t zen_GarbageCollectionReason_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTION_REASON_H */