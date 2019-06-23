// Friday, May 18, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_FUNCTION_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_FUNCTION_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * Function                                                                    *
 *******************************************************************************/

typedef struct zen_Class_t zen_Class_t;

struct zen_Function_t {
};

typedef struct zen_Function_t zen_Function_t;

bool zen_Function_isNative(zen_Function_t* function);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_FUNCTION_H */