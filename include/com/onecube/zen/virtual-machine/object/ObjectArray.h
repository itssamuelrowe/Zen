// Friday, May 18, 2018

#ifndef ZEN_VM_OBJECT_ARRAY_H
#define ZEN_VM_OBJECT_ARRAY_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/object/Object.h>

/*******************************************************************************
 * ObjectArray                                                                 *
 *******************************************************************************/

struct zen_ObjectArray_t {
};

typedef struct zen_ObjectArray_t zen_ObjectArray_t;

/* Value */

void zen_ObjectArray_setValue(zen_ObjectArray_t* array, int32_t index, zen_Object_t* value);
zen_Object_t* zen_ObjectArray_getValue(zen_ObjectArray_t* array, int32_t index);

#endif /* ZEN_VM_OBJECT_ARRAY_H */