// Monday, July 15, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_NATIVE_FUNCTION_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_NATIVE_FUNCTION_H

/*******************************************************************************
 * NativeFunction                                                              *
 *******************************************************************************/

/**
 * @memberof NativeFunction
 */
typedef void (*zen_NativeFunction_InvokeFunction_t)(jtk_Array_t* arguments);

/**
 * @class NativeFunction
 * @ingroup zen_virtualMachine_object
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_NativeFunction_t {
    zen_NativeFunction_InvokeFunction_t m_invoke;
};

/**
 * @memberof NativeFunction
 */
typedef struct zen_NativeFunction_t zen_NativeFunction_t;

// Constructor

/**
 * @memberof NativeFunction
 */
zen_NativeFunction_t* zen_NativeFunction_new(zen_NativeFunction_InvokeFunction_t invoke);

// Destructor

/**
 * @memberof NativeFunction
 */
void zen_NativeFunction_delete(zen_NativeFunction_t* function);
 
#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_NATIVE_FUNCTION_H */