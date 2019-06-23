// Tuesday, March 26, 2019

#include <com/onecube/zen/virtual-machine/object/Function.h>
#include <com/onecube/zen/virtual-machine/object/Class.h>

/*******************************************************************************
 * Function                                                                    *
 *******************************************************************************/

bool zen_Function_isNative(zen_Function_t* function) {
    return false;
}

uint8_t* zen_Function_getName(zen_Function_t* function) {
    return NULL;
}

uint8_t* zen_Function_getDescriptor(zen_Function_t* function) {
    return NULL;
}

zen_Class_t* zen_Function_getClass(zen_Function_t* function) {
    return NULL;
}