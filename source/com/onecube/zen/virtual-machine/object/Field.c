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

#include <com/onecube/zen/virtual-machine/object/Field.h>
#include <com/onecube/zen/virtual-machine/object/Class.h>
#include <jtk/core/CString.h>

/*******************************************************************************
 * Field                                                                       *
 *******************************************************************************/

// Constructor

zen_Field_t* zen_Field_new(zen_Class_t* class0,
    zen_FieldEntity_t* entity, int32_t offset) {
    zen_ConstantPool_t* constantPool = &class0->m_entityFile->m_constantPool;
    zen_ConstantPoolUtf8_t* nameEntry = constantPool->m_entries[entity->m_nameIndex];
    zen_ConstantPoolUtf8_t* descriptorEntry = constantPool->m_entries[entity->m_descriptorIndex];

    zen_Field_t* field = jtk_Memory_allocate(zen_Field_t, 1);
    field->m_name = jtk_CString_newEx(nameEntry->m_bytes, nameEntry->m_length);
    field->m_nameSize = nameEntry->m_length;
    field->m_descriptor = jtk_CString_newEx(descriptorEntry->m_bytes, descriptorEntry->m_length);
    field->m_descriptorSize = descriptorEntry->m_length;
    field->m_class = class0;
    field->m_offset = offset;
    field->m_entity = entity;

    return field;
}

// Destructor

void zen_Field_delete(zen_Field_t* field) {
    jtk_CString_delete(field->m_name);
    jtk_CString_delete(field->m_descriptor);
    jtk_Memory_deallocate(field);

}