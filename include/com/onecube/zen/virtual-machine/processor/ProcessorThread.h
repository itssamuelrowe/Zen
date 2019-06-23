// Sunday, September 16, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_PROCESSOR_THREAD_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_PROCESSOR_THREAD_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/processor/InvocationStack.h>

/*******************************************************************************
 * ProcessorThread                                                             *
 *******************************************************************************/

/**
 * @class ProcessorThread
 * @ingroup zen_vm_processor
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ProcessorThread_t {
    zen_InvocationStack_t* m_invocationStack;
};

/**
 * @memberof ProcessorThread
 */
typedef struct zen_ProcessorThread_t zen_ProcessorThread_t;

/* Constructor */

/**
 * @memberof ProcessorThread
 */
zen_ProcessorThread_t* zen_ProcessorThread_new();

/* Destructor */

/**
 * @memberof ProcessorThread
 */
void zen_ProcessorThread_delete(zen_ProcessorThread_t* thread);
 
#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_PROCESSOR_THREAD_H */