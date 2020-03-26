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

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_FUNCTION_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_FUNCTION_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/feb/FunctionEntity.h>
#include <com/onecube/zen/virtual-machine/feb/Type.h>

// Forward References

typedef struct zen_Class_t zen_Class_t;

/*******************************************************************************
 * Function                                                                    *
 *******************************************************************************/

/**
 * @class Function
 * @ingroup zen_virtualMachine_object
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_Function_t {
    uint8_t* m_name;
    int32_t m_nameSize;
    uint8_t* m_descriptor;
    int32_t m_descriptorSize;
    uint16_t m_flags;
    zen_Class_t* m_class;
    zen_FunctionEntity_t* m_functionEntity;
    zen_Type_t m_returnType;
    int32_t* m_parameters;
    int32_t m_parameterCount;
};

/**
 * @memberof Function
 */
typedef struct zen_Function_t zen_Function_t;

// Constructor

zen_Function_t* zen_Function_newFromFunctionEntity(zen_Class_t* class0, 
    zen_FunctionEntity_t* functionEntity);

// Destructor

void zen_Function_delete(zen_Function_t* function);

// Native

bool zen_Function_isNative(zen_Function_t* function);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_FUNCTION_H */