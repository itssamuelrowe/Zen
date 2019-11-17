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

// Saturday, September 22, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_AUXILLARY_ATTRIBUTE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_AUXILLARY_ATTRIBUTE_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * AuxillaryAttribute                                                          *
 *******************************************************************************/

/**
 * @class AuxillaryAttribute
 * @ingroup zen_vm_feb
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AuxillaryAttribute_t {
    uint16_t m_operandStackSize;
    uint16_t m_localVariableArraySize;
};

/**
 * @memberof AuxillaryAttribute
 */
typedef struct zen_AuxillaryAttribute_t zen_AuxillaryAttribute_t;
    
#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_AUXILLARY_ATTRIBUTE_H */