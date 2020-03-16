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

// Tuesday, October 02, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_CLASS_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_CLASS_H

#include <jtk/collection/map/HashMap.h>
#include <jtk/core/String.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/feb/EntityFile.h>
#include <com/onecube/zen/virtual-machine/object/Function.h>
#include <com/onecube/zen/virtual-machine/object/Field.h>

/*******************************************************************************
 * Class                                                                       *
 *******************************************************************************/

/**
 * @class Class
 * @ingroup zen_virtualMachine_object
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_Class_t {
    zen_EntityFile_t* m_entityFile;
    jtk_HashMap_t* m_functions;
    jtk_HashMap_t* m_fields;
    jtk_String_t* m_descriptor;
    uint32_t m_memoryRequirement;
};

/**
 * @memberof Class
 */
typedef struct zen_Class_t zen_Class_t;

// Constructor

/**
 * @memberof Class
 */
zen_Class_t* zen_Class_newFromEntityFile(zen_EntityFile_t* entityFile);

// Destructor

/**
 * @memberof Class
 */
void zen_Class_delete(zen_Class_t* class0);

// Entity File

zen_EntityFile_t* zen_Class_getEntityFile(zen_Class_t* class0);

// Field Index

int32_t zen_Class_findFieldOffset(zen_Class_t* class0, const uint8_t* name,
    int32_t nameSize);

// Function

zen_Function_t* zen_Class_getConstructor(zen_Class_t* class0,
    jtk_String_t* descriptor);

zen_Function_t* zen_Class_getStaticFunction(zen_Class_t* class0, jtk_String_t* name,
    jtk_String_t* descriptor);

zen_Function_t* zen_Class_getInstanceFunction(zen_Class_t* class0, jtk_String_t* name,
    jtk_String_t* descriptor);

// Initialize

void zen_Class_initialize(zen_Class_t* class0, zen_EntityFile_t* entityFile);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_CLASS_H */