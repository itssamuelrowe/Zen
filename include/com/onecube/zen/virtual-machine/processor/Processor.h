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

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_PROCESSOR_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_PROCESSOR_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/processor/Interpreter.h>

/*******************************************************************************
 * Processor                                                                   *
 *******************************************************************************/

/**
 * @class Processor
 * @ingroup zen_vm_processor
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_Processor_t {
    zen_Interpreter_t* m_interpreter;
};

/**
 * @memberof Processor
 */
typedef struct zen_Processor_t zen_Processor_t;

/* Constructor */

/**
 * @memberof Processor
 */
zen_Processor_t* zen_Processor_new();

/* Destructor */

/**
 * @memberof Processor
 */
void zen_Processor_delete(zen_Processor_t* processor);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_PROCESSOR_H */