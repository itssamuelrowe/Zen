// Saturday, April 28, 2018

#ifndef ZEN_FEB_ENTITY_TYPE_H
#define ZEN_FEB_ENTITY_TYPE_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * EntityType                                                                  *
 *******************************************************************************/

/**
 * @class EntityType
 * @ingroup zen_vm_feb
 * @author Samuel Rowe
 * @since zen 1.0
 */
enum zen_EntityType_t {
    ZEN_ENTITY_TYPE_CLASS = 1,
    ZEN_ENTITY_TYPE_INTERFACE = 2,
    ZEN_ENTITY_TYPE_ANNOTATION = 3,
    ZEN_ENTITY_TYPE_PACKAGE = 4
};

/**
 * @memberof EntityType
 */
typedef enum zen_EntityType_t zen_EntityType_t;

#endif /* ZEN_FEB_ENTITY_TYPE_H */