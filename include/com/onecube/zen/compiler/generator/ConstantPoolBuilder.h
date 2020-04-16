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

// Monday, October 07, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_GENERATOR_CONSTANT_POOL_BUILDER_H
#define COM_ONECUBE_ZEN_COMPILER_GENERATOR_CONSTANT_POOL_BUILDER_H

#include <jtk/collection/list/ArrayList.h>

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
 * ConstantPoolBuilder                                                         *
 *******************************************************************************/

/**
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ConstantPoolBuilder_t {
    jtk_ArrayList_t* m_entries;
};

/**
 * @memberof ConstantPoolBuilder
 */
typedef struct zen_ConstantPoolBuilder_t zen_ConstantPoolBuilder_t;

// Constructor

/**
 * @memberof ConstantPoolBuilder
 */
zen_ConstantPoolBuilder_t* zen_ConstantPoolBuilder_new();

// Destructor

/**
 * @memberof ConstantPoolBuilder
 */
void zen_ConstantPoolBuilder_delete(zen_ConstantPoolBuilder_t* builder);

// Clear

void zen_ConstantPoolBuilder_clear(zen_ConstantPoolBuilder_t* builder);

// Reset

void zen_ConstantPoolBuilder_reset(zen_ConstantPoolBuilder_t* builder);

// Entries

/**
 * @memberof ConstantPoolBuilder
 */
int32_t zen_ConstantPoolBuilder_countEntries(zen_ConstantPoolBuilder_t* builder);

/**
 * @memberof ConstantPoolBuilder
 */
zen_ConstantPoolEntry_t* zen_ConstantPoolBuilder_getEntry(zen_ConstantPoolBuilder_t* builder,
    int32_t index);

// Class Entry

zen_ConstantPoolClass_t* zen_ConstantPoolBuilder_getClassEntry(
    zen_ConstantPoolBuilder_t* builder, int32_t index);

int32_t zen_ConstantPoolBuilder_getClassEntryIndexEx(
    zen_ConstantPoolBuilder_t* builder, uint8_t* bytes, int32_t bytesSize);

// Field Entry

zen_ConstantPoolField_t* zen_ConstantPoolBuilder_getFieldEntry(
    zen_ConstantPoolBuilder_t* builder, int32_t index);

int32_t zen_ConstantPoolBuilder_getFieldEntryIndexEx(
    zen_ConstantPoolBuilder_t* builder, const uint8_t* class0, int32_t classSize,
    const uint8_t* descriptor, int32_t descriptorSize, const uint8_t* name,
    int32_t nameSize);

// Function Entry

zen_ConstantPoolFunction_t* zen_ConstantPoolBuilder_getFunctionEntry(
    zen_ConstantPoolBuilder_t* builder, int32_t index);

int32_t zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
    zen_ConstantPoolBuilder_t* builder, const uint8_t* class0, int32_t classSize,
    const uint8_t* descriptor, int32_t descriptorSize, const uint8_t* name,
    int32_t nameSize, uint16_t tableIndex);

// Integer Entry

zen_ConstantPoolInteger_t* zen_ConstantPoolBuilder_getIntegerEntry(
    zen_ConstantPoolBuilder_t* builder, int32_t index);

int32_t zen_ConstantPoolBuilder_getIntegerEntryIndex(
    zen_ConstantPoolBuilder_t* builder, int32_t value);

// String Entry

zen_ConstantPoolString_t* zen_ConstantPoolBuilder_getStringEntry(
    zen_ConstantPoolBuilder_t* builder, int32_t index);

int32_t zen_ConstantPoolBuilder_getStringEntryIndexEx(
    zen_ConstantPoolBuilder_t* builder, uint8_t* bytes, int32_t bytesSize);

// UTF-8 Entry

zen_ConstantPoolUtf8_t* zen_ConstantPoolBuilder_getUtf8Entry(
    zen_ConstantPoolBuilder_t* builder, int32_t index);

// Long Entry

zen_ConstantPoolLong_t* zen_ConstantPoolBuilder_getLongEntry(
    zen_ConstantPoolBuilder_t* builder, int32_t index);

int32_t zen_ConstantPoolBuilder_getLongEntryIndex(
    zen_ConstantPoolBuilder_t* builder, int64_t value);

int32_t zen_ConstantPoolBuilder_getUtf8EntryIndexEx(
    zen_ConstantPoolBuilder_t* builder, uint8_t* bytes, int32_t bytesSize);

#endif /* COM_ONECUBE_ZEN_COMPILER_GENERATOR_CONSTANT_POOL_BUILDER_H */