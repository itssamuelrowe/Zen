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

// Sunday, July 14, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_FIELD_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_FIELD_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/feb/FieldEntity.h>

// Forward References

typedef struct zen_Class_t zen_Class_t;

/*******************************************************************************
 * Field                                                                       *
 *******************************************************************************/

/**
 * @class Field
 * @ingroup zen_virtualMachine_object
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_Field_t {
    uint8_t* m_name;
    int32_t m_nameSize;
    uint8_t* m_descriptor;
    int32_t m_descriptorSize;
    zen_Class_t* m_class;
    int32_t m_offset;
    zen_FieldEntity_t* m_entity;
};

/**
 * @memberof Field
 */
typedef struct zen_Field_t zen_Field_t;

// Constructor

zen_Field_t* zen_Field_new(zen_Class_t* class0, 
    zen_FieldEntity_t* fieldEntity, int32_t offset);

// Destructor

void zen_Field_delete(zen_Field_t* field);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_FIELD_H */