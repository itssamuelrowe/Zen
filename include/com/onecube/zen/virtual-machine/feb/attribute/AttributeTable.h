// Friday, September 21, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_ATTRIBUTE_TABLE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_ATTRIBUTE_TABLE_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/feb/attribute/Attribute.h>

/*******************************************************************************
 * AttributeTable                                                              *
 *******************************************************************************/

/**
 * @class AttributeTable
 * @ingroup zen_vm_feb
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AttributeTable_t {
    /**
     * The number of additional attributes of this entity.
     */
    uint16_t m_size;

    /**
     * The additional attributes of this entity, each represented by the
     * {@code zen_Attribute_t} structure.
     */
    zen_Attribute_t** m_attributes;
};

/**
 * @memberof AttributeTable
 */
typedef struct zen_AttributeTable_t zen_AttributeTable_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_ATTRIBUTE_TABLE_H */