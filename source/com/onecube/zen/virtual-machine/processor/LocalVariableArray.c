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

// Sunday, September 30, 2018

#include <jtk/core/Assert.h>
#include <com/onecube/zen/virtual-machine/processor/LocalVariableArray.h>

/*******************************************************************************
 * LocalVariableArray                                                          *
 *******************************************************************************/

#define ZEN_LOCAL_VARIABLE_ARRAY_REFERENCE_SLOT_COUNT_1 1
#define ZEN_LOCAL_VARIABLE_ARRAY_REFERENCE_SLOT_COUNT_2 2
#define ZEN_LOCAL_VARIABLE_ARRAY_REFERENCE_SLOT_COUNT (sizeof (void*) <= 4)? \
    ZEN_LOCAL_VARIABLE_ARRAY_REFERENCE_SLOT_COUNT_1 : ((sizeof (void*) <= 8)? \
    ZEN_LOCAL_VARIABLE_ARRAY_REFERENCE_SLOT_COUNT_2 : zen_LocalVariableArray_referenceSlotCountError())

int32_t zen_LocalVariableArray_referenceSlotCountError() {
    fprintf(stderr, "[internal error] Pointer type occupies more than 8 bytes. A reference cannot occupy more than two slots in the local variable array.\n");
    exit(0);

    return -1;
}

/* The local variable array realises all its entries as 32-bit patterns.
 * Therefore, each entry is represented as `uint32_t`.
 */

/* Constructor */

/* NOTE: The virtual machine does not initialize the local variables. It is left
 * to the compiler to force initialization of local variables.
 */
zen_LocalVariableArray_t* zen_LocalVariableArray_new(int32_t size) {
    zen_LocalVariableArray_t* localVariableArray = jtk_Memory_allocate(zen_LocalVariableArray_t, 1);
    localVariableArray->m_values = (size > 0)? jtk_Memory_allocate(uint32_t, size) : NULL;
    localVariableArray->m_size = size;

    return localVariableArray;
}

/* Destructor */
void zen_LocalVariableArray_delete(zen_LocalVariableArray_t* localVariableArray) {
    jtk_Assert_assertObject(localVariableArray, "The specified local variable array is null.");

    // zen_MemoryManager_deallocate(memoryManager, localVariableArray->m_values);
    // zen_MemoryManager_deallocate(memoryManager, localVariableArray);
}

/* Double */

/* The entity verifier checks whether indexes into the local variable array
 * is valid, or not.
 */

double zen_LocalVariableArray_getDouble(zen_LocalVariableArray_t* localVariableArray,
    int32_t index) {
    jtk_Assert_assertObject(localVariableArray, "The specified local variable array is null.");
    jtk_Assert_assertTrue((index >= 0) && (index < localVariableArray->m_size), "The specified index (high part) is invalid.");
    jtk_Assert_assertTrue((index + 1 >= 0) && (index + 1 < localVariableArray->m_size), "The specified index (low part) is invalid.");

    uint32_t highPart = localVariableArray->m_values[index];
    uint32_t lowPart = localVariableArray->m_values[index + 1];
    uint64_t result = (((uint64_t)highPart << 32) | lowPart);

    return *((double*)(&result));
}

void zen_LocalVariableArray_setDouble(zen_LocalVariableArray_t* localVariableArray,
    int32_t index, double value) {
    jtk_Assert_assertObject(localVariableArray, "The specified local variable array is null.");
    jtk_Assert_assertTrue((index >= 0) && (index < localVariableArray->m_size), "The specified index (high part) is invalid.");
    jtk_Assert_assertTrue((index + 1 >= 0) && (index + 1 < localVariableArray->m_size), "The specified index (low part) is invalid.");

    uint64_t value0 = *((int64_t*)(&value));
    uint32_t highPart = (uint32_t)((value0 & 0xFFFFFFFF00000000L) >> 32);
    uint32_t lowPart = (uint32_t)(value0 & 0xFFFFFFFFL);

    localVariableArray->m_values[index] = highPart;
    localVariableArray->m_values[index + 1] = lowPart;
}

/* Float */

float zen_LocalVariableArray_getFloat(zen_LocalVariableArray_t* localVariableArray,
    int32_t index) {
    jtk_Assert_assertObject(localVariableArray, "The specified local variable array is null.");
    jtk_Assert_assertTrue((index >= 0) && (index < localVariableArray->m_size), "The specified index is invalid.");

    uint32_t result = localVariableArray->m_values[index];

    return *((float*)(&result));
}

void zen_LocalVariableArray_setFloat(zen_LocalVariableArray_t* localVariableArray,
    int32_t index, float value) {
    jtk_Assert_assertObject(localVariableArray, "The specified local variable array is null.");
    jtk_Assert_assertTrue((index >= 0) && (index < localVariableArray->m_size), "The specified index (high part) is invalid.");

    localVariableArray->m_values[index] = *((uint32_t*)(&value));
}

/* Integer */

int32_t zen_LocalVariableArray_getInteger(zen_LocalVariableArray_t* localVariableArray,
    int32_t index) {
    jtk_Assert_assertObject(localVariableArray, "The specified local variable array is null.");
    jtk_Assert_assertTrue((index >= 0) && (index < localVariableArray->m_size), "The specified index is invalid.");

    return (int32_t)localVariableArray->m_values[index];
}

void zen_LocalVariableArray_setInteger(zen_LocalVariableArray_t* localVariableArray,
    int32_t index, int32_t value) {
    jtk_Assert_assertObject(localVariableArray, "The specified local variable array is null.");
    jtk_Assert_assertTrue((index >= 0) && (index < localVariableArray->m_size), "The specified index is invalid.");

    localVariableArray->m_values[index] = value;
}

/* Long */

int64_t zen_LocalVariableArray_getLong(zen_LocalVariableArray_t* localVariableArray,
    int32_t index) {
    jtk_Assert_assertObject(localVariableArray, "The specified local variable array is null.");
    jtk_Assert_assertTrue((index >= 0) && (index < localVariableArray->m_size), "The specified index (high part) is invalid.");
    jtk_Assert_assertTrue((index + 1 >= 0) && (index + 1 < localVariableArray->m_size), "The specified index (low part) is invalid.");

    uint32_t highPart = localVariableArray->m_values[index];
    uint32_t lowPart = localVariableArray->m_values[index + 1];

    return (((uint64_t)highPart << 32) | lowPart);
}

void zen_LocalVariableArray_setLong(zen_LocalVariableArray_t* localVariableArray,
    int32_t index, int64_t value) {
    jtk_Assert_assertObject(localVariableArray, "The specified local variable array is null.");
    jtk_Assert_assertTrue((index >= 0) && (index < localVariableArray->m_size), "The specified index (high part) is invalid.");
    jtk_Assert_assertTrue((index + 1 >= 0) && (index + 1 < localVariableArray->m_size), "The specified index (low part) is invalid.");

    uint32_t highPart = (uint32_t)((value & 0xFFFFFFFF00000000L) >> 32);
    uint32_t lowPart = (uint32_t)(value & 0xFFFFFFFFL);

    localVariableArray->m_values[index] = highPart;
    localVariableArray->m_values[index + 1] = lowPart;
}

/* Reference */

uintptr_t zen_LocalVariableArray_getReference(zen_LocalVariableArray_t* localVariableArray,
    int32_t index) {
    jtk_Assert_assertObject(localVariableArray, "The specified local variable array is null.");

    uintptr_t result = 0;
    /* A null reference is represented with the integer value of 0. */
    switch (ZEN_LOCAL_VARIABLE_ARRAY_REFERENCE_SLOT_COUNT) {
        case ZEN_LOCAL_VARIABLE_ARRAY_REFERENCE_SLOT_COUNT_1: {
            jtk_Assert_assertTrue((index >= 0) && (index < localVariableArray->m_size), "The specified index is invalid.");

            result = (uintptr_t)localVariableArray->m_values[index];

            break;
        }

        case ZEN_LOCAL_VARIABLE_ARRAY_REFERENCE_SLOT_COUNT_2: {
            jtk_Assert_assertTrue((index >= 0) && (index < localVariableArray->m_size), "The specified index (high part) is invalid.");
            jtk_Assert_assertTrue((index + 1 >= 0) && (index + 1 < localVariableArray->m_size), "The specified index (low part) is invalid.");

            uint32_t highPart = localVariableArray->m_values[index];
            uint32_t lowPart = localVariableArray->m_values[index + 1];
            result = (uintptr_t)(((uint64_t)highPart << 32) | lowPart);

            break;
        }
    }

    return result;
}

/* NOTE: This function is implemented with the assumption that the size of
 *       a given platform is lesser than or equal to eight bytes.
 */
void zen_LocalVariableArray_setReference(zen_LocalVariableArray_t* localVariableArray,
    int32_t index, uintptr_t reference) {
    jtk_Assert_assertObject(localVariableArray, "The specified local variable array is null.");

    /* A null reference is represented with the integer value of 0. */
    switch (ZEN_LOCAL_VARIABLE_ARRAY_REFERENCE_SLOT_COUNT) {
        case ZEN_LOCAL_VARIABLE_ARRAY_REFERENCE_SLOT_COUNT_1: {
            jtk_Assert_assertTrue((index >= 0) && (index < localVariableArray->m_size), "The specified index is invalid.");

            localVariableArray->m_values[index] = (uint32_t)reference;

            break;
        }

        case ZEN_LOCAL_VARIABLE_ARRAY_REFERENCE_SLOT_COUNT_2: {
            jtk_Assert_assertTrue((index >= 0) && (index < localVariableArray->m_size), "The specified index (high part) is invalid.");
            jtk_Assert_assertTrue((index + 1 >= 0) && (index + 1 < localVariableArray->m_size), "The specified index (low part) is invalid.");

            localVariableArray->m_values[index] = (uint32_t)(((uint64_t)reference & 0xFFFFFFFF00000000L) >> 32);
            localVariableArray->m_values[index + 1] = (uint32_t)((uint64_t)reference & 0xFFFFFFFFL);

            break;
        }
    }
}

/* Size */

int32_t zen_LocalVariableArray_getSize(zen_LocalVariableArray_t* localVariableArray) {
    jtk_Assert_assertObject(localVariableArray, "The specified local variable array is null.");

    return localVariableArray->m_size;
}