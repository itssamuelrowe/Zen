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

// Thursday, July 11, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_PARENT_OTHER_ATTRIBUTE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_PARENT_OTHER_ATTRIBUTE_H

/*******************************************************************************
 * ParentOtherAttribute                                                     *
 *******************************************************************************/

/**
 * The parent other attribute is a secondary attribute, with fixed length.
 * It represents a component such as static initializer, instance initializer,
 * or field initializer within whose context a function was declared.
 *
 * @class ParentOtherAttribute
 * @ingroup zen_virtualMachine_feb_attribute
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ParentOtherAttribute_t {
    
    /**
     * A valid index into the constant pool. The entry at the specified index
     * should be a `ConstantPoolClass` instance.
     */
    uint16_t m_classIndex;
};

typedef struct zen_ParentOtherAttribute_t zen_ParentOtherAttribute_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_PARENT_OTHER_ATTRIBUTE_H */