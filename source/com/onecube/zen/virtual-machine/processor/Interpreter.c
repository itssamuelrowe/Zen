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

// Sunday, September 16, 2018

#include <jtk/math/Math.h>
#include <jtk/core/Float.h>
#include <jtk/core/Double.h>
#include <jtk/collection/array/Array.h>

#include <com/onecube/zen/virtual-machine/VirtualMachine.h>
#include <com/onecube/zen/virtual-machine/feb/ByteCode.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPool.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolFunction.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolInteger.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolLong.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolFloat.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolDouble.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolString.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolUtf8.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolTag.h>
#include <com/onecube/zen/virtual-machine/feb/Entity.h>
#include <com/onecube/zen/virtual-machine/object/Class.h>
#include <com/onecube/zen/virtual-machine/object/Object.h>
#include <com/onecube/zen/virtual-machine/object/Function.h>
#include <com/onecube/zen/virtual-machine/processor/Interpreter.h>
#include <com/onecube/zen/virtual-machine/processor/LocalVariableArray.h>
#include <com/onecube/zen/virtual-machine/memory/MemoryManager.h>

/* The interpreter is the heart of the virtual machine. */

#define ZEN_INTERPRETER_FLAG_WIDE_MODE (1 << 0)
#define ZEN_INTERPRETER_FLAG_EXCEPTION (1 << 1)

const uint8_t* ZEN_INTERPRETER_TAG = "com.onecube.zen.virtual-machine.processor.Interpreter";

/*******************************************************************************
 * BootstrapClass                                                              *
 *******************************************************************************/

const uint8_t* ZEN_BOOTSTRAP_CLASS_ZEN_CORE_INVALID_CAST_EXCEPTION = "zen.core.InvalidCastException";
const uint8_t* ZEN_BOOTSTRAP_CLASS_ZEN_CORE_DIVISION_BY_ZERO_EXCEPTION = "zen.core.DivisionByZeroException";
const uint8_t* ZEN_BOOTSTRAP_CLASS_ZEN_CORE_NULL_POINTER_EXCEPTION = "zen.core.NullPointerException";

/*******************************************************************************
 * Interpreter                                                                 *
 *******************************************************************************/

xjtk_Logger_debug(void* logger, const uint8_t* tag, const uint8_t* message, ...) {
}

/* Constructor */

zen_Interpreter_t* zen_Interpreter_new(zen_MemoryManager_t* manager,
    zen_VirtualMachine_t* virtualMachine, zen_ProcessorThread_t* processorThread) {
    // jtk_Assert_assertObject(manager, "The specified memory manager is null.");

    zen_Interpreter_t* interpreter = jtk_Memory_allocate(zen_Interpreter_t, 1);
    interpreter->m_counter = -1;
    interpreter->m_invocationStack = zen_InvocationStack_new();
    interpreter->m_processorThread = processorThread;
    interpreter->m_logger = NULL;
    interpreter->m_virtualMachine = virtualMachine;

    return interpreter;
}

/* Destructor */

void zen_Interpreter_delete(zen_Interpreter_t* interpreter) {
    jtk_Assert_assertObject(interpreter, "The specified interpreter is null.");

    jtk_Iterator_t* iterator = zen_InvocationStack_getIterator(interpreter->m_invocationStack);
    while (jtk_Iterator_hasNext(iterator)) {
        zen_StackFrame_t* stackFrame = (zen_StackFrame_t*)jtk_Iterator_getNext(iterator);
        zen_StackFrame_delete(stackFrame);
    }
    jtk_Iterator_delete(iterator);

    zen_InvocationStack_delete(interpreter->m_invocationStack);
    jtk_Memory_deallocate(interpreter);
}

/* Class Initialization */

void zen_Interpreter_handleClassInitialization(zen_Interpreter_t* interpreter,
    zen_Class_t* class0) {
    jtk_Assert_assertObject(interpreter, "The specified interpreter is null.");
}

/* Current Class */

uint8_t* zen_Interpreter_getCurrentClassName(zen_Interpreter_t* interpreter) {
    jtk_Assert_assertObject(interpreter, "The specified interpreter is null.");

    return NULL;
}

/* Current Function */

uint8_t* zen_Interpreter_getCurrentFunctionName(zen_Interpreter_t* interpreter) {
    jtk_Assert_assertObject(interpreter, "The specified interpreter is null.");

    return NULL;
}

/* Exception */

zen_Object_t* zen_Interpreter_makeException(zen_Interpreter_t* interpreter,
    const uint8_t* descriptor) {
    return NULL;
}

/* Interpret */

void zen_Interpreter_interpret(zen_Interpreter_t* interpreter) {
    jtk_Assert_assertObject(interpreter, "The specified interpreter is null.");
    jtk_Logger_t* logger = interpreter->m_virtualMachine->m_logger;

    zen_StackFrame_t* currentStackFrame = zen_InvocationStack_peekStackFrame(interpreter->m_invocationStack);
    uint32_t flags = 0;
    while (true) {
        // TODO: Check if the instruction stream is exhausted.
        zen_InstructionAttribute_t* instructionAttribute = currentStackFrame->m_instructionAttribute;

        // Temporary fix. In reality, the return instruction should be provided. */
        if ((currentStackFrame->m_ip + 1) > instructionAttribute->m_instructionLength) {
            break;
        }

        uint8_t instruction = instructionAttribute->m_instructions[currentStackFrame->m_ip++];

        jtk_Logger_debug(logger, "Fetched instruction... (instruction pointer = %d, instruction = 0x%X, function = %s -> %s)",
            currentStackFrame->m_ip, instruction, zen_Interpreter_getCurrentFunctionName(interpreter),
            zen_Interpreter_getCurrentClassName(interpreter));
        fflush(stdout);

        switch (instruction) {

            /* No Operation */

            case ZEN_BYTE_CODE_NOP: { /* nop */
                /* Perform no operation. */

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `nop` (No operation was performed.)");

                break;
            }

            /* Add */

            case ZEN_BYTE_CODE_ADD_I: { /* add_i */
                /* Retrieve the second operand from the operand stack. */
                int32_t operand2 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int32_t operand1 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Add the operands. Push the result on the operand stack. */
                int32_t result = operand1 + operand2;
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `add_i` (operand1 = %d, operand2 = %d, result = %d, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_ADD_L: { /* add_l */
                /* Retrieve the second operand from the operand stack. */
                int64_t operand2 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int64_t operand1 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Add the operands. Push the result on the operand stack. */
                int64_t result = operand1 + operand2;
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `add_l` (operand1 = %l, operand2 = %l, result = %l, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_ADD_F: { /* add_f */
                /* Retrieve the second operand from the operand stack. */
                float operand2 = zen_OperandStack_popFloat(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                float operand1 = zen_OperandStack_popFloat(currentStackFrame->m_operandStack);
                /* Add the operands. Push the result on the operand stack. */
                float result = operand1 + operand2;
                zen_OperandStack_pushFloat(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `add_f` (operand1 = %f, operand2 = %f, result = %f, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_ADD_D: { /* add_d */
                /* Retrieve the second operand from the operand stack. */
                double operand2 = zen_OperandStack_popDouble(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                double operand1 = zen_OperandStack_popDouble(currentStackFrame->m_operandStack);
                /* Add the operands. Push the result on the operand stack. */
                float result = operand1 + operand2;
                zen_OperandStack_pushDouble(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `add_d` (operand1 = %f, operand2 = %f, result = %f, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* Bitwise AND */

            case ZEN_BYTE_CODE_AND_I: { /* and_i */
                /* Retrieve the second operand from the operand stack. */
                int32_t operand2 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int32_t operand1 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Perform bitwise AND on the operands. Push the result on the
                 * operand stack.
                 */
                int32_t result = operand1 & operand2;
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `and_i` (operand1 = %d, operand2 = %d, result = %d, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_AND_L: { /* and_l */
                /* Retrieve the second operand from the operand stack. */
                int64_t operand2 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int64_t operand1 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Perform bitwise AND on the operands. Push the result on the
                 * operand stack.
                 */
                int64_t result = operand1 & operand2;
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `and_l` (operand1 = %l, operand2 = %l, result = %l, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* Bitwise OR */

            case ZEN_BYTE_CODE_OR_I: { /* or_i */
                /* Retrieve the second operand from the operand stack. */
                int32_t operand2 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int32_t operand1 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Perform bitwise OR on the operands. Push the result on the
                 * operand stack.
                 */
                int32_t result = operand1 | operand2;
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `or_i` (operand1 = %d, operand2 = %d, result = %d, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_OR_L: { /* or_l */
                /* Retrieve the second operand from the operand stack. */
                int64_t operand2 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int64_t operand1 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Perform bitwise OR on the operands. Push the result on the
                 * operand stack.
                 */
                int64_t result = operand1 | operand2;
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `or_l` (operand1 = %l, operand2 = %l, result = %l, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* Bitwise Shift */

            case ZEN_BYTE_CODE_SHIFT_LEFT_I: { /* shift_left_i */
                /* Retrieve the second operand from the operand stack. */
                int32_t operand2 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int32_t operand1 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Shift the least significant bit (LSB) of the first operand
                 * towards left, as specified by the second operand. Push the
                 * result on the operand stack.
                 *
                 * Additionally, the second operand is filtered with bitwise AND
                 * to prevent overflow. It should be in the range 0 to 31,
                 * inclusively.
                 */
                int32_t result = operand1 << (operand2 & 0x1F);
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `shift_left_l` (operand1 = %d, operand2 = %d, result = %d, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_SHIFT_LEFT_L: { /* shift_left_l */
                /* Retrieve the second operand from the operand stack. */
                int64_t operand2 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int64_t operand1 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Shift the least significant bit (LSB) of the first operand
                 * towards left, as specified by the second operand. Push the
                 * result on the operand stack.
                 *
                 * Additionally, the second operand is filtered with bitwise AND
                 * to prevent overflow. It should be in the range 0 to 63,
                 * inclusively.
                 */
                int64_t result = operand1 << (operand2 & 0x3FL);
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `shift_left_l` (operand1 = %l, operand2 = %l, result = %l, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_SHIFT_RIGHT_I: { /* shift_right_i */
                /* Retrieve the second operand from the operand stack. */
                int32_t operand2 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int32_t operand1 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Shift the most significant bit (MSB) of the first operand
                 * towards right, as specified by the second operand. Push the
                 * result on the operand stack.
                 *
                 * Additionally, the second operand is filtered with bitwise AND
                 * to prevent overflow. It should be in the range 0 to 31,
                 * inclusively.
                 */
                int32_t result = operand1 >> (operand2 & 0x1F);
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `shift_right_i` (operand1 = %d, operand2 = %d, result = %d, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_SHIFT_RIGHT_L: { /* shift_right_l */
                /* Retrieve the second operand from the operand stack. */
                int64_t operand2 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int64_t operand1 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Shift the most significant bit (MSB) of the first operand
                 * towards right, as specified by the second operand. Push the
                 * result on the operand stack.
                 *
                 * Additionally, the second operand is filtered with bitwise AND
                 * to prevent overflow. It should be in the range 0 to 63,
                 * inclusively.
                 */
                int64_t result = operand1 >> (operand2 & 0x3FL);
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `shift_right_l` (operand1 = %l, operand2 = %l, result = %l, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_SHIFT_RIGHT_UI: { /* shift_right_ui */
                /* Retrieve the second operand from the operand stack. */
                int32_t operand2 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                uint32_t operand1 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Shift the most significant bit (MSB) of the first operand
                 * towards right, as specified by the second operand. Push the
                 * result on the operand stack.
                 *
                 * Additionally, the second operand is filtered with bitwise AND
                 * to prevent overflow. It should be in the range 0 to 31,
                 * inclusively.
                 */
                int32_t result = operand1 >> (operand2 & 0x1F);
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `shift_right_ui` (operand1 = %u, operand2 = %d, result = %d, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_SHIFT_RIGHT_UL: { /* shift_right_ul */
                /* Retrieve the second operand from the operand stack. */
                int64_t operand2 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                uint64_t operand1 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Shift the most significant bit (MSB) of the first operand
                 * towards right, as specified by the second operand. Push the
                 * result on the operand stack.
                 *
                 * Additionally, the second operand is filtered with bitwise AND
                 * to prevent overflow. It should be in the range 0 to 63,
                 * inclusively.
                 */
                int64_t result = operand1 >> (operand2 & 0x3FL);
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `shift_right_ul` (operand1 = %l, operand2 = %l, result = %l, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* Bitwise XOR */

            case ZEN_BYTE_CODE_XOR_I: { /* xor_i */
                /* Retrieve the second operand from the operand stack. */
                int32_t operand2 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int32_t operand1 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Perform bitwise XOR on the operands. Push the result on the
                 * operand stack.
                 */
                int32_t result = operand1 ^ operand2;
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `xor_i` (operand1 = %d, operand2 = %d, result = %d, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_XOR_L: { /* xor_l */
                /* Retrieve the second operand from the operand stack. */
                int64_t operand2 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int64_t operand1 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Perform bitwise XOR on the operands. Push the result on the
                 * operand stack.
                 */
                int64_t result = operand1 ^ operand2;
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `xor_l` (operand1 = %l, operand2 = %l, result = %l, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* Cast */

            case ZEN_BYTE_CODE_CAST_ITL: { /* cast_itl */
                /* Retrieve the operand from the operand stack. */
                int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Cast the operand from 32-bit integer to 64-bit integer. Push
                 * the result on the operand stack.
                 */
                int64_t result = (int64_t)operand;
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `cast_itl` (operand = %d, result = %l, operand stack = %d)",
                    operand, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_CAST_ITF: { /* cast_itf */
                /* Retrieve the operand from the operand stack. */
                int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Cast the operand from 32-bit integer to 32-bit decimal. Push
                 * the result on the operand stack.
                 */
                float result = (float)operand;
                zen_OperandStack_pushFloat(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `cast_itf` (operand = %d, result = %f, operand stack = %d)",
                    operand, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_CAST_ITD: { /* cast_itd */
                /* Retrieve the operand from the operand stack. */
                int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Cast the operand from 32-bit integer to 64-bit decimal. Push
                 * the result on the operand stack.
                 */
                double result = (double)operand;
                zen_OperandStack_pushDouble(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `cast_itd` (operand = %d, result = %f, operand stack = %d)",
                    operand, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_CAST_LTI: { /* cast_lti */
                /* Retrieve the operand from the operand stack. */
                int64_t operand = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Cast the operand from 64-bit integer to 32-bit integer. Push
                 * the result on the operand stack.
                 */
                int32_t result = (int32_t)operand;
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `cast_lti` (operand = %l, result = %i, operand stack = %d)",
                    operand, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_CAST_LTF: { /* cast_ltf */
                /* Retrieve the operand from the operand stack. */
                int64_t operand = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Cast the operand from 64-bit integer to 32-bit decimal. Push
                 * the result on the operand stack.
                 */
                float result = (float)operand;
                zen_OperandStack_pushFloat(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `cast_ltf` (operand = %l, result = %f, operand stack = %d)",
                    operand, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_CAST_LTD: { /* cast_ltd */
                /* Retrieve the operand from the operand stack. */
                int64_t operand = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Cast the operand from 64-bit integer to 64-bit decimal. Push
                 * the result on the operand stack.
                 */
                double result = (double)operand;
                zen_OperandStack_pushDouble(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `cast_ltd` (operand = %l, result = %f, operand stack = %d)",
                    operand, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_CAST_FTI: { /* cast_fti */
                /* Retrieve the operand from the operand stack. */
                float operand = zen_OperandStack_popFloat(currentStackFrame->m_operandStack);
                /* Cast the operand from 32-bit decimal to 32-bit integer. Push
                 * the result on the operand stack.
                 */
                int32_t result = (int32_t)operand;
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `cast_fti` (operand = %f, result = %d, operand stack = %d)",
                    operand, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_CAST_FTL: { /* cast_ftl */
                /* Retrieve the operand from the operand stack. */
                float operand = zen_OperandStack_popFloat(currentStackFrame->m_operandStack);
                /* Cast the operand from 32-bit decimal to 64-bit integer. Push
                 * the result on the operand stack.
                 */
                int64_t result = (int64_t)operand;
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `cast_ftl` (operand = %f, result = %l, operand stack = %d)",
                    operand, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_CAST_FTD: { /* cast_ftd */
                /* Retrieve the operand from the operand stack. */
                float operand = zen_OperandStack_popFloat(currentStackFrame->m_operandStack);
                /* Cast the operand from 32-bit decimal to 64-bit decimal. Push
                 * the result on the operand stack.
                 */
                double result = (double)operand;
                zen_OperandStack_pushDouble(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `cast_ftd` (operand = %f, result = %f, operand stack = %d)",
                    operand, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_CAST_DTI: { /* cast_dti */
                /* Retrieve the operand from the operand stack. */
                double operand = zen_OperandStack_popDouble(currentStackFrame->m_operandStack);
                /* Cast the operand from 64-bit decimal to 32-bit integer. Push
                 * the result on the operand stack.
                 */
                int32_t result = (int32_t)operand;
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `cast_dti` (operand = %f, result = %i, operand stack = %d)",
                    operand, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_CAST_DTL: { /* cast_dtl */
                /* Retrieve the operand from the operand stack. */
                double operand = zen_OperandStack_popDouble(currentStackFrame->m_operandStack);
                /* Cast the operand from 64-bit decimal to 64-bit integer. Push
                 * the result on the operand stack.
                 */
                int64_t result = (int64_t)operand;
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `cast_dtl` (operand = %f, result = %l, operand stack = %d)",
                    operand, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_CAST_DTF: { /* cast_dtf */
                /* Retrieve the operand from the operand stack. */
                double operand = zen_OperandStack_popDouble(currentStackFrame->m_operandStack);
                /* Cast the operand from 64-bit decimal to 32-bit decimal. Push
                 * the result on the operand stack.
                 */
                float result = (float)operand;
                zen_OperandStack_pushFloat(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `cast_dtf` (operand = %f, result = %f, operand stack = %d)",
                    operand, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_CAST_ITB: { /* cast_itb */
                /* Retrieve the operand from the operand stack. */
                int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Cast the operand from 32-bit integer to 8-bit integer. Push
                 * the result on the operand stack.
                 */
                int8_t result = (int8_t)operand;
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `cast_itb` (operand = %d, result = %d, operand stack = %d)",
                    operand, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_CAST_ITC: { /* cast_itc */
                break;
            }

            case ZEN_BYTE_CODE_CAST_ITS: { /* cast_its */
                /* Retrieve the operand from the operand stack. */
                int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Cast the operand from 32-bit integer to 16-bit integer. Push
                 * the result on the operand stack.
                 */
                int16_t result = (int16_t)operand;
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `cast_its` (operand = %d, result = %d, operand stack = %d)",
                    operand, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* Check Cast */

            case ZEN_BYTE_CODE_CHECK_CAST: { /* check_cast */
                uint16_t index = zen_Interpreter_readShort(interpreter);
/*
                zen_Object_t* object = zen_OperandStack_peekReference(currentStackFrame->m_operandStack);
                if (object != NULL) {
                    zen_Class_t* targetClass = NULL; //zen_ConstantPool_resolveClass(constantPool, function, index);
                    if (jtk_ProcessorThread_hasException(thread)) {
                        goto exceptionHandler;
                    }

                    if (!zen_Object_isInstance(object, targetClass)) {
                        const uint8_t* objectClassName = zen_Class_getName(zen_Object_getClass(object));
                        const uint8_t* targetClassName = zen_Class_getName(targetClass);
                        zen_Object_t* exception = zen_Interpreter_makeException(interpreter, ZEN_BOOTSTRAP_CLASS_ZEN_CORE_INVALID_CAST_EXCEPTION,
                            "Cannot cast instance of `%s` to `%s`", objectClassName, targetClassName);
                        jtk_ProcessorThread_setException(thread, exception);

                        goto exceptionHandler;
                    }
                }
                */

                /* Log debugging information for assistance in debugging the interpreter. */
                //jtk_Logger_debug(logger, "Executed instruction `check_cast` (operand = 0x%X, result = 0x%X, operand stack = %d)",
                //    object, object, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* Compare */

            case ZEN_BYTE_CODE_COMPARE_L: { /* compare_l */
                /* Retrieve the second operand from the operand stack. */
                int64_t operand2 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int64_t operand1 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Compare the operands. Push the result on the operand stack.
                 * -> 1 indicates that the first operand is greater than the second
                 *    operand.
                 * -> 0 indicates that the first operand is equal to the second
                 *    operand.
                 * -> -1 indicates that the first operand is lesser than the second
                 *    operand.
                 */
                int32_t result = (operand1 > operand2)? 1 : ((operand1 == operand2)? 0 : -1);
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `compare_l` (operand1 = %l, operand2 = %l, result = %d, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_COMPARE_LT_F: { /* compare_lt_f */
                /* Retrieve the second operand from the operand stack. */
                float operand2 = zen_OperandStack_popFloat(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                float operand1 = zen_OperandStack_popFloat(currentStackFrame->m_operandStack);
                /* Compare the operands. Push the result on the operand stack.
                 * -> 1 indicates that the first operand is greater than the second
                 *    operand.
                 * -> 0 indicates that the first operand is equal to the second
                 *    operand.
                 * -> -1 indicates that the first operand is lesser than the second
                 *    operand.
                 * -> -1 in case one or both the operands represent not-a-number (NaN).
                 */
                int32_t result;
                if (jtk_Float_isNaN(operand1) || jtk_Float_isNaN(operand2)) {
                    result = -1;
                }
                else if (operand1 < operand2) {
                    result = -1;
                }
                else if (operand1 > operand2) {
                    result = 1;
                }
                else if (operand1 == operand2) {
                    result = 0;
                }
                else {
                    // jtk_Logger_error(interpreter->m_logger, ZEN_INTERPRETER_TAG, "Control should not reach here!");
                }
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `compare_lt_f` (operand1 = %f, operand2 = %f, result = %d, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_COMPARE_GT_F: { /* compare_gt_f */
                /* Retrieve the second operand from the operand stack. */
                float operand2 = zen_OperandStack_popFloat(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                float operand1 = zen_OperandStack_popFloat(currentStackFrame->m_operandStack);
                /* Compare the operands. Push the result on the operand stack.
                 * -> 1 indicates that the first operand is greater than the second
                 *    operand.
                 * -> 1 in case one or both the operands represent not-a-number (NaN).
                 * -> 0 indicates that the first operand is equal to the second
                 *    operand.
                 * -> -1 indicates that the first operand is lesser than the second
                 *    operand.
                 */
                int32_t result;
                if (jtk_Float_isNaN(operand1) || jtk_Float_isNaN(operand2)) {
                    result = 1;
                }
                else if (operand1 < operand2) {
                    result = -1;
                }
                else if (operand1 > operand2) {
                    result = 1;
                }
                else if (operand1 == operand2) {
                    result = 0;
                }
                else {
                    // jtk_Logger_error(interpreter->m_logger, ZEN_INTERPRETER_TAG, "Control should not reach here!");
                }
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `compare_gt_f` (operand1 = %f, operand2 = %f, result = %d, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_COMPARE_LT_D: { /* compare_lt_d */
                /* Retrieve the second operand from the operand stack. */
                double operand2 = zen_OperandStack_popDouble(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                double operand1 = zen_OperandStack_popDouble(currentStackFrame->m_operandStack);
                /* Compare the operands. Push the result on the operand stack.
                 * -> 1 indicates that the first operand is greater than the second
                 *    operand.
                 * -> 0 indicates that the first operand is equal to the second
                 *    operand.
                 * -> -1 indicates that the first operand is lesser than the second
                 *    operand.
                 * -> -1 in case one or both the operands represent not-a-number (NaN).
                 */
                int32_t result;
                if (jtk_Double_isNaN(operand1) || jtk_Double_isNaN(operand2)) {
                    result = -1;
                }
                else if (operand1 < operand2) {
                    result = -1;
                }
                else if (operand1 > operand2) {
                    result = 1;
                }
                else if (operand1 == operand2) {
                    result = 0;
                }
                else {
                    // jtk_Logger_error(interpreter->m_logger, ZEN_INTERPRETER_TAG, "Control should not reach here!");
                }
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `compare_lt_d` (operand1 = %f, operand2 = %f, result = %d, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_COMPARE_GT_D: { /* compare_gt_d */
                /* Retrieve the second operand from the operand stack. */
                double operand2 = zen_OperandStack_popDouble(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                double operand1 = zen_OperandStack_popDouble(currentStackFrame->m_operandStack);
                /* Compare the operands. Push the result on the operand stack.
                 * -> 1 indicates that the first operand is greater than the second
                 *    operand.
                 * -> 1 in case one or both the operands represent not-a-number (NaN).
                 * -> 0 indicates that the first operand is equal to the second
                 *    operand.
                 * -> -1 indicates that the first operand is lesser than the second
                 *    operand.
                 */
                int32_t result;
                if (jtk_Double_isNaN(operand1) || jtk_Double_isNaN(operand2)) {
                    result = 1;
                }
                else if (operand1 < operand2) {
                    result = -1;
                }
                else if (operand1 > operand2) {
                    result = 1;
                }
                else if (operand1 == operand2) {
                    result = 0;
                }
                else {
                    // jtk_Logger_error(interpreter->m_logger, ZEN_INTERPRETER_TAG, "Control should not reach here!");
                }
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `compare_gt_f` (operand1 = %f, operand2 = %f, result = %d, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* Divide */

            case ZEN_BYTE_CODE_DIVIDE_I: { /* divide_i */
                /* Retrieve the second operand from the operand stack. */
                int32_t operand2 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int32_t operand1 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);

                int32_t result = -1;
                if (operand2 == 0) {
                    // /* Throw an instance of the zen.core.DivisionByZeroException class. */
                    // zen_Object_t* exception = zen_Interpreter_makeException(interpreter,
                    //     ZEN_BOOTSTRAP_CLASS_ZEN_CORE_DIVISION_BY_ZERO_EXCEPTION);
                    // jtk_ProcessorThread_setException(thread, exception);

                    // goto exceptionHandler;
                }
                else {
                    /* Divide the operands. Push the result on the operand stack. */
                    result = operand1 / operand2;
                    zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);
                }

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `divide_i` (operand1 = %d, operand2 = %d, result = %d, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_DIVIDE_L: { /* divide_l */
                /* Retrieve the second operand from the operand stack. */
                int64_t operand2 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int64_t operand1 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);

                int64_t result = -1;
                if (operand2 == 0L) {
                    // /* Throw an instance of the zen.core.DivisionByZeroException class. */
                    // zen_Object_t* exception = zen_Interpreter_makeException(interpreter,
                    //     ZEN_BOOTSTRAP_CLASS_ZEN_CORE_DIVISION_BY_ZERO_EXCEPTION);
                    // jtk_ProcessorThread_setException(thread, exception);

                    // goto exceptionHandler;
                }
                else {
                    /* Divide the operands. Push the result on the operand stack. */
                    result = operand1 / operand2;
                    zen_OperandStack_pushLong(currentStackFrame->m_operandStack, result);
                }

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `divide_l` (operand1 = %l, operand2 = %l, result = %l, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_DIVIDE_F: { /* divide_f */
                /* Retrieve the second operand from the operand stack. */
                float operand2 = zen_OperandStack_popFloat(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                float operand1 = zen_OperandStack_popFloat(currentStackFrame->m_operandStack);
                /* Divide the operands. Push the result on the operand stack. */
                float result = operand1 / operand2;
                zen_OperandStack_pushFloat(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `divide_f` (operand1 = %f, operand2 = %f, result = %f, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_DIVIDE_D: { /* divide_d */
                /* Retrieve the second operand from the operand stack. */
                double operand2 = zen_OperandStack_popDouble(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                double operand1 = zen_OperandStack_popDouble(currentStackFrame->m_operandStack);
                /* Divide the operands. Push the result on the operand stack. */
                double result = operand1 / operand2;
                zen_OperandStack_pushDouble(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `divide_d` (operand1 = %f, operand2 = %f, result = %f, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* Duplicate */

            case ZEN_BYTE_CODE_DUPLICATE: { /* duplicate */
                /* Duplicate the operand on top of the stack. */
                zen_OperandStack_duplicate(currentStackFrame->m_operandStack);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `duplicate` (operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_DUPLICATE_X1: { /* duplicate_x1 */
                /* Duplicate the operand on top of the stack and insert it beneath the second
                 * operand.
                 */
                zen_OperandStack_duplicateX1(currentStackFrame->m_operandStack);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `duplicate_x1` (operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_DUPLICATE_X2: { /* duplicate_x2 */
                /* Duplicate the operand on top of the stack and insert it beneath the third
                 * operand.
                 */
                zen_OperandStack_duplicateX2(currentStackFrame->m_operandStack);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `duplicate_x2` (operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_DUPLICATE2: { /* duplicate2 */
                /* Duplicate the second operand from the top of the operand stack. */
                zen_OperandStack_duplicate2(currentStackFrame->m_operandStack);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `duplicate2` (operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_DUPLICATE2_X1: { /* duplicate2_x1 */
                /* Duplicate the second operand from the top of the operand stack.
                 * Insert the duplicate beneath the third item.
                 */
                zen_OperandStack_duplicate2X1(currentStackFrame->m_operandStack);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `duplicate2_x1` (operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_DUPLICATE2_X2: { /* duplicate2_x2 */
                /* Duplicate the second operand from the top of the operand stack.
                 * Insert the duplicate beneath the fourth item.
                 */
                zen_OperandStack_duplicate2X2(currentStackFrame->m_operandStack);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `duplicate2_x2` (operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* Jump */

            case ZEN_BYTE_CODE_JUMP_EQ0_I: { /* jump_eq0_i */
                // int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);

                #warning    "TODO: 32-bit instruction is implemented as 64-bit instruction!"
                int32_t operand = (int32_t)zen_OperandStack_popLong(currentStackFrame->m_operandStack);

                if (operand == 0) {
                    uint16_t offset = zen_Interpreter_readShort(interpreter);
                    currentStackFrame->m_ip = offset;
                }
                else {
                    currentStackFrame->m_ip += 2;

                    jtk_Logger_debug(logger, "Operand is %d, expected 0. Branch ignored.", operand);
                }


                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `jump_eq0_i` (operand = %d, expected = 0, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_JUMP_NE0_I: { /* jump_ne0_i */
                // int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);

#warning    "TODO: 32-bit instruction is implemented as 64-bit instruction!"
                int32_t operand = (int32_t)zen_OperandStack_popLong(currentStackFrame->m_operandStack);

                if (operand != 0) {
                    uint16_t offset = zen_Interpreter_readShort(interpreter);
                    currentStackFrame->m_ip = offset;
                }
                else {
                    currentStackFrame->m_ip += 2;

                    jtk_Logger_debug(logger, "Operand is %d, expected integer value other than 0. Branch ignored.",
                        operand);
                }

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `jump_ne0_i` (operand = %d, expected != 0, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_JUMP_LT0_I: { /* jump_lt0_i */
                int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);

                if (operand < 0) {
                    uint16_t offset = zen_Interpreter_readShort(interpreter);
                    currentStackFrame->m_ip += offset - 3;
                }
                else {
                    currentStackFrame->m_ip += 2;

                    jtk_Logger_debug(logger, "Operand is %d, expected integer value lesser than 0. Branch ignored.",
                        operand);
                }

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `jump_lt0_i` (operand = %d, expected < 0, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_JUMP_GT0_I: { /* jump_gt0_i */
                int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);

                if (operand > 0) {
                    uint16_t offset = zen_Interpreter_readShort(interpreter);
                    currentStackFrame->m_ip += offset - 3;
                }
                else {
                    currentStackFrame->m_ip += 2;

                    jtk_Logger_debug(logger, "Operand is %d, expected integer value greater than 0. Branch ignored.",
                        operand);
                }

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `jump_gt0_i` (operand = %d, expected > 0, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_JUMP_LE0_I: { /* jump_le0_i */
                int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);

                if (operand <= 0) {
                    uint16_t offset = zen_Interpreter_readShort(interpreter);
                    currentStackFrame->m_ip += offset - 3;
                }
                else {
                    currentStackFrame->m_ip += 2;

                    jtk_Logger_debug(logger, "Operand is %d, expected integer value lesser than or equal to 0. Branch ignored.",
                        operand);
                }

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `jump_le0_i` (operand = %d, expected <= 0, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_JUMP_GE0_I: { /* jump_ge0_i */
                int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);

                if (operand >= 0) {
                    uint16_t offset = zen_Interpreter_readShort(interpreter);
                    currentStackFrame->m_ip += offset - 3;
                }
                else {
                    currentStackFrame->m_ip += 2;

                    jtk_Logger_debug(logger, "Operand is %d, expected integer value greater than or equal to 0. Branch ignored.",
                        operand);
                }

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `jump_ge0_i` (operand = %d, expected >= 0, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_JUMP_EQ_I: { /* jump_eq_i */
                /* Retrieve the second operand from the operand stack. */
                int32_t operand2 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int32_t operand1 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);

                if (operand1 == operand2) {
                    int16_t offset = zen_Interpreter_readShort(interpreter);
                    currentStackFrame->m_ip += offset - 3;

                    jtk_Logger_debug(logger, "Operands are equal. Branch acknowledged, program counter adjusted accordingly.  (operand1 = %d, operand2 = %d, offset = %d)",
                        operand1, operand2, offset);
                }
                else {
                    currentStackFrame->m_ip += 2;

                    jtk_Logger_debug(logger, "Operands are unequal. Branch ignored. (operand1 = %d, operand2 = %d)",
                        operand1, operand2);
                }

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `jump_eq_i` (operand1 = %d, operand2 = %d, operand stack = %d)",
                    operand1, operand2, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_JUMP_NE_I: { /* jump_ne_i */
                /* Retrieve the second operand from the operand stack. */
                int32_t operand2 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int32_t operand1 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);

                if (operand1 != operand2) {
                    int16_t offset = zen_Interpreter_readShort(interpreter);
                    currentStackFrame->m_ip += offset - 3;

                    jtk_Logger_debug(logger, "Operands are unequal. Branch acknowledged, program counter adjusted accordingly.  (operand1 = %d, operand2 = %d, offset = %d)",
                        operand1, operand2, offset);
                }
                else {
                    currentStackFrame->m_ip += 2;

                    jtk_Logger_debug(logger, "Operands are equal. Branch ignored. (operand1 = %d, operand2 = %d)",
                        operand1, operand2);
                }

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `jump_ne_i` (operand1 = %d, operand2 = %d, operand stack = %d)",
                    operand1, operand2, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_JUMP_LT_I: { /* jump_lt_i */
                /* Retrieve the second operand from the operand stack. */
                int32_t operand2 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int32_t operand1 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);

                if (operand1 < operand2) {
                    int16_t offset = zen_Interpreter_readShort(interpreter);
                    currentStackFrame->m_ip += offset - 3;

                    jtk_Logger_debug(logger, "operand1 is lesser than operand2. Branch acknowledged, program counter adjusted accordingly.  (operand1 = %d, operand2 = %d, offset = %d)",
                        operand1, operand2, offset);
                }
                else {
                    currentStackFrame->m_ip += 2;

                    jtk_Logger_debug(logger, "operand1 is not lesser than operand2. Branch ignored. (operand1 = %d, operand2 = %d)",
                        operand1, operand2);
                }

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `jump_lt_i` (operand1 = %d, operand2 = %d, operand stack = %d)",
                    operand1, operand2, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_JUMP_GT_I: { /* jump_gt_i */
                /* Retrieve the second operand from the operand stack. */
                int32_t operand2 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int32_t operand1 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);

                if (operand1 > operand2) {
                    int16_t offset = zen_Interpreter_readShort(interpreter);
                    currentStackFrame->m_ip += offset - 3;

                    jtk_Logger_debug(logger, "operand1 is greater than operand2. Branch acknowledged, program counter adjusted accordingly.  (operand1 = %d, operand2 = %d, offset = %d)",
                        operand1, operand2, offset);
                }
                else {
                    currentStackFrame->m_ip += 2;

                    jtk_Logger_debug(logger, "operand1 is not greater than operand2. Branch ignored. (operand1 = %d, operand2 = %d)",
                        operand1, operand2);
                }

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `jump_gt_i` (operand1 = %d, operand2 = %d, operand stack = %d)",
                    operand1, operand2, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_JUMP_LE_I: { /* jump_le_i */
                /* Retrieve the second operand from the operand stack. */
                int32_t operand2 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int32_t operand1 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);

                if (operand1 <= operand2) {
                    int16_t offset = zen_Interpreter_readShort(interpreter);
                    currentStackFrame->m_ip += offset - 3;

                    jtk_Logger_debug(logger, "operand1 is lesser than or equal to operand2. Branch acknowledged, program counter adjusted accordingly.  (operand1 = %d, operand2 = %d, offset = %d)",
                        operand1, operand2, offset);
                }
                else {
                    currentStackFrame->m_ip += 2;

                    jtk_Logger_debug(logger, "operand1 is not lesser than or equal to operand2. Branch ignored. (operand1 = %d, operand2 = %d)",
                        operand1, operand2);
                }

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `jump_le_i` (operand1 = %d, operand2 = %d, operand stack = %d)",
                    operand1, operand2, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_JUMP_GE_I: { /* jump_ge_i */
                /* Retrieve the second operand from the operand stack. */
                int32_t operand2 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int32_t operand1 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);

                if (operand1 >= operand2) {
                    int16_t offset = zen_Interpreter_readShort(interpreter);
                    currentStackFrame->m_ip += offset - 3;

                    jtk_Logger_debug(logger, "operand1 is greater than or equal to operand2. Branch acknowledged, program counter adjusted accordingly.  (operand1 = %d, operand2 = %d, offset = %d)",
                        operand1, operand2, offset);
                }
                else {
                    currentStackFrame->m_ip += 2;

                    jtk_Logger_debug(logger, "operand1 is not greater than or equal to operand2. Branch ignored. (operand1 = %d, operand2 = %d)",
                        operand1, operand2);
                }

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `jump_ge_i` (operand1 = %d, operand2 = %d, operand stack = %d)",
                    operand1, operand2, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_JUMP_EQ_A: { /* jump_eq_a */
                /* Retrieve the second operand from the operand stack. */
                uintptr_t operand2 = zen_OperandStack_popReference(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                uintptr_t operand1 = zen_OperandStack_popReference(currentStackFrame->m_operandStack);

                if (operand1 == operand2) {
                    int16_t offset = zen_Interpreter_readShort(interpreter);
                    currentStackFrame->m_ip += offset - 3;

                    jtk_Logger_debug(logger, "Operands are equal. Branch acknowledged, program counter adjusted accordingly.  (operand1 = 0x%X, operand2 = 0x%X, offset = %d)",
                        operand1, operand2, offset);
                }
                else {
                    currentStackFrame->m_ip += 2;

                    jtk_Logger_debug(logger, "Operands are unequal. Branch ignored. (operand1 = 0x%X, operand2 = 0x%X)",
                        operand1, operand2);
                }

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `jump_eq_a` (operand1 = 0x%X, operand2 = 0x%X, operand stack = %d)",
                    operand1, operand2, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_JUMP_NE_A: { /* jump_ne_a */
                /* Retrieve the second operand from the operand stack. */
                uintptr_t operand2 = zen_OperandStack_popReference(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                uintptr_t operand1 = zen_OperandStack_popReference(currentStackFrame->m_operandStack);

                if (operand1 != operand2) {
                    int16_t offset = zen_Interpreter_readShort(interpreter);
                    currentStackFrame->m_ip += offset - 3;

                    jtk_Logger_debug(logger, "Operands are unequal. Branch acknowledged, program counter adjusted accordingly.  (operand1 = 0x%X, operand2 = 0x%X, offset = %d)",
                        operand1, operand2, offset);
                }
                else {
                    currentStackFrame->m_ip += 2;

                    jtk_Logger_debug(logger, "Operands are equal. Branch ignored. (operand1 = 0x%X, operand2 = 0x%X)",
                        operand1, operand2);
                }

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `jump_ne_a` (operand1 = 0x%X, operand2 = 0x%X, operand stack = %d)",
                    operand1, operand2, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_JUMP_EQN_A: { /* jump_eqn_a */
                /* Retrieve the first operand from the operand stack. */
                uintptr_t operand = zen_OperandStack_popReference(currentStackFrame->m_operandStack);

                if (operand == ZEN_INTERPRETER_NULL_REFERENCE) {
                    int16_t offset = zen_Interpreter_readShort(interpreter);
                    currentStackFrame->m_ip += offset - 3;

                    jtk_Logger_debug(logger, "Operand is equal to null. Branch acknowledged, program counter adjusted accordingly.  (operand = 0x%X, offset = %d)",
                        operand, offset);
                }
                else {
                    currentStackFrame->m_ip += 2;

                    jtk_Logger_debug(logger, "Operand is not equal to null. Branch ignored. (operand = 0x%X)",
                        operand);
                }

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `jump_eqn_a` (operand = 0x%X, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_JUMP_NEN_A: { /* jump_nen_a */
                /* Retrieve the first operand from the operand stack. */
                uintptr_t operand = zen_OperandStack_popReference(currentStackFrame->m_operandStack);

                if (operand != ZEN_INTERPRETER_NULL_REFERENCE) {
                    int16_t offset = zen_Interpreter_readShort(interpreter);
                    currentStackFrame->m_ip += offset - 3;

                    jtk_Logger_debug(logger, "Operand is not equal to null. Branch acknowledged, program counter adjusted accordingly.  (operand = 0x%X, offset = %d)",
                        operand, offset);
                }
                else {
                    currentStackFrame->m_ip += 2;

                    jtk_Logger_debug(logger, "Operand is equal to null. Branch ignored. (operand = 0x%X)",
                        operand);
                }

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `jump_nen_a` (operand = 0x%X, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* Increment */

            case ZEN_BYTE_CODE_INCREMENT_I: { /* increment_i */
                break;
            }

            /* Invoke */

            case ZEN_BYTE_CODE_INVOKE_SPECIAL: { /* invoke_special */
                uint16_t index = zen_Interpreter_readShort(interpreter);

                zen_EntityFile_t* entityFile = currentStackFrame->m_class->m_entityFile;
                zen_ConstantPool_t* constantPool = &entityFile->m_constantPool;
                zen_ConstantPoolFunction_t* constructorEntry =
                    (zen_ConstantPoolFunction_t*)constantPool->m_entries[index];
                zen_ConstantPoolUtf8_t* nameEntry = constantPool->m_entries[constructorEntry->m_nameIndex];
                zen_ConstantPoolUtf8_t* descriptorEntry = constantPool->m_entries[constructorEntry->m_descriptorIndex];
                zen_ConstantPoolClass_t* classEntry = constantPool->m_entries[constructorEntry->m_classIndex];
                zen_ConstantPoolUtf8_t* classNameEntry = constantPool->m_entries[classEntry->m_nameIndex];

                jtk_String_t* classDescriptor = jtk_String_newEx(classNameEntry->m_bytes, classNameEntry->m_length);
                zen_Class_t* targetClass = zen_VirtualMachine_getClass(interpreter->m_virtualMachine, classDescriptor);

                jtk_String_t* constructorDescriptor = jtk_String_newEx(descriptorEntry->m_bytes, descriptorEntry->m_length);                
                zen_Function_t* constructor = zen_Class_getConstructor(targetClass, constructorDescriptor);
                
                if (constructor != NULL) {
                    jtk_Array_t* arguments = NULL;
                    int32_t parameterCount = constructor->m_parameterCount;

                    if (constructor->m_parameterCount > 0) {
                        arguments = jtk_Array_new(parameterCount);

                        int32_t parameterIndex;
                        for (parameterIndex = parameterCount - 1; parameterIndex >= 0; parameterIndex--) {
                            void* argument = (void*)zen_OperandStack_popReference(currentStackFrame->m_operandStack);
                            jtk_Array_setValue(arguments, parameterIndex, argument);
                        }
                    }

                    zen_Object_t* object = zen_OperandStack_popReference(currentStackFrame->m_operandStack);
                    zen_Interpreter_invokeConstructor(interpreter, object, constructor, arguments);

                    if (arguments != NULL) {
                        jtk_Array_delete(arguments);
                    }
                }
                else {
                    /* TODO: Throw an instance of the UnknownFunctionException class. */
                    printf("[error] Unknown constructor in class %.*s with signature %.*s!", classDescriptor->m_size,
                        classDescriptor->m_value, constructorDescriptor->m_size, constructorDescriptor->m_value);
                }

                jtk_String_delete(classDescriptor);
                jtk_String_delete(constructorDescriptor);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `invoke_special` (index = %d, operand stack = %d)",
                    index, zen_OperandStack_getSize(currentStackFrame->m_operandStack));
                
                break;
            }

            case ZEN_BYTE_CODE_INVOKE_VIRTUAL: { /* invoke_virtual */
                uint16_t index = zen_Interpreter_readShort(interpreter);

                zen_EntityFile_t* entityFile = currentStackFrame->m_class->m_entityFile;
                zen_ConstantPool_t* constantPool = &entityFile->m_constantPool;
                zen_ConstantPoolFunction_t* functionEntry =
                    (zen_ConstantPoolFunction_t*)constantPool->m_entries[index];
                zen_ConstantPoolUtf8_t* nameEntry = constantPool->m_entries[functionEntry->m_nameIndex];
                zen_ConstantPoolUtf8_t* descriptorEntry = constantPool->m_entries[functionEntry->m_descriptorIndex];
                zen_ConstantPoolClass_t* classEntry = constantPool->m_entries[functionEntry->m_classIndex];
                zen_ConstantPoolUtf8_t* classNameEntry = constantPool->m_entries[classEntry->m_nameIndex];

                zen_Object_t* self = (zen_Object_t*)zen_OperandStack_popReference(currentStackFrame->m_operandStack);
                zen_Class_t* selfClass = zen_Object_getClass(self);

                jtk_String_t* functionName = jtk_String_newEx(nameEntry->m_bytes, nameEntry->m_length);
                jtk_String_t* functionDescriptor = jtk_String_newEx(descriptorEntry->m_bytes, descriptorEntry->m_length);
                /*zen_Function_t* function = zen_Class_getVirtualFunction(selfClass,
                    functionName, functionDescriptor);*/
                #warning "TODO: Replace the following statement with the previous statement!"
                zen_Function_t* function = zen_Class_getStaticFunction(selfClass,
                    functionName, functionDescriptor);

                if (function != NULL) {
                    int32_t parameterCount = function->m_parameterCount;
                    jtk_Array_t* arguments = NULL;

                    if (function->m_parameterCount > 0) {
                        arguments = jtk_Array_new(parameterCount);

                        int32_t parameterIndex;
                        for (parameterIndex = parameterCount - 1; parameterIndex >= 0; parameterIndex--) {
                            void* argument = (void*)zen_OperandStack_popReference(currentStackFrame->m_operandStack);
                            jtk_Array_setValue(arguments, parameterIndex, argument);
                        }
                    }

                    zen_Interpreter_invokeVirtualFunction(interpreter, function, self, arguments);

                    if (arguments != NULL) {
                        jtk_Array_delete(arguments);
                    }
                }
                else {
                    /* TODO: Throw an instance of the UnknownFunctionException class. */
                    printf("[error] An exception was thrown\n"
                        "[error] UnknownFunctionException: Cannot resolve function '%.*s' with signature '%.*s'.\n",
                        functionName->m_size, functionName->m_value, functionDescriptor->m_size,
                        functionDescriptor->m_value);
                }

                jtk_String_delete(functionDescriptor);
                jtk_String_delete(functionName);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `invoke_virtual` (index = %d, operand stack = %d)",
                    index, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_INVOKE_DYNAMIC: { /* invoke_dynamic */
                break;
            }

            case ZEN_BYTE_CODE_INVOKE_STATIC: { /* invoke_static */
                uint16_t index = zen_Interpreter_readShort(interpreter);

                zen_EntityFile_t* entityFile = currentStackFrame->m_class->m_entityFile;
                zen_ConstantPool_t* constantPool = &entityFile->m_constantPool;
                zen_ConstantPoolFunction_t* functionEntry =
                    (zen_ConstantPoolFunction_t*)constantPool->m_entries[index];
                zen_ConstantPoolUtf8_t* nameEntry = constantPool->m_entries[functionEntry->m_nameIndex];
                zen_ConstantPoolUtf8_t* descriptorEntry = constantPool->m_entries[functionEntry->m_descriptorIndex];
                zen_ConstantPoolClass_t* classEntry = constantPool->m_entries[functionEntry->m_classIndex];
                zen_ConstantPoolUtf8_t* classNameEntry = constantPool->m_entries[classEntry->m_nameIndex];

                jtk_String_t* classDescriptor = jtk_String_newEx(classNameEntry->m_bytes, classNameEntry->m_length);
                zen_Class_t* targetClass = zen_VirtualMachine_getClass(interpreter->m_virtualMachine, classDescriptor);
                jtk_String_delete(classDescriptor);

                jtk_String_t* functionName = jtk_String_newEx(nameEntry->m_bytes, nameEntry->m_length);
                jtk_String_t* functionDescriptor = jtk_String_newEx(descriptorEntry->m_bytes, descriptorEntry->m_length);
                zen_Function_t* function = zen_Class_getStaticFunction(targetClass,
                    functionName, functionDescriptor);

                if (function != NULL) {
                    int32_t parameterCount = function->m_parameterCount;
                    jtk_Array_t* arguments = NULL;

                    if (function->m_parameterCount > 0) {
                        arguments = jtk_Array_new(parameterCount);
                        int32_t parameterIndex;
                        for (parameterIndex = parameterCount - 1; parameterIndex >= 0; parameterIndex--) {
                            void* argument = (void*)zen_OperandStack_popReference(currentStackFrame->m_operandStack);
                            jtk_Array_setValue(arguments, parameterIndex, argument);
                        }
                    }
                    
                    zen_Interpreter_invokeStaticFunction(interpreter, function, arguments);
                    
                    if (arguments != NULL) {
                        jtk_Array_delete(arguments);
                    }
                }
                else {
                    /* TODO: Throw an instance of the UnknownFunctionException class. */
                    printf("[error] An exception was thrown\n"
                        "[error] UnknownFunctionException: Cannot resolve function '%.*s' with signature '%.*s'.\n",
                        functionName->m_size, functionName->m_value, functionDescriptor->m_size,
                        functionDescriptor->m_value);
                }

                jtk_String_delete(functionDescriptor);
                jtk_String_delete(functionName);
                
                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `invoke_static` (index = %d, operand stack = %d)",
                    index, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* Jump */

            case ZEN_BYTE_CODE_JUMP: { /* jump */
                int16_t offset = zen_Interpreter_readShort(interpreter);
                currentStackFrame->m_ip = offset;

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `jump` (offset = %d, operand stack = %d)",
                    offset);

                break;
            }

            /* Load */

            case ZEN_BYTE_CODE_LOAD_I: { /* load_i */
                /* Read the index of the local variable to load. */
                uint8_t index = zen_Interpreter_readByte(interpreter);

                /* Retrieve the 32-bit integer value stored in the local variable at
                 * the specified index.
                 */
                int32_t value = zen_LocalVariableArray_getInteger(currentStackFrame->m_localVariableArray, index);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_i` (index = %d, result = %d, operand stack = %d)",
                    index, value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_L: { /* load_l */
                /* Read the index of the local variable to load. */
                int32_t index = zen_Interpreter_readByte(interpreter);

                /* Retrieve the 32-bit integer value stored in the local variable at
                 * the specified index. It represents the low-part of the 64-bit
                 * integer value.
                 */
                int32_t lowPart = zen_LocalVariableArray_getInteger(currentStackFrame->m_localVariableArray, index);
                /* Retrieve the 32-bit integer value stored in the local variable at
                 * the specified index. It represents the high-part of the 64-bit
                 * integer value.
                 */
                int32_t highPart = zen_LocalVariableArray_getInteger(currentStackFrame->m_localVariableArray, index + 1);
                /* Push the retrieved values on the operand stack. */
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, highPart);
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, lowPart);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_l` (index = %d, high part = %d, low part = %d, operand stack = %d)",
                    index, highPart, lowPart, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_F: { /* load_f */
                /* Read the index of the local variable to load. */
                int32_t index = zen_Interpreter_readByte(interpreter);

                /* Retrieve the bit pattern of the 32-bit decimal value stored
                 * in the local variable at the specified index.
                 */
                int32_t value = zen_LocalVariableArray_getInteger(currentStackFrame->m_localVariableArray, index);
                /* Push the retrieved bit pattern of the value on the operand
                 * stack.
                 */
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_f` (index = %d, result = 0x%X, operand stack = %d)",
                    index, value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_D: { /* load_d */
                /* Read the index of the local variable to load. */
                int32_t index = zen_Interpreter_readByte(interpreter);

                /* Retrieve the 32-bit integer value stored in the local variable at
                 * the specified index. It represents the low-part of the bit pattern
                 * of the 64-bit decimal value.
                 */
                int32_t lowPart = zen_LocalVariableArray_getInteger(currentStackFrame->m_localVariableArray, index);
                /* Retrieve the 32-bit integer value stored in the local variable at
                 * the specified index. It represents the high-part of the bit pattern
                 * of the 64-bit decimal value.
                 */
                int32_t highPart = zen_LocalVariableArray_getInteger(currentStackFrame->m_localVariableArray, index + 1);
                /* Push the retrieved values on the operand stack. */
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, highPart);
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, lowPart);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_d` (index = %d, high part = 0x%X, low part = 0x%X, operand stack = %d)",
                    index, highPart, lowPart, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_A: { /* load_a */
                /* Read the index of the local variable to load. */
                uint8_t index = zen_Interpreter_readByte(interpreter);

                /* Retrieve the object reference stored in the local variable at
                 * the specified index.
                 */
                uintptr_t reference = zen_LocalVariableArray_getReference(currentStackFrame->m_localVariableArray, index);
                /* Push the retrieved reference on the operand stack. */
                zen_OperandStack_pushReference(currentStackFrame->m_operandStack, reference);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_a` (index = %d, result = 0x%X, operand stack = %d)",
                    index, reference, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_I0: { /* load_i0 */
                /* Retrieve the 32-bit integer value stored in the local variable at
                 * index 0.
                 */
                int32_t value = zen_LocalVariableArray_getInteger(currentStackFrame->m_localVariableArray, 0);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_i0` (index = 0, result = %d, operand stack = %d)",
                    value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_I1: { /* load_i1 */
                /* Retrieve the 32-bit integer value stored in the local variable at
                 * index 1.
                 */
                int32_t value = zen_LocalVariableArray_getInteger(currentStackFrame->m_localVariableArray, 1);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_i1` (index = 1, result = %d, operand stack = %d)",
                    value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_I2: { /* load_i2 */
                /* Retrieve the 32-bit integer value stored in the local variable at
                 * index 2.
                 */
                int32_t value = zen_LocalVariableArray_getInteger(currentStackFrame->m_localVariableArray, 2);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_i2` (index = 2, result = %d, operand stack = %d)",
                    value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_I3: { /* load_i3 */
                /* Retrieve the 32-bit integer value stored in the local variable at
                 * index 3.
                 */
                int32_t value = zen_LocalVariableArray_getInteger(currentStackFrame->m_localVariableArray, 3);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_i3` (index = 3, result = %d, operand stack = %d)",
                    value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* When copying 64-bit integer values, the high and low parts can be retrieved
             * in succession from the local variable array. Or it can be retrieved with
             * zen_LocalVariableArray_getLong() function in a single shot. A similar technique
             * applies when pushing 64-bit integer values onto the operand stack.
             *
             * I have not verified which of the above techniques is faster. Although,
             * the later technique appears faster because it uses two function calls.
             * The former technique uses 4 function calls.
             *
             * TODO: I have implemented some instructions with the former technique,
             * and others with the later technique. Please verify the faster technique
             * and bring about consistency in the code.
             */

            case ZEN_BYTE_CODE_LOAD_L0: { /* load_l0 */
                /* Retrieve the 64-bit integer value stored in the local variable at
                 * index 0.
                 */
                int64_t value = zen_LocalVariableArray_getLong(currentStackFrame->m_localVariableArray, 0);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_l0` (index = 0, result = %l, operand stack = %d)",
                    value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_L1: { /* load_l1 */
                /* Retrieve the 64-bit integer value stored in the local variable at
                 * index 1.
                 */
                int64_t value = zen_LocalVariableArray_getLong(currentStackFrame->m_localVariableArray, 1);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_l1` (index = 1, result = %l, operand stack = %d)",
                    value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_L2: { /* load_l2 */
                /* Retrieve the 64-bit integer value stored in the local variable at
                 * index 2.
                 */
                int64_t value = zen_LocalVariableArray_getLong(currentStackFrame->m_localVariableArray, 2);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_l2` (index = 2, result = %l, operand stack = %d)",
                    value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_L3: { /* load_l3 */
                /* Retrieve the 64-bit integer value stored in the local variable at
                 * index 3.
                 */
                int64_t value = zen_LocalVariableArray_getLong(currentStackFrame->m_localVariableArray, 3);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_l3` (index = 3, result = %l, operand stack = %d)",
                    value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_F0: { /* load_f0 */
                /* Retrieve the 32-bit decimal value stored in the local variable
                 * at index 0.
                 */
                float value = zen_LocalVariableArray_getFloat(currentStackFrame->m_localVariableArray, 0);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushFloat(currentStackFrame->m_operandStack, value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_f0` (index = 0, result = %f, operand stack = %d)",
                    value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_F1: { /* load_f1 */
                /* Retrieve the 32-bit decimal value stored in the local variable
                 * at index 1.
                 */
                float value = zen_LocalVariableArray_getFloat(currentStackFrame->m_localVariableArray, 1);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushFloat(currentStackFrame->m_operandStack, value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_f1` (index = 1, result = %f, operand stack = %d)",
                    value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_F2: { /* load_f2 */
                /* Retrieve the 32-bit decimal value stored in the local variable
                 * at index 2.
                 */
                float value = zen_LocalVariableArray_getFloat(currentStackFrame->m_localVariableArray, 2);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushFloat(currentStackFrame->m_operandStack, value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_f2` (index = 2, result = %f, operand stack = %d)",
                    value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_F3: { /* load_f3 */
                /* Retrieve the 32-bit decimal value stored in the local variable
                 * at index 3.
                 */
                float value = zen_LocalVariableArray_getFloat(currentStackFrame->m_localVariableArray, 3);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushFloat(currentStackFrame->m_operandStack, value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_f3` (index = 3, result = %f, operand stack = %d)",
                    value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_D0: { /* load_d0 */
                /* Retrieve the 64-bit decimal value stored in the local variable at
                 * index 0.
                 */
                double value = zen_LocalVariableArray_getDouble(currentStackFrame->m_localVariableArray, 0);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushDouble(currentStackFrame->m_operandStack, value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_d0` (index = 0, result = %f, operand stack = %d)",
                    value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_D1: { /* load_d1 */
                /* Retrieve the 64-bit decimal value stored in the local variable at
                 * index 1.
                 */
                double value = zen_LocalVariableArray_getDouble(currentStackFrame->m_localVariableArray, 1);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushDouble(currentStackFrame->m_operandStack, value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_d1` (index = 1, result = %f, operand stack = %d)",
                    value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_D2: { /* load_d2 */
                /* Retrieve the 64-bit decimal value stored in the local variable at
                 * index 2.
                 */
                double value = zen_LocalVariableArray_getDouble(currentStackFrame->m_localVariableArray, 2);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushDouble(currentStackFrame->m_operandStack, value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_d2` (index = 2, result = %f, operand stack = %d)",
                    value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_D3: { /* load_d3 */
                /* Retrieve the 64-bit decimal value stored in the local variable at
                 * index 3.
                 */
                double value = zen_LocalVariableArray_getDouble(currentStackFrame->m_localVariableArray, 3);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushDouble(currentStackFrame->m_operandStack, value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_d3` (index = 3, result = %f, operand stack = %d)",
                    value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_A0: { /* load_a0 */
                /* Retrieve the reference of an object stored in the local variable
                 * at index 0.
                 */
                intptr_t reference = zen_LocalVariableArray_getReference(
                    currentStackFrame->m_localVariableArray, 0);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushReference(currentStackFrame->m_operandStack, reference);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_a0` (index = 0, result = 0x%X, operand stack = %d)",
                    reference, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_A1: { /* load_a1 */
                /* Retrieve the reference of an object stored in the local variable
                 * at index 1.
                 */
                intptr_t reference = zen_LocalVariableArray_getReference(
                    currentStackFrame->m_localVariableArray, 1);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushReference(currentStackFrame->m_operandStack, reference);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_a1` (index = 1, result = 0x%X, operand stack = %d)",
                    reference, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_A2: { /* load_a2 */
                /* Retrieve the reference of an object stored in the local variable
                 * at index 2.
                 */
                intptr_t reference = zen_LocalVariableArray_getReference(
                    currentStackFrame->m_localVariableArray, 2);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushReference(currentStackFrame->m_operandStack, reference);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_a2` (index = 2, result = 0x%X, operand stack = %d)",
                    reference, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_A3: { /* load_a3 */
                /* Retrieve the reference of an object stored in the local variable
                 * at index 3.
                 */
                intptr_t reference = zen_LocalVariableArray_getReference(
                    currentStackFrame->m_localVariableArray, 3);
                /* Push the retrieved value on the operand stack. */
                zen_OperandStack_pushReference(currentStackFrame->m_operandStack, reference);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `load_a3` (index = 3, result = 0x%X, operand stack = %d)",
                    reference, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_LOAD_AB: { /* load_ab */
                break;
            }

            case ZEN_BYTE_CODE_LOAD_AC: { /* load_ac */
                break;
            }

            case ZEN_BYTE_CODE_LOAD_AS: { /* load_as */
                break;
            }

            case ZEN_BYTE_CODE_LOAD_AI: { /* load_ai */
                break;
            }

            case ZEN_BYTE_CODE_LOAD_AL: { /* load_al */
                break;
            }

            case ZEN_BYTE_CODE_LOAD_AF: { /* load_af */
                break;
            }

            case ZEN_BYTE_CODE_LOAD_AD: { /* load_ad */
                break;
            }

            case ZEN_BYTE_CODE_LOAD_AA: { /* load_aa */
                break;
            }

            case ZEN_BYTE_CODE_LOAD_INSTANCE_FIELD: { /* load_instance_field */
                break;
            }

            case ZEN_BYTE_CODE_LOAD_STATIC_FIELD: { /* load_static_field */
                break;
            }

            case ZEN_BYTE_CODE_LOAD_CPR: { /* load_cpr */
                int32_t index = zen_Interpreter_readByte(interpreter);

                zen_EntityFile_t* entityFile = currentStackFrame->m_class->m_entityFile;
                zen_ConstantPool_t* constantPool = &entityFile->m_constantPool;
                zen_ConstantPoolEntry_t* entry = constantPool->m_entries[index];
                switch (entry->m_tag) {
                    case ZEN_CONSTANT_POOL_TAG_INTEGER: {
                        zen_ConstantPoolInteger_t* constantPoolInteger = (zen_ConstantPoolInteger_t*)entry;
                        int32_t value = zen_ConstantPoolInteger_getValue(constantPoolInteger);
                        zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, value);

                        jtk_Logger_debug(logger, "Executed instruction `load_cpr` (index = %d, result = '%d', operand stack = %d)",
                            index, value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                        break;
                    }

                    case ZEN_CONSTANT_POOL_TAG_LONG: {
                        zen_ConstantPoolLong_t* constantPoolLong = (zen_ConstantPoolLong_t*)entry;
                        int64_t value = zen_ConstantPoolLong_getValue(constantPoolLong);
                        zen_OperandStack_pushLong(currentStackFrame->m_operandStack, value);

                        jtk_Logger_debug(logger, "Executed instruction `load_cpr` (index = %d, result = '%l', operand stack = %d)",
                            index, value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                        break;
                    }

                    case ZEN_CONSTANT_POOL_TAG_FLOAT: {
                        zen_ConstantPoolFloat_t* constantPoolFloat = (zen_ConstantPoolFloat_t*)entry;
                        float value = zen_ConstantPoolFloat_getValue(constantPoolFloat);
                        zen_OperandStack_pushFloat(currentStackFrame->m_operandStack, value);

                        jtk_Logger_debug(logger, "Executed instruction `load_cpr` (index = %d, result = '%f', operand stack = %d)",
                            index, value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                        break;
                    }

                    case ZEN_CONSTANT_POOL_TAG_DOUBLE: {
                        zen_ConstantPoolDouble_t* constantPoolDouble = (zen_ConstantPoolDouble_t*)entry;
                        double value = zen_ConstantPoolDouble_getValue(constantPoolDouble);
                        zen_OperandStack_pushDouble(currentStackFrame->m_operandStack, value);

                        jtk_Logger_debug(logger, "Executed instruction `load_cpr` (index = %d, result = '%f', operand stack = %d)",
                            index, value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                        break;
                    }

                    case ZEN_CONSTANT_POOL_TAG_STRING: {
                        zen_ConstantPoolString_t* constantPoolString = (zen_ConstantPoolString_t*)entry;
                        zen_ConstantPoolUtf8_t* constantPoolUtf8 =
                            (zen_ConstantPoolString_t*)constantPool->m_entries[constantPoolString->m_stringIndex];
                        // jtk_String_t* value = jtk_String_newEx(constantPoolUtf8->m_bytes, constantPoolUtf8->m_length);

                        // const uint8_t* arrayDescriptor = "b";
                        // int32_t arrayDescriptorSize = 1;
                        zen_Object_t* array = zen_VirtualMachine_newByteArray(interpreter->m_virtualMachine,
                            constantPoolUtf8->m_bytes, constantPoolUtf8->m_length);

                        const uint8_t* stringClassDescriptor = "zen/core/String";
                        int32_t stringClassDescriptorSize = 15;
                        const uint8_t* stringConstructorDescriptor = "v:(zen/core/Object)";
                        int32_t stringConstructorDescriptorSize = 19;

                        jtk_Array_t* arguments = jtk_Array_new(1);
                        jtk_Array_setValue(arguments, 0, array);

                        zen_Object_t* value = zen_VirtualMachine_newObjectEx(interpreter->m_virtualMachine,
                            stringClassDescriptor, stringClassDescriptorSize, stringConstructorDescriptor,
                            stringConstructorDescriptorSize, arguments);

                        jtk_Array_delete(arguments);

                        zen_OperandStack_pushReference(currentStackFrame->m_operandStack, value);

                        jtk_Logger_debug(logger, "Executed instruction `load_cpr` (index = %d, result = '%.*s', operand stack = %d)",
                            index, constantPoolUtf8->m_length, constantPoolUtf8->m_bytes, zen_OperandStack_getSize(currentStackFrame->m_operandStack));
                        
                        jtk_Logger_debug(logger, "String object at 0x%X, array object at 0x%x", value, array);

                        break;
                    }

                    default: {
                        // ERROR: load_cpr cannot load other types of constants.
                    }
                }

                break;
            }

            case ZEN_BYTE_CODE_LOAD_ARRAY_SIZE: { /* load_array_size */
                break;
            }

            /* Modulo */

            case ZEN_BYTE_CODE_MODULO_I: { /* modulo_i */
                /* Retrieve the second operand from the operand stack. */
                int32_t operand2 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int32_t operand1 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);

                if (operand2 == 0) {
                    /* TODO: Throw an instance of the zen.core.DivisionByZeroException class. */
                }

                /* Divide the operands to evaluate the remainder. Push the result
                 * on the operand stack.
                 */
                int32_t result = operand1 % operand2;
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `modulo_i` (operand1 = %d, operand2 = %d, result = %d, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_MODULO_L: { /* modulo_l */
                /* Retrieve the second operand from the operand stack. */
                int64_t operand2 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int64_t operand1 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);

                if (operand2 == 0L) {
                    /* TODO: Throw an instance of the zen.core.DivisionByZeroException class. */
                }

                /* Divide the operands to evaluate the remainder. Push the result
                 * on the operand stack.
                 */
                int64_t result = operand1 % operand2;
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `modulo_l` (operand1 = %l, operand2 = %l, result = %l, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_MODULO_F: { /* modulo_f */
                /* Retrieve the second operand from the operand stack. */
                float operand2 = zen_OperandStack_popFloat(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                float operand1 = zen_OperandStack_popFloat(currentStackFrame->m_operandStack);
                /* Divide the operands to evaluate the remainder. Push the result
                 * on the operand stack.
                 */
                float result = jtk_Math_modulo_f(operand1, operand2);
                zen_OperandStack_pushFloat(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `modulo_f` (operand1 = %f, operand2 = %f, result = %f, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_MODULO_D: { /* modulo_d */
                /* Retrieve the second operand from the operand stack. */
                double operand2 = zen_OperandStack_popDouble(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                double operand1 = zen_OperandStack_popDouble(currentStackFrame->m_operandStack);
                /* Divide the operands to evaluate the remainder. Push the result
                 * on the operand stack.
                 */
                double result = jtk_Math_modulo_d(operand1, operand2);
                zen_OperandStack_pushDouble(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `modulo_d` (operand1 = %f, operand2 = %f, result = %f, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* Multiply */

            case ZEN_BYTE_CODE_MULTIPLY_I: { /* multiply_i */
                /* Retrieve the second operand from the operand stack. */
                int32_t operand2 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int32_t operand1 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Multiply the operands. Push the result on the operand stack. */
                int32_t result = operand1 * operand2;
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `multiply_i` (operand1 = %d, operand2 = %d, result = %d, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_MULTIPLY_L: { /* multiply_l */
                /* Retrieve the second operand from the operand stack. */
                int64_t operand2 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int64_t operand1 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Multiply the operands. Push the result on the operand stack. */
                int64_t result = operand1 * operand2;
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `multiply_l` (operand1 = %l, operand2 = %l, result = %l, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_MULTIPLY_F: { /* multiply_f */
                /* Retrieve the second operand from the operand stack. */
                float operand2 = zen_OperandStack_popFloat(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                float operand1 = zen_OperandStack_popFloat(currentStackFrame->m_operandStack);
                /* Multiply the operands. Push the result on the operand stack. */
                float result = operand1 * operand2;
                zen_OperandStack_pushFloat(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `multiply_f` (operand1 = %f, operand2 = %f, result = %f, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_MULTIPLY_D: { /* multiply_d */
                /* Retrieve the second operand from the operand stack. */
                double operand2 = zen_OperandStack_popDouble(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                double operand1 = zen_OperandStack_popDouble(currentStackFrame->m_operandStack);
                /* Multiply the operands. Push the result on the operand stack. */
                double result = operand1 * operand2;
                zen_OperandStack_pushDouble(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `multiply_d` (operand1 = %f, operand2 = %f, result = %f, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* Negate */

            case ZEN_BYTE_CODE_NEGATE_I: { /* negate_i */
                /* Retrieve the operand from the operand stack. */
                int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Negate the operand. Push the result on the operand stack. */
                int32_t result = -operand;
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `negate_i` (operand = %d, result = %d, operand stack = %d)",
                    operand, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_NEGATE_L: { /* negate_l */
                /* Retrieve the operand from the operand stack. */
                int64_t operand = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Negate the operand. Push the result on the operand stack. */
                int64_t result = -operand;
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `negate_l` (operand = %l, result = %l, operand stack = %d)",
                    operand, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_NEGATE_F: { /* negate_f */
                /* Retrieve the operand from the operand stack. */
                float operand = zen_OperandStack_popFloat(currentStackFrame->m_operandStack);
                /* Negate the operand. Push the result on the operand stack. */
                float result = -operand;
                zen_OperandStack_pushFloat(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `negate_f` (operand = %f, result = %f, operand stack = %d)",
                    operand, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_NEGATE_D: { /* negate_d */
                /* Retrieve the operand from the operand stack. */
                double operand = zen_OperandStack_popDouble(currentStackFrame->m_operandStack);
                /* Negate the operand. Push the result on the operand stack. */
                double result = -operand;
                zen_OperandStack_pushDouble(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `negate_d` (operand = %f, result = %f, operand stack = %d)",
                    operand, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* New */

            case ZEN_BYTE_CODE_NEW: { /* new */
                uint16_t index = zen_Interpreter_readShort(interpreter);

                zen_EntityFile_t* entityFile = currentStackFrame->m_class->m_entityFile;
                zen_ConstantPool_t* constantPool = &entityFile->m_constantPool;
                zen_ConstantPoolClass_t* classEntry =
                    (zen_ConstantPoolClass_t*)constantPool->m_entries[index];
                zen_ConstantPoolUtf8_t* nameEntry = constantPool->m_entries[classEntry->m_nameIndex];
                
                jtk_String_t* classDescriptor = jtk_String_newEx(nameEntry->m_bytes, nameEntry->m_length);
                zen_Class_t* targetClass = zen_VirtualMachine_getClass(interpreter->m_virtualMachine, classDescriptor);
                jtk_String_delete(classDescriptor);

                if (targetClass != NULL) {
                    zen_Object_t* result = zen_VirtualMachine_allocateObject(interpreter->m_virtualMachine,
                        targetClass);
                    /* Push the reference of the newly allocated object onto the operand stack. */
                    zen_OperandStack_pushReference(currentStackFrame->m_operandStack, result);
                }
                else {
                    /* TODO: Throw an instance of the UnknownClassException class. */
                }

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `new` (index = %d, operand stack = %d)",
                    index, zen_OperandStack_getSize(currentStackFrame->m_operandStack));
                
                break;
            }

            case ZEN_BYTE_CODE_NEW_ARRAY: { /* new_array */
                break;
            }

            case ZEN_BYTE_CODE_NEW_ARRAY_A: { /* new_array_a */
                /* Read the class index from the instruction stream. */
                uint16_t index = zen_Interpreter_readShort(interpreter);
                /* Retrieve the size of the array from the operand stack. */
                int32_t size = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* TODO: Create a new reference array. */
                jtk_Array_t* result = jtk_Array_new(size);
                /* Push the reference of the newly created reference array. */
                zen_OperandStack_pushReference(currentStackFrame->m_operandStack, (void*)result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `new_array_a` (index = %d, size = %d, result = 0x%X, operand stack = %d)",
                    index, size, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_NEW_ARRAY_AN: { /* new_array_an */
                break;
            }

            /* Pop */

            case ZEN_BYTE_CODE_POP: { /* pop */
                /* Discard the operand on top of the operand stack. */
                zen_OperandStack_pop(currentStackFrame->m_operandStack);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `pop` (operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_POP2: { /* pop2 */
                /* Discard the first two operands on top of the operand stack. */
                zen_OperandStack_popEx(currentStackFrame->m_operandStack, 2);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `pop2` (operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* Push */

            case ZEN_BYTE_CODE_PUSH_NULL: { /* push_null */
                /* Push an integer value of 0, which represents the null reference,
                 * on the operand stack.
                 */
                zen_OperandStack_pushReference(currentStackFrame->m_operandStack, ZEN_INTERPRETER_NULL_REFERENCE);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `push_null` (operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_PUSH_IN1: { /* push_in1 */
                /* Push a 32-bit integer value of -1 on the operand stack. */
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, -1);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `push_in1` (result = -1, operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_PUSH_I0: { /* push_i0 */
                /* Push a 32-bit integer value of 0 on the operand stack. */
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, 0);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `push_i0` (result = 0, operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_PUSH_I1: { /* push_i1 */
                /* Push a 32-bit integer value of 1 on the operand stack. */
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, 1);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `push_i1` (result = 1, operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_PUSH_I2: { /* push_i2 */
                /* Push a 32-bit integer value of 2 on the operand stack. */
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, 2);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `push_i2` (result = 2, operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_PUSH_I3: { /* push_i3 */
                /* Push a 32-bit integer value of 3 on the operand stack. */
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, 3);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `push_i3` (result = 3, operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_PUSH_I4: { /* push_i4 */
                /* Push a 32-bit integer value of 4 on the operand stack. */
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, 4);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `push_i4` (result = 4, operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_PUSH_I5: { /* push_i5 */
                /* Push a 32-bit integer value of 5 on the operand stack. */
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, 5);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `push_i5` (result = 5, operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_PUSH_L0: { /* push_l0 */
                /* Push a 64-bit integer value of 0 on the operand stack. */
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, 0L);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `push_l0` (result = 0, operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_PUSH_L1: { /* push_l1 */
                /* Push a 64-bit integer value of 1 on the operand stack. */
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, 1L);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `push_l1` (result = 1, operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_PUSH_L2: { /* push_l2 */
                /* Push a 64-bit integer value of 2 on the operand stack. */
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, 2L);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `push_l2` (result = 2, operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_PUSH_F0: { /* push_f0 */
                /* Push a 32-bit decimal value of 0.0 on the operand stack. */
                zen_OperandStack_pushFloat(currentStackFrame->m_operandStack, 0.0f);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `push_f0` (result = 0.0, operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_PUSH_F1: { /* push_f1 */
                /* Push a 32-bit decimal value of 1.0 on the operand stack. */
                zen_OperandStack_pushFloat(currentStackFrame->m_operandStack, 1.0f);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `push_f1` (result = 1.0, operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_PUSH_F2: { /* push_f2 */
                /* Push a 32-bit decimal value of 2.0 on the operand stack. */
                zen_OperandStack_pushFloat(currentStackFrame->m_operandStack, 2.0f);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `push_f2` (result = 2.0, operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_PUSH_D0: { /* push_d0 */
                /* Push a 64-bit decimal value of 0.0 on the operand stack. */
                zen_OperandStack_pushDouble(currentStackFrame->m_operandStack, 0.0);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `push_d0` (result = 0.0, operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_PUSH_D1: { /* push_d1 */
                /* Push a 64-bit decimal value of 1.0 on the operand stack. */
                zen_OperandStack_pushDouble(currentStackFrame->m_operandStack, 1.0);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `push_d1` (result = 1.0, operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_PUSH_D2: { /* push_d2 */
                /* Push a 64-bit decimal value of 2.0 on the operand stack. */
                zen_OperandStack_pushDouble(currentStackFrame->m_operandStack, 2.0);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `push_d2` (result = 2.0, operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_PUSH_B: { /* push_b */
                /* Read the 8-bit integer value to push on the stack. */
                int8_t value = zen_Interpreter_readByte(interpreter);
                /* Push the 8-bit integer value on the stack. It is extended to
                 * a 32-bit integer; zeroes are used as padding.
                 */
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, (int32_t)value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `push_b` (result = %d, operand stack = %d)",
                    value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_PUSH_S: { /* push_s */
                /* Read the 16-bit integer value to push on the stack. */
                int16_t value = zen_Interpreter_readShort(interpreter);
                /* Push the 16-bit integer value on the stack. It is extended to
                 * a 32-bit integer; zeroes are used as padding.
                 */
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, (int32_t)value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `push_s` (result = %d, operand stack = %d)",
                    value, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* Return */

            case ZEN_BYTE_CODE_RETURN: { /* return */
                /* The currently executing function is returning to the caller.
                 * Therefore, pop the current stack frame.
                 */
                currentStackFrame = zen_InvocationStack_popStackFrame(interpreter->m_processorThread->m_invocationStack);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `return` (operand stack = %d)",
                    zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_RETURN_I: { /* return_i */
                /* Retrieve the operand from the operand stack. */
                int32_t returnValue = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);

                /* The currently executing function is returning to the caller.
                 * Therefore, pop the current stack frame.
                 */
                currentStackFrame = zen_InvocationStack_popStackFrame(interpreter->m_processorThread->m_invocationStack);

                /* Push the operand on the "new" current stack frame. */
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, returnValue);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `return_i` (operand = %d, operand stack = %d)",
                    returnValue, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_RETURN_L: { /* return_l */
                /* Retrieve the operand from the operand stack. */
                int64_t returnValue = zen_OperandStack_popLong(currentStackFrame->m_operandStack);

                /* The currently executing function is returning to the caller.
                 * Therefore, pop the current stack frame.
                 */
                currentStackFrame = zen_InvocationStack_popStackFrame(interpreter->m_processorThread->m_invocationStack);

                /* Push the operand on the "new" current stack frame. */
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, returnValue);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `return_l` (operand = %l, operand stack = %d)",
                    returnValue, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* NOTE: The return_f and return_d instructions are implemented in terms of
             * 32-bit and 64-bit integers.
             *
             * Since, we are retriving and pushing the operand from the OperandStack
             * class it does not really affect. In fact, internally, the operand
             * stack stores decimal values as integers.
             */

            case ZEN_BYTE_CODE_RETURN_F: { /* return_f */
                /* Retrieve the operand from the operand stack. */
                int32_t returnValue = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);

                /* The currently executing function is returning to the caller.
                 * Therefore, pop the current stack frame.
                 */
                currentStackFrame = zen_InvocationStack_popStackFrame(interpreter->m_processorThread->m_invocationStack);

                /* Push the operand on the "new" current stack frame. */
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, returnValue);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `return_f` (operand = 0x%X, operand stack = %d)",
                    returnValue, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_RETURN_D: { /* return_d */
                /* Retrieve the operand from the operand stack. */
                int64_t returnValue = zen_OperandStack_popLong(currentStackFrame->m_operandStack);

                /* The currently executing function is returning to the caller.
                 * Therefore, pop the current stack frame.
                 */
                currentStackFrame = zen_InvocationStack_popStackFrame(interpreter->m_processorThread->m_invocationStack);

                /* Push the operand on the "new" current stack frame. */
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, returnValue);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `return_d` (operand = 0x%X, operand stack = %d)",
                    returnValue, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_RETURN_A: { /* return_a */
                /* Retrieve the operand from the operand stack. */
                intptr_t returnValue = zen_OperandStack_popReference(currentStackFrame->m_operandStack);

                /* The currently executing function is returning to the caller.
                 * Therefore, pop the current stack frame.
                 */
                currentStackFrame = zen_InvocationStack_popStackFrame(interpreter->m_processorThread->m_invocationStack);

                /* Push the operand on the "new" current stack frame. */
                zen_OperandStack_pushReference(currentStackFrame->m_operandStack, returnValue);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `return_a` (operand = 0x%X, operand stack = %d)",
                    returnValue, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* RTTI */

            case ZEN_BYTE_CODE_RTTI: { /* rtti */
                break;
            }

            /* Store */

            case ZEN_BYTE_CODE_STORE_I: { /* store_i */
                /* Read the index of the local variable to modify. */
                uint8_t index = zen_Interpreter_readByte(interpreter);
                /* Retrieve the operand from the operand stack. */
                int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Update the value of the local variable with the specified index. */
                zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, index, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_i` (operand = %d, index = %d, operand stack = %d)",
                    operand, index, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_I0: { /* store_i0 */
                /* Retrieve the operand from the operand stack. */
                int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Store the retrieved operand in the local variable at index 0. */
                zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 0, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_i0` (operand = %d, index = 0, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_I1: { /* store_i1 */
                /* Retrieve the operand from the operand stack. */
                int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Store the retrieved operand in the local variable at index 1. */
                zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 1, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_i1` (operand = %d, index = 1, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_I2: { /* store_i2 */
                /* Retrieve the operand from the operand stack. */
                int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Store the retrieved operand in the local variable at index 2. */
                zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 2, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_i2` (operand = %d, index = 2, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_I3: { /* store_i3 */
                /* Retrieve the operand from the operand stack. */
                int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Store the retrieved operand in the local variable at index 3. */
                zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 3, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_i3` (operand = %d, index = 3, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_L: { /* store_l */
                /* Read the index of the local variable to modify. */
                uint8_t index = zen_Interpreter_readByte(interpreter);
                /* Retrieve the operand from the operand stack. */
                int64_t operand = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Update the value of the local variable with the specified index. */
                zen_LocalVariableArray_setLong(currentStackFrame->m_localVariableArray, index, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_l` (operand = %d, index = %d, operand stack = %d)",
                    operand, index, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* NOTE: The order in which the high and low parts are stored should
             *       match with the order in which the high and low parts are
             *       loaded.
             */

            case ZEN_BYTE_CODE_STORE_L0: { /* store_l0 */
                /* Retrieve a 32-bit integer operand from the operand stack.
                 * It represents the low-part of the 64-bit integer.
                 */
                // int32_t lowPart = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve a 32-bit integer operand from the operand stack.
                 * It represents the high-part of the 64-bit integer.
                 */
                // int32_t highPart = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Store the retrieved operands in the local variable at index 0.
                 *
                 * A 64-bit integer value occupies two indexes in the local variable
                 * array. The high part is stored at index 0. The low part is stored
                 * in the subsequent index, that is 1.
                 */
                // zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 0, highPart);
                // zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 1, lowPart);

                /* The above code implements the previous algorithm. The new algorithm is
                 * a humble attempt at optimizing the interpreter.
                 */

                /* Retrieve the operand from the operand stack. */
                int64_t operand = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Store the retrieved operand in the local variable at index 0. */
                zen_LocalVariableArray_setLong(currentStackFrame->m_localVariableArray, 0, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_l0` (operand = %l, index = 0, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_L1: { /* store_l1 */
                /* Retrieve a 32-bit integer operand from the operand stack.
                 * It represents the low-part of the 64-bit integer.
                 */
                // int32_t lowPart = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve a 32-bit integer operand from the operand stack.
                 * It represents the high-part of the 64-bit integer.
                 */
                // int32_t highPart = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Store the retrieved operands in the local variable at index 1.
                 *
                 * A 64-bit integer value occupies two indexes in the local variable
                 * array. The high part is stored at index 1. The low part is stored
                 * in the subsequent index, that is 2.
                 */
                // zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 1, highPart);
                // zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 2, lowPart);

                /* The above code implements the previous algorithm. The new algorithm is
                 * a humble attempt at optimizing the interpreter.
                 */

                /* Retrieve the operand from the operand stack. */
                int64_t operand = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Store the retrieved operand in the local variable at index 1. */
                zen_LocalVariableArray_setLong(currentStackFrame->m_localVariableArray, 1, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_l1` (operand = %l, index = 1, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_L2: { /* store_l2 */
                /* Retrieve a 32-bit integer operand from the operand stack.
                 * It represents the low-part of the 64-bit integer.
                 */
                // int32_t lowPart = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve a 32-bit integer operand from the operand stack.
                 * It represents the high-part of the 64-bit integer.
                 */
                // int32_t highPart = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Store the retrieved operands in the local variable at index 2.
                 *
                 * A 64-bit integer value occupies two indexes in the local variable
                 * array. The high part is stored at index 2. The low part is stored
                 * in the subsequent index, that is 3.
                 */
                // zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 2, highPart);
                // zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 3, lowPart);

                /* The above code implements the previous algorithm. The new algorithm is
                 * a humble attempt at optimizing the interpreter.
                 */

                /* Retrieve the operand from the operand stack. */
                int64_t operand = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Store the retrieved operand in the local variable at index 2. */
                zen_LocalVariableArray_setLong(currentStackFrame->m_localVariableArray, 2, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_l2` (operand = %l, index = 2, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_L3: { /* store_l3 */
                /* Retrieve a 32-bit integer operand from the operand stack.
                 * It represents the low-part of the 64-bit integer.
                 */
                // int32_t lowPart = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve a 32-bit integer operand from the operand stack.
                 * It represents the high-part of the 64-bit integer.
                 */
                // int32_t highPart = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Store the retrieved operands in the local variable at index 3.
                 *
                 * A 64-bit integer value occupies two indexes in the local variable
                 * array. The high part is stored at index 3. The low part is stored
                 * in the subsequent index, that is 4.
                 */
                // zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 3, highPart);
                // zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 4, lowPart);

                /* The above code implements the previous algorithm. The new algorithm is
                 * a humble attempt at optimizing the interpreter.
                 */

                /* Retrieve the operand from the operand stack. */
                int64_t operand = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Store the retrieved operand in the local variable at index 3. */
                zen_LocalVariableArray_setLong(currentStackFrame->m_localVariableArray, 3, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_l3` (operand = %l, index = 3, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* NOTE: The store_f and store_d instructions are implemented in terms of
             * 32-bit and 64-bit integers.
             *
             * Since, we are retriving and updating from the operand from the
             * OperandStack and LocalVariableArray classes it does not really
             * affect. In fact, internally, the operand stack and local variable
             * array store decimal values as integers.
             */

            case ZEN_BYTE_CODE_STORE_F: { /* store_f */
                /* Read the index of the local variable to modify. */
                uint8_t index = zen_Interpreter_readByte(interpreter);
                /* Retrieve the operand from the operand stack. */
                int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Update the value of the local variable with the specified index. */
                zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, index, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_f` (operand = 0x%X, index = %d, operand stack = %d)",
                    operand, index, zen_OperandStack_getSize(currentStackFrame->m_operandStack));


                break;
            }

            case ZEN_BYTE_CODE_STORE_F0: { /* store_f0 */
                /* Retrieve the operand from the operand stack. It represents
                 * the bit pattern of 32-bit decimal value.
                 */
                int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Store the retrieved bit pattern of the 32-decimal value operand
                 * in the local variable at index 0.
                 */
                zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 0, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_f0` (operand = %d, index = 0, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_F1: { /* store_f1 */
                /* Retrieve the operand from the operand stack. It represents
                 * the bit pattern of 32-bit decimal value.
                 */
                int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Store the retrieved bit pattern of the 32-decimal value operand
                 * in the local variable at index 1.
                 */
                zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 1, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_f1` (operand = %d, index = 1, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_F2: { /* store_f2 */
                /* Retrieve the operand from the operand stack. It represents
                 * the bit pattern of 32-bit decimal value.
                 */
                int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Store the retrieved bit pattern of the 32-decimal value operand
                 * in the local variable at index 2.
                 */
                zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 2, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_f2` (operand = %d, index = 2, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_F3: { /* store_f3 */
                /* Retrieve the operand from the operand stack. It represents
                 * the bit pattern of 32-bit decimal value.
                 */
                int32_t operand = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Store the retrieved bit pattern of the 32-decimal value operand
                 * in the local variable at index 3.
                 */
                zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 3, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_f3` (operand = %d, index = 3, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* NOTE: The order in which the high and low parts are stored should
             *       match with the order in which the high and low parts are
             *       loaded.
             */

            case ZEN_BYTE_CODE_STORE_D: { /* store_d */
                /* Read the index of the local variable to modify. */
                uint8_t index = zen_Interpreter_readByte(interpreter);
                /* Retrieve the operand from the operand stack. */
                int64_t operand = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Update the value of the local variable with the specified index. */
                zen_LocalVariableArray_setLong(currentStackFrame->m_localVariableArray, index, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_d` (operand = 0x%X, index = %d, operand stack = %d)",
                    operand, index, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_D0: { /* store_d0 */
                /* Retrieve a 32-bit integer operand from the operand stack.
                 * It represents the low-part of the bit pattern of the 64-bit
                 * decimal value.
                 */
                // int32_t lowPart = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve a 32-bit integer operand from the operand stack.
                 * It represents the high-part of the bit pattern of the 64-bit
                 * decimal value.
                 */
                // int32_t highPart = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Store the retrieved operands in the local variable at index 0.
                 *
                 * A 64-bit decimal value occupies two indexes in the local variable
                 * array. The high part is stored at index 0. The low part is stored
                 * in the subsequent index, that is 1.
                 */
                // zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 0, highPart);
                // zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 1, lowPart);

                /* The above code implements the previous algorithm. The new algorithm is
                 * a humble attempt at optimizing the interpreter.
                 */

                /* Retrieve the operand from the operand stack. */
                int64_t operand = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Store the retrieved operand in the local variable at index 0. */
                zen_LocalVariableArray_setLong(currentStackFrame->m_localVariableArray, 0, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_d0` (operand = %l, index = 0, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_D1: { /* store_d1 */
                /* Retrieve a 32-bit integer operand from the operand stack.
                 * It represents the low-part of the bit pattern of the 64-bit
                 * decimal value.
                 */
                // int32_t lowPart = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve a 32-bit integer operand from the operand stack.
                 * It represents the high-part of the bit pattern of the 64-bit
                 * decimal value.
                 */
                // int32_t highPart = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Store the retrieved operands in the local variable at index 1.
                 *
                 * A 64-bit decimal value occupies two indexes in the local variable
                 * array. The high part is stored at index 1. The low part is stored
                 * in the subsequent index, that is 2.
                 */
                // zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 1, highPart);
                // zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 2, lowPart);

                /* The above code implements the previous algorithm. The new algorithm is
                 * a humble attempt at optimizing the interpreter.
                 */

                /* Retrieve the operand from the operand stack. */
                int64_t operand = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Store the retrieved operand in the local variable at index 1. */
                zen_LocalVariableArray_setLong(currentStackFrame->m_localVariableArray, 1, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_d1` (operand = %l, index = 1, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_D2: { /* store_d2 */
                /* Retrieve a 32-bit integer operand from the operand stack.
                 * It represents the low-part of the bit pattern of the 64-bit
                 * decimal value.
                 */
                // int32_t lowPart = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve a 32-bit integer operand from the operand stack.
                 * It represents the high-part of the bit pattern of the 64-bit
                 * decimal value.
                 */
                // int32_t highPart = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Store the retrieved operands in the local variable at index 2.
                 *
                 * A 64-bit decimal value occupies two indexes in the local variable
                 * array. The high part is stored at index 2. The low part is stored
                 * in the subsequent index, that is 3.
                 */
                // zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 2, highPart);
                // zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 3, lowPart);

                /* Retrieve the operand from the operand stack. */
                int64_t operand = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Store the retrieved operand in the local variable at index 0. */
                zen_LocalVariableArray_setLong(currentStackFrame->m_localVariableArray, 0, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_d2` (operand = %l, index = 1, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_D3: { /* store_d3 */
                /* Retrieve a 32-bit integer operand from the operand stack.
                 * It represents the low-part of the bit pattern of the 64-bit
                 * decimal value.
                 */
                // int32_t lowPart = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve a 32-bit integer operand from the operand stack.
                 * It represents the high-part of the bit pattern of the 64-bit
                 * decimal value.
                 */
                // int32_t highPart = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Store the retrieved operands in the local variable at index 3.
                 *
                 * A 64-bit decimal value occupies two indexes in the local variable
                 * array. The high part is stored at index 3. The low part is stored
                 * in the subsequent index, that is 4.
                 */
                // zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 3, highPart);
                // zen_LocalVariableArray_setInteger(currentStackFrame->m_localVariableArray, 4, lowPart);

                /* Retrieve the operand from the operand stack. */
                int64_t operand = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Store the retrieved operand in the local variable at index 3. */
                zen_LocalVariableArray_setLong(currentStackFrame->m_localVariableArray, 3, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_d3` (operand = %l, index = 3, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_A: { /* store_a */
                /* Read the index of the local variable to modify. */
                uint8_t index = zen_Interpreter_readByte(interpreter);
                /* Retrieve the operand from the operand stack. */
                intptr_t operand = zen_OperandStack_popReference(currentStackFrame->m_operandStack);
                /* Update the value of the local variable with the specified index. */
                zen_LocalVariableArray_setReference(currentStackFrame->m_localVariableArray, index, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_a` (operand = 0x%X, index = %d, operand stack = %d)",
                    operand, index, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_A0: { /* store_a0 */
                /* Retrieve the operand from the operand stack. It represents
                 * the address of an object.
                 */
                intptr_t operand = zen_OperandStack_popReference(currentStackFrame->m_operandStack);
                /* Store the retrieved address of the object in the local variable
                 * at index 0.
                 */
                zen_LocalVariableArray_setReference(currentStackFrame->m_localVariableArray, 0, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_a0` (operand = 0x%X, index = 0, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_A1: { /* store_a1 */
                /* Retrieve the operand from the operand stack. It represents
                 * the address of an object.
                 */
                intptr_t operand = zen_OperandStack_popReference(currentStackFrame->m_operandStack);
                /* Store the retrieved address of the object in the local variable
                 * at index 1.
                 */
                zen_LocalVariableArray_setReference(currentStackFrame->m_localVariableArray, 1, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_a1` (operand = 0x%X, index = 1, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_A2: { /* store_a2 */
                /* Retrieve the operand from the operand stack. It represents
                 * the address of an object.
                 */
                intptr_t operand = zen_OperandStack_popReference(currentStackFrame->m_operandStack);
                /* Store the retrieved address of the object in the local variable
                 * at index 2.
                 */
                zen_LocalVariableArray_setReference(currentStackFrame->m_localVariableArray, 2, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_a2` (operand = 0x%X, index = 2, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_A3: { /* store_a3 */
                /* Retrieve the operand from the operand stack. It represents
                 * the address of an object.
                 */
                intptr_t operand = zen_OperandStack_popReference(currentStackFrame->m_operandStack);
                /* Store the retrieved address of the object in the local variable
                 * at index 3.
                 */
                zen_LocalVariableArray_setReference(currentStackFrame->m_localVariableArray, 3, operand);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_a3` (operand = 0x%X, index = 3, operand stack = %d)",
                    operand, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_AB: { /* store_ab */
                break;
            }

            case ZEN_BYTE_CODE_STORE_AC: { /* store_ac */
                break;
            }

            case ZEN_BYTE_CODE_STORE_AS: { /* store_as */
                break;
            }

            case ZEN_BYTE_CODE_STORE_AI: { /* store_ai */
                break;
            }

            case ZEN_BYTE_CODE_STORE_AL: { /* store_al */
                break;
            }

            case ZEN_BYTE_CODE_STORE_AF: { /* store_af */
                break;
            }

            case ZEN_BYTE_CODE_STORE_AD: { /* store_ad */
                break;
            }

            case ZEN_BYTE_CODE_STORE_AA: { /* store_aa */
                /* Retrieve the value from the operand stack. */
                uintptr_t value = zen_OperandStack_popReference(currentStackFrame->m_operandStack);
                /* Retrieve the index from the operand stack. */
                int32_t index = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve the array from the operand stack. */
                jtk_Array_t* array = (jtk_Array_t*)zen_OperandStack_popReference(currentStackFrame->m_operandStack);
                /* Update the value at the specified index. */
                jtk_Array_setValue(array, index, value);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `store_aa` (value = 0x%X, index = %d, array = 0x%X, operand stack = %d)",
                    value, index, array, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_STORE_STATIC_FIELD: { /* store_static_field */
                break;
            }

            case ZEN_BYTE_CODE_STORE_INSTANCE_FIELD: { /* store_instance_field */
                break;
            }

            /* Subtract */

            case ZEN_BYTE_CODE_SUBTRACT_I: { /* subtract_i */
                /* Retrieve the second operand from the operand stack. */
                int32_t operand2 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int32_t operand1 = zen_OperandStack_popInteger(currentStackFrame->m_operandStack);
                /* Subtract the operands. Push the result on the operand stack. */
                int32_t result = operand1 - operand2;
                zen_OperandStack_pushInteger(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `subtract_i` (operand1 = %d, operand2 = %d, result = %d, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_SUBTRACT_L: { /* subtract_l */
                /* Retrieve the second operand from the operand stack. */
                int64_t operand2 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                int64_t operand1 = zen_OperandStack_popLong(currentStackFrame->m_operandStack);
                /* Subtract the operands. Push the result on the operand stack. */
                int64_t result = operand1 - operand2;
                zen_OperandStack_pushLong(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `subtract_l` (operand1 = %l, operand2 = %l, result = %l, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_SUBTRACT_F: { /* subtract_f */
                /* Retrieve the second operand from the operand stack. */
                float operand2 = zen_OperandStack_popFloat(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                float operand1 = zen_OperandStack_popFloat(currentStackFrame->m_operandStack);
                /* Subtract the operands. Push the result on the operand stack. */
                float result = operand1 - operand2;
                zen_OperandStack_pushFloat(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `subtract_f` (operand1 = %f, operand2 = %f, result = %f, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            case ZEN_BYTE_CODE_SUBTRACT_D: { /* subtract_d */
                /* Retrieve the second operand from the operand stack. */
                double operand2 = zen_OperandStack_popDouble(currentStackFrame->m_operandStack);
                /* Retrieve the first operand from the operand stack. */
                double operand1 = zen_OperandStack_popDouble(currentStackFrame->m_operandStack);
                /* Subtract the operands. Push the result on the operand stack. */
                double result = operand1 - operand2;
                zen_OperandStack_pushDouble(currentStackFrame->m_operandStack, result);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `subtract_d` (operand1 = %f, operand2 = %f, result = %f, operand stack = %d)",
                    operand1, operand2, result, zen_OperandStack_getSize(currentStackFrame->m_operandStack));

                break;
            }

            /* Swap */

            case ZEN_BYTE_CODE_SWAP: { /* swap */
                /* Swap the two operands on top of the stack. */
                zen_OperandStack_swap(currentStackFrame->m_operandStack);

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `swap`");

                break;
            }

            /* Switch */

            case ZEN_BYTE_CODE_SWITCH_TABLE: { /* switch_table */
                break;
            }

            case ZEN_BYTE_CODE_SWITCH_SEARCH: { /* switch_search */
                break;
            }

            /* Throw */

            case ZEN_BYTE_CODE_THROW: { /* throw */
                /* Retrieve the reference to the exception object from the operand stack. */
                uintptr_t reference = zen_OperandStack_popReference(currentStackFrame->m_operandStack);

                /* Retrieve the class of the exception object. */
                zen_Object_t* exception = (zen_Object_t*)reference;
                zen_Class_t* exceptionClass = zen_Object_getClass(exception);

                zen_ExceptionTable_t* exceptionTable = &instructionAttribute->m_exceptionTable;

                /* Recursively search the stack trace for the most appropriate
                 * exception handler, nearest to the current stack frame.
                 * The search results in popping of the stack frames. Which goes
                 * to say, the currently executing function may terminate.
                 */
                while (zen_InvocationStack_getSize(interpreter->m_invocationStack) > 0) {
                    if (exceptionTable->m_size > 0) {
                        /* Scan the exception table for an exception handler site with a catch
                         * filter corresponding to the exception currently being thrown.
                         */
                        int32_t siteIndex;
                        for (siteIndex = 0; siteIndex < exceptionTable->m_size; siteIndex++) {
                            zen_ExceptionHandlerSite_t* site = exceptionTable->m_exceptionHandlerSites[siteIndex];
                            /* Make sure that the exception was caused by an instruction
                             * within the boundaries of the exception handler site.
                             */
                            if ((currentStackFrame->m_ip >= site->m_startIndex) &&
                                (currentStackFrame->m_ip <= site->m_stopIndex)) {
                                zen_EntityFile_t* entityFile = currentStackFrame->m_class->m_entityFile;
                                zen_ConstantPool_t* constantPool = &entityFile->m_constantPool;
                                zen_ConstantPoolClass_t* classEntry =
                                    (zen_ConstantPoolClass_t*)constantPool->m_entries[site->m_exceptionClassIndex];
                                zen_ConstantPoolUtf8_t* nameEntry = constantPool->m_entries[classEntry->m_nameIndex];
                
                                jtk_String_t* classDescriptor = jtk_String_newEx(nameEntry->m_bytes, nameEntry->m_length);
                                zen_Class_t* filterClass = zen_VirtualMachine_getClass(interpreter->m_virtualMachine, classDescriptor);
                                jtk_String_delete(classDescriptor);

                                if (exceptionClass == filterClass) {
                                    /* Update the instruction pointer of the current stack frame to
                                     * the exception handler.
                                     */
                                    currentStackFrame->m_ip = site->m_handlerIndex;

                                    /* Push the reference to the exception object on top of the operand
                                     * stack belonging to the function with the suitable exception
                                     * handler. This reference is required by the "catch clause".
                                     */
                                    zen_OperandStack_pushReference(currentStackFrame->m_operandStack, reference);

                                    /* A suitable exception handler has been discovered. Terminate the search
                                    * loop.
                                    */
                                    break;
                                }
                            }
                        }
                    }

                    /* A suitable exception handler was not found. Move to the previous stack
                     * frame and repeat.
                     * 
                     * TODO: Create a stacktrace.
                     */
                    currentStackFrame = zen_InvocationStack_popStackFrame(interpreter->m_invocationStack);
                }

                /* No exception handler has been discovered in the stack trace. Invoke the thread
                 * level exception handler.
                 */
                if (zen_InvocationStack_getSize(interpreter->m_invocationStack) == 0) {
                    /* Invoking an exception handler function causes the interpreter to insert
                     * a stack frame. The primary loop continues normally. It is terminated
                     * when a `return` (includes the other variations, too) instruction is
                     * encountered.
                     */
                    zen_Interpreter_invokeThreadExceptionHandler(interpreter);
                }

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `throw`");

                break;
            }

            /* Wide */

            case ZEN_BYTE_CODE_WIDE: { /* wide */
                /* Flag the interpreter to switch to wide mode. Only certain instructions are
                 * supported in the wide mode.
                 */
                flags |= ZEN_INTERPRETER_FLAG_WIDE_MODE;

                /* Log debugging information for assistance in debugging the interpreter. */
                jtk_Logger_debug(logger, "Executed instruction `wide`");

                break;
            }

            default: {
                fprintf(stderr, "[error] Unknown instruction `%X`\n", instruction);
                break;
            }
        }
    }
}

/* Invoke Constructor */

typedef void (*zen_NativeFunction_InvokeConstructorFunction_t)(zen_VirtualMachine_t* virtualMachine, zen_Object_t* self, jtk_VariableArguments_t arguments);

void zen_Interpreter_invokeConstructor(zen_Interpreter_t* interpreter,
    zen_Object_t* object, zen_Function_t* constructor,
    jtk_Array_t* arguments) {
    zen_StackFrame_t* oldStackFrame = zen_InvocationStack_peekStackFrame(interpreter->m_invocationStack);
    zen_StackFrame_t* stackFrame = zen_StackFrame_new(constructor);
    zen_InvocationStack_pushStackFrame(interpreter->m_invocationStack, stackFrame);

    /* Pass the self reference to the constructor. */
    zen_LocalVariableArray_setReference(stackFrame->m_localVariableArray, 0, object);

    zen_EntityFile_t* entityFile = constructor->m_class->m_entityFile;
    zen_Entity_t* entity = &entityFile->m_entity;
    zen_ConstantPoolUtf8_t* name =
        (zen_ConstantPoolUtf8_t*)entityFile->m_constantPool.m_entries[
            entity->m_reference];
    jtk_String_t* className = jtk_String_newEx(name->m_bytes, name->m_length);

    if (zen_Function_isNative(constructor)) {
        zen_NativeFunction_t* nativeConstructor = zen_VirtualMachine_getNativeFunction(
            interpreter->m_virtualMachine, className, constructor->m_name,
            constructor->m_descriptor);

        if (nativeConstructor != NULL) {
            zen_NativeFunction_InvokeConstructorFunction_t invokeConstructor =
                (zen_NativeFunction_InvokeConstructorFunction_t)nativeConstructor->m_invoke;
            invokeConstructor(interpreter->m_virtualMachine, object, arguments);
        }
        else {
            printf("[error] Unknown native constructor (class=%.*s, name=%.*s, descriptor=%.*s)\n",
                className->m_size, className->m_value, constructor->m_name->m_size,
                constructor->m_name->m_value, constructor->m_descriptor->m_size,
                constructor->m_descriptor->m_value);
        }
    }
    else {
        zen_Interpreter_loadArguments(interpreter, constructor, stackFrame->m_localVariableArray,
            arguments, true);
        zen_Interpreter_interpret(interpreter);
    }

    jtk_String_delete(className);
    zen_InvocationStack_popStackFrame(interpreter->m_invocationStack);
    zen_StackFrame_delete(stackFrame);
}
/*
void zen_Interpreter_invokeConstructor(zen_Interpreter_t* interpreter,
    zen_Object_t* object, zen_Function_t* constructor,
    jtk_VariableArguments_t arguments) {

    zen_StackFrame_t* oldStackFrame = zen_InvocationStack_peekStackFrame(interpreter->m_invocationStack);
    zen_StackFrame_t* stackFrame = zen_StackFrame_new(constructor);
    zen_InvocationStack_pushStackFrame(interpreter->m_invocationStack, stackFrame);

    zen_EntityFile_t* entityFile = constructor->m_class->m_entityFile;
    zen_Entity_t* entity = &entityFile->m_entity;
    zen_ConstantPoolUtf8_t* name =
        (zen_ConstantPoolUtf8_t*)entityFile->m_constantPool.m_entries[
            entity->m_reference];
    jtk_String_t* className = jtk_String_newEx(name->m_bytes, name->m_length);

    if (zen_Function_isNative(constructor)) {
        zen_NativeFunction_t* nativeConstructor = zen_VirtualMachine_getNativeFunction(
            interpreter->m_virtualMachine, className, constructor->m_name,
            constructor->m_descriptor);

        if (nativeConstructor != NULL) {
            zen_NativeFunction_InvokeConstructorFunction_t invokeConstructor =
                (zen_NativeFunction_InvokeConstructorFunction_t)nativeConstructor->m_invoke;
            invokeConstructor(interpreter->m_virtualMachine, object, arguments);
        }
        else {
            printf("[error] Unknown native constructor (class=%.*s, name=%.*s, descriptor=%.*s)\n",
                className->m_size, className->m_value, constructor->m_name->m_size,
                constructor->m_name->m_value, constructor->m_descriptor->m_size,
                constructor->m_descriptor->m_value);
        }
    }
    else {
        zen_Interpreter_interpret(interpreter);
    }

    jtk_String_delete(className);
    zen_InvocationStack_popStackFrame(interpreter->m_invocationStack);
    zen_StackFrame_delete(stackFrame);
}
*/

/* Invoke Native */

void zen_Interpreter_invokeNativeFunction(zen_Interpreter_t* interpreter,
    zen_Class_t* targetClass, zen_Function_t* targetFunction, zen_OperandStack_t* operandStack) {
}

/* Invoke Static Function */

zen_Object_t* zen_Interpreter_invokeStaticFunction(zen_Interpreter_t* interpreter,
    zen_Function_t* function, jtk_Array_t* arguments) {

    zen_StackFrame_t* oldStackFrame = zen_InvocationStack_peekStackFrame(interpreter->m_invocationStack);
    zen_StackFrame_t* stackFrame = zen_StackFrame_new(function);
    zen_InvocationStack_pushStackFrame(interpreter->m_invocationStack, stackFrame);

    zen_EntityFile_t* entityFile = function->m_class->m_entityFile;
    zen_Entity_t* entity = &entityFile->m_entity;
    zen_ConstantPoolUtf8_t* name =
        (zen_ConstantPoolUtf8_t*)entityFile->m_constantPool.m_entries[
            entity->m_reference];
    jtk_String_t* className = jtk_String_newEx(name->m_bytes, name->m_length);

    zen_Object_t* result = NULL;
    if (zen_Function_isNative(function)) {
        zen_NativeFunction_t* nativeFunction = zen_VirtualMachine_getNativeFunction(
            interpreter->m_virtualMachine, className, function->m_name, function->m_descriptor);

        if (nativeFunction != NULL) {
            zen_NativeFunction_InvokeFunction_t invoke = nativeFunction->m_invoke;
            result = invoke(interpreter->m_virtualMachine, NULL, arguments);

            if (function->m_returnType != ZEN_TYPE_VOID) {
                zen_OperandStack_pushReference(oldStackFrame->m_operandStack, result);
            }
        }
        else {
            printf("[error] Unknown native function (class=%.*s, name=%.*s, descriptor=%.*s)\n",
                className->m_size, className->m_value, function->m_name->m_size,
                function->m_name->m_value, function->m_descriptor->m_size,
                function->m_descriptor->m_value);
        }
    }
    else {
        zen_Interpreter_loadArguments(interpreter, function, stackFrame->m_localVariableArray,
            arguments, false);
        zen_Interpreter_interpret(interpreter);
        // result = zen_OperandStack_popReference(stackFrame->m_operandStack);

        if (function->m_returnType != ZEN_TYPE_VOID) {
            result = zen_OperandStack_popReference(stackFrame->m_operandStack);
            zen_OperandStack_pushReference(oldStackFrame->m_operandStack, result);
        }
    }

    zen_InvocationStack_popStackFrame(interpreter->m_invocationStack);
    zen_StackFrame_delete(stackFrame);

    return result;
}

void zen_Interpreter_invokeStaticFunctionEx(zen_Interpreter_t* interpreter,
    zen_Function_t* function, jtk_VariableArguments_t variableArguments) {

    zen_StackFrame_t* stackFrame = zen_StackFrame_new(function);
    zen_InvocationStack_pushStackFrame(interpreter->m_invocationStack, stackFrame);
    zen_Interpreter_interpret(interpreter);
}

/* Invoke Virtual Function */

zen_Object_t* zen_Interpreter_invokeVirtualFunction(zen_Interpreter_t* interpreter,
    zen_Function_t* function, zen_Object_t* object, jtk_Array_t* arguments) {
    zen_StackFrame_t* oldStackFrame = zen_InvocationStack_peekStackFrame(interpreter->m_invocationStack);
    zen_StackFrame_t* stackFrame = zen_StackFrame_new(function);
    zen_InvocationStack_pushStackFrame(interpreter->m_invocationStack, stackFrame);

    zen_LocalVariableArray_setReference(stackFrame->m_localVariableArray, 0, object);

    zen_EntityFile_t* entityFile = function->m_class->m_entityFile;
    zen_Entity_t* entity = &entityFile->m_entity;
    zen_ConstantPoolUtf8_t* name =
        (zen_ConstantPoolUtf8_t*)entityFile->m_constantPool.m_entries[
            entity->m_reference];
    jtk_String_t* className = jtk_String_newEx(name->m_bytes, name->m_length);

    zen_Object_t* result = NULL;
    if (zen_Function_isNative(function)) {
        zen_NativeFunction_t* nativeFunction = zen_VirtualMachine_getNativeFunction(
            interpreter->m_virtualMachine, className, function->m_name, function->m_descriptor);

        if (nativeFunction != NULL) {
            zen_NativeFunction_InvokeFunction_t invoke = nativeFunction->m_invoke;
            result = invoke(interpreter->m_virtualMachine, object, arguments);

            if (function->m_returnType != ZEN_TYPE_VOID) {
                zen_OperandStack_pushReference(oldStackFrame->m_operandStack, result);
            }
        }
        else {
            printf("[error] Unknown native function (class=%.*s, name=%.*s, descriptor=%.*s)\n",
                className->m_size, className->m_value, function->m_name->m_size,
                function->m_name->m_value, function->m_descriptor->m_size,
                function->m_descriptor->m_value);
        }
    }
    else {
        zen_Interpreter_loadArguments(interpreter, function, stackFrame->m_localVariableArray,
            arguments, true);
        zen_Interpreter_interpret(interpreter);

        if (function->m_returnType != ZEN_TYPE_VOID) {
            result = zen_OperandStack_popReference(stackFrame->m_operandStack);
            zen_OperandStack_pushReference(oldStackFrame->m_operandStack, result);
        }
    }

    zen_InvocationStack_popStackFrame(interpreter->m_invocationStack);
    zen_StackFrame_delete(stackFrame);

    return result;
}

/* Invoke Thread Exception Handler */

void zen_Interpreter_invokeThreadExceptionHandler(zen_Interpreter_t* interpreter) {
    printf("[error] An exception was thrown!\n");
}

/* Load Arguments */

void zen_Interpreter_loadArguments(zen_Interpreter_t* interpreter,
    zen_Function_t* function, zen_LocalVariableArray_t* array,
    jtk_Array_t* arguments, bool instance) {
    int32_t i;
    int32_t j;
    /* Start from the second slot, if the specified function is an instance function.
     * The first slot is reserved for self reference.
     */
    j = instance? ZEN_LOCAL_VARIABLE_ARRAY_REFERENCE_SLOT_COUNT : 0;
    for (i = 0; i < function->m_parameterCount; i++) {
        zen_Type_t parameterType = (zen_Type_t)function->m_parameters[i * 2];
        switch (parameterType) {
            case ZEN_TYPE_BOOLEAN: {
                // TODO: Change the arguments type from jtk_Array_t to zen_Arguments_t!
                bool argument = (bool)jtk_Array_getValue(arguments, i);
                zen_LocalVariableArray_setInteger(array, j, (int32_t)argument);
                j += 1;
                break;
            }

            case ZEN_TYPE_CHARACTER: {
                // TODO: Change the arguments type from jtk_Array_t to zen_Arguments_t!
                // TODO: Is character 1 byte?
                uint8_t argument = (uint8_t)jtk_Array_getValue(arguments, i);
                zen_LocalVariableArray_setInteger(array, j, (int32_t)argument);
                j += 1;
                break;
            }

            case ZEN_TYPE_INTEGER_8: {
                // TODO: Change the arguments type from jtk_Array_t to zen_Arguments_t!
                int8_t argument = (int8_t)jtk_Array_getValue(arguments, i);
                zen_LocalVariableArray_setInteger(array, j, (int32_t)argument);
                j += 1;
                break;
            }

            case ZEN_TYPE_INTEGER_16: {
                // TODO: Change the arguments type from jtk_Array_t to zen_Arguments_t!
                int16_t argument = (int16_t)jtk_Array_getValue(arguments, i);
                zen_LocalVariableArray_setInteger(array, j, (int32_t)argument);
                j += 1;
                break;
            }

            case ZEN_TYPE_INTEGER_32: {
                // TODO: Change the arguments type from jtk_Array_t to zen_Arguments_t!
                int32_t argument = (int32_t)jtk_Array_getValue(arguments, i);
                zen_LocalVariableArray_setInteger(array, j, argument);
                j += 1;
                break;
            }

            case ZEN_TYPE_DECIMAL_32: {
                // TODO: Change the arguments type from jtk_Array_t to zen_Arguments_t!
                // NOTE: We are using bit pattern instead of float values.
                // Otherwise, all the casting could result in loss of value.
                int32_t argument = (int32_t)jtk_Array_getValue(arguments, i);
                zen_LocalVariableArray_setInteger(array, j, argument);
                j += 1;
                break;
            }

            case ZEN_TYPE_INTEGER_64: {
                // TODO: Change the arguments type from jtk_Array_t to zen_Arguments_t!
                int64_t argument = (int64_t)jtk_Array_getValue(arguments, i);
                zen_LocalVariableArray_setLong(array, j, argument);
                j += 1;
                break;
            }

            case ZEN_TYPE_DECIMAL_64: {
                // TODO: Change the arguments type from jtk_Array_t to zen_Arguments_t!
                // NOTE: We are using bit pattern instead of double values.
                // Otherwise, all the casting could result in loss of value.
                int64_t argument = (int64_t)jtk_Array_getValue(arguments, i);
                zen_LocalVariableArray_setLong(array, j, argument);
                j += 1;
                break;
            }
            
            case ZEN_TYPE_REFERENCE: {
                void* argument = jtk_Array_getValue(arguments, i);
                zen_LocalVariableArray_setReference(array, j, argument);
                j += ZEN_LOCAL_VARIABLE_ARRAY_REFERENCE_SLOT_COUNT;
                break;
            }

            default: {
                printf("[error] Invalid parameter type %d!\n", parameterType);
            }
        }
    }
}

/* Read */

uint8_t zen_Interpreter_readByte(zen_Interpreter_t* interpreter) {
    zen_StackFrame_t* currentStackFrame = zen_InvocationStack_peekStackFrame(interpreter->m_invocationStack);
    zen_InstructionAttribute_t* instructionAttribute = currentStackFrame->m_instructionAttribute;

    return instructionAttribute->m_instructions[currentStackFrame->m_ip++];
}

uint16_t zen_Interpreter_readShort(zen_Interpreter_t* interpreter) {
zen_StackFrame_t* currentStackFrame = zen_InvocationStack_peekStackFrame(interpreter->m_invocationStack);
    zen_InstructionAttribute_t* instructionAttribute = currentStackFrame->m_instructionAttribute;

    uint8_t byte0 = instructionAttribute->m_instructions[currentStackFrame->m_ip++];
    uint8_t byte1 = instructionAttribute->m_instructions[currentStackFrame->m_ip++];
    return (byte0 << 8) | byte1;
}






uint8_t* zen_MemoryManager_allocateEx(zen_MemoryManager_t* manager, uint32_t size, zen_AlignmentConstraint_t alignment,
    int32_t flags) {
    return NULL;
}