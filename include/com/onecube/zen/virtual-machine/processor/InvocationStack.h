// Sunday, September 16, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_INVOCATION_STACK_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_INVOCATION_STACK_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/object/Class.h>
#include <com/onecube/zen/virtual-machine/object/Function.h>
#include <com/onecube/zen/virtual-machine/processor/StackFrame.h>

/*******************************************************************************
 * InvocationStack                                                             *
 *******************************************************************************/

/**
 * @class InvocationStack
 * @ingroup zen_vm_processor
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_InvocationStack_t {
    zen_StackFrame_t* m_currentStackFrame;
    int32_t m_size;
    int32_t m_capacity;
};

/**
 * @memberof InvocationStack
 */
typedef struct zen_InvocationStack_t zen_InvocationStack_t;

/* Constructor */

/**
 * @memberof InvocationStack
 */
zen_InvocationStack_t* zen_InvocationStack_new();

/* Destructor */

/**
 * @memberof InvocationStack
 */
void zen_InvocationStack_delete(zen_InvocationStack_t* stack);

/* Current Stack Frame */

zen_StackFrame_t* zen_InvocationStack_getCurrentStackFrame(zen_InvocationStack_t* stack);

/* Push/Pop Stack Frame */

/**
 * @memberof InvocationStack
 */
zen_StackFrame_t* zen_InvocationStack_pushStackFrame(zen_InvocationStack_t* stack, zen_Class_t* class0, zen_Function_t* function);

/**
 * @memberof InvocationStack
 */
zen_StackFrame_t* zen_InvocationStack_popStackFrame(zen_InvocationStack_t* stack);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_INVOCATION_STACK_H */