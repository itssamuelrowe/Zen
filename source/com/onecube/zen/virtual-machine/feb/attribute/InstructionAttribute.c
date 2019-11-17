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

// Tuesday, October 08, 2019

#include <com/onecube/zen/virtual-machine/feb/attribute/InstructionAttribute.h>

/*******************************************************************************
 * InstructionAttribute                                                        *
 *******************************************************************************/

// Constructor

zen_InstructionAttribute_t* zen_InstructionAttribute_new(uint16_t nameIndex,
    uint32_t length,
    uint16_t maxStackSize,
    uint16_t localVariableCount,
    uint32_t instructionLength,
    uint8_t* instructions) {
    zen_InstructionAttribute_t* attribute = zen_Memory_allocate(zen_InstructionAttribute_t, 1);
    attribute->m_nameIndex = nameIndex;
    attribute->m_length = length;
    attribute->m_maxStackSize = maxStackSize;
    attribute->m_localVariableCount = localVariableCount;
    attribute->m_instructionLength = instructionLength;
    attribute->m_instructions = instructions;
    attribute->m_exceptionTable.m_size = 0;
    attribute->m_exceptionTable.m_exceptionHandlerSites = NULL;

    return attribute;
}

// Destructor

void zen_InstructionAttribute_delete(zen_InstructionAttribute_t* attribute) {
    jtk_Assert_assertObject(attribute, "The specified instruction attribute is null.");

    zen_Memory_deallocate(attribute);
}