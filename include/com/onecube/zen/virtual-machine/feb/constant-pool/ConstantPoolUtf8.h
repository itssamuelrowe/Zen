// Saturday, April 28, 2018

#ifndef ZEN_FEB_CONSTANT_POOL_UTF8_CONSTANT_POOL_H
#define ZEN_FEB_CONSTANT_POOL_UTF8_CONSTANT_POOL_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ConstantPoolUtf8                                                            *
 *******************************************************************************/

/**
 * Represents various strings referenced by the code.
 */
struct zen_ConstantPoolUtf8_t {

    /**
     * The number of bytes the {@code m_bytes} array occupies.
     */
    uint16_t m_length;

    /**
     * The bytes contained in the string, encoded in UTF-8.
     */
    uint8_t* m_bytes;
};

typedef struct zen_ConstantPoolUtf8_t zen_ConstantPoolUtf8_t;

#endif /* ZEN_FEB_CONSTANT_POOL_UTF8_CONSTANT_POOL_H */