// Saturday, April 28, 2018

#ifndef ZEN_FEB_CONSTANT_POOL_DOUBLE_H
#define ZEN_FEB_CONSTANT_POOL_DOUBLE_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ConstantPoolDouble                                                          *
 *******************************************************************************/

/**
 * Represents a double floating-value reference.
 *
 * @author Samuel Rowe
 * @since  zen 1.0
 */
struct zen_ConstantPoolDouble_t {
    uint32_t m_highBytes;
    uint32_t m_lowBytes;
};

typedef struct zen_ConstantPoolDouble_t zen_ConstantPoolDouble_t;

#endif /* ZEN_FEB_CONSTANT_POOL_DOUBLE_H */