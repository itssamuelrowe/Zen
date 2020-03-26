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

#include <jtk/collection/Iterator.h>
#include <jtk/collection/map/HashMapEntry.h>
#include <jtk/core/CStringObjectAdapter.h>
#include <jtk/core/CString.h>

#include <com/onecube/zen/virtual-machine/loader/ClassLoader.h>

/*******************************************************************************
 * ClassLoader                                                                 *
 *******************************************************************************/

// Constructor

zen_ClassLoader_t* zen_ClassLoader_new(zen_VirtualMachine_t* virtualMachine,
    zen_EntityLoader_t* entityLoader) {
    jtk_ObjectAdapter_t* stringObjectAdapter = jtk_CStringObjectAdapter_getInstance();

    zen_ClassLoader_t* classLoader = jtk_Memory_allocate(zen_ClassLoader_t, 1);
    classLoader->m_entityLoader = entityLoader;
    classLoader->m_virtualMachine = virtualMachine;
    classLoader->m_classes = jtk_HashMap_newEx(stringObjectAdapter, NULL,
        ZEN_CLASS_LOADER_DEFAULT_CLASSES_MAP_CAPCITY, JTK_HASH_MAP_DEFAULT_LOAD_FACTOR);

    return classLoader;
}

// Destructor

void zen_ClassLoader_delete(zen_ClassLoader_t* classLoader) {
    jtk_Assert_assertObject(classLoader, "The specified class loader is null.");

    jtk_Iterator_t* entryIterator = jtk_HashMap_getEntryIterator(classLoader->m_classes);
    while (jtk_Iterator_hasNext(entryIterator)) {
        jtk_HashMapEntry_t* entry = (jtk_HashMapEntry_t*)jtk_Iterator_getNext(entryIterator);
        uint8_t* descriptor = (uint8_t*)jtk_HashMapEntry_getKey(entry);
        zen_Class_t* class0 = (zen_Class_t*)jtk_HashMapEntry_getValue(entry);

        jtk_CString_delete(descriptor);
        zen_Class_delete(class0);
    }
    jtk_Iterator_delete(entryIterator);

    jtk_HashMap_delete(classLoader->m_classes);
    jtk_Memory_deallocate(classLoader);
}

// Class

zen_Class_t* zen_ClassLoader_findClass(zen_ClassLoader_t* classLoader,
    const uint8_t* descriptor, int32_t descriptorSize) {
    jtk_Assert_assertObject(classLoader, "The specified class loader is null.");

    zen_Class_t* class0 = (zen_Class_t*)jtk_HashMap_getValue(classLoader->m_classes,
        descriptor);

    /* The class with the specified descriptor was not found. Try to load it from
     * the entity loader.
     */
    if (class0 == NULL) {
        zen_EntityFile_t* entityFile = zen_EntityLoader_findEntity(
            classLoader->m_entityLoader, descriptor, descriptorSize);

        /* An entity file was found. Convert it to a class. */
        if (entityFile != NULL) {
            class0 = zen_ClassLoader_loadFromEntityFile(classLoader,
                descriptor, descriptorSize, entityFile);
        }
    }

    return class0;
}

// Load

zen_Class_t* zen_ClassLoader_loadFromEntityFile(zen_ClassLoader_t* classLoader,
    const uint8_t* descriptor, int32_t descriptorSize,
    zen_EntityFile_t* entityFile) {
    jtk_Assert_assertObject(classLoader, "The specified class loader is null.");
    jtk_Assert_assertObject(descriptor, "The specified class descriptor is null.");
    jtk_Assert_assertObject(entityFile, "The specified entity file is null.");

    zen_Class_t* class0 = zen_Class_new(classLoader->m_virtualMachine,
        entityFile);
    uint8_t* descriptorCopy = jtk_CString_newEx(descriptor, descriptorSize);
    jtk_HashMap_put(classLoader->m_classes, descriptorCopy, class0);

    return class0;
}
