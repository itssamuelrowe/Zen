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

// Sunday, March 16, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTION_PROFILE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTION_PROFILE_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/memory/collector/GarbageCollectionReason.h>
#include <com/onecube/zen/virtual-machine/memory/collector/GarbageCollectionType.h>

/*******************************************************************************
 * GarbageCollectionProfile                                                    *
 *******************************************************************************/

/**
 * @class GarbageCollectionProfile
 * @ingroup zen_vm_memory_collector
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_GarbageCollectionProfile_t {
    zen_GarbageCollectionReason_t m_reason;
    zen_GarbageCollectionType_t m_type;
    uint64_t m_memoryFreePreviously;
    uint64_t m_memoryUsedPreviously;
    uint64_t m_duration;
    
};

/**
 * @memberof GarbageCollectionProfile
 */
typedef struct zen_GarbageCollectionProfile_t zen_GarbageCollectionProfile_t;

zen_GarbageCollectionProfile_t* zen_GarbageCollectionProfile_new(
    zen_GarbageCollectionReason_t m_reason, zen_GarbageCollectionType_t type,
    uint64_t memoryFreePreviously, uint64_t memoryUsedPreviously);

/**
 * @memberof GarbageCollectionProfile
 */
zen_GarbageCollectionProfile_t* zen_GarbageCollectionProfile_newEx(
    zen_GarbageCollectionReason_t m_reason, zen_GarbageCollectionType_t type,
    uint64_t memoryFreePreviously, uint64_t memoryUsedPreviously,
    uint64_t memoryFreeCurrently, uint64_t memoryUsedCurrently, uint64_t duration);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTION_PROFILE_H */