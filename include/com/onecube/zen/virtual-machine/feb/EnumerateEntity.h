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

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ENUMERATE_ENTITY_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ENUMERATE_ENTITY_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * EnumerateEntity                                                             *
 *******************************************************************************/

/**
 * A field enumerate.
 *
 * @author Samuel Rowe
 * @since  zen 1.0
 */
struct zen_EnumerateEntity_t {

    /**
     * A valid index into the constant pool table. The entry at this index is
     * an UTF-8 entry which represents the name of this field.
     */
    uint16_t m_nameIndex;

    /**
     * A valid index into the constant pool table. The entry at this index is
     * an UTF-8 entry which represents the descriptor of this field.
     */
    uint16_t m_descriptorIndex;
};

typedef struct zen_EnumerateEntity_t zen_EnumerateEntity_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ENUMERATE_ENTITY_H */