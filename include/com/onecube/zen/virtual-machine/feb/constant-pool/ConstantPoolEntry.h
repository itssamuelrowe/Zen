// Saturday, April 28, 2018

#ifndef ZEN_FEB_CONSTANT_POOL_CONSTANT_POOL_ENTRY_H
#define ZEN_FEB_CONSTANT_POOL_CONSTANT_POOL_ENTRY_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ConstantPoolEntry                                                           *
 *******************************************************************************/

/**
 * Represents a entry.
 *
 * @author Samuel Rowe
 * @since  zen 1.0
 */
struct zen_ConstantPoolEntry_t {

    /**
     * The type of the constant pool entry.
     */
    uint8_t m_tag;
};

typedef struct zen_ConstantPoolEntry_t zen_ConstantPoolEntry_t;

#endif /* ZEN_FEB_CONSTANT_POOL_CONSTANT_POOL_ENTRY_H */