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

// Friday, August 31, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_HEAP_GENERATION_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_HEAP_GENERATION_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * Generation                                                                       *
 *******************************************************************************/

/**
 * @class Generation
 * @ingroup zen_heap
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_Generation_t {
    uintptr_t* m_words;
    uint32_t m_position;
    uint32_t m_capacity;
};

/**
 * @memberof Generation
 */
typedef struct zen_Generation_t zen_Generation_t;

/* Constructor */

/**
 * @memberof Generation
 */
zen_Generation_t* zen_Generation_new(zen_Planet_t* planet, zen_Directory_t* directory);

/* Destructor */

/**
 * @memberof Generation
 */
void zen_Generation_delete(zen_Generation_t* generation);

/* Allocate */

/**
 * @memberof Generation
 */
void* zen_Generation_allocate(zen_Generation_t* generation, uint32_t size);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_HEAP_GENERATION_H */