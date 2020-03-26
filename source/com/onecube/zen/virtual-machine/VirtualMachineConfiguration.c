/*
 * Copyright 2018-2020 Samuel Rowe
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

// Friday, June 08, 2018

#include <jtk/core/CStringObjectAdapter.h>
#include <com/onecube/zen/virtual-machine/VirtualMachineConfiguration.h>

/*******************************************************************************
 * VirtualMachineConfiguration                                                 *
 *******************************************************************************/

/* Constructor */

zen_VirtualMachineConfiguration_t* zen_VirtualMachineConfiguration_new() {
    jtk_ObjectAdapter_t* stringObjectAdapter = jtk_CStringObjectAdapter_getInstance();

    zen_VirtualMachineConfiguration_t* configuration = zen_Memory_allocate(zen_VirtualMachineConfiguration_t, 1);
    configuration->m_entityDirectories = jtk_ArrayList_new();
    configuration->m_variables = jtk_HashMap_new(stringObjectAdapter, stringObjectAdapter);
    configuration->m_logLevel = JTK_LOG_LEVEL_NONE;

    return configuration;
}

/* Destructor */

void zen_VirtualMachineConfiguration_delete(zen_VirtualMachineConfiguration_t* configuration) {
    jtk_Assert_assertObject(configuration, "The specified virtual machine configuration is null.");

    int32_t size = jtk_ArrayList_getSize(configuration->m_entityDirectories);
    int32_t i;
    for (i = 0; i < size; i++) {
        uint8_t* string = (uint8_t*)jtk_ArrayList_getValue(configuration->m_entityDirectories, i);
        jtk_CString_delete(string);
    }
    jtk_ArrayList_delete(configuration->m_entityDirectories);
    
    jtk_HashMap_delete(configuration->m_variables);
    jtk_Memory_deallocate(configuration);
}

/* Entity Directory */

void zen_VirtualMachineConfiguration_addEntityDirectory(
    zen_VirtualMachineConfiguration_t* virtualMachine, const uint8_t* directory) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    jtk_Assert_assertObject(directory, "The specified directory is null.");
    
    uint8_t* copy = jtk_CString_new(directory);
    jtk_ArrayList_add(virtualMachine->m_entityDirectories, copy);
}