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

// Sunday, September 16, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_STACK_FRAME_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_STACK_FRAME_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/feb/attribute/InstructionAttribute.h>
#include <com/onecube/zen/virtual-machine/object/Class.h>
#include <com/onecube/zen/virtual-machine/object/Function.h>
#include <com/onecube/zen/virtual-machine/processor/OperandStack.h>
#include <com/onecube/zen/virtual-machine/processor/LocalVariableArray.h>

/*******************************************************************************
 * StackFrame                                                                  *
 *******************************************************************************/

/**
 * @memberof StackFrame
 */
typedef struct zen_StackFrame_t zen_StackFrame_t;

/**
 * @class StackFrame
 * @ingroup zen_vm_processor
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_StackFrame_t {
    zen_OperandStack_t* m_operandStack;
    zen_LocalVariableArray_t* m_localVariableArray;
    zen_Class_t* m_class;
    zen_Function_t* m_function;
    zen_InstructionAttribute_t* m_instructionAttribute;
    int32_t m_ip;
};

/* Constructor */

/**
 * @memberof StackFrame
 */
zen_StackFrame_t* zen_StackFrame_new(zen_Function_t* function);

/* Destructor */

/**
 * @memberof StackFrame
 */
void zen_StackFrame_delete(zen_StackFrame_t* frame);

/* Class */

/**
 * @memberof StackFrame
 */
zen_Class_t* zen_StackFrame_getClass(zen_StackFrame_t* frame);

/* Function */

/**
 * @memberof StackFrame
 */
zen_Function_t* zen_StackFrame_getFunction(zen_StackFrame_t* frame);

/* Local Variable Array */

zen_LocalVariableArray_t* zen_StackFrame_getLocalVariableArray(zen_StackFrame_t* frame);

/* Operand Stack */

/**
 * @memberof StackFrame
 */
zen_OperandStack_t* zen_StackFrame_getOperandStack(zen_StackFrame_t* frame);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_STACK_FRAME_H */
