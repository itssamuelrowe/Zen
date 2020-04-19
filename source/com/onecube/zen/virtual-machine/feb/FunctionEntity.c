/*
 * Copyright 2018-2019 Samuel Rowe
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

// Monday, October 07, 2019

#include <com/onecube/zen/virtual-machine/feb/FunctionEntity.h>

/*******************************************************************************
 * FunctionEntity                                                              *
 *******************************************************************************/

// Constructor

zen_FunctionEntity_t* zen_FunctionEntity_new(uint16_t flags, uint16_t nameIndex,
    uint16_t descriptorIndex, uint16_t tableIndex) {
    zen_FunctionEntity_t* functionEntity = zen_Memory_allocate(zen_FunctionEntity_t, 1);
    functionEntity->m_flags = flags;
    functionEntity->m_nameIndex = nameIndex;
    functionEntity->m_descriptorIndex = descriptorIndex;
    functionEntity->m_tableIndex = tableIndex;

    return functionEntity;
}

// Destructor

void zen_FunctionEntity_delete(zen_FunctionEntity_t* functionEntity) {
    jtk_Assert_assertObject(functionEntity, "The specified function entity is null.");

    zen_Memory_deallocate(functionEntity);
}