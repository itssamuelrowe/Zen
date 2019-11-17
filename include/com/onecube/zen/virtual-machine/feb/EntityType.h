/*
 * Copyright 2018-2019 OneCube
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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