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

// Friday, June 08, 2018

#include <jtk/core/VariableArguments.h>
#include <jtk/core/StringBuilder.h>
#include <com/onecube/zen/virtual-machine/VirtualMachine.h>
#include <com/onecube/zen/virtual-machine/loader/EntityLoader.h>
#include <com/onecube/zen/virtual-machine/ExceptionManager.h>

void zen_print(zen_VirtualMachine_t* virtualMachine, jtk_Array_t* arguments) {
    jtk_String_t* format = (jtk_String_t*)jtk_Array_getValue(arguments, 0);
    fwrite(format->m_value, 1, format->m_size, stdout);
    puts("");
    fflush(stdout);
}

jtk_String_t* zen_String_add(zen_VirtualMachine_t* virtualMachine,
    jtk_Array_t* arguments) {
    jtk_String_t* self = (jtk_String_t*)jtk_Array_getValue(arguments, 0);
    jtk_String_t* other = (jtk_String_t*)jtk_Array_getValue(arguments, 1);
    jtk_String_t* result = jtk_String_newFromJoinEx(self->m_value, self->m_size,
        other->m_value, other->m_size);
    
    printf("%.*s\n", result->m_size, result->m_value);
    
    return result;
}

/* TODO: The evaluate function builds a cache of operator-function pairs
 * as the program runs. This helps in reducing lookup time for mapping operators
 * to functions.
 */
void* zen_ZenKernel_evaluate(zen_VirtualMachine_t* virtualMachine,
    jtk_Array_t* arguments) {
    return zen_String_add(virtualMachine, arguments);
}

void zen_ZenKernel_invokeStatic(zen_VirtualMachine_t* virtualMachine,
    jtk_Array_t* arguments) {
    jtk_String_t* entity = (jtk_String_t*)jtk_Array_getValue(arguments, 0);
    jtk_String_t* targetFunctionName = (jtk_String_t*)jtk_Array_getValue(arguments, 1);
    jtk_Array_t* targetArguments = (jtk_Array_t*)jtk_Array_getValue(arguments, 2);
    jtk_StringBuilder_t* builder = jtk_StringBuilder_new();
    jtk_StringBuilder_append_z(builder, "(zen/core/Object):", 18);
    int32_t i;
    int32_t parameterCount = jtk_Array_getSize(targetArguments);
    for (i = 0; i < parameterCount; i++) {
        jtk_StringBuilder_append_z(builder, "(zen/core/Object)", 17);
    }
    jtk_String_t* targetFunctionDescriptor = jtk_StringBuilder_toString(builder);

    zen_Class_t* targetClass = zen_VirtualMachine_getClass(virtualMachine,
        entity);
    zen_Function_t* targetFunction = zen_VirtualMachine_getStaticFunction(virtualMachine,
        targetClass, targetFunctionName, targetFunctionDescriptor);
    
    zen_Interpreter_invokeStaticFunction(virtualMachine->m_interpreter, targetFunction,
        targetArguments);
}

/*******************************************************************************
 * VirtualMachine                                                              *
 *******************************************************************************/

/* Constructor */

zen_VirtualMachine_t* zen_VirtualMachine_new(zen_VirtualMachineConfiguration_t* configuration) {
    jtk_Assert_assertObject(configuration, "The specified virtual machine configuration is null.");

    jtk_ObjectAdapter_t* stringObjectAdapter = jtk_StringObjectAdapter_getInstance();
    jtk_Iterator_t* entityDirectoryIterator = jtk_ArrayList_getIterator(configuration->m_entityDirectories);

    zen_VirtualMachine_t* virtualMachine = zen_Memory_allocate(zen_VirtualMachine_t, 1);
    virtualMachine->m_configuration = configuration;
    virtualMachine->m_entityLoader = zen_EntityLoader_newWithEntityDirectories(entityDirectoryIterator);
    virtualMachine->m_classLoader = zen_ClassLoader_new(virtualMachine->m_entityLoader);
    virtualMachine->m_interpreter = zen_Interpreter_new(NULL, virtualMachine, NULL);
    virtualMachine->m_nativeFunctions = jtk_HashMap_newEx(stringObjectAdapter, NULL,
        JTK_HASH_MAP_DEFAULT_CAPACITY, JTK_HASH_MAP_DEFAULT_LOAD_FACTOR);

    zen_VirtualMachine_loadDefaultLibraries(virtualMachine);

    return virtualMachine;
}

/* Destructor */

void zen_VirtualMachine_delete(zen_VirtualMachine_t* virtualMachine) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");

    zen_VirtualMachine_unloadLibraries(virtualMachine);

    zen_ClassLoader_delete(virtualMachine->m_classLoader);
    zen_EntityLoader_delete(virtualMachine->m_entityLoader);
    jtk_Memory_deallocate(virtualMachine);
}

/* Context */
/*
zen_VirtualMachine_t* zen_VirtualMachine_getContext(zen_VirtualMachine_t* virtualMachine) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");

    return virtualMachine->m_context;
}
*/

/* Class */

zen_Class_t* zen_VirtualMachine_getClass(zen_VirtualMachine_t* virtualMachine,
    jtk_String_t* descriptor) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    jtk_Assert_assertObject(descriptor, "The specified class descriptor is null.");

    zen_Class_t* class0 = zen_ClassLoader_findClass(
        virtualMachine->m_classLoader, descriptor);

    if (class0 == NULL) {
        zen_VirtualMachine_raiseClassNotFoundException(virtualMachine, descriptor);
    }

    return class0;
}

/* Load Library */

void zen_VirtualMachine_loadDefaultLibraries(zen_VirtualMachine_t* virtualMachine) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");

    zen_NativeFunction_t* printFunction = zen_NativeFunction_new(zen_print);
    jtk_String_t* printKey = jtk_String_newEx("print(zen/core/Object):(zen/core/Object)", 40);
    jtk_HashMap_put(virtualMachine->m_nativeFunctions, printKey, printFunction);

    zen_NativeFunction_t* invokeStaticFunction = zen_NativeFunction_new(zen_ZenKernel_invokeStatic);
    jtk_String_t* invokeStaticKey = jtk_String_newEx("invokeStatic(zen/core/Object):(zen/core/Object)(zen/core/Object)@(zen/core/Object)", 82);
    jtk_HashMap_put(virtualMachine->m_nativeFunctions, invokeStaticKey, invokeStaticFunction);

    zen_NativeFunction_t* evaluateFunction = zen_NativeFunction_new(zen_ZenKernel_evaluate);
    jtk_String_t* evaluateKey = jtk_String_newEx("evaluate(zen/core/Object):(zen/core/Object)(zen/core/Object)(zen/core/Object)", 77);
    jtk_HashMap_put(virtualMachine->m_nativeFunctions, evaluateKey, evaluateFunction);

    // TODO: Unload native functions
}

void zen_VirtualMachine_unloadLibraries(zen_VirtualMachine_t* virtualMachine) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");

    jtk_Iterator_t* entryIterator = jtk_HashMap_getEntryIterator(virtualMachine->m_nativeFunctions);
    while (jtk_Iterator_hasNext(entryIterator)) {
        jtk_HashMapEntry_t* entry = (jtk_HashMapEntry_t*)jtk_Iterator_getNext(entryIterator);
        jtk_String_t* key = (jtk_String_t*)jtk_HashMapEntry_getKey(entry);
        zen_NativeFunction_t* value = (zen_NativeFunction_t*)jtk_HashMapEntry_getValue(entry);

        jtk_String_delete(key);
        zen_NativeFunction_delete(value);
    }
}

/* Raise Exception */

void zen_VirtualMachine_raiseClassNotFoundException(zen_VirtualMachine_t* virtualMachine,
    const uint8_t* reason) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    jtk_Assert_assertObject(reason, "The specified class descriptor is null.");

    /* Find the `ClassNotFoundException` class. */
    // const uint8_t* descriptor = "zen.core.ClassNotFoundException";

    // zen_Class_t* handle = NULL;

    // zen_EntityLoader_findClass(entityLoader, descriptor);
    // zen_EntityLoader_findEntity(virtualMachine->m_entityLoader, descriptor);

    /* The message of the exception. */
    // zen_Object_t* message = zen_VirtualMachine_newString(virtualMachine, reason);

    /* The descriptor of the constructor which will initializer the exception
     * object.
     */
    // const uint8_t* constructor = "(Czen.core.String;)";

    /* Create an instance of the `zen.core.ClassNotFoundException` class. */
    // zen_Object_t* exception = zen_VirtualMachine_newObject(virtualMachine, descriptor, constructor, message);

    /* Throw the exception. */
    // zen_VirtualMachine_raiseException(virtualMachine, exception);
}

void zen_VirtualMachine_raiseFunctionNotFoundException(zen_VirtualMachine_t* virtualMachine,
    const uint8_t* identifier, const uint8_t* reason) {
    // jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    // jtk_Assert_assertObject(reason, "The specified class descriptor is null.");

    /* Find the `FunctionNotFoundException` class. */
    // const uint8_t* descriptor = "zen.core.FunctionNotFoundException";
    // zen_Class_t* handle = NULL;

    // zen_EntityLoader_findClass(entityLoader, descriptor);
    // zen_EntityLoader_findEntity(virtualMachine->m_entityLoader, descriptor);

    /* The message of the exception. */
    // zen_Object_t* message = zen_VirtualMachine_newString(virtualMachine, reason);

    /* The descriptor of the constructor which will initializer the exception
     * object.
     */
    // const uint8_t* constructor = "(Czen.core.String;)";

    /* Create an instance of the `zen.core.ClassNotFoundException` class. */
    // zen_Object_t* exception = zen_VirtualMachine_newObject(virtualMachine, descriptor, constructor, message);

    /* Throw the exception. */
    // zen_VirtualMachine_raiseException(virtualMachine, exception);
}

/* Clear */

bool zen_VirtualMachine_isClear(zen_VirtualMachine_t* virtualMachine) {
    zen_ExceptionManager_t* manager = zen_VirtualMachine_getExceptionManager(virtualMachine);
    return zen_ExceptionManager_isClear(manager);
}

/* Entity Loader */

zen_EntityLoader_t* zen_VirtualMachine_getEntityLoader(
    zen_VirtualMachine_t* virtualMachine) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");

    return virtualMachine->m_entityLoader;
}

/* Exception Manager */

zen_ExceptionManager_t* zen_VirtualMachine_getExceptionManager(zen_VirtualMachine_t* virtualMachine) {
    return NULL;
}

/* Handle Exception */

void zen_VirtualMachine_handleException(zen_VirtualMachine_t* virtualMachine) {
}

/* Native Function */

zen_NativeFunction_t* zen_VirtualMachine_getNativeFunction(
    zen_VirtualMachine_t* virtualMachine, jtk_String_t* name,
    jtk_String_t* descriptor) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    jtk_Assert_assertObject(name, "The specified name is null.");
    jtk_Assert_assertObject(descriptor, "The specified descriptor is null.");

    jtk_String_t* key = jtk_String_append(name, descriptor);
    zen_NativeFunction_t* result = (zen_NativeFunction_t*)jtk_HashMap_getValue(
        virtualMachine->m_nativeFunctions, key);
    jtk_String_delete(key);

    return result;
}

/* Object */

/**
 * The layout of the object in memory is shown below.
 * | Object Header | Object Body |
 *
 * The object header is constant, that is, the size is fixed based on the
 * implementation of the virtual machine. On the other hand, the size of the
 * object body is variable, contingent on both the implementation and the
 * fields declared in the class whose instance the object is.
 *
 * [Object Header]
 * struct jtk_ObjectHeader_t {
 *     uintptr_t m_class;       // Depends on the platform.
 *     uint32_t m_hashCode;     // 4 bytes
 * };
 */
#define ZEN_OBJECT_HEADER_CLASS_SIZE sizeof (uintptr_t)
#define ZEN_OBJECT_HEADER_HASH_CODE_SIZE sizeof (uint32_t)
#define ZEN_OBJECT_HEADER_SIZE ZEN_OBJECT_HEADER_CLASS_SIZE + \
                               ZEN_OBJECT_HEADER_HASH_CODE_SIZE
#define ZEN_OBJECT_HEADER_CLASS_OFFSET 0
#define ZEN_OBJECT_HEADER_HASH_CODE_OFFSET ZEN_OBJECT_HEADER_CLASS_SIZE

int32_t zen_VirtualMachine_identityHash(zen_Object_t* object) {
    if (sizeof (uintptr_t) == 4) {
        return (int32_t)object;
    }
    else if (sizeof (uintptr_t) == 8) {
        return (int32_t)((uintptr_t)object ^ ((uintptr_t)object >> 32));
    }
    else {
        return (int32_t)object;
    }
}

zen_Object_t* zen_VirtualMachine_makeObjectEx(zen_VirtualMachine_t* virtualMachine,
    zen_Function_t* constructor, jtk_VariableArguments_t arguments) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    jtk_Assert_assertObject(constructor, "The specified constructor is null.");
    
    zen_Class_t* class0 = constructor->m_class;
    /* The allocation should be clean such that all the bits are initialized
     * to 0.
     */
    zen_Object_t* object = jtk_Memory_allocate(uint8_t, ZEN_OBJECT_HEADER_SIZE + class0->m_memoryRequirement);
    *((uintptr_t*)object + ZEN_OBJECT_HEADER_CLASS_OFFSET) = (uintptr_t)class0;
    *((uint32_t*)object + ZEN_OBJECT_HEADER_HASH_CODE_OFFSET) = zen_VirtualMachine_identityHash(object);
    
    zen_Interpreter_invokeConstructor(virtualMachine->m_interpreter, object,
        constructor, arguments);
    
    /* Chances are an exception may have been thrown in the constructor. Further,
     * a reference to the object may be stored somewhere by the constructor.
     * Therefore, the object should not be immediately deallocated. Let the garbage
     * collector do its job.
     */
    
    return object;
}

zen_Object_t* zen_VirtualMachine_makeObject(zen_VirtualMachine_t* virtualMachine,
    zen_Function_t* constructor, ...) {
    jtk_VariableArguments_t arguments;
    jtk_VariableArguments_start(arguments, constructor);
    zen_Object_t* result = zen_VirtualMachine_makeObjectEx(virtualMachine,
        constructor, arguments);
    jtk_VariableArguments_end(arguments);
    return result;
}

zen_Object_t* zen_VirtualMachine_newObjectEx(zen_VirtualMachine_t* virtualMachine,
    const uint8_t* classDescriptor, int32_t classDescriptorSize,
    const uint8_t* constructorDescriptor, int32_t constructorDescriptorSize,
    jtk_VariableArguments_t arguments) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    jtk_Assert_assertObject(classDescriptor, "The specified class descriptor is null.");
    jtk_Assert_assertObject(constructorDescriptor, "The specified constructor descriptor is null.");
    
    jtk_String_t* classDescriptor0 = jtk_String_newEx(classDescriptor, classDescriptorSize);
    zen_Class_t* class0 = zen_VirtualMachine_getClass(virtualMachine, classDescriptor0);
    jtk_String_delete(classDescriptor0);
    
    zen_Object_t* result = NULL;
    if (zen_VirtualMachine_isClear(virtualMachine)) {
        jtk_String_t* constructorDescriptor0 = jtk_String_newEx(constructorDescriptor, constructorDescriptorSize);
        zen_Function_t* constructor = zen_Class_getConstructor(class0, constructorDescriptor0);
        if (zen_VirtualMachine_isClear(virtualMachine)) {
            
            result = zen_VirtualMachine_makeObjectEx(virtualMachine, constructor, arguments);
        }
    }
    
    return result;
}

zen_Object_t* zen_VirtualMachine_newObject(zen_VirtualMachine_t* virtualMachine,
    const uint8_t* classDescriptor, int32_t classDescriptorSize,
    const uint8_t* constructorDescriptor, int32_t constructorDescriptorSize, ...) {
    jtk_VariableArguments_t arguments;
    jtk_VariableArguments_start(arguments, constructorDescriptorSize);
    zen_Object_t* object = zen_VirtualMachine_newObjectEx(virtualMachine,
        classDescriptor, classDescriptorSize, constructorDescriptor,
        constructorDescriptorSize, arguments);
    jtk_VariableArguments_end(arguments);
    return object;
}

/* Object Array */

zen_ObjectArray_t* zen_VirtualMachine_newObjectArray(zen_VirtualMachine_t* virtualMachine,
    zen_Class_t* class0, int32_t size) {
    return NULL;
}

/* Raise Exception */

void zen_VirtualMachine_raiseException(zen_VirtualMachine_t* virtualMachine,
    zen_Object_t* exception) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    jtk_Assert_assertObject(exception, "The specified exception is null.");

    zen_ExceptionManager_t* manager = zen_VirtualMachine_getExceptionManager(virtualMachine);
    zen_ExceptionManager_raiseException(manager, exception);
}

/* Static Function */

zen_Function_t* zen_VirtualMachine_getStaticFunction(zen_VirtualMachine_t* virtualMachine,
    zen_Class_t* handle, jtk_String_t* identifier, jtk_String_t* signature) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    jtk_Assert_assertObject(handle, "The specified class is null.");
    jtk_Assert_assertObject(identifier, "The specified identifier is null.");
    jtk_Assert_assertObject(signature, "The specified signature is null.");

    zen_Function_t* function = zen_Class_getStaticFunction(handle, identifier, signature);
    if (function == NULL) {
        zen_VirtualMachine_raiseFunctionNotFoundException(virtualMachine, identifier, signature);
    }
    return function;
}

void zen_VirtualMachine_start(zen_VirtualMachine_t* virtualMachine,
    zen_Function_t* function, ...) {
    jtk_VariableArguments_t variableArguments;
    jtk_VariableArguments_start(variableArguments, function);

    zen_Interpreter_invokeStaticFunctionEx(virtualMachine->m_interpreter,
        function, variableArguments);

    jtk_VariableArguments_end(variableArguments);
}

/* String */

zen_Object_t* zen_VirtualMachine_getEmptyString(zen_VirtualMachine_t* virtualMachine) {
    return zen_VirtualMachine_newStringFromUtf8(virtualMachine, NULL, 0);
}

zen_Object_t* zen_VirtualMachine_newStringFromUtf8(zen_VirtualMachine_t* virtualMachine,
    const uint8_t* string, int32_t size) {
    jtk_Assert_assertObject(virtualMachine, "The specified virtual machine is null.");
    /*
    size = (size < 0)? jtk_Utf8String_getSize(string) : size;
    const uint8_t* descriptor

    return result;
    */
    return NULL;
}

zen_Object_t* zen_VirtualMachine_newString(zen_VirtualMachine_t* virtualMachine,
    const uint8_t* string) {
    return NULL;
}

/* Shutdown */

void zen_VirtualMachine_shutDown(zen_VirtualMachine_t* virtualMachine) {
    /* In C, the process is terminated if the main thread exits. Therefore, we
     * need to wait for the threads spawned by the virtual machine to complete.
     */
    zen_VirtualMachine_waitForThreads(virtualMachine);

    /* Notify all the registered listeners of the "shut down" event. */
    zen_VirtualMachine_notifyShutDown(virtualMachine);

    /* The listeners may have spawned threads. The virtual machine has to wait
     * for their completion.
     */
    zen_VirtualMachine_waitForThreads(virtualMachine);

    /* Tear down the virtual machine. */
    zen_VirtualMachine_tearDown(virtualMachine);
}

void zen_VirtualMachine_waitForThreads(zen_VirtualMachine_t* virtualMachine) {
}

void zen_VirtualMachine_notifyShutDown(zen_VirtualMachine_t* virtualMachine) {
}

void zen_VirtualMachine_tearDown(zen_VirtualMachine_t* virtualMachine) {
}