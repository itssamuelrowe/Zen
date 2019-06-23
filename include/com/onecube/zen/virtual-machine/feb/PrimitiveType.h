// Wednesday, January 23, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_PRIMITIVE_TYPE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_PRIMITIVE_TYPE_H

/**
 * @class PrimaryMemoryManager
 * @ingroup zen_vm_memory_allocator
 * @author Samuel Rowe
 * @since zen 1.0
 */
enum zen_PrimitiveType_t {
    ZEN_PRIMITIVE_TYPE_BOOLEAN,
    ZEN_PRIMITIVE_TYPE_CHARACTER,
    ZEN_PRIMITIVE_TYPE_INTEGER_8,
    ZEN_PRIMITIVE_TYPE_INTEGER_16,
    ZEN_PRIMITIVE_TYPE_INTEGER_32,
    ZEN_PRIMITIVE_TYPE_INTEGER_64,
    ZEN_PRIMITIVE_TYPE_DECIMAL_32,
    ZEN_PRIMITIVE_TYPE_DECIMAL_64
};

/**
 * @memberof PrimitiveType
 */
typedef enum zen_PrimitiveType_t zen_PrimitiveType_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_PRIMITIVE_TYPE_H */