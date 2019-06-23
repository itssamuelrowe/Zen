// Sunday, September 02, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTION_LISTENER_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTION_LISTENER_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/memory/collector/GarbageCollector.h>
#include <com/onecube/zen/virtual-machine/memory/collector/GarbageCollectionProfile.h>

/*******************************************************************************
 * GarbageCollectionListener                                                    *
 *******************************************************************************/

// Forward Declarations

/**
 * @memberof GarbageCollectionListener
 */
typedef struct zen_GarbageCollectionListener_t zen_GarbageCollectionListener_t;

// Collection

/**
 * @memberof GarbageCollectionListener
 */
typedef void (*zen_GarbageCollectionListener_OnBeforeGarbageCollectionFunction_t)(zen_GarbageCollectionListener_t* listener, zen_GarbageCollectionProfile_t* profile);

/**
 * @memberof GarbageCollectionListener
 */
typedef void (*zen_GarbageCollectionListener_OnAfterGarbageCollectionFunction_t)(zen_GarbageCollectionListener_t* listener, zen_GarbageCollectionProfile_t* profile);

// Initialization

/**
 * @memberof GarbageCollectionListener
 */
typedef void (*zen_GarbageCollectionListener_OnBeforeInitializationFunction_t)(zen_GarbageCollectionListener_t* listener, zen_GarbageCollector_t* collector);

/**
 * @memberof GarbageCollectionListener
 */
typedef void (*zen_GarbageCollectionListener_OnAfterInitializationFunction_t)(zen_GarbageCollectionListener_t* listener, zen_GarbageCollector_t* collector);

/**
 * @class GarbageCollectionListener
 * @ingroup zen_vm_memory_collector
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_GarbageCollectionListener_t {
    zen_GarbageCollectionListener_OnBeforeInitializationFunction_t m_onBeforeInitialization;
    zen_GarbageCollectionListener_OnAfterInitializationFunction_t m_onAfterInitialization;
    zen_GarbageCollectionListener_OnBeforeGarbageCollectionFunction_t m_onBeforeGarbageCollection;
    zen_GarbageCollectionListener_OnAfterGarbageCollectionFunction_t m_onAfterGarbageCollection;
};

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTION_LISTENER_H */