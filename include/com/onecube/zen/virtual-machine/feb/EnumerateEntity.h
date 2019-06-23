// Saturday, April 28, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ENUMERATE_ENTITY_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ENUMERATE_ENTITY_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * EnumerateEntity                                                             *
 *******************************************************************************/

/**
 * A field enumerate.
 *
 * @author Samuel Rowe
 * @since  zen 1.0
 */
struct zen_EnumerateEntity_t {

    /**
     * A valid index into the constant pool table. The entry at this index is
     * an UTF-8 entry which represents the name of this field.
     */
    uint16_t m_nameIndex;

    /**
     * A valid index into the constant pool table. The entry at this index is
     * an UTF-8 entry which represents the descriptor of this field.
     */
    uint16_t m_descriptorIndex;
};

typedef struct zen_EnumerateEntity_t zen_EnumerateEntity_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ENUMERATE_ENTITY_H */