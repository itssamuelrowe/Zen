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

#include <com/onecube/zen/virtual-machine/feb/FieldEntity.h>

/*******************************************************************************
 * FieldEntity                                                                 *
 *******************************************************************************/

// Constructor

zen_FieldEntity_t* zen_FieldEntity_new(uint16_t flags, uint16_t nameIndex,
    uint16_t descriptorIndex) {
    zen_FieldEntity_t* entity = zen_Memory_allocate(zen_FieldEntity_t, 1);
    entity->m_flags = flags;
    entity->m_nameIndex = nameIndex;
    entity->m_descriptorIndex = descriptorIndex;
    
    return entity;
}

// Destructor

void zen_FieldEntity_delete(zen_FieldEntity_t* entity) {
    jtk_Assert_assertObject(entity, "The specified field entity is null.");
    
    zen_Memory_deallocate(entity);
}