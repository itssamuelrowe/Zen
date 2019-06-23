// Saturday, April 28, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ENUMERATION_ENTITY_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ENUMERATION_ENTITY_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/feb/Entity.h>

/*******************************************************************************
 * EnumerationEntity                                                           *
 *******************************************************************************/

/**
 * An enumeration entity.
 *
 * @class EnumerationEntity
 * @ingroup zen_vm_feb
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EnumerationEntity_t {

    uint16_t m_superClass;
    
    uint16_t m_enumerateCount;
    
    zen_Entity_t** m_enumerates;
    
    zen_Entity_t* m_initializer;
};

/**
 * @memberof EnumerationEntity
 */
typedef struct zen_EnumerationEntity_t zen_EnumerationEntity_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ENUMERATION_ENTITY_H */