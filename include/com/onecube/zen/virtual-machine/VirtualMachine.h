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

// Friday, May 18, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_VIRTUAL_MACHINE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_VIRTUAL_MACHINE_H

#include <com/onecube/zen/Configuration.h>

#include <jtk/collection/map/HashMap.h>
#include <jtk/collection/array/Array.h>
#include <jtk/log/Logger.h>

#include <com/onecube/zen/virtual-machine/ExceptionManager.h>
#include <com/onecube/zen/virtual-machine/VirtualMachineConfiguration.h>
#include <com/onecube/zen/virtual-machine/object/Class.h>
#include <com/onecube/zen/virtual-machine/object/Function.h>
#include <com/onecube/zen/virtual-machine/object/NativeFunction.h>
#include <com/onecube/zen/virtual-machine/object/ObjectArray.h>
#include <com/onecube/zen/virtual-machine/loader/EntityLoader.h>
#include <com/onecube/zen/virtual-machine/loader/ClassLoader.h>
#include <com/onecube/zen/virtual-machine/processor/Interpreter.h>

/*******************************************************************************
 * VirtualMachine                                                              *
 *******************************************************************************/

/**
 * @class VirtualMachine
 * @ingroup zen_virtual_machine_native
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_VirtualMachine_t {
    jtk_Logger_t* m_logger;
    zen_VirtualMachineConfiguration_t* m_configuration;
    zen_EntityLoader_t* m_entityLoader;
    zen_ClassLoader_t* m_classLoader;
    zen_Interpreter_t* m_interpreter;
    jtk_HashMap_t* m_nativeFunctions;
};

/**
 * @memberof VirtualMachine
 */
typedef struct zen_VirtualMachine_t zen_VirtualMachine_t;

// Identity Hash

int32_t zen_VirtualMachine_identityHash(zen_Object_t* object);

// Constructor

/**
 * @memberof VirtualMachine
 */
zen_VirtualMachine_t* zen_VirtualMachine_new(zen_VirtualMachineConfiguration_t* configuration);

// Destructor

/**
 * @memberof VirtualMachine
 */
void zen_VirtualMachine_delete(zen_VirtualMachine_t* virtualMachine);

// Array

zen_ObjectArray_t* zen_VirtualMachine_newObjectArray(zen_VirtualMachine_t* virtualMachine,
    zen_Class_t* class0, int32_t size);

zen_ObjectArray_t* zen_VirtualMachine_newByteArray(
    zen_VirtualMachine_t* virtualMachine, int8_t* bytes, int32_t size);

// Boolean

zen_Object_t* zen_VirtualMachine_newBoolean(zen_VirtualMachine_t* virtualMachine,
    bool value);

// Context

/**
 * The standard provides two classes to manipulate the virtual machine directly.
 * - `VirtualMachine`
 *   The `VirtualMachine` class provides a low-level API.
 * - `Context`
 *   The `Context` class provides a high-level API. Internally, this class is
 *   powered by the low-level APIs provided by the `VirtualMachine` class.
 */
// zen_VirtualMachineContext_t* zen_VirtualMachine_getContext(zen_VirtualMachine_t* virtualMachine);

// Class

zen_Class_t* zen_VirtualMachine_getClass(zen_VirtualMachine_t* context, jtk_String_t* descriptor);
zen_Class_t* zen_VirtualMachine_getClassEx(zen_VirtualMachine_t* context, const uint8_t* descriptor, int32_t size);

/* Clear */

bool zen_VirtualMachine_isClear(zen_VirtualMachine_t* virtualMachine);

// Entity Loader

zen_EntityLoader_t* zen_VirtualMachine_getEntityLoader(
    zen_VirtualMachine_t* virtualMachine);

// Field

void zen_VirtualMachine_setObjectField(zen_VirtualMachine_t* virtualMachine,
    zen_Object_t* object, const uint8_t* fieldDescriptor, int32_t fieldDescriptorSize,
    zen_Object_t* value);

zen_Object_t* zen_VirtualMachine_getObjectField(zen_VirtualMachine_t* virtualMachine,
    zen_Object_t* object, uint8_t* fieldName, int32_t fieldNameSize);

// Integer

zen_Object_t* zen_VirtualMachine_newInteger(zen_VirtualMachine_t* virtualMachine,
    int64_t value);

// Libraries

void zen_VirtualMachine_loadDefaultLibraries(zen_VirtualMachine_t* virtualMachine);

void zen_VirtualMachine_unloadLibraries(zen_VirtualMachine_t* virtualMachine);

// Native Function

zen_NativeFunction_t* zen_VirtualMachine_registerNativeFunction(
    zen_VirtualMachine_t* virtualMachine, const uint8_t* className,
    int32_t classNameSize, const uint8_t* functionName, int32_t functionNameSize,
    const uint8_t* functionDescriptor, int32_t functionDescriptorSize,
    zen_NativeFunction_InvokeFunction_t function);

zen_NativeFunction_t* zen_VirtualMachine_getNativeFunction(
    zen_VirtualMachine_t* virtualMachine, jtk_String_t* className,
    jtk_String_t* functionName, jtk_String_t* functionDescriptor);

// Object

zen_Object_t* zen_VirtualMachine_allocateObject(zen_VirtualMachine_t* virtualMachine,
    zen_Class_t* class0);

zen_Object_t* zen_VirtualMachine_makeObjectEx(zen_VirtualMachine_t* virtualMachine,
    zen_Function_t* constructor, jtk_Array_t* arguments);

/*
zen_Object_t* zen_VirtualMachine_makeObject(zen_VirtualMachine_t* virtualMachine,
    zen_Function_t* constructor, ...);
*/

zen_Object_t* zen_VirtualMachine_newObjectEx(zen_VirtualMachine_t* virtualMachine,
    const uint8_t* classDescriptor, int32_t classDescriptorSize,
    const uint8_t* constructorDescriptor, int32_t constructorDescriptorSize,
    jtk_Array_t* arguments);

/*
zen_Object_t* zen_VirtualMachine_newObject(zen_VirtualMachine_t* virtualMachine,
    const uint8_t* classDescriptor, int32_t classDescriptorSize,
    const uint8_t* constructorDescriptor, int32_t constructorDescriptorSize, ...);
*/

// Raise Exception

void zen_VirtualMachine_raiseException(zen_VirtualMachine_t* virtualMachine,
    zen_Object_t* exception);

void zen_VirtualMachine_raiseClassNotFoundException(zen_VirtualMachine_t* virtualMachine,
    const uint8_t* reason);

void zen_VirtualMachine_raiseFunctionNotFoundException(zen_VirtualMachine_t* virtualMachine,
    const uint8_t* identifier,  const uint8_t* reason);

void zen_VirtualMachine_raiseUnknownFieldException(zen_VirtualMachine_t* virtualMachine,
    const uint8_t* fieldDescriptor, int32_t fieldDescriptorSize);

void zen_VirtualMachine_raiseNullReferenceException(zen_VirtualMachine_t* virtualMachine);

// Shutdown

void zen_VirtualMachine_shutDown(zen_VirtualMachine_t* virtualMachine);
void zen_VirtualMachine_notifyShutDown(zen_VirtualMachine_t* virtualMachine);
void zen_VirtualMachine_tearDown(zen_VirtualMachine_t* virtualMachine);

// Static Function

zen_Function_t* zen_VirtualMachine_getStaticFunction(zen_VirtualMachine_t* virtualMachine,
    zen_Class_t* handle, jtk_String_t* identifier, jtk_String_t* signature);

// String

zen_Object_t* zen_VirtualMachine_getEmptyString(zen_VirtualMachine_t* virtualMachine);
zen_Object_t* zen_VirtualMachine_newStringFromUtf8(zen_VirtualMachine_t* virtualMachine,
    const uint8_t* string, int32_t size);
zen_Object_t* zen_VirtualMachine_newString(zen_VirtualMachine_t* virtualMachine,
    const uint8_t* string);

// Thread

void zen_VirtualMachine_waitForThreads(zen_VirtualMachine_t* virtualMachine);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_VIRTUAL_MACHINE_H */