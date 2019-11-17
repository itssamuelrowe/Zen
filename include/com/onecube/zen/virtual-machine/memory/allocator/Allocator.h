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

// Thursday, August 30, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_ALLOCATOR_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_ALLOCATOR_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * Allocator                                                                   *
 *******************************************************************************/

/**
 * @class Allocator
 * @ingroup zen_heap
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_Allocator_t {
};

/**
 * @memberof Allocator
 */
typedef struct zen_Allocator_t zen_Allocator_t;
 
/* Allocate */

/**
 * @memberof Allocator
 */
void* zen_Allocator_allocate(zen_Allocator_t* allocator, uint32_t size);

/**
 * @memberof Allocator
 */
void* zen_Allocator_tryAllocate(zen_Allocator_t* allocator, uint32_t size);

/* Deallocate */

/**
 * @memberof Allocator
 */
void zen_Allocator_deallocate(zen_Allocator_t* allocator, void* buffer, uint32_t size);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_ALLOCATOR_H */