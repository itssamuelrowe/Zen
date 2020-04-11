/*
 * Copyright 2018-2020 Samuel Rowe
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