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

// Monday, September 17, 2018

#include <jtk/core/Assert.h>

#include <com/onecube/zen/virtual-machine/processor/OperandStack.h>

#define ZEN_OPERAND_STACK_REFERENCE_SLOT_COUNT_1 1
#define ZEN_OPERAND_STACK_REFERENCE_SLOT_COUNT_2 2
#define ZEN_OPERAND_STACK_REFERENCE_SLOT_COUNT (sizeof (void*) <= 4)? \
    ZEN_OPERAND_STACK_REFERENCE_SLOT_COUNT_1 : ((sizeof (void*) <= 8)? \
    ZEN_OPERAND_STACK_REFERENCE_SLOT_COUNT_2 : zen_OperandStack_referenceSlotCountError())

int32_t zen_OperandStack_referenceSlotCountError() {
    fprintf(stderr, "[internal error] Pointer type occupies more than 8 bytes. A reference cannot occupy more than two slots in the operand stack.\n");
    exit(0);

    return -1;
}

/*******************************************************************************
 * OperandStack                                                                *
 *******************************************************************************/

/* Constructor */

zen_OperandStack_t* zen_OperandStack_new(int32_t capacity) {
    zen_OperandStack_t* stack = jtk_Memory_allocate(zen_OperandStack_t, 1);
    stack->m_values = (capacity > 0)? jtk_Memory_allocate(uint32_t, capacity) : NULL;
    stack->m_size = 0;
    stack->m_capacity = capacity;

    return stack;
}

/* Destructor */

void zen_OperandStack_delete(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    // zen_MemoryManager_deallocate(manager, stack->m_values);
    // zen_MemoryManager_deallocate(manager, stack);

    jtk_Memory_deallocate(stack->m_values);
    jtk_Memory_deallocate(stack);
}

/* Available */

bool zen_OperandStack_isAvailable(zen_OperandStack_t* stack, int32_t required) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    return (stack->m_size >= required);
}

/* Capacity */

int32_t zen_OperandStack_getCapacity(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    return stack->m_capacity;
}

/* Duplicate */

void zen_OperandStack_duplicate(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");
}

void zen_OperandStack_duplicateX1(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");
}

void zen_OperandStack_duplicateX2(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");
}

/* Duplicate 2 */

void zen_OperandStack_duplicate2(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");
}

void zen_OperandStack_duplicate2X1(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");
}

void zen_OperandStack_duplicate2X2(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");
}

/* Empty */

bool zen_OperandStack_isEmpty(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    return stack->m_size == 0;
}

/* Peek */

double zen_OperandStack_peekDouble(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    /* Check whether the operand stack has, at least, 2 words.
     *
     * TODO: If there is a shortage, an exception should be raised.
     */
    jtk_Assert_assertTrue(zen_OperandStack_isAvailable(stack, 2), "Operand stack underflow");

    int32_t lowPart = stack->m_values[stack->m_size - 1];
    int32_t highPart = stack->m_values[stack->m_size - 2];
    int64_t result = (((uint32_t)highPart << 32) | lowPart);

    return *((double*)(&result));
}

float zen_OperandStack_peekFloat(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    /* Check whether the operand stack has, at least, 1 word.
     *
     * TODO: If there is a shortage, an exception should be raised.
     */
    jtk_Assert_assertTrue(zen_OperandStack_isAvailable(stack, 1), "Operand stack underflow");

    uint32_t result = stack->m_values[stack->m_size - 1];

    return *((float*)(&result));
}

int32_t zen_OperandStack_peekInteger(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    /* Check whether the operand stack has, at least, 1 word.
     *
     * TODO: If there is a shortage, an exception should be raised.
     */
    jtk_Assert_assertTrue(zen_OperandStack_isAvailable(stack, 1), "Operand stack underflow");

    return stack->m_values[stack->m_size - 1];
}

int64_t zen_OperandStack_peekLong(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    /* Check whether the operand stack has, at least, 2 words.
     *
     * TODO: If there is a shortage, an exception should be raised.
     */
    jtk_Assert_assertTrue(zen_OperandStack_isAvailable(stack, 2), "Operand stack underflow");

    uint32_t lowPart = stack->m_values[stack->m_size - 1];
    uint32_t highPart = stack->m_values[stack->m_size - 2];

    return (((uint64_t)highPart << 32) | lowPart);
}

uintptr_t zen_OperandStack_peekReference(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    /* Check whether the operand stack has, at least, 2 words.
     *
     * TODO: If there is a shortage, an exception should be raised.
     */
    jtk_Assert_assertTrue(zen_OperandStack_isAvailable(stack,
        ZEN_OPERAND_STACK_REFERENCE_SLOT_COUNT), "Operand stack underflow");

    uintptr_t result = 0;
        /* A null reference is represented with the integer value of 0. */
    switch (ZEN_OPERAND_STACK_REFERENCE_SLOT_COUNT) {
        case ZEN_OPERAND_STACK_REFERENCE_SLOT_COUNT_1: {
            result = (uintptr_t)stack->m_values[stack->m_size - 1];

            break;
        }

        case ZEN_OPERAND_STACK_REFERENCE_SLOT_COUNT_2: {
            uint32_t lowPart = stack->m_values[stack->m_size - 1];
            uint32_t highPart = stack->m_values[stack->m_size - 2];
            result = (uintptr_t)(((uint64_t)highPart << 32) | lowPart);

            break;
        }
    }

    return result;
}

// Push

void zen_OperandStack_pushDouble(zen_OperandStack_t* stack, double value) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    /* Check whether the operand stack has, at least, 2 words.
     *
     * TODO: If there is a shortage, an exception should be raised.
     */
    jtk_Assert_assertTrue(zen_OperandStack_isSpaceAvailable(stack, 2), "Operand stack overflow");

    int64_t value0 = *((int64_t*)(&value));
    int32_t highPart = (uint32_t)((value0 & 0xFFFFFFFF00000000L) >> 32);
    int32_t lowPart = (uint32_t)(value0 & 0xFFFFFFFFL);

    stack->m_values[stack->m_size++] = highPart;
    stack->m_values[stack->m_size++] = lowPart;
}

void zen_OperandStack_pushFloat(zen_OperandStack_t* stack, float value) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    /* Check whether the operand stack has, at least, 1 word.
     *
     * TODO: If there is a shortage, an exception should be raised.
     */
    jtk_Assert_assertTrue(zen_OperandStack_isSpaceAvailable(stack, 1), "Operand stack overflow");

    stack->m_values[stack->m_size++] = *((uint32_t*)(&value));
}

void zen_OperandStack_pushInteger(zen_OperandStack_t* stack, int32_t value) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    /* Check whether the operand stack has, at least, 1 word.
     *
     * TODO: If there is a shortage, an exception should be raised.
     */
    jtk_Assert_assertTrue(zen_OperandStack_isSpaceAvailable(stack, 1), "Operand stack overflow");

    stack->m_values[stack->m_size++] = value;
}

void zen_OperandStack_pushLong(zen_OperandStack_t* stack, int64_t value) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    /* Check whether the operand stack has, at least, 2 words.
     *
     * TODO: If there is a shortage, an exception should be raised.
     */
    jtk_Assert_assertTrue(zen_OperandStack_isSpaceAvailable(stack, 2), "Operand stack overflow");

    int32_t highPart = (int32_t)((value & 0xFFFFFFFF00000000L) >> 32);
    int32_t lowPart = (int32_t)(value & 0xFFFFFFFFL);

    stack->m_values[stack->m_size++] = highPart;
    stack->m_values[stack->m_size++] = lowPart;
}

void zen_OperandStack_pushNullReference(zen_OperandStack_t* stack) {
    zen_OperandStack_pushReference(stack, 0);
}

/* NOTE: This function is implemented with the assumption that the size of
 *       a given platform is lesser than or equal to eight bytes.
 */
void zen_OperandStack_pushReference(zen_OperandStack_t* stack, uintptr_t reference) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    /* Check whether the operand stack has, at least, ZEN_OPERAND_STACK_REFERENCE_SLOT_COUNT
     * words.
     *
     * TODO: If there is a shortage, an exception should be raised.
     */
    jtk_Assert_assertTrue(zen_OperandStack_isSpaceAvailable(stack,
        ZEN_OPERAND_STACK_REFERENCE_SLOT_COUNT), "Operand stack overflow");

    /* A null reference is represented with the integer value of 0. */
    switch (ZEN_OPERAND_STACK_REFERENCE_SLOT_COUNT) {
        case ZEN_OPERAND_STACK_REFERENCE_SLOT_COUNT_1: {
            stack->m_values[stack->m_size++] = (uint32_t)reference;

            break;
        }

        case ZEN_OPERAND_STACK_REFERENCE_SLOT_COUNT_2: {
            stack->m_values[stack->m_size++] = (uint32_t)(((uint64_t)reference & 0xFFFFFFFF00000000L) >> 32);
            stack->m_values[stack->m_size++] = (uint32_t)((uint64_t)reference & 0xFFFFFFFFL);

            break;
        }
    }
}

// Pop

int32_t zen_OperandStack_pop(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    /* Check whether the operand stack has, at least, 1 word.
     *
     * TODO: If there is a shortage, an exception should be raised.
     */
    jtk_Assert_assertTrue(zen_OperandStack_isAvailable(stack, 1), "Operand stack underflow");

    return stack->m_values[--stack->m_size];
}

int64_t zen_OperandStack_popEx(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    /* Check whether the operand stack has, at least, 2 words.
     *
     * TODO: If there is a shortage, an exception should be raised.
     */
    jtk_Assert_assertTrue(zen_OperandStack_isAvailable(stack, 2), "Operand stack underflow");

    int32_t lowPart = stack->m_values[--stack->m_size];
    int32_t highPart = stack->m_values[--stack->m_size];
    return (((uint64_t)highPart << 32L) | lowPart);
}

double zen_OperandStack_popDouble(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    /* Check whether the operand stack has, at least, 2 words.
     *
     * TODO: If there is a shortage, an exception should be raised.
     */
    jtk_Assert_assertTrue(zen_OperandStack_isAvailable(stack, 2), "Operand stack underflow");

    uint32_t lowPart = stack->m_values[--stack->m_size];
    uint32_t highPart = stack->m_values[--stack->m_size];
    uint64_t result = (((uint64_t)highPart << 32L) | lowPart);

    return *((double*)(&result));
}

float zen_OperandStack_popFloat(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    /* Check whether the operand stack has, at least, 1 word.
     *
     * TODO: If there is a shortage, an exception should be raised.
     */
    jtk_Assert_assertTrue(zen_OperandStack_isAvailable(stack, 1), "Operand stack underflow");

    uint32_t result = stack->m_values[--stack->m_size];

    return *((float*)(&result));
}

int32_t zen_OperandStack_popInteger(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    /* Check whether the operand stack has, at least, 1 word.
     *
     * TODO: If there is a shortage, an exception should be raised.
     */
    jtk_Assert_assertTrue(zen_OperandStack_isAvailable(stack, 1), "Operand stack underflow");

    return stack->m_values[--stack->m_size];
}

int64_t zen_OperandStack_popLong(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    /* Check whether the operand stack has, at least, 2 words.
     *
     * TODO: If there is a shortage, an exception should be raised.
     */
    jtk_Assert_assertTrue(zen_OperandStack_isAvailable(stack, 2), "Operand stack underflow");

    int32_t lowPart = stack->m_values[--stack->m_size];
    int32_t highPart = stack->m_values[--stack->m_size];
    return (((uint64_t)highPart << 32L) | lowPart);
}

uintptr_t zen_OperandStack_popReference(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    /* Check whether the operand stack has, at least, 2 words.
     *
     * TODO: If there is a shortage, an exception should be raised.
     */
    jtk_Assert_assertTrue(zen_OperandStack_isAvailable(stack,
        ZEN_OPERAND_STACK_REFERENCE_SLOT_COUNT), "Operand stack underflow");

    uintptr_t result = 0;
        /* A null reference is represented with the integer value of 0. */
    switch (ZEN_OPERAND_STACK_REFERENCE_SLOT_COUNT) {
        case ZEN_OPERAND_STACK_REFERENCE_SLOT_COUNT_1: {
            result = (uintptr_t)stack->m_values[--stack->m_size];

            break;
        }

        case ZEN_OPERAND_STACK_REFERENCE_SLOT_COUNT_2: {
            uint32_t lowPart = stack->m_values[--stack->m_size];
            uint32_t highPart = stack->m_values[--stack->m_size];
            result = (uintptr_t)(((uint64_t)highPart << 32) | lowPart);

            break;
        }
    }

    return result;
}

/* Size */

int32_t zen_OperandStack_getSize(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    return stack->m_size;
}

/* Space */

bool zen_OperandStack_isSpaceAvailable(zen_OperandStack_t* stack, int32_t required) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    return (stack->m_size + required) <= stack->m_capacity;
}

/* Swap */

void zen_OperandStack_swap(zen_OperandStack_t* stack) {
    jtk_Assert_assertObject(stack, "The specified operand stack is null.");

    /* Check whether the operand stack has, at least, 2 words.
     *
     * TODO: If there is a shortage, an exception should be raised.
     */
    jtk_Assert_assertTrue(zen_OperandStack_isAvailable(stack, 2), "Operand stack underflow");

    int32_t index0 = stack->m_size - 1;
    int32_t index1 = stack->m_size - 2;
    uint32_t temporary = stack->m_values[index0];
    stack->m_values[index0] = stack->m_values[index1];
    stack->m_values[index1] = temporary;
}