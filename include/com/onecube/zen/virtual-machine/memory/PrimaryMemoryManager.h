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

// Wednesday, January 23, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_PRIMARY_MEMORY_MANAGER_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_PRIMARY_MEMORY_MANAGER_H

/*******************************************************************************
 * PrimaryMemoryManager                                                        *
 *******************************************************************************/

/**
 * @class PrimaryMemoryManager
 * @ingroup zen_vm_memory_allocator
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_PrimaryMemoryManager_t {
    jtk_Allocator_t* m_allocator;
    zen_MemoryManager_t* m_memoryManager;
};

/**
 * @memberof PrimaryMemoryManager
 */
typedef struct zen_PrimaryMemoryManager_t zen_PrimaryMemoryManager_t;

/* Constructor */

zen_PrimaryMemoryManager_t* zen_PrimaryMemoryManager_new(jtk_Allocator_t* allocator);

/* Destructor */

void zen_PrimaryMemoryManager_delete(zen_PrimaryMemoryManager_t* primaryMemoryManager);

/* Allocate */

uint8_t* zen_PrimaryMemoryManager_allocateRawPrimitiveArray(
    zen_PrimaryMemoryManager_t* primaryMemoryManager, zen_Type_t type,
    int32_t length);

uint8_t* zen_PrimaryMemoryManager_allocatePrimitiveArray(
    zen_PrimaryMemoryManager_t* primaryMemoryManager, zen_Type_t type,
    int32_t length);

uint8_t* zen_PrimaryMemoryManager_allocateBooleanArray(
    zen_PrimaryMemoryManager_t* primaryMemoryManager, int32_t length);

uint8_t* zen_PrimaryMemoryManager_allocateInteger8Array(
    zen_PrimaryMemoryManager_t* primaryMemoryManager, int32_t length);

uint8_t* zen_PrimaryMemoryManager_allocateInteger16Array(
    zen_PrimaryMemoryManager_t* primaryMemoryManager, int32_t length);

uint8_t* zen_PrimaryMemoryManager_allocateInteger32Array(
    zen_PrimaryMemoryManager_t* primaryMemoryManager, int32_t length);

uint8_t* zen_PrimaryMemoryManager_allocateInteger64Array(
    zen_PrimaryMemoryManager_t* primaryMemoryManager, int32_t length);

uint8_t* zen_PrimaryMemoryManager_allocateDecimal32Array(
    zen_PrimaryMemoryManager_t* primaryMemoryManager, int32_t length);

uint8_t* zen_PrimaryMemoryManager_allocateDecimal64Array(
    zen_PrimaryMemoryManager_t* primaryMemoryManager, int32_t length);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_PRIMARY_MEMORY_MANAGER_H */


