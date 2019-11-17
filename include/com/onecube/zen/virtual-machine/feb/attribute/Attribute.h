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

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_ATTRIBUTE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_ATTRIBUTE_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * Attribute                                                                   *
 *******************************************************************************/

/**
 * Attributes are categorized into three groups based on their purposes.
 * <ul>
 *     <li>
 *     Primary Attributes - They are absolutely necessary for correct
 *     interpretation of a binary entity. A virtual machine should implement
 *     these attributes in order to adhere to this specification.
 *         <ul>
 *             <li>InstructionAttribute</li>
 *             <li>ExceptionsAttribute</li>
 *         </ul>
 *     </li>
 *     <li>
 *     Secondary Attributes - Such attributes are not necessary for correct
 *     interpretation of a binary entity. They serve as special annotations to
 *     tools (such as debuggers, profilers, and other such tools). A virtual
 *     machine may not implement any of these attributes and still adhere to
 *     the specification. In any case, the virtual machine may process the
 *     information stored in the attribute, if capable. Otherwise, it must
 *     silently ignore these attributes.
 *     
 *         <ul>
 *             <li>SourceFileAttribute</li>
 *             <li>LineNumberAttribute</li>
 *         </ul>
 *     </li>
 *     <li>
 *     Custom Attributes - Such attributes are specific to certain implementations.
 *     Such attributes are not necessary for correct interpretation of a binary
 *     entity. Futher, the virtual machine may process the information stored in
 *     the attribute, if capable. Otherwise, it must silently ignore these
 *     attributes.
 *     </li>
 * </ul>
 *
 * @author Samuel Rowe
 * @since  zen 1.0
 */
struct zen_Attribute_t {
    uint16_t m_nameIndex;
    uint32_t m_length;
};

typedef struct zen_Attribute_t zen_Attribute_t;

/*******************************************************************************
 * Primary Attribute Identifiers                                               *
 *******************************************************************************/

/* Code Attribute */

extern const uint8_t* ZEN_PRIMARY_ATTRIBUTE_CODE;
extern const int32_t ZEN_PRIMARY_ATTRIBUTE_CODE_LENGTH;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_ATTRIBUTE_H */