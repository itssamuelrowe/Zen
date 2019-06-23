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