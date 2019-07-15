// Monday, July 15, 2019

#include <com/onecube/zen/virtual-machine/object/NativeFunction.h>

/*******************************************************************************
 * NativeFunction                                                              *
 *******************************************************************************/

// Constructor

zen_NativeFunction_t* zen_NativeFunction_new(
    zen_NativeFunction_InvokeFunction_t invoke) {
    zen_NativeFunction_t* nativeFunction = jtk_Memory_allocate(zen_NativeFunction_t, 1);
    nativeFunction->m_invoke = invoke;

    return nativeFunction;
}

// Destructor

void zen_NativeFunction_delete(zen_NativeFunction_t* function) {
    jtk_Assert_assertObject(function, "The specified native function is null.");

    jtk_Memory_deallocate(function);
}