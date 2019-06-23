// Saturday, April 28, 2018

#ifndef ZEN_FEB_CONSTANT_POOL_INTEGER_H
#define ZEN_FEB_CONSTANT_POOL_INTEGER_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ConstantPoolInteger                                                         *
 *******************************************************************************/

/**
 * Represents an integer reference.
 *
 * @author Samuel Rowe
 * @since  STK 1.0
 */
struct zen_ConstantPoolInteger_t {

    /**
     * The value of the integer constant. The bytes are stored in the
     * big-endian order.
     */
    uint32_t m_bytes;
};

/**
 * @memberof ConstantPoolInteger
 */
typedef struct zen_ConstantPoolInteger_t zen_ConstantPoolInteger_t;

/* Value */

/**
 * @memberof ConstantPoolInteger
 */
int32_t zen_ConstantPoolInteger_getValue(zen_ConstantPoolInteger_t* constantPoolInteger);

#endif /* ZEN_FEB_CONSTANT_POOL_INTEGER_H */