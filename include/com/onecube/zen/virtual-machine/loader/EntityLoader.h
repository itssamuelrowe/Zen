/*
 * Copyright 2018-2020 Samuel Rowe
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

// Saturday, September 15, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_ENTITY_LOADER_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_ENTITY_LOADER_H


/*******************************************************************************
 * EntityLoader                                                                *
 *******************************************************************************/

#define ZEN_ENTITY_LOADER_DEFAULT_ENTITIES_MAP_CAPCITY 128

/**
 * @class EntityLoader
 * @ingroup zen_virtual_machine_loader
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EntityLoader_t {

    /**
     * The parser that contructs an entity from a binary input stream.
     */
    zen_BinaryEntityParser_t* m_parser;

    jtk_HashMap_t* m_entities;

};

/**
 * @memberof EntityLoader
 */
typedef struct zen_EntityLoader_t zen_EntityLoader_t;

/* Constructor */

zen_EntityLoader_t* zen_EntityLoader_new();
zen_EntityLoader_t* zen_EntityLoader_newWithEntityDirectories();

/* Destructor */

void zen_EntityLoader_delete(zen_EntityLoader_t* loader);

/* Class */


zen_EntityFile_t* zen_EntityLoader_findEntity(zen_EntityLoader_t* loader,
    const uint8_t* descriptor, int32_t descriptorSize);

zen_EntityFile_t* zen_EntityLoader_getEntity(zen_EntityLoader_t* loader,
    const uint8_t* descriptor, int32_t descriptorSize);

// Load Class From



#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_ENTITY_LOADER_H */