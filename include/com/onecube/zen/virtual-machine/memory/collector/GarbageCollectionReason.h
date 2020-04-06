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

// Thursday, August 30, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTION_REASON_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTION_REASON_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * GarbageCollectionReason                                                     *
 *******************************************************************************/

/**
 * @class GarbageCollectionReason
 * @ingroup zen_vm_memory_collector
 * @author Samuel Rowe
 * @since zen 1.0
 */
enum zen_GarbageCollectionReason_t {
    ZEN_GARBAGE_COLLECTION_REASON_NATIVE_REQUEST,
    ZEN_GARBAGE_COLLECTION_REASON_APPLICATION_REQUEST
};

/**
 * @memberof GarbageCollectionReason
 */
typedef enum zen_GarbageCollectionReason_t zen_GarbageCollectionReason_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTION_REASON_H */