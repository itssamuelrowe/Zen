// Saturday, September 15, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_EXCEPTION_MANAGER_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_EXCEPTION_MANAGER_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/object/Object.h>

/* Forward References */
typedef struct zen_VirtualMachine_t zen_VirtualMachine_t;

/*******************************************************************************
 * ExceptionManager                                                            *
 *******************************************************************************/

/**
 * @class ExceptionManager
 * @ingroup zen_virtual_machine
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ExceptionManager_t {
    zen_VirtualMachine_t* m_virtualMachine;
};

/**
 * @memberof ExceptionManager
 */
typedef struct zen_ExceptionManager_t zen_ExceptionManager_t;

/* Constructor */

/**
 * @memberof ExceptionManager
 */
zen_ExceptionManager_t* zen_ExceptionManager_new(zen_VirtualMachine_t* virtualMachine);

/* Destructor */

/**
 * @memberof ExceptionManager
 */
void zen_ExceptionManager_delete(zen_ExceptionManager_t* manager);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_EXCEPTION_MANAGER_H */