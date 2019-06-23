// Friday, August 31, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_HEAP_OBJECT_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_HEAP_OBJECT_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * Object                                                                      *
 *******************************************************************************/

/**
 * @class Object
 * @ingroup zen_heap
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_Object_t {
    int32_t m_flags;
    int32_t m_size;
};

/**
 * @memberof Object
 */
typedef struct zen_Object_t zen_Object_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_HEAP_OBJECT_H */