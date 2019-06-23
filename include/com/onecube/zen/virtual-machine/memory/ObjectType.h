// Friday, January 25, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_OBJECT_TYPE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_OBJECT_TYPE_H

/*******************************************************************************
 * ObjectType                                                                  *
 *******************************************************************************/

/**
 * @class ObjectType
 * @ingroup zen_virtualMachine_memory
 * @author Samuel Rowe
 * @since zen 1.0
 */
enum zen_ObjectType_t {
    ZEN_OBJECT_TYPE_PRIMITIVE_ARRAY = 0,
    ZEN_OBJECT_TYPE_REFERENCE_ARRAY = 1,
    ZEN_OBJECT_TYPE_AGGREGATE_OBJECT = 2,
};

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_OBJECT_TYPE_H */