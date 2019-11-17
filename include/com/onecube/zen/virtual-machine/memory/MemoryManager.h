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

// Monday, September 03, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_MEMORY_MANAGER_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_MEMORY_MANAGER_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/memory/allocator/Allocator.h>
#include <com/onecube/zen/virtual-machine/memory/allocator/AllocationFlag.h>
#include <com/onecube/zen/virtual-machine/memory/allocator/AlignmentConstraint.h>
#include <com/onecube/zen/virtual-machine/memory/collector/GarbageCollectionType.h>
#include <com/onecube/zen/virtual-machine/memory/collector/GarbageCollectionReason.h>
#include <com/onecube/zen/virtual-machine/memory/collector/GarbageCollectionListener.h>
#include <com/onecube/zen/virtual-machine/memory/collector/GarbageCollectionFlag.h>
#include <com/onecube/zen/virtual-machine/memory/heap/NewGeneration.h>
#include <com/onecube/zen/virtual-machine/memory/heap/OldGeneration.h>
#include <com/onecube/zen/virtual-machine/memory/heap/ImmortalGeneration.h>

/*******************************************************************************
 * MemoryManager                                                               *
 *******************************************************************************/

/**
 * Each heap generation receives its own system allocator, private allocator
 * and a garbage collector.
 *
 * @class MemoryManager
 * @ingroup zen_mms
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_MemoryManager_t {
    zen_ImmortalGeneration_t* m_immortalGeneration;

    zen_NewGeneration_t* m_newGeneration;

    zen_OldGeneration_t* m_oldGeneration;

    /*
    zen_SegregatedFitsAllocator_t* m_segregatedFitsAllocator1;
    zen_SegregatedFitsAllocator_t* m_segregatedFitsAllocator2;
    zen_SequentialAllocator_t* m_sequentialAllocator;


    zen_MarkSweepGarbageCollector_t* m_markSweepGarbageCollector;
    zen_MarkCompactGarbageCollector_t* m_markCompactGarbageCollector;
    zen_CopyingGarbageCollector_t* m_copyingGarbageCollector;
    */
};

/**
 * @memberof MemoryManager
 */
typedef struct zen_MemoryManager_t zen_MemoryManager_t;

/* Constructor */

/**
 * @memberof MemoryManager
 */
zen_MemoryManager_t* zen_MemoryManager_new();

/* Destructor */

/**
 * @memberof MemoryManager
 */
void zen_MemoryManager_delete(zen_MemoryManager_t* manager);

/* Allocate */

/**
 * @memberof MemoryManager
 */
uint8_t* zen_MemoryManager_allocate(zen_MemoryManager_t* manager, uint32_t size);

/**
 * @memberof MemoryManager
 */
uint8_t* zen_MemoryManager_allocateEx(zen_MemoryManager_t* manager, uint32_t size,
    zen_AlignmentConstraint_t alignmentConstraint, int32_t flags);

/* Collect */

/**
 * @memberof MemoryManager
 */
void zen_MemoryManager_collect(zen_MemoryManager_t* manager,
    zen_GarbageCollectionType_t type, zen_GarbageCollectionReason_t reason);

/**
 * @memberof MemoryManager
 */
void zen_MemoryManager_collectEx(zen_MemoryManager_t* manager,
    zen_GarbageCollectionType_t type, zen_GarbageCollectionReason_t reason,
    zen_GarbageCollectionListener_t* listener, int32_t flags);

/* Deallocate */

/**
 * @memberof MemoryManager
 */
void zen_MemoryManager_deallocate(zen_MemoryManager_t* manager, uint8_t* pointer);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_MEMORY_MANAGER_H */