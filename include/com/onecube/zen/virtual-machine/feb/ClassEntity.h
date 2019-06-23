// Saturday, April 28, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_CLASS_ENTITY_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_CLASS_ENTITY_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/feb/Entity.h>

/*******************************************************************************
 * ClassEntity                                                                 *
 *******************************************************************************/

/**
 * A class entity.
 *
 * @class ClassEntity
 * @ingroup zen_vm_feb
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ClassEntity_t {

    /**
     * The number of super-classes.
     */
    uint16_t m_superClassCount;
    
    uint16_t* m_superClasses;
    
    uint16_t m_fieldCount;
    
    zen_Entity_t** m_fields;
    
    uint16_t m_functionCount;
    
    zen_Entity_t** m_functions;
};

/**
 * @memberof ClassEntity
 */
typedef struct zen_ClassEntity_t zen_ClassEntity_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_CLASS_ENTITY_H */