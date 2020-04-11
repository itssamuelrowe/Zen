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

// Tuesday, March 26, 2019

#include <com/onecube/zen/virtual-machine/processor/InvocationStack.h>

/*******************************************************************************
 * InvocationStack                                                             *
 *******************************************************************************/

/* Constructor */

zen_InvocationStack_t* zen_InvocationStack_new() {
    zen_InvocationStack_t* invocationStack = jtk_Memory_allocate(zen_InvocationStack_t, 1);
    invocationStack->m_stackFrames = jtk_DoublyLinkedList_new();
    invocationStack->m_trace = jtk_DoublyLinkedList_new();
    invocationStack->m_tracing = false;

    return invocationStack;
}

/* Destructor */

void zen_InvocationStack_delete(zen_InvocationStack_t* invocationStack) {
    jtk_Assert_assertObject(invocationStack, "The specified invocation stack is null.");

    jtk_Iterator_t* iterator = zen_InvocationStack_getIterator(invocationStack->m_stackFrames);
    while (jtk_Iterator_hasNext(iterator)) {
        zen_StackFrame_t* stackFrame = (zen_StackFrame_t*)jtk_Iterator_getNext(iterator);
        zen_StackFrame_delete(stackFrame);
    }
    jtk_Iterator_delete(iterator);

    iterator = zen_InvocationStack_getIterator(invocationStack->m_trace);
    while (jtk_Iterator_hasNext(iterator)) {
        zen_StackFrame_t* stackFrame = (zen_StackFrame_t*)jtk_Iterator_getNext(iterator);
        zen_StackFrame_delete(stackFrame);
    }
    jtk_Iterator_delete(iterator);

    jtk_DoublyLinkedList_delete(invocationStack->m_trace);
    jtk_DoublyLinkedList_delete(invocationStack->m_stackFrames);
    jtk_Memory_deallocate(invocationStack);
}

/* Empty */

bool zen_InvocationStack_isEmpty(zen_InvocationStack_t* invocationStack) {
    jtk_Assert_assertObject(invocationStack, "The specified invocation stack is null.");

    return jtk_DoublyLinkedList_isEmpty(invocationStack->m_stackFrames);
}

/* Iterator */

jtk_Iterator_t* zen_InvocationStack_getIterator(zen_InvocationStack_t* invocationStack) {
    jtk_Assert_assertObject(invocationStack, "The specified invocation stack is null.");

    return jtk_DoublyLinkedList_getIterator(invocationStack->m_stackFrames);
}

/* Push/Pop Stack Frame */

zen_StackFrame_t* zen_InvocationStack_pushStackFrame(zen_InvocationStack_t* invocationStack,
    zen_Function_t* function) {
    jtk_Assert_assertObject(invocationStack, "The specified invocation stack is null.");
    jtk_Assert_assertObject(function, "The specified function is null.");

    zen_StackFrame_t* stackFrame = zen_StackFrame_new(function);
    jtk_DoublyLinkedList_addFirst(invocationStack->m_stackFrames, stackFrame);

    return stackFrame;
}

zen_StackFrame_t* zen_InvocationStack_peekStackFrame(zen_InvocationStack_t* invocationStack) {
    jtk_Assert_assertObject(invocationStack, "The specified invocation stack is null.");

    return jtk_DoublyLinkedList_isEmpty(invocationStack->m_stackFrames)?
        NULL : (zen_StackFrame_t*)jtk_DoublyLinkedList_getFirst(invocationStack->m_stackFrames);
}

void zen_InvocationStack_popStackFrame(zen_InvocationStack_t* invocationStack) {
    jtk_Assert_assertTrue(!jtk_DoublyLinkedList_isEmpty(invocationStack->m_stackFrames),
        "The specified invocation stack is empty.");

    zen_StackFrame_t* currentStackFrame =
        (zen_StackFrame_t*)jtk_DoublyLinkedList_getFirst(invocationStack->m_stackFrames);
    jtk_DoublyLinkedList_removeFirst(invocationStack->m_stackFrames);

    if (invocationStack->m_tracing) {
        jtk_DoublyLinkedList_addLast(invocationStack->m_trace, currentStackFrame);
    }
    else {
        /* NOTE: The stack frame should not be accessible to the world outside the interpreter.
            * Which means no reference to the stack frame will be maintained outside the interpreter.
            * Thus, we can destroy the stack frame here.
            */
        zen_StackFrame_delete(currentStackFrame);
    }
}

/* Size */

int32_t zen_InvocationStack_getSize(zen_InvocationStack_t* invocationStack) {
    jtk_Assert_assertObject(invocationStack, "The specified invocation stack is null.");
    
    return jtk_DoublyLinkedList_getSize(invocationStack->m_stackFrames);
}

// Tracing

void zen_InvocationStack_startTracing(zen_InvocationStack_t* invocationStack) {
    invocationStack->m_tracing = true;
}

void zen_InvocationStack_stopTracing(zen_InvocationStack_t* invocationStack) {
    invocationStack->m_tracing = false;

    jtk_Iterator_t* iterator = jtk_DoublyLinkedList_getIterator(invocationStack->m_trace);
    while (jtk_Iterator_hasNext(iterator)) {
        zen_StackFrame_t* stackFrame = (zen_StackFrame_t*)jtk_Iterator_getNext(iterator);
        zen_StackFrame_delete(stackFrame);
    }
    jtk_Iterator_delete(iterator);
    jtk_DoublyLinkedList_clear(invocationStack->m_trace);
}