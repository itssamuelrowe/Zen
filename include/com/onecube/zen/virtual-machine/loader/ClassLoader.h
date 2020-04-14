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

#include <jtk/collection/Iterator.h>
#include <jtk/collection/map/HashMap.h>
#include <jtk/collection/list/DoublyLinkedList.h>
#include <jtk/fs/Path.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/object/Class.h>
#include <com/onecube/zen/virtual-machine/feb/EntityFile.h>
#include <com/onecube/zen/virtual-machine/loader/BinaryEntityParser.h>
#include <com/onecube/zen/virtual-machine/loader/AttributeParseRules.h>

/*******************************************************************************
 * ClassLoader                                                                 *
 *******************************************************************************/

#define ZEN_ENTITY_LOADER_FLAG_PRIORITIZE_DIRECTORIES (1 << 0)
#define ZEN_ENTITY_LOADER_FLAG_IGNORE_CORRUPT_ENTITY (1 << 1)

#define ZEN_CLASS_LOADER_DEFAULT_CLASSES_MAP_CAPCITY 128

/* A small experiment on a deployed project written in a certain virtual
 * machine that powers my favorite language revealed that on an average
 * most binary files are 3 kilobytes in size. The buffer size was evaluated
 * to reduce the cache misses and increase the cache hits in the buffered
 * input stream.
 */
#define ZEN_ENTITY_LOADER_BUFFER_SIZE (3 * 1024)

/**
 * The loading mechanism of the virtual machine makes use of a one-level cache.
 *
 * @class ClassLoader
 * @ingroup zen_virtualMachine_loader
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ClassLoader_t {

    /**
     * The list of directories where the loader looks for the definitions
     * of entities.
     */
    jtk_DoublyLinkedList_t* m_directories;

    uint32_t m_flags;

    /**
     * Cache to store classes loaded previously.
     */
    jtk_HashMap_t* m_classes;

    zen_VirtualMachine_t* m_virtualMachine;

    zen_AttributeParseRules_t* m_attributeParseRules;
};

/**
 * @memberof ClassLoader
 */
typedef struct zen_ClassLoader_t zen_ClassLoader_t;

// Constructor

zen_ClassLoader_t* zen_ClassLoader_new(
    zen_VirtualMachine_t* virtualMachine, jtk_Iterator_t* iterator);

// Destructor

void zen_ClassLoader_delete(zen_ClassLoader_t* classLoader);

// Directory

bool zen_ClassLoader_addDirectory(zen_ClassLoader_t* loader, const uint8_t* directory,
    int32_t directorySize);

// Find

/**
 * First, it tries to find a previously loaded class with the specified
 * descriptor in the class registry. If not found, it tries to load it
 * from a physical description, i.e., a binary entity. It fails if a
 * corresponding binary entity is not found, without raising any exception.
 *
 * @memberof ClassLoader
 */
zen_Class_t* zen_ClassLoader_findClass(zen_ClassLoader_t* classLoader,
    const uint8_t* descriptor, int32_t descriptorSize);


/**
 * It tries to find a previously loaded class with the specified descriptor in
 * the class registry. If not found, it fails without raising any exception.
 *
 * @memberof ClassLoader
 */
zen_Class_t* zen_ClassLoader_getClass(zen_ClassLoader_t* classLoader,
    const uint8_t* descriptor, int32_t descriptorSize);

// Load

/**
 * It tries to load a class with the specified descriptor from a physical
 * description, i.e., a binary entity. It fails if a class was previously
 * loaded, or if a corresponding binary entity is not found.
 *
 * @memberof ClassLoader
 */
zen_Class_t* zen_ClassLoader_loadClass(zen_ClassLoader_t* loader,
    const uint8_t* descriptor, int32_t descriptorSize);


/**
 * It tries to load a class from the specified regular file path. If the file
 * does not exist or is corrupt, it fails without raising an exception.
 *
 * @memberof ClassLoader
 */
zen_EntityFile_t* zen_ClassLoader_loadEntityFromFile(zen_ClassLoader_t* loader,
    jtk_Path_t* path);

/**
 * It tries to load a class from the specified regular file handle. If the file
 * does not exist or is corrupt, it fails without raising an exception.
 *
 * @memberof ClassLoader
 */
zen_EntityFile_t* zen_ClassLoader_loadEntityFromHandle(zen_ClassLoader_t* loader,
    jtk_PathHandle_t* handle);

/**
 * This function does not ensure that a class with the specified descriptor was
 * previously loaded. This behavior can cause memory leaks if used incorrectly.
 */
zen_Class_t* zen_ClassLoader_loadFromEntityFile(zen_ClassLoader_t* classLoader,
    const uint8_t* descriptor, int32_t descriptorSize, zen_EntityFile_t* entityFile);

// Ignore Corrupt Entity

bool zen_ClassLoader_shouldIgnoreCorruptEntity(zen_ClassLoader_t* loader);
void zen_ClassLoader_setIgnoreCorruptEntity(zen_ClassLoader_t* loader, bool ignoreCorruptEntity);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_CLASS_LOADER_H */