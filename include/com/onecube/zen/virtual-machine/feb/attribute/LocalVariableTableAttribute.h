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

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_LOCAL_VARIABLE_TABLE_ATTRIBUTE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_LOCAL_VARIABLE_TABLE_ATTRIBUTE_H

/*******************************************************************************
 * LocalVariableTableAttribute                                                 *
 *******************************************************************************/

/**
 * The local variable table attribute is a secondary attribute, with variable
 * length. It is designed to accommodate external tools such as debuggers.
 *
 * @class LocalVariableTableAttribute
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_LocalVariableTableAttribute_t {
    uint16_t m_size;
    zen_LocalVariable_t** m_localVariables;
};

/**
 * @memberof LocalVariableTableAttribute
 */
typedef struct zen_LocalVariableTableAttribute_t zen_LocalVariableTableAttribute_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_LOCAL_VARIABLE_TABLE_ATTRIBUTE_H */