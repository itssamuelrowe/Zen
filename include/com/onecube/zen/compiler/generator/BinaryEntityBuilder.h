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

// Tuesday, July 16, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_GENERATOR_BINARY_ENTITY_BUILDER_H
#define COM_ONECUBE_ZEN_COMPILER_GENERATOR_BINARY_ENTITY_BUILDER_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/generator/DataChannel.h>

#include <com/onecube/zen/virtual-machine/feb/ByteCode.h>
#include <com/onecube/zen/virtual-machine/feb/attribute/ExceptionTable.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolEntry.h>

/*******************************************************************************
 * BinaryEntityBuilder                                                         *
 *******************************************************************************/

/**
 * The BinaryEntityBuilder exposes an interesting approach in assembling
 * binary entities. It consists of a stack of data channels. A data channel
 * has three states: active, inactive, and dead. The data channel at the
 * top of the stack, is the only active data channel.
 *
 * Whenever a new channel is pushed, it becomes the active channel making the
 * previous channel inactive. When an active data channel is popped, it moves
 * to the dead state. The data channel below the active channel becomes the next
 * active channel. When a data channel is in the dead state, the bytes of the
 * channel are appended to the bytes of the next active channel. After which,
 * the dead data channel is destroyed.
 *
 * The data channels allow the generators to lazily evaluate jump offsets,
 * header sizes, and many other values which would otherwise require complex
 * algorithms.
 *
 * @class BinaryEntityBuilder
 * @ingroup zen_compiler_generator
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_BinaryEntityBuilder_t {
    jtk_ArrayList_t* m_channels;
    uint16_t m_constantPoolIndex;
    int32_t m_identifier;
    int32_t m_activeChannelIndex;
};

/**
 * @memberof BinaryEntityBuilder
 */
typedef struct zen_BinaryEntityBuilder_t zen_BinaryEntityBuilder_t;

// Constructor

/**
 * @memberof BinaryEntityBuilder
 */
zen_BinaryEntityBuilder_t* zen_BinaryEntityBuilder_new();

// Destructor

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_delete(zen_BinaryEntityBuilder_t* builder);

// Channel

zen_DataChannel_t* zen_BinaryEntityBuilder_getChannel(zen_BinaryEntityBuilder_t* builder,
    int32_t index);

int32_t zen_BinaryEntityBuilder_pushChannel(zen_BinaryEntityBuilder_t* builder);

void zen_BinaryEntityBuilder_popChannel(zen_BinaryEntityBuilder_t* builder);

zen_DataChannel_t* zen_BinaryEntityBuilder_getActiveChannel(zen_BinaryEntityBuilder_t* builder);

void zen_BinaryEntityBuilder_setActiveChannelIndex(zen_BinaryEntityBuilder_t* builder,
    int32_t activeChannelIndex);

int32_t zen_BinaryEntityBuilder_getActiveChannelIndex(zen_BinaryEntityBuilder_t* builder);

// Emit

void zen_BinaryEntityBuilder_emitByteCode(zen_BinaryEntityBuilder_t* builder,
    zen_ByteCode_t byteCode, ...);

// Print

void zen_BinaryEntityBuilder_printChannel(zen_BinaryEntityBuilder_t* builder);

void zen_BinaryEntityBuilder_printChannels(zen_BinaryEntityBuilder_t* builder);

// Magic Number

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_writeMagicNumber(zen_BinaryEntityBuilder_t* builder);

// Version

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_writeMajorVersion(zen_BinaryEntityBuilder_t* builder,
    uint16_t majorVersion);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_writeMinorVersion(zen_BinaryEntityBuilder_t* builder,
    uint16_t minorVersion);

// Stream

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_writeStreamSize(zen_BinaryEntityBuilder_t* builder,
    uint32_t streamSize);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_writeStreamFlags(zen_BinaryEntityBuilder_t* builder,
    uint16_t streamFlags);

// Constant Pool

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_writeConstantPoolHeader(zen_BinaryEntityBuilder_t* builder, int32_t entries);

/**
 * @memberof BinaryEntityBuilder
 */
uint16_t zen_BinaryEntityBuilder_writeConstantPoolInteger(zen_BinaryEntityBuilder_t* builder, uint32_t value);

/**
 * @memberof BinaryEntityBuilder
 */
uint16_t zen_BinaryEntityBuilder_writeConstantPoolLong(zen_BinaryEntityBuilder_t* builder, uint64_t value);

/**
 * @memberof BinaryEntityBuilder
 */
uint16_t zen_BinaryEntityBuilder_writeConstantPoolLongEx(zen_BinaryEntityBuilder_t* builder,
    uint32_t highBytes, uint32_t lowBytes);

/**
 * @memberof BinaryEntityBuilder
 */
uint16_t zen_BinaryEntityBuilder_writeConstantPoolFloat(zen_BinaryEntityBuilder_t* builder, float value);

/**
 * @memberof BinaryEntityBuilder
 */
uint16_t zen_BinaryEntityBuilder_writeConstantPoolDouble(zen_BinaryEntityBuilder_t* builder, double value);

/**
 * @memberof BinaryEntityBuilder
 */
uint16_t zen_BinaryEntityBuilder_writeConstantPoolUtf8(zen_BinaryEntityBuilder_t* builder, uint16_t length, uint8_t* value);

/**
 * @memberof BinaryEntityBuilder
 */
uint16_t zen_BinaryEntityBuilder_writeConstantPoolString(zen_BinaryEntityBuilder_t* builder, uint16_t stringIndex);

/**
 * @memberof BinaryEntityBuilder
 */
uint16_t zen_BinaryEntityBuilder_writeConstantPoolFunction(zen_BinaryEntityBuilder_t* builder, uint16_t classIndex, uint16_t descriptorIndex,
    uint16_t nameIndex, uint16_t tableIndex);

/**
 * @memberof BinaryEntityBuilder
 */
uint16_t zen_BinaryEntityBuilder_writeConstantPoolField(zen_BinaryEntityBuilder_t* builder, uint16_t classIndex,
    uint16_t descriptorIndex, uint16_t nameIndex);

/**
 * @memberof BinaryEntityBuilder
 */
uint16_t zen_BinaryEntityBuilder_writeConstantPoolClass(zen_BinaryEntityBuilder_t* builder, uint16_t nameIndex);

// Entity

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_writeEntityHeader(zen_BinaryEntityBuilder_t* builder, uint8_t type, uint16_t flags,
    uint16_t reference);

// Class

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_writeClass(zen_BinaryEntityBuilder_t* builder, uint16_t flags, uint16_t reference,
    uint16_t superclassCount, uint16_t* superclassIndexes);

// Attribute

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_writeAttributeCount(zen_BinaryEntityBuilder_t* builder, uint16_t attributeCount);

// Field

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_writeFieldsHeader(zen_BinaryEntityBuilder_t* builder,
    uint16_t fieldCount, uint16_t fieldTableSize);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_writeField(zen_BinaryEntityBuilder_t* builder,
    uint16_t flags, uint16_t nameIndex, uint16_t descriptorIndex,
    uint16_t tableIndex);

// Function

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_writeFunctionsHeader(zen_BinaryEntityBuilder_t* builder,
    uint16_t functionCount, uint16_t functionTableSize);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_writeFunction(zen_BinaryEntityBuilder_t* builder,
    uint16_t flags, uint16_t nameIndex, uint16_t descriptorIndex,
    uint16_t tableIndex);

// Instruction Attribute

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_writeInstructionAttributeHeader(zen_BinaryEntityBuilder_t* builder, uint16_t nameIndex,
    uint32_t length, uint16_t maxStackSize, uint16_t localVariableCount,
    uint32_t instructionCount);

void zen_BinaryEntityBuilder_writeExceptionTableHeader(zen_BinaryEntityBuilder_t* builder, uint16_t size);

// Instructions

/* NOP */

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitNop(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitAddInteger(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitAddLong(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitAddFloat(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitAddDouble(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitAndInteger(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitAndLong(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitOrInteger(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitOrLong(zen_BinaryEntityBuilder_t* builder);

/* Shift Left */

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitShiftLeftInteger(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitShiftLeftLong(zen_BinaryEntityBuilder_t* builder);

/* Shift Right */

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitShiftRightInteger(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitShiftRightLong(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitShiftRightUnsignedInteger(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitShiftRightUnsignedLong(zen_BinaryEntityBuilder_t* builder);

/* XOR */

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitXorInteger(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitXorLong(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCastITB(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCastITS(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCastITL(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCastITF(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCastITD(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCastLTB(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCastLTS(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCastLTI(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCastLTF(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCastLTD(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCastFTI(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCastFTL(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCastFTD(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCastDTI(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCastDTL(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCastDTF(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCastITC(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCheckCast(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCompareLong(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCompareLesserThanFloat(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCompareGreaterThanFloat(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCompareLesserThanDouble(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitCompareGreaterThanDouble(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitDivideInteger(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitDivideLong(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitDivideFloat(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitDivideDouble(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitDuplicate(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitDuplicateX1(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitDuplicateX2(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitDuplicate2(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitDuplicate2X1(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitDuplicate2X2(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitJumpEqual0Integer(zen_BinaryEntityBuilder_t* builder, int16_t offset);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitJumpNotEqual0Integer(zen_BinaryEntityBuilder_t* builder, int16_t offset);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitJumpLesserThan0Integer(zen_BinaryEntityBuilder_t* builder, int16_t offset);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitJumpGreaterThan0Integer(zen_BinaryEntityBuilder_t* builder, int16_t offset);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitJumpLesserThanOrEqualTo0Integer(zen_BinaryEntityBuilder_t* builder, int16_t offset);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitJumpGreaterThanOrEqualTo0Integer(zen_BinaryEntityBuilder_t* builder, int16_t offset);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitJumpEqualInteger(zen_BinaryEntityBuilder_t* builder, int16_t offset);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitJumpNotEqualInteger(zen_BinaryEntityBuilder_t* builder, int16_t offset);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitJumpLesserThanInteger(zen_BinaryEntityBuilder_t* builder, int16_t offset);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitJumpGreaterThanInteger(zen_BinaryEntityBuilder_t* builder, int16_t offset);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitJumpLesserThanOrEqualInteger(zen_BinaryEntityBuilder_t* builder, int16_t offset);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitJumpGreaterThanOrEqualInteger(zen_BinaryEntityBuilder_t* builder, int16_t offset);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitJumpEqualReference(zen_BinaryEntityBuilder_t* builder, int16_t offset);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitJumpNotEqualReference(zen_BinaryEntityBuilder_t* builder, int16_t offset);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitJumpEqualNullReference(zen_BinaryEntityBuilder_t* builder, int16_t offset);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitJumpNotEqualNullReference(zen_BinaryEntityBuilder_t* builder, int16_t offset);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitIncrementInteger(zen_BinaryEntityBuilder_t* builder, uint8_t index, uint8_t constant);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitWideIncrementInteger(zen_BinaryEntityBuilder_t* builder, uint16_t index, uint16_t constant);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitInvokeSpecial(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitInvokeVirtual(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitInvokeDynamic(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitInvokeStatic(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/* Jump */

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitJump(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/* Load */

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoadInteger(zen_BinaryEntityBuilder_t* builder, uint8_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitWideLoadInteger(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoadLong(zen_BinaryEntityBuilder_t* builder, uint8_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitWideLoadLong(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoadFloat(zen_BinaryEntityBuilder_t* builder, uint8_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitWideLoadFloat(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoadDouble(zen_BinaryEntityBuilder_t* builder, uint8_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitWideLoadDouble(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoadReference(zen_BinaryEntityBuilder_t* builder, uint8_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitWideLoadReference(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoad0Integer(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoad1Integer(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoad2Integer(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoad3Integer(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoad0Long(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoad1Long(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoad2Long(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoad3Long(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoad0Float(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoad1Float(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoad2Float(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoad3Float(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoad0Double(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoad1Double(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoad2Double(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoad3Double(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoad0Reference(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoad1Reference(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoad2Reference(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoad3Reference(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoadArrayByte(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoadArrayCharacter(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoadArrayShort(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoadArrayInteger(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoadArrayLong(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoadArrayFloat(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoadArrayDouble(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoadArrayReference(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoadInstanceField(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoadStaticField(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoadCPR(zen_BinaryEntityBuilder_t* builder, uint8_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitWideLoadCPR(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitLoadArraySize(zen_BinaryEntityBuilder_t* builder);

/* Modulo */

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitModuloInteger(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitModuloLong(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitModuloFloat(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitModuloDouble(zen_BinaryEntityBuilder_t* builder);

/* Multiply */

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitMultiplyInteger(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitMultiplyLong(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitMultiplyFloat(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitMultiplyDouble(zen_BinaryEntityBuilder_t* builder);

/* Negate */

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitNegateInteger(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitNegateLong(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitNegateFloat(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitNegateDouble(zen_BinaryEntityBuilder_t* builder);

/* New */

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitNew(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitNewArray(zen_BinaryEntityBuilder_t* builder, uint8_t type);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitNewReferenceArray(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitNewDimensionalArray(zen_BinaryEntityBuilder_t* builder, uint16_t index,
    uint8_t dimensions);

/* Pop */

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPop(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPop2(zen_BinaryEntityBuilder_t* builder);

/* Push */

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPushNull(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPushIntegerNegative1(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPushInteger0(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPushInteger1(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPushInteger2(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPushInteger3(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPushInteger4(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPushInteger5(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPushLong0(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPushLong1(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPushLong2(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPushFloat0(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPushFloat1(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPushFloat2(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPushDouble0(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPushDouble1(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPushDouble2(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPushByte(zen_BinaryEntityBuilder_t* builder, int8_t value);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitPushShort(zen_BinaryEntityBuilder_t* builder, int16_t value);

/* Return */

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitReturn(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitReturnInteger(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitReturnLong(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitReturnFloat(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitReturnDouble(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitReturnReference(zen_BinaryEntityBuilder_t* builder);

/* RTTI */

/* Store */

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreInteger(zen_BinaryEntityBuilder_t* builder, uint8_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitWideStoreInteger(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreInteger0(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreInteger1(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreInteger2(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreInteger3(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreLong(zen_BinaryEntityBuilder_t* builder, uint8_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitWideStoreLong(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreLong0(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreLong1(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreLong2(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreLong3(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreFloat(zen_BinaryEntityBuilder_t* builder, uint8_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitWideStoreFloat(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreFloat0(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreFloat1(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreFloat2(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreFloat3(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreDouble(zen_BinaryEntityBuilder_t* builder, uint8_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitWideStoreDouble(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreDouble0(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreDouble1(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreDouble2(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreDouble3(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreReference(zen_BinaryEntityBuilder_t* builder, uint8_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitWideStoreReference(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreReference0(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreReference1(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreReference2(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreReference3(zen_BinaryEntityBuilder_t* builder);

/* Store Array */

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreArrayByte(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreArrayCharacter(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreArrayShort(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreArrayInteger(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreArrayLong(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreArrayFloat(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreArrayDouble(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreArrayReference(zen_BinaryEntityBuilder_t* builder);

/* Store Field */

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreInstanceField(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitStoreStaticField(zen_BinaryEntityBuilder_t* builder, uint16_t index);

/* Subtract */

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitSubtractInteger(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitSubtractLong(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitSubtractFloat(zen_BinaryEntityBuilder_t* builder);

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitSubtractDouble(zen_BinaryEntityBuilder_t* builder);

/* Swap */

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitSwap(zen_BinaryEntityBuilder_t* builder);

/* Throw */

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitThrow(zen_BinaryEntityBuilder_t* builder);

/* Wide */

/**
 * @memberof BinaryEntityBuilder
 */
void zen_BinaryEntityBuilder_emitWide(zen_BinaryEntityBuilder_t* builder);

#endif /* COM_ONECUBE_ZEN_COMPILER_GENERATOR_BINARY_ENTITY_BUILDER_H */