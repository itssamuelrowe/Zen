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

// Sunday, September 16, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_PROCESSOR_THREAD_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_PROCESSOR_THREAD_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/processor/InvocationStack.h>

/*******************************************************************************
 * ProcessorThread                                                             *
 *******************************************************************************/

/**
 * @class ProcessorThread
 * @ingroup zen_vm_processor
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ProcessorThread_t {
    zen_InvocationStack_t* m_invocationStack;
};

/**
 * @memberof ProcessorThread
 */
typedef struct zen_ProcessorThread_t zen_ProcessorThread_t;

/* Constructor */

/**
 * @memberof ProcessorThread
 */
zen_ProcessorThread_t* zen_ProcessorThread_new();

/* Destructor */

/**
 * @memberof ProcessorThread
 */
void zen_ProcessorThread_delete(zen_ProcessorThread_t* thread);
 
#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_PROCESSOR_THREAD_H */