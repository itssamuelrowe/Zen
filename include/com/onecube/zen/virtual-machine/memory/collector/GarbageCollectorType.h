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

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTOR_TYPE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTOR_TYPE_H

/*******************************************************************************
 * GarbageCollectorType                                                        *
 *******************************************************************************/

enum zen_GarbageCollectorType_t {
    ZEN_GARBAGE_COLLECTOR_TYPE_MARK_SWEEP,
    ZEN_GARBAGE_COLLECTOR_TYPE_COPYING
};

typedef enum zen_GarbageCollectorType_t zen_GarbageCollectorType_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTOR_TYPE_H */