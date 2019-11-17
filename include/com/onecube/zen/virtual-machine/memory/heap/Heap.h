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

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_HEAP_HEAP_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_HEAP_HEAP_H

/*******************************************************************************
 * Heap                                                                        *
 *******************************************************************************/

/**
 * @class Heap
 * @ingroup zen_heap
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_Heap_t {
};

/**
 * @memberof Heap
 */
typedef struct zen_Heap_t zen_Heap_t;

/* Constructor */

/**
 * @memberof Heap
 */
zen_Heap_t* zen_Heap_new();

/* Destructor */

/**
 * @memberof Heap
 */
void zen_Heap_delete(zen_Heap_t* heap);

/* Allocate */

/**
 * @memberof Heap
 */
void* zen_Heap_allocate(zen_Heap_t* heap, zen_Allocator_t* allocator);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_HEAP_HEAP_H */