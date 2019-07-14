// Saturday, April 28, 2018

#ifndef ZEN_FEB_CONSTANT_POOL_CLASS_CONSTANT_POOL_H
#define ZEN_FEB_CONSTANT_POOL_CLASS_CONSTANT_POOL_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ConstantPoolClass                                                           *
 *******************************************************************************/

/**
 * Represents a class reference.
 *
 * @author Samuel Rowe
 * @since  zen 1.0
 */
struct zen_ConstantPoolClass_t {
    
    /**
     * The type of the constant pool entry.
     */
    uint8_t m_tag;

    /**
     * A valid index into the constant pool. The entry at this index is an
     * UTF-8 entry which represents the encoded name of a class.
     *
     * NOTE: An array class name should consist of the encoded component class
     *       name along with its dimensions. Further, the dimensions are limited
     *       to 255, inclusively.
     */
    uint16_t m_nameIndex;
};

typedef struct zen_ConstantPoolClass_t zen_ConstantPoolClass_t;

#endif /* ZEN_FEB_CONSTANT_POOL_CLASS_CONSTANT_POOL_H */