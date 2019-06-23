// Friday, June 08, 2018

#include <jtk/core/StringObjectAdapter.h>
#include <com/onecube/zen/virtual-machine/VirtualMachineConfiguration.h>

/*******************************************************************************
 * VirtualMachineConfiguration                                                 *
 *******************************************************************************/

/* Constructor */

zen_VirtualMachineConfiguration_t* zen_VirtualMachineConfiguration_new() {
    zen_VirtualMachineConfiguration_t* configuration = zen_Memory_allocate(zen_VirtualMachineConfiguration_t, 1);
    configuration->m_entityDirectories = jtk_ArrayList_new();
    configuration->m_variables = jtk_HashMap_new(jtk_StringObjectAdapter_getInstance(), jtk_StringObjectAdapter_getInstance());

    return configuration;
}

/* Destructor */

void zen_VirtualMachineConfiguration_delete(zen_VirtualMachineConfiguration_t* configuration) {
    jtk_Assert_assertObject(configuration, "The specified virtual machine configuration is null.");

    int32_t size = jtk_ArrayList_getSize(configuration->m_entityDirectories);
    int32_t i;
    for (i = 0; i < size; i++) {
        jtk_String_t* string = (jtk_String_t*)jtk_ArrayList_getValue(configuration->m_entityDirectories, i);
        jtk_String_delete(string);
    }
    jtk_ArrayList_delete(configuration->m_entityDirectories);
    
    jtk_HashMap_delete(configuration->m_variables);
    zen_Memory_deallocate(configuration);
}

/* Entity Directory */

void zen_VirtualMachineConfiguration_addEntityDirectory(
    zen_VirtualMachineConfiguration_t* virtualMachine, const uint8_t* directory) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    jtk_Assert_assertObject(directory, "The specified directory is null.");
    
    jtk_String_t* directoryAsString = jtk_String_new(directory);
    jtk_ArrayList_add(virtualMachine->m_entityDirectories, directoryAsString);
}