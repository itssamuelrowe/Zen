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

// Monday, July 15, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_NATIVE_FUNCTION_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_NATIVE_FUNCTION_H

#include <jtk/collection/array/Array.h>
#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * NativeFunction                                                              *
 *******************************************************************************/

typedef struct zen_VirtualMachine_t zen_VirtualMachine_t;

typedef struct zen_Object_t zen_Object_t;

/**
 * @memberof NativeFunction
 */
typedef zen_Object_t* (*zen_NativeFunction_InvokeFunction_t)(zen_VirtualMachine_t* virtualMachine, jtk_Array_t* arguments);

/**
 * @class NativeFunction
 * @ingroup zen_virtualMachine_object
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_NativeFunction_t {
    zen_NativeFunction_InvokeFunction_t m_invoke;
};

/**
 * @memberof NativeFunction
 */
typedef struct zen_NativeFunction_t zen_NativeFunction_t;

// Constructor

/**
 * @memberof NativeFunction
 */
zen_NativeFunction_t* zen_NativeFunction_new(zen_NativeFunction_InvokeFunction_t invoke);

// Destructor

/**
 * @memberof NativeFunction
 */
void zen_NativeFunction_delete(zen_NativeFunction_t* function);
 
#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_NATIVE_FUNCTION_H */