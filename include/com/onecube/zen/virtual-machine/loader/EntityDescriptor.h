// Saturday, May 11, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_ENTITY_DESCRIPTOR_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_ENTITY_DESCRIPTOR_H

#include <com/onecube/zen/Configuration.h>
#include <jtk/core/String.h>

/*******************************************************************************
 * EntityDescriptor                                                            *
 *******************************************************************************/

/**
 * @class EntityDescriptor
 * @ingroup com_onecube_virtual_machine_loader
 * @author Samuel Rowe
 * @since Newton Virtual Machine 1.0
 */
struct zen_EntityDescriptor_t {
    jtk_String_t* m_value;
};

/**
 * @memberof EntityDescriptor
 */
typedef struct zen_EntityDescriptor_t zen_EntityDescriptor_t;

// Constructor

zen_EntityDescriptor_t* zen_EntityDescriptor_new(jtk_String_t* value);
zen_EntityDescriptor_t* zen_EntityDescriptor_newEx(const uint8_t* value);

// Destructor

void zen_EntityDescriptor_delete(zen_EntityDescriptor_t* descriptor);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_ENTITY_DESCRIPTOR_H */
