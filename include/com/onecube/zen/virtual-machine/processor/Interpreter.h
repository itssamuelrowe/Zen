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

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_INTERPRETER_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_INTERPRETER_H

#include <jtk/log/Logger.h>
#include <jtk/core/VariableArguments.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/object/Class.h>
#include <com/onecube/zen/virtual-machine/object/Function.h>
#include <com/onecube/zen/virtual-machine/object/NativeFunction.h>
#include <com/onecube/zen/virtual-machine/processor/InvocationStack.h>
#include <com/onecube/zen/virtual-machine/processor/ProcessorThread.h>

#define ZEN_INTERPRETER_NULL_REFERENCE 0

typedef struct zen_VirtualMachine_t zen_VirtualMachine_t;

/*******************************************************************************
 * Interpreter                                                                 *
 *******************************************************************************/

/**
 * @class Interpreter
 * @ingroup zen_vm_processor
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_Interpreter_t {
    int32_t m_counter;
    zen_InvocationStack_t* m_invocationStack;
    zen_ProcessorThread_t* m_processorThread;
    jtk_Logger_t* m_logger;
    zen_VirtualMachine_t* m_virtualMachine;
};

/**
 * @memberof Interpreter
 */
typedef struct zen_Interpreter_t zen_Interpreter_t;

/* Constructor */

/**
 * @memberof Interpreter
 */
zen_Interpreter_t* zen_Interpreter_new(zen_MemoryManager_t* memoryManager,
    zen_VirtualMachine_t* virtualMachine, zen_ProcessorThread_t* processorThread);

/* Destructor */

/**
 * @memberof Interpreter
 */
void zen_Interpreter_delete(zen_Interpreter_t* interpreter);

/* Invoke Constructor */

void zen_Interpreter_invokeConstructor(zen_Interpreter_t* interpreter,
    zen_Object_t* object, zen_Function_t* constructor,
    jtk_VariableArguments_t arguments);
    
/* Invoke Static Function */

void zen_Interpreter_invokeStaticFunctionEx(zen_Interpreter_t* interpreter,
    zen_Function_t* function, jtk_VariableArguments_t variableArguments);

/* Read */

uint8_t zen_Interpreter_readByte(zen_Interpreter_t* interpreter);
uint16_t zen_Interpreter_readShort(zen_Interpreter_t* interpreter);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_INTERPRETER_H */