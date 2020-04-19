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

#include <jtk/core/Double.h>
#include <jtk/core/Float.h>
#include <jtk/core/VariableArguments.h>

#include <com/onecube/zen/compiler/generator/BinaryEntityBuilder.h>
#include <com/onecube/zen/virtual-machine/feb/ByteCode.h>
#include <com/onecube/zen/virtual-machine/feb/EntityType.h>
#include <com/onecube/zen/virtual-machine/feb/Instruction.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPool.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolClass.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolDouble.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolEntry.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolField.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolFloat.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolFunction.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolInteger.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolLong.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolString.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolTag.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolUtf8.h>

/*******************************************************************************
 * BinaryEntityBuilder                                                         *
 *******************************************************************************/

// Constructor

zen_BinaryEntityBuilder_t* zen_BinaryEntityBuilder_new() {
    zen_BinaryEntityBuilder_t* builder = jtk_Memory_allocate(zen_BinaryEntityBuilder_t, 1);
    builder->m_channels = jtk_ArrayList_new();
    builder->m_constantPoolIndex = 0;
    builder->m_identifier = 0;
    builder->m_activeChannelIndex = -1;

    return builder;
}

// Destructor

void zen_BinaryEntityBuilder_delete(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified binary entity builder is null.");

    jtk_Iterator_t* iterator = jtk_ArrayList_getIterator(builder->m_channels);
    while (jtk_Iterator_hasNext(iterator)) {
        zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_Iterator_getNext(iterator);
        if (channel != NULL) {
            zen_DataChannel_delete(channel);
        }
    }
    jtk_Iterator_delete(iterator);

    jtk_ArrayList_delete(builder->m_channels);
    jtk_Memory_deallocate(builder);
}

// Channel

int32_t zen_BinaryEntityBuilder_addChannel(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified binary entity builder is null.");

    int32_t result = jtk_ArrayList_getSize(builder->m_channels);

    zen_DataChannel_t* channel = zen_DataChannel_new(builder->m_identifier++);
    jtk_ArrayList_add(builder->m_channels, channel);

    return result;
}

void zen_BinaryEntityBuilder_removeChannel(zen_BinaryEntityBuilder_t* builder,
    int32_t index) {
    jtk_Assert_assertObject(builder, "The specified binary entity builder is null.");

    // TODO: Add a return value to jtk_ArrayList_removeLast()
    zen_DataChannel_t* deadChannel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels,
        index);
    // jtk_ArrayList_removeIndex(builder->m_channels, index);
    // jtk_ArrayList_setValue(builder->m_channels, index, NULL);
    jtk_ArrayList_removeIndex(builder->m_channels, index);

    /* if (!jtk_ArrayList_isEmpty(builder->m_channels)) {
        zen_DataChannel_t* activeChannel = (zen_DataChannel_t*)jtk_ArrayList_getValue(
            builder->m_channels, builder->m_primaryChannel);
        zen_DataChannel_appendChannel(activeChannel, deadChannel);
    }
    */

    zen_DataChannel_delete(deadChannel);
}

zen_DataChannel_t* zen_BinaryEntityBuilder_getChannel(zen_BinaryEntityBuilder_t* builder,
    int32_t index) {
    jtk_Assert_assertObject(builder, "The specified binary entity builder is null.");

    return (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, index);
}

zen_DataChannel_t* zen_BinaryEntityBuilder_getActiveChannel(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified binary entity builder is null.");

    /* NOTE: As of now, the active channel is located at zeroth index.
     * However, in the future this behavior may be subjected to change.
     */
    // return jtk_ArrayList_isEmpty(builder->m_channels) || (builder->m_activeChannelIndex < 0)?
    //    NULL : jtk_ArrayList_getValue(builder->m_channels, 0);

    return jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
}

void zen_BinaryEntityBuilder_setActiveChannelIndex(zen_BinaryEntityBuilder_t* builder,
    int32_t activeChannelIndex) {
    builder->m_activeChannelIndex = activeChannelIndex;
}

int32_t zen_BinaryEntityBuilder_getActiveChannelIndex(zen_BinaryEntityBuilder_t* builder) {
    return builder->m_activeChannelIndex;
}

// Emit

void zen_BinaryEntityBuilder_emitByteCode(zen_BinaryEntityBuilder_t* builder,
    zen_ByteCode_t byteCode, ...) {
    jtk_Assert_assertObject(builder, "The specified binary entity builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);

    zen_Instruction_t* instruction = zen_Instruction_getInstance(byteCode);
    uint8_t argumentCount = zen_Instruction_getArgumentCount(instruction);
    if (argumentCount > 0) {
        zen_DataChannel_requestCapacity(channel, argumentCount + 1);

        jtk_VariableArguments_t variableArguments;
        jtk_VariableArguments_start(variableArguments, byteCode);

        channel->m_bytes[channel->m_index++] = (uint8_t)byteCode;
        int32_t i;
        for (i = 0; i < argumentCount; i++) {
            channel->m_bytes[channel->m_index++] =
                // (uint8_t)jtk_VariableArguments_argument(variableArguments, int32_t);
                (uint8_t)va_arg(variableArguments, int32_t);
        }

        jtk_VariableArguments_end(variableArguments);
    }
}

// Print

void zen_BinaryEntityBuilder_printChannel(zen_BinaryEntityBuilder_t* builder) {
}

void zen_BinaryEntityBuilder_printChannels(zen_BinaryEntityBuilder_t* builder) {
}

// Magic Number

void zen_BinaryEntityBuilder_writeMagicNumber(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 4);

    channel->m_bytes[channel->m_index++] = 0xFE;
    channel->m_bytes[channel->m_index++] = 0xB7;
    channel->m_bytes[channel->m_index++] = 0x20;
    channel->m_bytes[channel->m_index++] = 0x00;
}

void zen_BinaryEntityBuilder_writeMajorVersion(zen_BinaryEntityBuilder_t* builder,
    uint16_t majorVersion) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 2);

    channel->m_bytes[channel->m_index++] = (majorVersion & 0x0000FF00) >> 8;
    channel->m_bytes[channel->m_index++] = (majorVersion & 0x000000FF);
}

void zen_BinaryEntityBuilder_writeMinorVersion(zen_BinaryEntityBuilder_t* builder,
    uint16_t minorVersion) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 2);

    channel->m_bytes[channel->m_index++] = (minorVersion & 0x0000FF00) & 0x00;
    channel->m_bytes[channel->m_index++] = (minorVersion & 0x000000FF);
}

/*
void zen_BinaryEntityBuilder_writeStreamSize(zen_BinaryEntityBuilder_t* builder,
    uint32_t streamSize) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 4);

    channel->m_bytes[channel->m_index++] = (streamSize & 0xFF000000) >> 24; // stream size
    channel->m_bytes[channel->m_index++] = (streamSize & 0x00FF0000) >> 16;
    channel->m_bytes[channel->m_index++] = (streamSize & 0x0000FF00) >> 8;
    channel->m_bytes[channel->m_index++] = (streamSize & 0x000000FF);
}
*/

void zen_BinaryEntityBuilder_writeStreamFlags(zen_BinaryEntityBuilder_t* builder,
    uint16_t streamFlags) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 2);

    channel->m_bytes[channel->m_index++] = (streamFlags & 0x0000FF00) >> 8; // flags
    channel->m_bytes[channel->m_index++] = (streamFlags & 0x000000FF);
}

void zen_BinaryEntityBuilder_writeConstantPoolHeader(zen_BinaryEntityBuilder_t* builder, int32_t entries) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 4);

    channel->m_bytes[channel->m_index++] = (entries & 0x0000FF00) >> 8;
    channel->m_bytes[channel->m_index++] = (entries & 0x000000FF);
}

uint16_t zen_BinaryEntityBuilder_writeConstantPoolInteger(zen_BinaryEntityBuilder_t* builder, uint32_t value) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 5);

    channel->m_bytes[channel->m_index++] = ZEN_CONSTANT_POOL_TAG_INTEGER; // Tag
    channel->m_bytes[channel->m_index++] = (value & 0xFF000000) >> 24;
    channel->m_bytes[channel->m_index++] = (value & 0x00FF0000) >> 16;
    channel->m_bytes[channel->m_index++] = (value & 0x0000FF00) >> 8;
    channel->m_bytes[channel->m_index++] = (value & 0x000000FF);

    return builder->m_constantPoolIndex++;
}

uint16_t zen_BinaryEntityBuilder_writeConstantPoolLong(zen_BinaryEntityBuilder_t* builder, uint64_t value) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 9);

    channel->m_bytes[channel->m_index++] = ZEN_CONSTANT_POOL_TAG_LONG; // Tag
    channel->m_bytes[channel->m_index++] = (value & 0xFF00000000000000L) >> 56; // Value
    channel->m_bytes[channel->m_index++] = (value & 0x00FF000000000000L) >> 48;
    channel->m_bytes[channel->m_index++] = (value & 0x0000FF0000000000L) >> 40;
    channel->m_bytes[channel->m_index++] = (value & 0x000000FF00000000L) >> 32;
    channel->m_bytes[channel->m_index++] = (value & 0x00000000FF000000L) >> 24;
    channel->m_bytes[channel->m_index++] = (value & 0x0000000000FF0000L) >> 16;
    channel->m_bytes[channel->m_index++] = (value & 0x000000000000FF00L) >> 8;
    channel->m_bytes[channel->m_index++] = (value & 0x00000000000000FFL);

    return builder->m_constantPoolIndex++;
}

uint16_t zen_BinaryEntityBuilder_writeConstantPoolLongEx(zen_BinaryEntityBuilder_t* builder,
    uint32_t highBytes, uint32_t lowBytes) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 5);

    channel->m_bytes[channel->m_index++] = ZEN_CONSTANT_POOL_TAG_INTEGER; // Tag
    channel->m_bytes[channel->m_index++] = (highBytes & 0xFF000000) >> 24;
    channel->m_bytes[channel->m_index++] = (highBytes & 0x00FF0000) >> 16;
    channel->m_bytes[channel->m_index++] = (highBytes & 0x0000FF00) >> 8;
    channel->m_bytes[channel->m_index++] = (highBytes & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (lowBytes & 0xFF000000) >> 24;
    channel->m_bytes[channel->m_index++] = (lowBytes & 0x00FF0000) >> 16;
    channel->m_bytes[channel->m_index++] = (lowBytes & 0x0000FF00) >> 8;
    channel->m_bytes[channel->m_index++] = (lowBytes & 0x000000FF);

    return builder->m_constantPoolIndex++;
}

uint16_t zen_BinaryEntityBuilder_writeConstantPoolFloat(zen_BinaryEntityBuilder_t* builder, float value) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 5);

    uint32_t value0 = jtk_Float_unpack(value);

    channel->m_bytes[channel->m_index++] = ZEN_CONSTANT_POOL_TAG_FLOAT; // Tag
    channel->m_bytes[channel->m_index++] = (value0 & 0xFF000000) >> 24; // Value
    channel->m_bytes[channel->m_index++] = (value0 & 0x00FF0000) >> 16;
    channel->m_bytes[channel->m_index++] = (value0 & 0x0000FF00) >> 8;
    channel->m_bytes[channel->m_index++] = (value0 & 0x000000FF);

    return builder->m_constantPoolIndex++;
}

uint16_t zen_BinaryEntityBuilder_writeConstantPoolFloatEx(zen_BinaryEntityBuilder_t* builder,
    uint32_t value) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 5);

    channel->m_bytes[channel->m_index++] = ZEN_CONSTANT_POOL_TAG_FLOAT; // Tag
    channel->m_bytes[channel->m_index++] = (value & 0xFF000000) >> 24; // Value
    channel->m_bytes[channel->m_index++] = (value & 0x00FF0000) >> 16;
    channel->m_bytes[channel->m_index++] = (value & 0x0000FF00) >> 8;
    channel->m_bytes[channel->m_index++] = (value & 0x000000FF);

    return builder->m_constantPoolIndex++;
}

uint16_t zen_BinaryEntityBuilder_writeConstantPoolDouble(zen_BinaryEntityBuilder_t* builder, double value) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 9);

    uint64_t value0 = jtk_Double_unpack(value);

    channel->m_bytes[channel->m_index++] = ZEN_CONSTANT_POOL_TAG_DOUBLE; // Tag
    channel->m_bytes[channel->m_index++] = (value0 & 0xFF00000000000000L) >> 56; // Value
    channel->m_bytes[channel->m_index++] = (value0 & 0x00FF000000000000L) >> 48;
    channel->m_bytes[channel->m_index++] = (value0 & 0x0000FF0000000000L) >> 40;
    channel->m_bytes[channel->m_index++] = (value0 & 0x000000FF00000000L) >> 32;
    channel->m_bytes[channel->m_index++] = (value0 & 0x00000000FF000000L) >> 24;
    channel->m_bytes[channel->m_index++] = (value0 & 0x0000000000FF0000L) >> 16;
    channel->m_bytes[channel->m_index++] = (value0 & 0x000000000000FF00L) >> 8;
    channel->m_bytes[channel->m_index++] = (value0 & 0x00000000000000FFL);

    return builder->m_constantPoolIndex++;
}

uint16_t zen_BinaryEntityBuilder_writeConstantPoolDoubleEx(zen_BinaryEntityBuilder_t* builder,
    uint32_t highBytes, uint32_t lowBytes) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 5);

    channel->m_bytes[channel->m_index++] = ZEN_CONSTANT_POOL_TAG_INTEGER; // Tag
    channel->m_bytes[channel->m_index++] = (highBytes & 0xFF000000) >> 24;
    channel->m_bytes[channel->m_index++] = (highBytes & 0x00FF0000) >> 16;
    channel->m_bytes[channel->m_index++] = (highBytes & 0x0000FF00) >> 8;
    channel->m_bytes[channel->m_index++] = (highBytes & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (lowBytes & 0xFF000000) >> 24;
    channel->m_bytes[channel->m_index++] = (lowBytes & 0x00FF0000) >> 16;
    channel->m_bytes[channel->m_index++] = (lowBytes & 0x0000FF00) >> 8;
    channel->m_bytes[channel->m_index++] = (lowBytes & 0x000000FF);

    return builder->m_constantPoolIndex++;
}

uint16_t zen_BinaryEntityBuilder_writeConstantPoolUtf8(zen_BinaryEntityBuilder_t* builder, uint16_t length, uint8_t* value) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3 + length);

    channel->m_bytes[channel->m_index++] = ZEN_CONSTANT_POOL_TAG_UTF8; // Tag
    channel->m_bytes[channel->m_index++] = (length & 0x0000FF00) >> 8;
    channel->m_bytes[channel->m_index++] = (length & 0x000000FF);
    int32_t j = 0;
    while (j < length) {
        channel->m_bytes[channel->m_index + j] = value[j];
        j++;
    }
    channel->m_index += length;

    return builder->m_constantPoolIndex++;
}

uint16_t zen_BinaryEntityBuilder_writeConstantPoolString(zen_BinaryEntityBuilder_t* builder, uint16_t stringIndex) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_CONSTANT_POOL_TAG_STRING; // Tag
    channel->m_bytes[channel->m_index++] = (stringIndex & 0x0000FF00) >> 8;
    channel->m_bytes[channel->m_index++] = (stringIndex & 0x000000FF);

    return builder->m_constantPoolIndex++;
}

uint16_t zen_BinaryEntityBuilder_writeConstantPoolFunction(
    zen_BinaryEntityBuilder_t* builder, uint16_t classIndex, uint16_t descriptorIndex,
    uint16_t nameIndex, uint16_t tableIndex) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 9);

    channel->m_bytes[channel->m_index++] = ZEN_CONSTANT_POOL_TAG_FUNCTION; // Tag
    channel->m_bytes[channel->m_index++] = (classIndex & 0x0000FF00) >> 8; // class index
    channel->m_bytes[channel->m_index++] = (classIndex & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (descriptorIndex & 0x0000FF00) >> 8; // descriptor index
    channel->m_bytes[channel->m_index++] = (descriptorIndex & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (nameIndex & 0x0000FF00) >> 8; // name index
    channel->m_bytes[channel->m_index++] = (nameIndex & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (tableIndex & 0x0000FF00) >> 8; // table index
    channel->m_bytes[channel->m_index++] = (tableIndex & 0x000000FF);

    return builder->m_constantPoolIndex++;
}

uint16_t zen_BinaryEntityBuilder_writeConstantPoolField(zen_BinaryEntityBuilder_t* builder, uint16_t classIndex, uint16_t descriptorIndex,
    uint16_t nameIndex) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 7);

    channel->m_bytes[channel->m_index++] = ZEN_CONSTANT_POOL_TAG_FIELD; // Tag
    channel->m_bytes[channel->m_index++] = (classIndex & 0x0000FF00) >> 8; // class index
    channel->m_bytes[channel->m_index++] = (classIndex & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (descriptorIndex & 0x0000FF00) >> 8; // descriptor index
    channel->m_bytes[channel->m_index++] = (descriptorIndex & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (nameIndex & 0x0000FF00) >> 8; // name index
    channel->m_bytes[channel->m_index++] = (nameIndex & 0x000000FF);

    return builder->m_constantPoolIndex++;
}

uint16_t zen_BinaryEntityBuilder_writeConstantPoolClass(zen_BinaryEntityBuilder_t* builder, uint16_t nameIndex) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_CONSTANT_POOL_TAG_CLASS; // Tag
    channel->m_bytes[channel->m_index++] = (nameIndex & 0x0000FF00) >> 8; // name index
    channel->m_bytes[channel->m_index++] = (nameIndex & 0x000000FF);

    return builder->m_constantPoolIndex++;
}

int32_t zen_BinaryEntityBuilder_writeConstantPoolEntry(zen_BinaryEntityBuilder_t* builder,
    zen_ConstantPoolEntry_t* entry) {
    int32_t result = -1;
    switch (entry->m_tag) {
        case ZEN_CONSTANT_POOL_TAG_INTEGER: {
            /* Convert the constant pool entry to its specific type. */
            zen_ConstantPoolInteger_t* constantPoolInteger =
                (zen_ConstantPoolInteger_t*)entry;
            /* Write the bytes of the constant pool entry to the data channel. */
            result = zen_BinaryEntityBuilder_writeConstantPoolInteger(builder,
                constantPoolInteger->m_bytes);

            break;
        }

        case ZEN_CONSTANT_POOL_TAG_LONG: {
            /* Convert the constant pool entry to its specific type. */
            zen_ConstantPoolLong_t* constantPoolLong =
                (zen_ConstantPoolLong_t*)entry;
            /* Write the bytes of the constant pool entry to the data channel. */
            result = zen_BinaryEntityBuilder_writeConstantPoolLongEx(builder,
                constantPoolLong->m_highBytes, constantPoolLong->m_lowBytes);

            break;
        }

        case ZEN_CONSTANT_POOL_TAG_FLOAT: {
            /* Convert the constant pool entry to its specific type. */
            zen_ConstantPoolFloat_t* constantPoolFloat =
                (zen_ConstantPoolFloat_t*)entry;
            /* Write the bytes of the constant pool entry to the data channel. */
            result = zen_BinaryEntityBuilder_writeConstantPoolFloatEx(builder,
                constantPoolFloat->m_bytes);

            break;
        }

        case ZEN_CONSTANT_POOL_TAG_DOUBLE: {
            /* Convert the constant pool entry to its specific type. */
            zen_ConstantPoolDouble_t* constantPoolDouble =
                (zen_ConstantPoolDouble_t*)entry;
            /* Write the bytes of the constant pool entry to the data channel. */
            result = zen_BinaryEntityBuilder_writeConstantPoolDoubleEx(builder,
                constantPoolDouble->m_highBytes, constantPoolDouble->m_lowBytes);

            break;
        }

        case ZEN_CONSTANT_POOL_TAG_UTF8: {
            /* Convert the constant pool entry to its specific type. */
            zen_ConstantPoolUtf8_t* constantPoolUtf8 =
                (zen_ConstantPoolUtf8_t*)entry;
            /* Write the bytes of the constant pool entry to the data channel. */
            result = zen_BinaryEntityBuilder_writeConstantPoolUtf8(builder,
                constantPoolUtf8->m_length, constantPoolUtf8->m_bytes);

            break;
        }

        case ZEN_CONSTANT_POOL_TAG_STRING: {
            /* Convert the constant pool entry to its specific type. */
            zen_ConstantPoolString_t* constantPoolString =
                (zen_ConstantPoolString_t*)entry;
            /* Write the bytes of the constant pool entry to the data channel. */
            result = zen_BinaryEntityBuilder_writeConstantPoolString(builder,
                constantPoolString->m_stringIndex);

            break;
        }

        case ZEN_CONSTANT_POOL_TAG_FUNCTION: {
            /* Convert the constant pool entry to its specific type. */
            zen_ConstantPoolFunction_t* constantPoolFunction =
                (zen_ConstantPoolFunction_t*)entry;
            /* Write the bytes of the constant pool entry to the data channel. */
            result = zen_BinaryEntityBuilder_writeConstantPoolFunction(builder,
                constantPoolFunction->m_classIndex,
                constantPoolFunction->m_descriptorIndex,
                constantPoolFunction->m_nameIndex,
                constantPoolFunction->m_tableIndex);

            break;
        }

        case ZEN_CONSTANT_POOL_TAG_FIELD: {
            /* Convert the constant pool entry to its specific type. */
            zen_ConstantPoolField_t* constantPoolField =
                (zen_ConstantPoolField_t*)entry;
            /* Write the bytes of the constant pool entry to the data channel. */
            result = zen_BinaryEntityBuilder_writeConstantPoolField(builder,
                constantPoolField->m_classIndex,
                constantPoolField->m_descriptorIndex,
                constantPoolField->m_nameIndex);

            break;
        }

        case ZEN_CONSTANT_POOL_TAG_CLASS: {
            /* Convert the constant pool entry to its specific type. */
            zen_ConstantPoolClass_t* constantPoolClass =
                (zen_ConstantPoolClass_t*)entry;
            /* Write the bytes of the constant pool entry to the data channel. */
            result = zen_BinaryEntityBuilder_writeConstantPoolClass(builder,
                constantPoolClass->m_nameIndex);

            break;
        }

        default: {
            fprintf(stderr, "[internal error] Control should not reach here.\n");
        }
    }
    return result;
}

void zen_BinaryEntityBuilder_writeEntityHeader(zen_BinaryEntityBuilder_t* builder, uint8_t type, uint16_t flags,
    uint16_t reference) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 5);

    channel->m_bytes[channel->m_index++] = type; // type
    channel->m_bytes[channel->m_index++] = (flags & 0x0000FF00) >> 8; // flags
    channel->m_bytes[channel->m_index++] = (flags & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (reference & 0x0000FF00) >> 8; // reference
    channel->m_bytes[channel->m_index++] = (reference & 0x000000FF);
}

void zen_BinaryEntityBuilder_writeSuperclasses(zen_BinaryEntityBuilder_t* builder,
    uint16_t superclassCount, uint16_t* superclassIndexes) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 7 + (superclassCount * 2));

    channel->m_bytes[channel->m_index++] = (superclassCount & 0x0000FF00) >> 8; // superclass count
    channel->m_bytes[channel->m_index++] = (superclassCount & 0x000000FF);
    int32_t j;
    for (j = 0; j < superclassCount; j++) {
        uint16_t superclassIndex = superclassIndexes[j];
        channel->m_bytes[channel->m_index++] = (superclassIndex & 0x0000FF00) >> 8; // superclass index
        channel->m_bytes[channel->m_index++] = (superclassIndex & 0x000000FF);
    }
}

void zen_BinaryEntityBuilder_writeAttributeCount(zen_BinaryEntityBuilder_t* builder, uint16_t attributeCount) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 2);

    channel->m_bytes[channel->m_index++] = (attributeCount & 0x0000FF00) >> 8; // attribute count
    channel->m_bytes[channel->m_index++] = (attributeCount & 0x000000FF);
}

void zen_BinaryEntityBuilder_writeFieldsHeader(zen_BinaryEntityBuilder_t* builder,
    uint16_t fieldCount, uint16_t fieldTableSize) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 4);

    channel->m_bytes[channel->m_index++] = (fieldCount & 0x0000FF00) >> 8; // field count
    channel->m_bytes[channel->m_index++] = (fieldCount & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (fieldTableSize & 0x0000FF00) >> 8; // field table size
    channel->m_bytes[channel->m_index++] = (fieldTableSize & 0x000000FF);
}

void zen_BinaryEntityBuilder_writeField(zen_BinaryEntityBuilder_t* builder,
    uint16_t flags, uint16_t nameIndex, uint16_t descriptorIndex,
    uint16_t tableIndex) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 8);

    channel->m_bytes[channel->m_index++] = (flags & 0x0000FF00) >> 8; // flags
    channel->m_bytes[channel->m_index++] = (flags & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (nameIndex & 0x0000FF00) >> 8; // name index
    channel->m_bytes[channel->m_index++] = (nameIndex & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (descriptorIndex & 0x0000FF00) >> 8; // descriptor index
    channel->m_bytes[channel->m_index++] = (descriptorIndex & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (tableIndex & 0x0000FF00) >> 8; // table index
    channel->m_bytes[channel->m_index++] = (tableIndex & 0x000000FF);
}

void zen_BinaryEntityBuilder_writeFunctionsHeader(zen_BinaryEntityBuilder_t* builder,
    uint16_t functionCount, uint16_t functionTableSize) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 4);

    channel->m_bytes[channel->m_index++] = (functionCount & 0x0000FF00) >> 8; // function count
    channel->m_bytes[channel->m_index++] = (functionCount & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (functionTableSize & 0x0000FF00) >> 8; // function table size
    channel->m_bytes[channel->m_index++] = (functionTableSize & 0x000000FF);
}

void zen_BinaryEntityBuilder_writeFunction(zen_BinaryEntityBuilder_t* builder,
    uint16_t flags, uint16_t nameIndex, uint16_t descriptorIndex,
    uint16_t tableIndex) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 8);

    channel->m_bytes[channel->m_index++] = (flags & 0x0000FF00) >> 8; // flags
    channel->m_bytes[channel->m_index++] = (flags & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (nameIndex & 0x0000FF00) >> 8; // name index
    channel->m_bytes[channel->m_index++] = (nameIndex & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (descriptorIndex & 0x0000FF00) >> 8; // descriptor index
    channel->m_bytes[channel->m_index++] = (descriptorIndex & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (tableIndex & 0x0000FF00) >> 8; // table index
    channel->m_bytes[channel->m_index++] = (tableIndex & 0x000000FF);
}

/* The localVariableCount and maxStackSize are in terms of 4-m_bytes. Therefore,
 * long, double and reference types each occupy a magnitude of 2.
 */
void zen_BinaryEntityBuilder_writeInstructionAttributeHeader(
    zen_BinaryEntityBuilder_t* builder, uint16_t nameIndex,
    uint32_t length, uint16_t maxStackSize, uint16_t localVariableCount,
    uint32_t instructionCount) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 14);

    channel->m_bytes[channel->m_index++] = (nameIndex & 0x0000FF00) >> 8; // Name Index
    channel->m_bytes[channel->m_index++] = (nameIndex & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (length & 0xFF000000) >> 24; // Length
    channel->m_bytes[channel->m_index++] = (length & 0x00FF0000) >> 16;
    channel->m_bytes[channel->m_index++] = (length & 0x0000FF00) >> 8;
    channel->m_bytes[channel->m_index++] = (length & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (maxStackSize & 0x0000FF00) >> 8; // Max Stack Size
    channel->m_bytes[channel->m_index++] = (maxStackSize & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (localVariableCount & 0x0000FF00) >> 8; // Local Variable Count
    channel->m_bytes[channel->m_index++] = (localVariableCount & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (instructionCount & 0xFF000000) >> 24; // Instruction Count
    channel->m_bytes[channel->m_index++] = (instructionCount & 0x00FF0000) >> 16;
    channel->m_bytes[channel->m_index++] = (instructionCount & 0x0000FF00) >> 8;
    channel->m_bytes[channel->m_index++] = (instructionCount & 0x000000FF);
}

void zen_BinaryEntityBuilder_writeInstructionAttribute(
    zen_BinaryEntityBuilder_t* builder, uint16_t nameIndex,
    uint32_t length, uint16_t maxStackSize, uint16_t localVariableCount,
    uint32_t instructionCount, uint8_t* instructions, zen_ExceptionTable_t* exceptionTable) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_BinaryEntityBuilder_writeInstructionAttributeHeader(
        builder, nameIndex, length, maxStackSize, localVariableCount,
        instructionCount);

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(
        builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, instructionCount);

    jtk_Arrays_copyEx_b(instructions, instructionCount, 0, channel->m_bytes,
        channel->m_capacity, channel->m_index, instructionCount);
    channel->m_index += instructionCount;

    int32_t exceptionHandlerSiteCount = exceptionTable->m_size;
    zen_DataChannel_requestCapacity(channel, 2 + (exceptionHandlerSiteCount * 8));

    channel->m_bytes[channel->m_index++] = (exceptionHandlerSiteCount & 0x0000FF00) >> 8; // Exception Table Size
    channel->m_bytes[channel->m_index++] = (exceptionHandlerSiteCount & 0x000000FF);

    int32_t exceptionHandlerSiteIndex;
    for (exceptionHandlerSiteIndex = 0;
        exceptionHandlerSiteIndex < exceptionHandlerSiteCount;
        exceptionHandlerSiteIndex++) {
        zen_ExceptionHandlerSite_t* exceptionHandlerSite = (zen_ExceptionHandlerSite_t*)
            exceptionTable->m_exceptionHandlerSites[exceptionHandlerSiteIndex];

        channel->m_bytes[channel->m_index++] = (exceptionHandlerSite->m_startIndex & 0x0000FF00) >> 8; // Start Index
        channel->m_bytes[channel->m_index++] = (exceptionHandlerSite->m_startIndex & 0x000000FF);

        channel->m_bytes[channel->m_index++] = (exceptionHandlerSite->m_stopIndex & 0x0000FF00) >> 8; // Stop Index
        channel->m_bytes[channel->m_index++] = (exceptionHandlerSite->m_stopIndex & 0x000000FF);

        channel->m_bytes[channel->m_index++] = (exceptionHandlerSite->m_handlerIndex & 0x0000FF00) >> 8; // Handler Index
        channel->m_bytes[channel->m_index++] = (exceptionHandlerSite->m_handlerIndex & 0x000000FF);

        channel->m_bytes[channel->m_index++] = (exceptionHandlerSite->m_exceptionClassIndex & 0x0000FF00) >> 8; // Exception Class Index
        channel->m_bytes[channel->m_index++] = (exceptionHandlerSite->m_exceptionClassIndex & 0x000000FF);
    }
}

void zen_BinaryEntityBuilder_writeExceptionTableHeader(zen_BinaryEntityBuilder_t* builder, uint16_t size) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 2);

    channel->m_bytes[channel->m_index++] = (size & 0x0000FF00) >> 8; // Size
    channel->m_bytes[channel->m_index++] = (size & 0x000000FF);
}

// Instructions

/* NOP */

void zen_BinaryEntityBuilder_emitNop(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_NOP;
}

void zen_BinaryEntityBuilder_emitAddInteger(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_ADD_I;
}

void zen_BinaryEntityBuilder_emitAddLong(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_ADD_L;
}

void zen_BinaryEntityBuilder_emitAddFloat(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_ADD_F;
}

void zen_BinaryEntityBuilder_emitAddDouble(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_ADD_D;
}

void zen_BinaryEntityBuilder_emitAndInteger(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_AND_I;
}

void zen_BinaryEntityBuilder_emitAndLong(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_AND_L;
}

void zen_BinaryEntityBuilder_emitOrInteger(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_OR_I;
}

void zen_BinaryEntityBuilder_emitOrLong(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_OR_L;
}

/* Shift Left */

void zen_BinaryEntityBuilder_emitShiftLeftInteger(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_SHIFT_LEFT_I;
}

void zen_BinaryEntityBuilder_emitShiftLeftLong(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_SHIFT_LEFT_L;
}

/* Shift Right */

void zen_BinaryEntityBuilder_emitShiftRightInteger(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_SHIFT_RIGHT_I;
}

void zen_BinaryEntityBuilder_emitShiftRightLong(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_SHIFT_RIGHT_L;
}

void zen_BinaryEntityBuilder_emitShiftRightUnsignedInteger(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_SHIFT_RIGHT_UI;
}

void zen_BinaryEntityBuilder_emitShiftRightUnsignedLong(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_SHIFT_RIGHT_UL;
}

/* XOR */

void zen_BinaryEntityBuilder_emitXorInteger(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_XOR_I;
}

void zen_BinaryEntityBuilder_emitXorLong(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_XOR_L;
}

void zen_BinaryEntityBuilder_emitCastITB(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_CAST_ITB;
}

void zen_BinaryEntityBuilder_emitCastITS(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_CAST_ITS;
}

void zen_BinaryEntityBuilder_emitCastITL(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_CAST_ITL;
}

void zen_BinaryEntityBuilder_emitCastITF(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_CAST_ITF;
}

void zen_BinaryEntityBuilder_emitCastITD(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_CAST_ITD;
}

void zen_BinaryEntityBuilder_emitCastLTB(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_CAST_LTB;
}

void zen_BinaryEntityBuilder_emitCastLTS(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_CAST_LTS;
}

void zen_BinaryEntityBuilder_emitCastLTI(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_CAST_LTI;
}

void zen_BinaryEntityBuilder_emitCastLTF(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_CAST_LTF;
}

void zen_BinaryEntityBuilder_emitCastLTD(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_CAST_LTD;
}

void zen_BinaryEntityBuilder_emitCastFTI(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_CAST_FTI;
}

void zen_BinaryEntityBuilder_emitCastFTL(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_CAST_FTL;
}

void zen_BinaryEntityBuilder_emitCastFTD(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_CAST_FTD;
}

void zen_BinaryEntityBuilder_emitCastDTI(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_CAST_DTI;
}

void zen_BinaryEntityBuilder_emitCastDTL(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_CAST_DTL;
}

void zen_BinaryEntityBuilder_emitCastDTF(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_CAST_DTF;
}

void zen_BinaryEntityBuilder_emitCastITC(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_CAST_ITC;
}

void zen_BinaryEntityBuilder_emitCheckCast(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_CHECK_CAST;
}

void zen_BinaryEntityBuilder_emitCompareLong(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_COMPARE_L;
}

void zen_BinaryEntityBuilder_emitCompareLesserThanFloat(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_COMPARE_LT_F;
}

void zen_BinaryEntityBuilder_emitCompareGreaterThanFloat(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_COMPARE_GT_F;
}

void zen_BinaryEntityBuilder_emitCompareLesserThanDouble(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_COMPARE_LT_D;
}

void zen_BinaryEntityBuilder_emitCompareGreaterThanDouble(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_COMPARE_GT_D;
}

void zen_BinaryEntityBuilder_emitDivideInteger(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_DIVIDE_I;
}

void zen_BinaryEntityBuilder_emitDivideLong(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_DIVIDE_L;
}
void zen_BinaryEntityBuilder_emitDivideFloat(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_DIVIDE_F;
}

void zen_BinaryEntityBuilder_emitDivideDouble(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_DIVIDE_D;
}

void zen_BinaryEntityBuilder_emitDuplicate(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_DUPLICATE;
}

void zen_BinaryEntityBuilder_emitDuplicateX1(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_DUPLICATE_X1;
}

void zen_BinaryEntityBuilder_emitDuplicateX2(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_DUPLICATE_X2;
}

void zen_BinaryEntityBuilder_emitDuplicate2(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_DUPLICATE2;
}

void zen_BinaryEntityBuilder_emitDuplicate2X1(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_DUPLICATE2_X1;
}

void zen_BinaryEntityBuilder_emitDuplicate2X2(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_DUPLICATE2_X2;
}

void zen_BinaryEntityBuilder_emitJumpEqual0Integer(zen_BinaryEntityBuilder_t* builder, int16_t offset) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_JUMP_EQ0_I; // Byte Code
    channel->m_bytes[channel->m_index++] = (offset & 0x0000FF00) >> 8; // Offset
    channel->m_bytes[channel->m_index++] = (offset & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitJumpNotEqual0Integer(zen_BinaryEntityBuilder_t* builder, int16_t offset) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_JUMP_NE0_I; // Byte Code
    channel->m_bytes[channel->m_index++] = (offset & 0x0000FF00) >> 8; // Offset
    channel->m_bytes[channel->m_index++] = (offset & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitJumpLesserThan0Integer(zen_BinaryEntityBuilder_t* builder, int16_t offset) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_JUMP_LT0_I; // Byte Code
    channel->m_bytes[channel->m_index++] = (offset & 0x0000FF00) >> 8; // Offset
    channel->m_bytes[channel->m_index++] = (offset & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitJumpGreaterThan0Integer(zen_BinaryEntityBuilder_t* builder, int16_t offset) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_JUMP_GT0_I; // Byte Code
    channel->m_bytes[channel->m_index++] = (offset & 0x0000FF00) >> 8; // Offset
    channel->m_bytes[channel->m_index++] = (offset & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitJumpLesserThanOrEqualTo0Integer(zen_BinaryEntityBuilder_t* builder, int16_t offset) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_JUMP_LE0_I; // Byte Code
    channel->m_bytes[channel->m_index++] = (offset & 0x0000FF00) >> 8; // Offset
    channel->m_bytes[channel->m_index++] = (offset & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitJumpGreaterThanOrEqualTo0Integer(zen_BinaryEntityBuilder_t* builder, int16_t offset) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_JUMP_GE0_I; // Byte Code
    channel->m_bytes[channel->m_index++] = (offset & 0x0000FF00) >> 8; // Offset
    channel->m_bytes[channel->m_index++] = (offset & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitJumpEqualInteger(zen_BinaryEntityBuilder_t* builder, int16_t offset) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_JUMP_EQ_I; // Byte Code
    channel->m_bytes[channel->m_index++] = (offset & 0x0000FF00) >> 8; // Offset
    channel->m_bytes[channel->m_index++] = (offset & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitJumpNotEqualInteger(zen_BinaryEntityBuilder_t* builder, int16_t offset) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_JUMP_NE_I; // Byte Code
    channel->m_bytes[channel->m_index++] = (offset & 0x0000FF00) >> 8; // Offset
    channel->m_bytes[channel->m_index++] = (offset & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitJumpLesserThanInteger(zen_BinaryEntityBuilder_t* builder, int16_t offset) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_JUMP_LT_I; // Byte Code
    channel->m_bytes[channel->m_index++] = (offset & 0x0000FF00) >> 8; // Offset
    channel->m_bytes[channel->m_index++] = (offset & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitJumpGreaterThanInteger(zen_BinaryEntityBuilder_t* builder, int16_t offset) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_JUMP_GT_I; // Byte Code
    channel->m_bytes[channel->m_index++] = (offset & 0x0000FF00) >> 8; // Offset
    channel->m_bytes[channel->m_index++] = (offset & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitJumpLesserThanOrEqualInteger(zen_BinaryEntityBuilder_t* builder, int16_t offset) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_JUMP_LE_I; // Byte Code
    channel->m_bytes[channel->m_index++] = (offset & 0x0000FF00) >> 8; // Offset
    channel->m_bytes[channel->m_index++] = (offset & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitJumpGreaterThanOrEqualInteger(zen_BinaryEntityBuilder_t* builder, int16_t offset) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_JUMP_GE_I; // Byte Code
    channel->m_bytes[channel->m_index++] = (offset & 0x0000FF00) >> 8; // Offset
    channel->m_bytes[channel->m_index++] = (offset & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitJumpEqualReference(zen_BinaryEntityBuilder_t* builder, int16_t offset) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_JUMP_EQ_A; // Byte Code
    channel->m_bytes[channel->m_index++] = (offset & 0x0000FF00) >> 8; // Offset
    channel->m_bytes[channel->m_index++] = (offset & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitJumpNotEqualReference(zen_BinaryEntityBuilder_t* builder, int16_t offset) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_JUMP_NE_A; // Byte Code
    channel->m_bytes[channel->m_index++] = (offset & 0x0000FF00) >> 8; // Offset
    channel->m_bytes[channel->m_index++] = (offset & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitJumpEqualNullReference(zen_BinaryEntityBuilder_t* builder, int16_t offset) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_JUMP_EQN_A; // Byte Code
    channel->m_bytes[channel->m_index++] = (offset & 0x0000FF00) >> 8; // Offset
    channel->m_bytes[channel->m_index++] = (offset & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitJumpNotEqualNullReference(zen_BinaryEntityBuilder_t* builder, int16_t offset) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_JUMP_NEN_A; // Byte Code
    channel->m_bytes[channel->m_index++] = (offset & 0x0000FF00) >> 8; // Offset
    channel->m_bytes[channel->m_index++] = (offset & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitIncrementInteger(zen_BinaryEntityBuilder_t* builder, uint8_t index, uint8_t constant) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_INCREMENT_I; // Byte Code
    channel->m_bytes[channel->m_index++] = index; // Index
    channel->m_bytes[channel->m_index++] = constant; // Constant
}

void zen_BinaryEntityBuilder_emitWideIncrementInteger(zen_BinaryEntityBuilder_t* builder, uint16_t index, uint16_t constant) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 6);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_WIDE; // Byte Code
    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_INCREMENT_I; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
    channel->m_bytes[channel->m_index++] = (constant & 0x0000FF00) >> 8; // Constant
    channel->m_bytes[channel->m_index++] = (constant & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitInvokeSpecial(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_INVOKE_SPECIAL; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitInvokeVirtual(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_INVOKE_VIRTUAL; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitInvokeDynamic(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_INVOKE_DYNAMIC; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitInvokeStatic(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_INVOKE_STATIC; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

/* Jump */

void zen_BinaryEntityBuilder_emitJump(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_JUMP; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

/* Load */

void zen_BinaryEntityBuilder_emitLoadInteger(zen_BinaryEntityBuilder_t* builder, uint8_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 2);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_I; // Byte Code
    channel->m_bytes[channel->m_index++] = index;
}

void zen_BinaryEntityBuilder_emitWideLoadInteger(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 4);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_WIDE; // Byte Code
    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_I; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitLoadLong(zen_BinaryEntityBuilder_t* builder, uint8_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 2);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_L; // Byte Code
    channel->m_bytes[channel->m_index++] = index;
}

void zen_BinaryEntityBuilder_emitWideLoadLong(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 4);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_WIDE; // Byte Code
    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_L; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitLoadFloat(zen_BinaryEntityBuilder_t* builder, uint8_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 2);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_F; // Byte Code
    channel->m_bytes[channel->m_index++] = index;
}

void zen_BinaryEntityBuilder_emitWideLoadFloat(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 4);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_WIDE; // Byte Code
    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_F; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitLoadDouble(zen_BinaryEntityBuilder_t* builder, uint8_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 2);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_D; // Byte Code
    channel->m_bytes[channel->m_index++] = index;
}

void zen_BinaryEntityBuilder_emitWideLoadDouble(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 4);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_WIDE; // Byte Code
    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_D; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitLoadReference(zen_BinaryEntityBuilder_t* builder, uint8_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 2);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_A; // Byte Code
    channel->m_bytes[channel->m_index++] = index;
}

void zen_BinaryEntityBuilder_emitWideLoadReference(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 4);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_WIDE; // Byte Code
    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_A; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitLoad0Integer(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_I0; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoad1Integer(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_I1; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoad2Integer(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_I2; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoad3Integer(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_I3; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoad0Long(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_L0; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoad1Long(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_L1; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoad2Long(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_L2; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoad3Long(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_L3; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoad0Float(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_F0; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoad1Float(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_F1; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoad2Float(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_F2; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoad3Float(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_F3; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoad0Double(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_D0; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoad1Double(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_D1; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoad2Double(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_D2; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoad3Double(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_D3; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoad0Reference(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_A0; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoad1Reference(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_A1; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoad2Reference(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_A2; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoad3Reference(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_A3; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoadArrayByte(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_AB; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoadArrayCharacter(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_AC; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoadArrayShort(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_AS; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoadArrayInteger(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_AI; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoadArrayLong(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_AL; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoadArrayFloat(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_AF; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoadArrayDouble(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_AD; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoadArrayReference(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_AA; // Byte Code
}

void zen_BinaryEntityBuilder_emitLoadInstanceField(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_INSTANCE_FIELD; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitLoadStaticField(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_STATIC_FIELD; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitLoadCPR(zen_BinaryEntityBuilder_t* builder, uint8_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 2);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_CPR; // Byte Code
    channel->m_bytes[channel->m_index++] = index; // Index
}

void zen_BinaryEntityBuilder_emitWideLoadCPR(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 4);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_WIDE; // Byte Code
    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_CPR; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitLoadArraySize(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_LOAD_ARRAY_SIZE; // Byte Code
}

/* Modulo */

void zen_BinaryEntityBuilder_emitModuloInteger(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_MODULO_I; // Byte Code
    zen_DataChannel_requestCapacity(channel, 1);
}

void zen_BinaryEntityBuilder_emitModuloLong(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_MODULO_L; // Byte Code
}

void zen_BinaryEntityBuilder_emitModuloFloat(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_MODULO_F; // Byte Code
}

void zen_BinaryEntityBuilder_emitModuloDouble(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_MODULO_D; // Byte Code
}

/* Multiply */

void zen_BinaryEntityBuilder_emitMultiplyInteger(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_MULTIPLY_I; // Byte Code
}

void zen_BinaryEntityBuilder_emitMultiplyLong(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_MULTIPLY_L; // Byte Code
}

void zen_BinaryEntityBuilder_emitMultiplyFloat(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_MULTIPLY_F; // Byte Code
}

void zen_BinaryEntityBuilder_emitMultiplyDouble(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_MULTIPLY_D; // Byte Code
}

/* Negate */

void zen_BinaryEntityBuilder_emitNegateInteger(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_NEGATE_I; // Byte Code
}

void zen_BinaryEntityBuilder_emitNegateLong(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_NEGATE_L; // Byte Code
}

void zen_BinaryEntityBuilder_emitNegateFloat(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_NEGATE_F; // Byte Code
}

void zen_BinaryEntityBuilder_emitNegateDouble(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_NEGATE_D; // Byte Code
}

/* New */

void zen_BinaryEntityBuilder_emitNew(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_NEW; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitNewArray(zen_BinaryEntityBuilder_t* builder, uint8_t type) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_NEW; // Byte Code
    channel->m_bytes[channel->m_index++] = (type & 0x0000FF00) >> 8; // Type
    channel->m_bytes[channel->m_index++] = (type & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitNewReferenceArray(zen_BinaryEntityBuilder_t* builder,
    uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_NEW_ARRAY_A; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitNewDimensionalArray(zen_BinaryEntityBuilder_t* builder, uint16_t index,
    uint8_t dimensions) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 4);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_NEW_ARRAY_A; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
    channel->m_bytes[channel->m_index++] = dimensions; // Dimensions
}

/* Pop */

void zen_BinaryEntityBuilder_emitPop(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_POP; // Byte Code
}

void zen_BinaryEntityBuilder_emitPop2(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_POP2; // Byte Code
}

/* Push */

void zen_BinaryEntityBuilder_emitPushNull(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_PUSH_NULL; // Byte Code
}

void zen_BinaryEntityBuilder_emitPushIntegerNegative1(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_PUSH_IN1; // Byte Code
}

void zen_BinaryEntityBuilder_emitPushInteger0(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_PUSH_I0; // Byte Code
}

void zen_BinaryEntityBuilder_emitPushInteger1(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_PUSH_I1; // Byte Code
}

void zen_BinaryEntityBuilder_emitPushInteger2(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_PUSH_I2; // Byte Code
}

void zen_BinaryEntityBuilder_emitPushInteger3(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_PUSH_I3; // Byte Code
}

void zen_BinaryEntityBuilder_emitPushInteger4(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_PUSH_I4; // Byte Code
}

void zen_BinaryEntityBuilder_emitPushInteger5(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_PUSH_I5; // Byte Code
}

void zen_BinaryEntityBuilder_emitPushLong0(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_PUSH_L0; // Byte Code
}

void zen_BinaryEntityBuilder_emitPushLong1(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_PUSH_L1; // Byte Code
}

void zen_BinaryEntityBuilder_emitPushLong2(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_PUSH_L2; // Byte Code
}

void zen_BinaryEntityBuilder_emitPushFloat0(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_PUSH_F0; // Byte Code
}

void zen_BinaryEntityBuilder_emitPushFloat1(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_PUSH_F1; // Byte Code
}

void zen_BinaryEntityBuilder_emitPushFloat2(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_PUSH_F2; // Byte Code
}

void zen_BinaryEntityBuilder_emitPushDouble0(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_PUSH_D0; // Byte Code
}

void zen_BinaryEntityBuilder_emitPushDouble1(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_PUSH_D1; // Byte Code
}

void zen_BinaryEntityBuilder_emitPushDouble2(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_PUSH_D2; // Byte Code
}

void zen_BinaryEntityBuilder_emitPushByte(zen_BinaryEntityBuilder_t* builder, int8_t value) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 2);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_PUSH_B; // Byte Code
    channel->m_bytes[channel->m_index++] = value;
}

void zen_BinaryEntityBuilder_emitPushShort(zen_BinaryEntityBuilder_t* builder, int16_t value) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_PUSH_S; // Byte Code
    channel->m_bytes[channel->m_index++] = (value & 0x0000FF00) >> 8; // Value
    channel->m_bytes[channel->m_index++] = (value & 0x000000FF);
}

/* Return */

void zen_BinaryEntityBuilder_emitReturn(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_RETURN; // Byte Code
}

void zen_BinaryEntityBuilder_emitReturnInteger(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_RETURN_I; // Byte Code
}

void zen_BinaryEntityBuilder_emitReturnLong(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_RETURN_L; // Byte Code
}

void zen_BinaryEntityBuilder_emitReturnFloat(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_RETURN_F; // Byte Code
}

void zen_BinaryEntityBuilder_emitReturnDouble(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_RETURN_D; // Byte Code
}

void zen_BinaryEntityBuilder_emitReturnReference(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_RETURN_A; // Byte Code
}

/* RTTI */

/* Store */

void zen_BinaryEntityBuilder_emitStoreInteger(zen_BinaryEntityBuilder_t* builder,
    uint8_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 2);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_I; // Byte Code
    channel->m_bytes[channel->m_index++] = index; // Index
}

void zen_BinaryEntityBuilder_emitWideStoreInteger(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_I; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitStoreInteger0(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_I0; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreInteger1(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_I1; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreInteger2(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_I2; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreInteger3(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_I3; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreLong(zen_BinaryEntityBuilder_t* builder, uint8_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 2);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_L; // Byte Code
    channel->m_bytes[channel->m_index++] = index; // Index
}

void zen_BinaryEntityBuilder_emitWideStoreLong(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_L; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitStoreLong0(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_L0; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreLong1(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_L1; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreLong2(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_L2; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreLong3(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_L3; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreFloat(zen_BinaryEntityBuilder_t* builder, uint8_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 2);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_F; // Byte Code
    channel->m_bytes[channel->m_index++] = index; // Index
}

void zen_BinaryEntityBuilder_emitWideStoreFloat(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_F; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitStoreFloat0(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_F0; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreFloat1(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_F1; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreFloat2(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_F2; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreFloat3(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_F3; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreDouble(zen_BinaryEntityBuilder_t* builder, uint8_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 2);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_D; // Byte Code
    channel->m_bytes[channel->m_index++] = index; // Index
}

void zen_BinaryEntityBuilder_emitWideStoreDouble(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_D; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitStoreDouble0(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_D0; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreDouble1(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_D1; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreDouble2(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_D2; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreDouble3(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_D3; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreReference(zen_BinaryEntityBuilder_t* builder, uint8_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 2);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_A; // Byte Code
    channel->m_bytes[channel->m_index++] = index; // Index
}

void zen_BinaryEntityBuilder_emitWideStoreReference(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_A; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitStoreReference0(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_A0; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreReference1(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_A1; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreReference2(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_A2; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreReference3(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_A3; // Byte Code
}

/* Store Array */

void zen_BinaryEntityBuilder_emitStoreArrayByte(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_AB; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreArrayCharacter(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_AC; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreArrayShort(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_AS; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreArrayInteger(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_AI; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreArrayLong(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_AL; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreArrayFloat(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_AF; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreArrayDouble(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_AD; // Byte Code
}

void zen_BinaryEntityBuilder_emitStoreArrayReference(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_AA; // Byte Code
}

/* Store Field */

void zen_BinaryEntityBuilder_emitStoreInstanceField(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_INSTANCE_FIELD; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

void zen_BinaryEntityBuilder_emitStoreStaticField(zen_BinaryEntityBuilder_t* builder, uint16_t index) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 3);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_STORE_STATIC_FIELD; // Byte Code
    channel->m_bytes[channel->m_index++] = (index & 0x0000FF00) >> 8; // Index
    channel->m_bytes[channel->m_index++] = (index & 0x000000FF);
}

/* Subtract */

void zen_BinaryEntityBuilder_emitSubtractInteger(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_SUBTRACT_I; // Byte Code
}

void zen_BinaryEntityBuilder_emitSubtractLong(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_SUBTRACT_L; // Byte Code
}

void zen_BinaryEntityBuilder_emitSubtractFloat(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_SUBTRACT_F; // Byte Code
}

void zen_BinaryEntityBuilder_emitSubtractDouble(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_SUBTRACT_D; // Byte Code
}

/* Swap */

void zen_BinaryEntityBuilder_emitSwap(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_SWAP; // Byte Code
}

/* Throw */

void zen_BinaryEntityBuilder_emitThrow(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_THROW; // Byte Code
}

/* Wide */

void zen_BinaryEntityBuilder_emitWide(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");
    jtk_Assert_assertTrue(builder->m_activeChannelIndex >= 0, "Please activate a channel before emitting instructions.");

    zen_DataChannel_t* channel = (zen_DataChannel_t*)jtk_ArrayList_getValue(builder->m_channels, builder->m_activeChannelIndex);
    zen_DataChannel_requestCapacity(channel, 1);

    channel->m_bytes[channel->m_index++] = ZEN_BYTE_CODE_WIDE; // Byte Code
}