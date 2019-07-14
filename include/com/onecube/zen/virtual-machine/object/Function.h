// Friday, May 18, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_FUNCTION_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_FUNCTION_H

#include <jtk/core/String.h>
#include <com/onecube/zen/Configuration.h>

// Forward References

typedef struct zen_Class_t zen_Class_t;

/*******************************************************************************
 * Function                                                                    *
 *******************************************************************************/

/**
 * @class Function
 * @ingroup zen_virtualMachine_object
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_Function_t {
    jtk_String_t* m_name;
    jtk_String_t* m_descriptor;
    uint16_t m_flags;
    zen_Class_t* m_class;
};

/**
 * @memberof Function
 */
typedef struct zen_Function_t zen_Function_t;

// Constructor

zen_Function_t* zen_Function_newFromFunctionEntity(zen_Class_t* class0, 
    zen_FunctionEntity_t* functionEntity);

// Destructor

void zen_Function_delete(zen_Function_t* function);

// Native

bool zen_Function_isNative(zen_Function_t* function);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_FUNCTION_H */