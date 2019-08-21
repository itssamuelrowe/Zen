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

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_HEAP_OBJECT_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_HEAP_OBJECT_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * Object                                                                      *
 *******************************************************************************/

/**
 * @class Object
 * @ingroup zen_heap
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_Object_t {
    int32_t m_flags;
    int32_t m_size;
};

/**
 * @memberof Object
 */
typedef struct zen_Object_t zen_Object_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_HEAP_OBJECT_H */