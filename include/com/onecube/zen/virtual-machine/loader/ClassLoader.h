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

// Sunday, July 14, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_CLASS_LOADER_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_CLASS_LOADER_H

#include <jtk/collection/map/HashMap.h>
#include <jtk/core/String.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/feb/EntityFile.h>
#include <com/onecube/zen/virtual-machine/loader/EntityLoader.h>
#include <com/onecube/zen/virtual-machine/object/Class.h>

/*******************************************************************************
 * ClassLoader                                                                 *
 *******************************************************************************/

#define ZEN_CLASS_LOADER_DEFAULT_CLASSES_MAP_CAPCITY 128

/**
 * Along with the entity loader, the loading mechanism of the virtual machine
 * makes use of two level cache.
 *
 * @class ClassLoader
 * @ingroup zen_virtualMachine_loader
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ClassLoader_t {
    zen_EntityLoader_t* m_entityLoader;
    jtk_HashMap_t* m_classes;
};

/**
 * @memberof ClassLoader
 */
typedef struct zen_ClassLoader_t zen_ClassLoader_t;

// Constructor

zen_ClassLoader_t* zen_ClassLoader_new(zen_EntityLoader_t* entityLoader);

// Destructor

void zen_ClassLoader_delete(zen_ClassLoader_t* classLoader);

// Class

zen_Class_t* zen_ClassLoader_findClassEx(zen_ClassLoader_t* classLoader,
    const uint8_t* descriptor, int32_t size);
    
zen_Class_t* zen_ClassLoader_findClass(zen_ClassLoader_t* classLoader,
    jtk_String_t* descriptor);

// Load

/**
 * This function does not ensure that a class with the specified descriptor was
 * previously loaded. This behavior can cause memory leaks if used incorrectly.
 */
zen_Class_t* zen_ClassLoader_loadFromEntityFile(zen_ClassLoader_t* classLoader,
    jtk_String_t* descriptor, zen_EntityFile_t* entityFile);
    
#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_CLASS_LOADER_H */