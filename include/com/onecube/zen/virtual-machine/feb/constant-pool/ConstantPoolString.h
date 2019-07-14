// Saturday, April 28, 2018

#ifndef ZEN_FEB_CONSTANT_POOL_STRING_CONSTANT_POOL_H
#define ZEN_FEB_CONSTANT_POOL_STRING_CONSTANT_POOL_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ConstantPoolString                                                          *
 *******************************************************************************/

/**
 * Represents a string reference.
 *
 * @author Samuel Rowe
 * @since  zen 1.0
 */
struct zen_ConstantPoolString_t {

    /**
     * A valid index into the constant pool. The entry at this index is an
     * UTF-8 element which represents the sequence of Unicode code points
     * contained in this string entry.
     */
    uint16_t m_stringIndex;
};

typedef struct zen_ConstantPoolString_t zen_ConstantPoolString_t;

#endif /* ZEN_FEB_CONSTANT_POOL_STRING_CONSTANT_POOL_H */