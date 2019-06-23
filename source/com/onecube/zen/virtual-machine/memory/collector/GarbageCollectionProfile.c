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
