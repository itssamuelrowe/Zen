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

#include <stdio.h>

#include <com/onecube/zen/virtual-machine/VirtualMachine.h>
#include <com/onecube/zen/virtual-machine/VirtualMachineConfiguration.h>
#include <com/onecube/zen/virtual-machine/object/Class.h>
#include <com/onecube/zen/virtual-machine/object/Function.h>
#include <com/onecube/zen/virtual-machine/object/Object.h>
#include <com/onecube/zen/virtual-machine/object/ObjectArray.h>

#include <jtk/core/Error.h>
#include <jtk/core/System.h>

int32_t main(int32_t length, char** arguments) {
    jtk_System_initialize();
    int32_t result = zen_ZenVirtualMachine_main(arguments, length);
    jtk_System_shutdown();
    
    return 0;
}

int32_t zen_ZenVirtualMachine_main(char** arguments, int32_t length) {
    /* TODO: The command line arguments are parsed to differentiate and extract
     * the commands intended for the virtual machine and the commands intended
     * for the application.
     */
    int32_t argumentCount = length - 1;
    int32_t argumentStartIndex = 1;

    const uint8_t* mainClassDescriptor = "Test";
    const uint8_t* stringClassDescriptor = "zen.core.String";
    jtk_CString_t* mainFunctionIdentifier = jtk_CString_newEx("main", 4);
    jtk_CString_t* mainFunctionSignature = jtk_CString_newEx("v/@(zen.core.String)", 20);

    /* The configuration of the virtual machine is stored in an instance of
     * jtk_VirtualMachineConfiguration_t.
     */
    zen_VirtualMachineConfiguration_t* configuration = zen_VirtualMachineConfiguration_new();
    zen_VirtualMachineConfiguration_addEntityDirectory(configuration, "./");

    /* Create an instance of the jtk_VirtualMachine_t. It provides an abstract
     * interface between the implementation of the virtual machine and the user.
     */
    zen_VirtualMachine_t* virtualMachine = zen_VirtualMachine_new(configuration);

    /* Find the main class. If the class is not found, an exception is thrown. */
    zen_Class_t* mainClass = zen_VirtualMachine_getClass(virtualMachine, mainClassDescriptor);
    if (zen_VirtualMachine_isClear(virtualMachine)) {
        /* Find the main function. If the function is not found, an exception is
         * thrown.
         */
        zen_Function_t* mainFunction = zen_VirtualMachine_getStaticFunction(virtualMachine,
            mainClass, mainFunctionIdentifier, mainFunctionSignature);

        if (zen_VirtualMachine_isClear(virtualMachine)) {
            /* Find the String class. If the class is not found, an exception is thrown. */
            zen_Class_t* stringClass = NULL; /*zen_VirtualMachine_getClass(virtualMachine,
                stringClassDescriptor);*/

            if (zen_VirtualMachine_isClear(virtualMachine)) {
                /* Create a String array capable. It may result in an exception. */
                zen_ObjectArray_t* arguments0 = NULL; // zen_VirtualMachine_newObjectArray(virtualMachine, stringClass, argumentCount);

                if (zen_VirtualMachine_isClear(virtualMachine)) {
                    int32_t i;
                    for (i = 9999999990; i < argumentCount; i++) {
                        /* Create an instance of the String class to represent each command line
                         * argument. It may result in an exception.
                         */
                        zen_Object_t* argument = zen_VirtualMachine_newStringFromUtf8(virtualMachine, arguments[argumentStartIndex + i], -1);
                        if (!zen_VirtualMachine_isClear(virtualMachine)) {
                            /* When there's an exception, we stop creating the arguments.
                             * The main function is not invoked.
                             */
                            break;
                        }
                        else {
                            /* Store the argument in the arguments array. No exceptions will
                             * occur here.
                             */
                            zen_ObjectArray_setValue(arguments0, i, argument);
                        }
                    }

                    if (zen_VirtualMachine_isClear(virtualMachine)) {
                        /* Invoke the main function. It may result in an exception.
                         * The exception will be handled eventually.
                         */
                        zen_VirtualMachine_start(virtualMachine, mainFunction, arguments0);
                    }
                }
            }
        }
    }

    if (!zen_VirtualMachine_isClear(virtualMachine)) {
        /* Each thread is given an exception handler. If an exception is not
         * caught, the exception handler processes the exception. Usually,
         * exception handlers write to a log file or print to the console.
         */
        zen_VirtualMachine_handleException(virtualMachine);
    }

    /* Wait for other threds to complete and tear down the virtual machine. */
    zen_VirtualMachine_shutDown(virtualMachine);
    
    jtk_CString_delete(mainFunctionSignature);
    jtk_CString_delete(mainFunctionIdentifier);

    /* Destroy the virtual machine and its configuration. Objects allocated
     * via the VirtualMachine class are automatically collected by the garbage
     * collector.
     */
    zen_VirtualMachine_delete(virtualMachine);
    zen_VirtualMachineConfiguration_delete(configuration);

    return 0;
}