// Saturday, April 28, 2018

#ifndef ZEN_FEB_CONSTANT_POOL_FLOAT_H
#define ZEN_FEB_CONSTANT_POOL_FLOAT_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ConstantPoolFloat                                                           *
 *******************************************************************************/

/**
 * Represents an floating value reference.
 *
 * @author Samuel Rowe
 * @since  STK 1.0
 */
struct zen_ConstantPoolFloat_t {

    /**
     * The value of the float constant. The bytes are stored in the
     * big-endian order.
     *
     * The floating value is encoded in IEEE 754 floating-point single format.
     * The bits
     */
    uint32_t m_bytes;
};

/**
 * @memberof ConstantPoolFloat
 */
typedef struct zen_ConstantPoolFloat_t zen_ConstantPoolFloat_t;

/* Value */

/**
 * @memberof ConstantPoolFloat
 */
float zen_ConstantPoolFloat_getValue(zen_ConstantPoolFloat_t* constantPoolFloat);

#endif /* ZEN_FEB_CONSTANT_POOL_FLOAT_H */