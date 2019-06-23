// Saturday, April 28, 2018

#ifndef ZEN_FEB_FIELD_ENTITY_H
#define ZEN_FEB_FIELD_ENTITY_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * FieldEntity                                                                 *
 *******************************************************************************/

/**
 * A field entity.
 *
 * @author Samuel Rowe
 * @since  zen 1.0
 */
struct zen_FieldEntity_t {

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

typedef struct zen_FieldEntity_t zen_FieldEntity_t;

#endif /* ZEN_FEB_FIELD_ENTITY_H */