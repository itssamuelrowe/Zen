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

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_LOCAL_VARIABLE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_LOCAL_VARIABLE_H

/*******************************************************************************
 * LocalVariable                                                               *
 *******************************************************************************/

/**
 * @class LocalVariable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_LocalVariable_t {
    /**
The index at which the local variable is first visible. In reality,
the binary entity format does not acknowledge the concept of local varaibles.
Instead the compilers should be designed to handle the scopes of local
variables.
     *
Further, the specified index should be within the boundaries of the
instructions.
     */
    uint16_t m_startIndex;
    
    /**
The number of instructions that can "access" the local variable. In reality,
the binary entity format does not acknowledge the concept of local varaibles.
Instead the compilers should be designed to handle the scopes of local
variables.
     *
Further, the sum of the specified start index and count should be within
the boundaries of the instructions. In other words, the following condition
should be true.
`m_startIndex + m_count < instructionAttribute.m_numberOfInstructions`
     */
    uint16_t m_count;
    
    /**
A valid index into the constant pool. The entry at the specified index
should be a `ConstantPoolUtf8` instance. The descriptor represents the
type of the local variable.
     */
    uint16_t m_descriptorIndex;
    
    /**
A valid index into the constant pool. The entry at the specified index
should be a `ConstantPoolUtf8` instance. The name represents the identifier
of the local variable as it appears in the source code.
     */
    uint16_t m_nameIndex;
    
    /**
A valid index into the local variable array in the current frame. The entry
at the specified index contains the value of the variable. The entry may
occupy more than one spot, provided that the variable is long or double.
     */
    uint16_t m_index;
};

/**
 * @memberof LocalVariable
 */
typedef struct zen_LocalVariable_t zen_LocalVariable_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_LOCAL_VARIABLE_H */