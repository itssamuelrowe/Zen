// Saturday, April 28, 2018

#ifndef ZEN_FEB_CONSTANT_POOL_FUNCTION_H
#define ZEN_FEB_CONSTANT_POOL_FUNCTION_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ConstantPoolFunction                                                        *
 *******************************************************************************/

/**
 * Represents a function reference.
 *
 * @author Samuel Rowe
 * @since  zen 1.0
 */
struct zen_ConstantPoolFunction_t {

    /**
     * A valid index into the constant pool table. The entry at this index is
     * a {@code zen_ConstantPoolClass_t} entry which encloses this field.
     */
    uint16_t m_classIndex;

    /**
     * A valid index into the constant pool table. The entry at this index is
     * a {@code zen_ConstantPoolUtf8_t} entry which represents the descriptor
     * of this function.
     */
    uint16_t m_descriptorIndex;

    /**
     * A valid index into the constant pool table. The entry at this index is
     * a {@code zen_ConstantPoolUtf_t} entry which represents the name of this
     * function.
     */
    uint16_t m_nameIndex;
};

typedef struct zen_ConstantPoolFunction_t zen_ConstantPoolFunction_t;

#endif /* ZEN_FEB_CONSTANT_POOL_FUNCTION_H */