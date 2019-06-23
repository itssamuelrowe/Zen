// Saturday, April 28, 2018

#ifndef ZEN_FEB_CONSTANT_POOL_TAG_H
#define ZEN_FEB_CONSTANT_POOL_TAG_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ConstantPoolTag                                                             *
 *******************************************************************************/

/**
 * @author Samuel Rowe
 * @since  zen 1.0
 */
enum zen_ConstantPoolTag_t {
    ZEN_CONSTANT_POOL_TAG_INTEGER = 0,
    ZEN_CONSTANT_POOL_TAG_LONG = 1,
    ZEN_CONSTANT_POOL_TAG_FLOAT = 2,
    ZEN_CONSTANT_POOL_TAG_DOUBLE = 3,
    ZEN_CONSTANT_POOL_TAG_UTF8 = 4,
    ZEN_CONSTANT_POOL_TAG_STRING = 5,
    ZEN_CONSTANT_POOL_TAG_FUNCTION = 6,
    ZEN_CONSTANT_POOL_TAG_FIELD = 7,
    ZEN_CONSTANT_POOL_TAG_CLASS = 8,
    ZEN_CONSTANT_POOL_TAG_ENUMERATE = 9,
    ZEN_CONSTANT_POOL_TAG_ENUMERATION = 10
};

typedef enum zen_ConstantPoolTag_t zen_ConstantPoolTag_t;

#endif /* ZEN_FEB_CONSTANT_POOL_TAG_H */