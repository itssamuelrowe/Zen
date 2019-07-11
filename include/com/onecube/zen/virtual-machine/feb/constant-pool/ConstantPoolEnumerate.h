// Saturday, April 28, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_CONSTANT_POOL_ENUMERATE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_CONSTANT_POOL_ENUMERATE_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ConstantPoolEnumerate                                                       *
 *******************************************************************************/

/**
 * Represents a field reference.
 *
 * @author Samuel Rowe
 * @since  zen 1.0
 */
struct zen_ConstantPoolEnumerate_t {

    /**
     * A valid index into the constant pool table. The entry at this index is
     * a {@code zen_ConstantPoolClass_t} entry which encloses this field.
     */
    uint16_t m_classIndex;

    /**
     * A valid index into the constant pool table. The entry at this index is
     * a {@code zen_ConstantPoolUtf8_t} entry which represents the descriptor
     * of this field.
     */
    uint16_t m_descriptorIndex;

    /**
     * A valid index into the constant pool table. The entry at this index is
     * a {@code zen_ConstantPoolUtf_t} entry which represents the name of this
     * field.
     */
    uint16_t m_nameIndex;
};

typedef struct zen_ConstantPoolEnumerate_t zen_ConstantPoolEnumerate_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_CONSTANT_POOL_ENUMERATE_H */