// Friday, June 08, 2018

#include <com/onecube/zen/virtual-machine/VirtualMachineContext.h>
#include <com/onecube/zen/virtual-machine/ExceptionManager.h>

/*******************************************************************************
 * VirtualMachineContext                                                                   *
 *******************************************************************************/

/* Constructor */

zen_VirtualMachineContext_t* zen_VirtualMachineContext_new(zen_VirtualMachine_t* virtualMachine) {
    zen_VirtualMachineContext_t* context = zen_Memory_allocate(zen_VirtualMachineContext_t, 1);
    context->m_virtualMachine = virtualMachine;

    return context;
}

/* Destructor */

void zen_VirtualMachineContext_delete(zen_VirtualMachineContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

