/*
 * Copyright 2017-2020 Samuel Rowe
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

// Thursday, April 12, 2018

#ifndef ZEN_FEB_INSTRUCTION_H
#define ZEN_FEB_INSTRUCTION_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/feb/ByteCode.h>

/*******************************************************************************
 * Instruction                                                                 *
 *******************************************************************************/

struct zen_Instruction_t {
    const uint8_t m_text[30];
    uint32_t m_byteCode;
    uint32_t m_argumentCount;
    uint32_t m_operandCount;
    uint32_t m_resultCount;
};

typedef struct zen_Instruction_t zen_Instruction_t;

bool zen_Instruction_verify();
zen_Instruction_t* zen_Instruction_getInstance(zen_ByteCode_t byteCode);
const uint8_t* zen_Instruction_getText(zen_Instruction_t* instruction);
uint8_t zen_Instruction_getByteCode(zen_Instruction_t* instruction);
uint8_t zen_Instruction_getArgumentCount(zen_Instruction_t* instruction);
uint8_t zen_Instruction_getOperandCount(zen_Instruction_t* instruction);
uint8_t zen_Instruction_getResultCount(zen_Instruction_t* instruction);

#endif /* ZEN_FEB_INSTRUCTION_H */