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

// Saturday, September 15, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_EXCEPTION_MANAGER_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_EXCEPTION_MANAGER_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/object/Object.h>

/* Forward References */
typedef struct zen_VirtualMachine_t zen_VirtualMachine_t;

/*******************************************************************************
 * ExceptionManager                                                            *
 *******************************************************************************/

/**
 * @class ExceptionManager
 * @ingroup zen_virtual_machine
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ExceptionManager_t {
    zen_VirtualMachine_t* m_virtualMachine;
};

/**
 * @memberof ExceptionManager
 */
typedef struct zen_ExceptionManager_t zen_ExceptionManager_t;

/* Constructor */

/**
 * @memberof ExceptionManager
 */
zen_ExceptionManager_t* zen_ExceptionManager_new(zen_VirtualMachine_t* virtualMachine);

/* Destructor */

/**
 * @memberof ExceptionManager
 */
void zen_ExceptionManager_delete(zen_ExceptionManager_t* manager);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_EXCEPTION_MANAGER_H */