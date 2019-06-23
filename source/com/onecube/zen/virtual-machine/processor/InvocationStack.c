// Tuesday, March 26, 2019

#include <com/onecube/zen/virtual-machine/processor/InvocationStack.h>

/*******************************************************************************
 * InvocationStack                                                             *
 *******************************************************************************/

/* Constructor */

zen_InvocationStack_t* zen_InvocationStack_new() {
    return NULL;
}

/* Destructor */

void zen_InvocationStack_delete(zen_InvocationStack_t* stack) {
}

/* Current Stack Frame */

zen_StackFrame_t* zen_InvocationStack_getCurrentStackFrame(zen_InvocationStack_t* stack) {
}

/* Push/Pop Stack Frame */

zen_StackFrame_t* zen_InvocationStack_pushStackFrame(zen_InvocationStack_t* stack,
    zen_Class_t* class0, zen_Function_t* function) {
    return NULL;
}

zen_StackFrame_t* zen_InvocationStack_peekStackFrame(zen_InvocationStack_t* stack) {
    return NULL;
}

zen_StackFrame_t* zen_InvocationStack_popStackFrame(zen_InvocationStack_t* stack) {
    return NULL;
}