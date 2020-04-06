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

// Friday, June 08, 2018

#include <com/onecube/zen/virtual-machine/VirtualMachineContext.h>
#include <com/onecube/zen/virtual-machine/ExceptionManager.h>

/*******************************************************************************
 * VirtualMachineContext                                                                   *
 *******************************************************************************/

/* Constructor */

zen_VirtualMachineContext_t* zen_VirtualMachineContext_new(zen_VirtualMachine_t* virtualMachine) {
    zen_VirtualMachineContext_t* context = zen_Memory_allocate(zen_VirtualMachineContext_t, 1);
    context->m_virtualMachine = virtualMachine;

    return context;
}

/* Destructor */

void zen_VirtualMachineContext_delete(zen_VirtualMachineContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

