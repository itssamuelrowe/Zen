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

// Friday, May 18, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_VIRTUAL_MACHINE_CONFIGURATION_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_VIRTUAL_MACHINE_CONFIGURATION_H

#include <jtk/collection/list/ArrayList.h>
#include <jtk/collection/map/HashMap.h>

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * VirtualMachineConfiguration                                                 *
 *******************************************************************************/

/**
 * @class VirtualMachineConfiguration
 * @ingroup zen_virtual_machine_native
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_VirtualMachineConfiguration_t {
    jtk_ArrayList_t* m_entityDirectories;
    jtk_HashMap_t* m_variables;
};

/**
 * @memberof VirtualMachineConfiguration
 */
typedef struct zen_VirtualMachineConfiguration_t zen_VirtualMachineConfiguration_t;

/* Constructor */

/**
 * @memberof VirtualMachineConfiguration
 */
zen_VirtualMachineConfiguration_t* zen_VirtualMachineConfiguration_new();

/* Destructor */

/**
 * @memberof VirtualMachineConfiguration
 */
void zen_VirtualMachineConfiguration_delete(zen_VirtualMachineConfiguration_t* configuration);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_VIRTUAL_MACHINE_CONFIGURATION_H */