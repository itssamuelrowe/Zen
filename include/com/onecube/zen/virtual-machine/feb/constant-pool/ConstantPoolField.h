// Saturday, April 28, 2018

#ifndef ZEN_FEB_CONSTANT_POOL_FIELD_CONSTANT_POOL_H
#define ZEN_FEB_CONSTANT_POOL_FIELD_CONSTANT_POOL_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ConstantPoolField                                                           *
 *******************************************************************************/

/**
 * Represents a field reference.
 *
 * @author Samuel Rowe
 * @since  zen 1.0
 */
struct zen_ConstantPoolField_t {

    /**
     * The type of the constant pool entry.
     */
    uint8_t m_tag;

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

typedef struct zen_ConstantPoolField_t zen_ConstantPoolField_t;

#endif /* ZEN_FEB_CONSTANT_POOL_FIELD_CONSTANT_POOL_H */