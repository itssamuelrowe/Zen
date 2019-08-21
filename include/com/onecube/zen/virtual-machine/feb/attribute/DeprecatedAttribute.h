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

// Thursday, July 11, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_DEPRECATED_ATTRIBUTE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_DEPRECATED_ATTRIBUTE_H

/*******************************************************************************
 * DeprecatedAttribute                                                         *
 *******************************************************************************/

/**
 * The deprecated attribute is a secondary attribute, with fixed length. It
 * represents the deprecation of a component. The virtual machine does not
 * treat this attribute specially. It is designed to accommodate external tools
 * such as compilers and debuggers.
 *
 * @class DeprecatedAttribute
 * @ingroup zen_virtualMachine_feb_attribute
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_DeprecatedAttribute_t {
};

/**
 * @memberof DeprecatedAttribute
 */
typedef struct zen_DeprecatedAttribute_t zen_DeprecatedAttribute_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_DEPRECATED_ATTRIBUTE_H */