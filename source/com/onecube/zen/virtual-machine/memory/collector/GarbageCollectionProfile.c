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

// Saturday, March 16, 2019

#include <com/onecube/zen/virtual-machine/memory/collector/GarbageCollectionProfile.h>

/*******************************************************************************
 * GarbageCollectionProfile                                                    *
 *******************************************************************************/

zen_GarbageCollectionProfile_t* zen_GarbageCollectionProfile_new(
    zen_GarbageCollectionReason_t m_reason, zen_GarbageCollectionType_t type,
    uint64_t memoryFreePreviously, uint64_t memoryUsedPreviously) {
    
    return zen_GarbageCollectionProfile_newEx(reason, type, memoryFreePreviously,
        memoryUsedPreviously, 0, 0, 0);
}

zen_GarbageCollectionProfile_t* zen_GarbageCollectionProfile_newEx(
    zen_GarbageCollectionReason_t m_reason, zen_GarbageCollectionType_t type,
    uint64_t memoryFreePreviously, uint64_t memoryUsedPreviously,
    uint64_t memoryFreeCurrently, uint64_t memoryUsedCurrently, uint64_t duration) {
        zen_GarbageCollectionProfile_t* profile = zen_ReserveAllocator_allocate(zen_GarbageCollectionProfile_t, 1);
    profile->m_reason = reason;
    profile->m_type = type;
    profile->m_memoryFreePreviously = memoryFreePreviously;
    profile->m_memoryUsedPreviously = memoryUsedPreviously;
    profile->m_memoryFreeCurrently = memoryFreeCurrently;
    profile->m_memoryUsedPreviously = memoryUsedCurrently;
    profile->m_duration = duration;
    
    return profile;
}
