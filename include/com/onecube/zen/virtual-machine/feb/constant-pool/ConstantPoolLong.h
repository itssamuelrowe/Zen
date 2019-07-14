// Saturday, April 28, 2018

#ifndef ZEN_FEB_CONSTANT_POOL_LONG_CONSTANT_POOL_H
#define ZEN_FEB_CONSTANT_POOL_LONG_CONSTANT_POOL_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ConstantPoolLong                                                            *
 *******************************************************************************/

/**
 * Represents a long integer reference.
 *
 * @author Samuel Rowe
 * @since  zen 1.0
 */
struct zen_ConstantPoolLong_t {

    /**
     * The type of the constant pool entry.
     */
    uint8_t m_tag;

    /**
     * The high bytes of the long integer.
     */
    uint32_t m_highBytes;

    /**
     * The low bytes of the long integer.
     */
    uint32_t m_lowBytes;
};

typedef struct zen_ConstantPoolLong_t zen_ConstantPoolLong_t;

#endif /* ZEN_FEB_CONSTANT_POOL_LONG_CONSTANT_POOL_H */