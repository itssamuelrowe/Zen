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

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_CLASS_ENTITY_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_CLASS_ENTITY_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/feb/FunctionEntity.h>
#include <com/onecube/zen/virtual-machine/feb/FieldEntity.h>

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

    /**
     * The number of super-classes.
     */
    uint16_t m_superclassCount;

    uint16_t* m_superclasses;

    zen_AttributeTable_t m_attributeTable;

    uint16_t m_fieldCount;

    zen_FieldEntity_t** m_fields;

    uint16_t m_functionCount;

    zen_FunctionEntity_t** m_functions;
};

/**
 * @memberof ClassEntity
 */
typedef struct zen_ClassEntity_t zen_ClassEntity_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_CLASS_ENTITY_H */