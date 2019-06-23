// Sunday, September 02, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_VIRTUAL_MACHINE_GARBAGE_COLLECTOR_LISTENER_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_VIRTUAL_MACHINE_GARBAGE_COLLECTOR_LISTENER_H

/*******************************************************************************
 * VirtualMachineGarbageCollectorListener                                      *
 *******************************************************************************/

/**
 * @class VirtualMachineGarbageCollectorListener
 * @ingroup zen_vm
 * @author Samuel Rowe
 * @since zen 1.0
 */

/**
 * @memberof VirtualMachineGarbageCollectorListener
 */
void zen_VirtualMachineGarbageCollectorListener_onAfterInitialization(zen_GarbageCollectorListener_t* listener, zen_GarbageCollector_t* collector);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_VIRTUAL_MACHINE_GARBAGE_COLLECTOR_LISTENER_H */