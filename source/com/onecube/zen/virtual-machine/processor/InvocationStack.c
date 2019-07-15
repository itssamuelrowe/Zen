// Tuesday, March 26, 2019

#include <com/onecube/zen/virtual-machine/processor/InvocationStack.h>

/*******************************************************************************
 * InvocationStack                                                             *
 *******************************************************************************/

/* Constructor */

zen_InvocationStack_t* zen_InvocationStack_new() {
    zen_InvocationStack_t* invocationStack = jtk_Memory_allocate(zen_InvocationStack_t, 1);
    invocationStack->m_stackFrames = jtk_DoublyLinkedList_new();

    return invocationStack;
}

/* Destructor */

void zen_InvocationStack_delete(zen_InvocationStack_t* invocationStack) {
    jtk_Assert_assertObject(invocationStack, "The specified invocation stack is null.");

    jtk_DoublyLinkedList_delete(invocationStack->m_stackFrames);
    jtk_Memory_deallocate(invocationStack);
}

/* Current Stack Frame */

zen_StackFrame_t* zen_InvocationStack_getCurrentStackFrame(zen_InvocationStack_t* invocationStack) {
}

/* Iterator */

jtk_Iterator_t* zen_InvocationStack_getIterator(zen_InvocationStack_t* invocationStack) {
    jtk_Assert_assertObject(invocationStack, "The specified invocation stack is null.");

    return jtk_DoublyLinkedList_getIterator(invocationStack->m_stackFrames);
}

/* Push/Pop Stack Frame */

void zen_InvocationStack_pushStackFrame(zen_InvocationStack_t* invocationStack,
    zen_StackFrame_t* stackFrame) {
    jtk_Assert_assertObject(invocationStack, "The specified invocation stack is null.");
    jtk_Assert_assertObject(stackFrame, "The specified stack frame is null.");

    jtk_DoublyLinkedList_addFirst(invocationStack->m_stackFrames, stackFrame);
}

zen_StackFrame_t* zen_InvocationStack_peekStackFrame(zen_InvocationStack_t* invocationStack) {
    jtk_Assert_assertObject(invocationStack, "The specified invocation stack is null.");

    return jtk_DoublyLinkedList_isEmpty(invocationStack->m_stackFrames)?
        NULL : (zen_StackFrame_t*)jtk_DoublyLinkedList_getFirst(invocationStack->m_stackFrames);
}

zen_StackFrame_t* zen_InvocationStack_popStackFrame(zen_InvocationStack_t* invocationStack) {
    jtk_Assert_assertTrue(!jtk_DoublyLinkedList_isEmpty(invocationStack->m_stackFrames),
        "The specified invocation stack is empty.");

    return (zen_StackFrame_t*)jtk_DoublyLinkedList_getFirst(invocationStack->m_stackFrames);
}