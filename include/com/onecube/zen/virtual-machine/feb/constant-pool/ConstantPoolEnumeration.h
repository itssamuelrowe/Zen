// Saturday, April 28, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_CONSTANT_POOL_ENUMERATION_CONSTANT_POOL_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_CONSTANT_POOL_ENUMERATION_CONSTANT_POOL_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ConstantPoolEnumeration                                                           *
 *******************************************************************************/

/**
 * Represents a enumeration reference.
 *
 * @author Samuel Rowe
 * @since  zen 1.0
 */
struct zen_ConstantPoolEnumeration_t {

    /**
     * A valid index into the constant pool. The entry at this index is an
     * UTF-8 entry which represents the encoded name of a enumeration.
     */
    uint16_t m_nameIndex;
};

typedef struct zen_ConstantPoolEnumeration_t zen_ConstantPoolEnumeration_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_CONSTANT_POOL_ENUMERATION_CONSTANT_POOL_H */