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

// Saturday, May 11, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_ENTITY_DESCRIPTOR_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_ENTITY_DESCRIPTOR_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * EntityDescriptor                                                            *
 *******************************************************************************/

/**
 * @class EntityDescriptor
 * @ingroup com_onecube_virtual_machine_loader
 * @author Samuel Rowe
 * @since Newton Virtual Machine 1.0
 */
struct zen_EntityDescriptor_t {
    jtk_String_t* m_value;
};

/**
 * @memberof EntityDescriptor
 */
typedef struct zen_EntityDescriptor_t zen_EntityDescriptor_t;

// Constructor

zen_EntityDescriptor_t* zen_EntityDescriptor_new(jtk_String_t* value);
zen_EntityDescriptor_t* zen_EntityDescriptor_newEx(const uint8_t* value);

// Destructor

void zen_EntityDescriptor_delete(zen_EntityDescriptor_t* descriptor);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_ENTITY_DESCRIPTOR_H */
