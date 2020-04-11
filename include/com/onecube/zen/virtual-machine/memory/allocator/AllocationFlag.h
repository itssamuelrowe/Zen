/*
 * Copyright 2017-2020 Samuel Rowe
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

// Wednesday, January 23, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_ALLOCATION_FLAG_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_ALLOCATION_FLAG_H

/*******************************************************************************
 * AllocationFlag                                                              *
 *******************************************************************************/

enum zen_AllocationFlag_t {
    ZEN_ALLOCATION_FLAG_MANUAL = (1 << 0),
    ZEN_ALLOCATION_FLAG_AUTOMATIC = (1 << 1),
    ZEN_ALLOCATION_FLAG_IMMORTAL = (1 << 2)
};

typedef enum zen_AllocationFlag_t zen_AllocationFlag_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_ALLOCATION_FLAG_H */