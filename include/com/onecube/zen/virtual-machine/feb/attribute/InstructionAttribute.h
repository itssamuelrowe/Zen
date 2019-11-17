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

#ifndef ZEN_FEB_INSTRUCTION_ATTRIBUTE_H
#define ZEN_FEB_INSTRUCTION_ATTRIBUTE_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/feb/attribute/ExceptionTable.h>
#include <com/onecube/zen/virtual-machine/feb/attribute/AttributeTable.h>

/*******************************************************************************
 * InstructionAttribute                                                        *
 *******************************************************************************/

/**
 * The instruction attribute is a primary attribute with variable-length. It
 * can be associated with a function, class and enumeration entities. It
 * contains the instructions defined in a function, constructor, or an
 * initializer. In the case of native and abstract functions, this attribute
 * is absent.
 *
 * @class InstructionAttribute
 * @ingroup zen_vm_feb
 * @author Samuel Rowe
 * @since zen 1.0 
 */
struct zen_InstructionAttribute_t {
    uint16_t m_nameIndex;
    uint32_t m_length;
    uint16_t m_maxStackSize;
    uint16_t m_localVariableCount;
    uint32_t m_instructionLength;
    uint8_t* m_instructions;
    zen_ExceptionTable_t m_exceptionTable;
};

/**
 * @memberof InstructionAttribute
 */
typedef struct zen_InstructionAttribute_t zen_InstructionAttribute_t;

// Constructor

/**
 * @memberof InstructionAttribute
 */
zen_InstructionAttribute_t* zen_InstructionAttribute_new(uint16_t nameIndex,
    uint32_t length,
    uint16_t maxStackSize,
    uint16_t localVariableCount,
    uint32_t instructionLength,
    uint8_t* instructions);

// Destructor

/**
 * @memberof InstructionAttribute
 */
void zen_InstructionAttribute_delete(zen_InstructionAttribute_t* attribute);

#endif /* ZEN_FEB_INSTRUCTION_ATTRIBUTE_H */