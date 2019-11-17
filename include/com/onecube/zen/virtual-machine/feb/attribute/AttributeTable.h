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