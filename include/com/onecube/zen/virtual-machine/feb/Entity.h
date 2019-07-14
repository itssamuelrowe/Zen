// Saturyda, April 28, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ENTITY_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ENTITY_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/feb/AttributeTable.h>
#include <com/onecube/zen/virtual-machine/feb/ConstantPool.h>

/*******************************************************************************
 * Entity                                                                      *
 *******************************************************************************/

struct zen_Entity_t {

    /**
     * The tag byte which determines the type of this entity.
     */
    uint8_t m_type;

    /**
     * A mask of flags which denote various properties of this entity.
     */
    uint16_t m_flags;

    /**
     * A valid index into the constant pool table. The entry at this index
     * should be a {@code ConstantPoolClass} which represents this entity.
     */
    uint16_t m_reference;

    zen_AttributeTable_t m_attributeTable;

    /**
     * The body of this entity. Contingent on the tag byte, the length of the
     * body may vary. Please refer to the documentation of various entities
     * for further information.
     */
    // void* m_body;
};

typedef struct zen_Entity_t zen_Entity_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ENTITY_H */