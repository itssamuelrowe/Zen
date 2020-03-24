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

// Sunday, September 16, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_INVOCATION_STACK_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_INVOCATION_STACK_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/object/Class.h>
#include <com/onecube/zen/virtual-machine/object/Function.h>
#include <com/onecube/zen/virtual-machine/processor/StackFrame.h>

#include <jtk/collection/Iterator.h>
#include <jtk/collection/list/DoublyLinkedList.h>

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
    // TODO: Replace this with a singly-linked list.
    jtk_DoublyLinkedList_t* m_stackFrames;
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

/* Iterator */

jtk_Iterator_t* zen_InvocationStack_getIterator(zen_InvocationStack_t* invocationStack);

/* Stack Frame */

/**
 * @memberof InvocationStack
 */
void zen_InvocationStack_pushStackFrame(zen_InvocationStack_t* invocationStack,
    zen_StackFrame_t* stackFrame);

/**
 * @memberof InvocationStack
 */
zen_StackFrame_t* zen_InvocationStack_popStackFrame(zen_InvocationStack_t* stack);

/**
 * @memberof InvocationStack
 */
zen_StackFrame_t* zen_InvocationStack_peekStackFrame(zen_InvocationStack_t* stack);

/* Size */

int32_t zen_InvocationStack_getSize(zen_InvocationStack_t* invocationStack);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_INVOCATION_STACK_H */