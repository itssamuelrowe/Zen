// Sunday, September 23, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_ENTITY_RESOLVER_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_ENTITY_RESOLVER_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/loader/ResolutionManager.h>

/*******************************************************************************
 * EntityResolver                                                              *
 *******************************************************************************/

/**
 * @class EntityResolver
 * @ingroup zen_vm_loader
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EntityResolver_t {
    zen_ResolutionManager_t* m_resolutionManager;
};

/**
 * @memberof EntityResolver
 */
typedef struct zen_EntityResolver_t zen_EntityResolver_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_ENTITY_RESOLVER_H */