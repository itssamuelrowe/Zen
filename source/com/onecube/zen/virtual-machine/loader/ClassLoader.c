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
#include <jtk/collection/array/Array.h>
#include <jtk/collection/array/Arrays.h>
#include <jtk/collection/list/DoublyLinkedList.h>
#include <jtk/fs/Path.h>
#include <jtk/fs/PathHandle.h>
#include <jtk/fs/FileInputStream.h>
#include <jtk/io/BufferedInputStream.h>
#include <jtk/io/InputStream.h>

#include <com/onecube/zen/virtual-machine/loader/ClassLoader.h>

/*******************************************************************************
 * ClassLoader                                                                 *
 *******************************************************************************/

// Constructor

zen_ClassLoader_t* zen_ClassLoader_new(zen_VirtualMachine_t* virtualMachine,
    jtk_Iterator_t* entityDirectoryIterator) {
    jtk_ObjectAdapter_t* stringObjectAdapter = jtk_CStringObjectAdapter_getInstance();

    zen_ClassLoader_t* classLoader = jtk_Memory_allocate(zen_ClassLoader_t, 1);
    classLoader->m_directories = jtk_DoublyLinkedList_new();
    classLoader->m_flags = ZEN_ENTITY_LOADER_FLAG_PRIORITIZE_DIRECTORIES;
    classLoader->m_virtualMachine = virtualMachine;
    classLoader->m_classes = jtk_HashMap_newEx(stringObjectAdapter, NULL,
        ZEN_CLASS_LOADER_DEFAULT_CLASSES_MAP_CAPCITY, JTK_HASH_MAP_DEFAULT_LOAD_FACTOR);
    classLoader->m_attributeParseRules = zen_AttributeParseRules_new();
    classLoader->m_parser = zen_BinaryEntityParser_new(
        classLoader->m_attributeParseRules);

    while (jtk_Iterator_hasNext(entityDirectoryIterator)) {
        uint8_t* directory = (uint8_t*)jtk_Iterator_getNext(entityDirectoryIterator);
        zen_ClassLoader_addDirectory(classLoader, directory, -1);
    }

    return classLoader;
}

// Destructor

void zen_ClassLoader_delete(zen_ClassLoader_t* classLoader) {
    jtk_Assert_assertObject(classLoader, "The specified class loader is null.");

    int32_t size = jtk_DoublyLinkedList_getSize(classLoader->m_directories);
    jtk_Iterator_t* iterator = jtk_DoublyLinkedList_getIterator(classLoader->m_directories);
    while (jtk_Iterator_hasNext(iterator)) {
        jtk_Path_t* path = (jtk_Path_t*)jtk_Iterator_getNext(iterator);
        jtk_Path_delete(path);
    }
    jtk_Iterator_delete(iterator);
    jtk_DoublyLinkedList_delete(classLoader->m_directories);


    jtk_Iterator_t* entryIterator = jtk_HashMap_getEntryIterator(classLoader->m_classes);
    while (jtk_Iterator_hasNext(entryIterator)) {
        jtk_HashMapEntry_t* entry = (jtk_HashMapEntry_t*)jtk_Iterator_getNext(entryIterator);
        uint8_t* descriptor = (uint8_t*)jtk_HashMapEntry_getKey(entry);
        zen_Class_t* class0 = (zen_Class_t*)jtk_HashMapEntry_getValue(entry);

#warning "Should the entity be destroyed this way?"
        // zen_EntityFile_delete(class0->m_entityFile);

        jtk_CString_delete(descriptor);
        zen_Class_delete(class0);
    }
    jtk_Iterator_delete(entryIterator);

    zen_AttributeParseRules_delete(classLoader->m_attributeParseRules);
    zen_BinaryEntityParser_delete(classLoader->m_parser);
    jtk_HashMap_delete(classLoader->m_classes);
    jtk_Memory_deallocate(classLoader);
}

// Directory

/* The original algorithm ensured that a valid directory was added to the entity
 * directoires. Since the directories are validated when loading entities the
 * algorithm was modified to include all the directories without checking.
 */
bool zen_ClassLoader_addDirectory(zen_ClassLoader_t* classLoader,
    const uint8_t* directory, int32_t directorySize) {
    jtk_Assert_assertObject(classLoader, "The specified entity loader is null.");
    jtk_Assert_assertObject(directory, "The specified directory is null.");

    jtk_Path_t* path = jtk_Path_newFromStringEx(directory, directorySize);
    jtk_DoublyLinkedList_add(classLoader->m_directories, path);

    // bool result = jtk_Path_isDirectory(path);
    // if (result) {
    //    jtk_DoublyLinkedList_add(classLoader->m_directories, path);
    // }
    // else {
    //    jtk_Path_delete(path);
    // }

    return true;
}

// Find

zen_Class_t* zen_ClassLoader_findClassEx(zen_ClassLoader_t* classLoader,
    const uint8_t* descriptor, int32_t descriptorSize,
    int32_t* descriptorHashCode) {
    jtk_Assert_assertObject(classLoader, "The specified class loader is null.");

    zen_Class_t* class0 = (zen_Class_t*)jtk_HashMap_getValueFast(classLoader->m_classes,
        descriptor, descriptorHashCode);

    /* The class with the specified descriptor was not found. Try to load it from
     * the entity loader.
     */
    if (class0 == NULL) {
        class0 = zen_ClassLoader_loadClass(classLoader, descriptor, descriptorSize);
    }

    return class0;
}


zen_Class_t* zen_ClassLoader_findClass(zen_ClassLoader_t* classLoader,
    const uint8_t* descriptor, int32_t descriptorSize) {
    jtk_Assert_assertObject(classLoader, "The specified class loader is null.");

    zen_Class_t* class0 = (zen_Class_t*)jtk_HashMap_getValue(classLoader->m_classes,
        descriptor);

    /* The class with the specified descriptor was not found. Try to load it from
     * the entity loader.
     */
    if (class0 == NULL) {
        class0 = zen_ClassLoader_loadClass(classLoader, descriptor, descriptorSize);
    }

    return class0;
}

zen_Class_t* zen_ClassLoader_getClass(zen_ClassLoader_t* classLoader,
    const uint8_t* descriptor, int32_t descriptorSize) {
    jtk_Assert_assertObject(classLoader, "The specified class loader is null.");

    return (zen_Class_t*)jtk_HashMap_getValue(classLoader->m_classes,
        descriptor);
}

// Load

// Tuesday, March 17, 2020

/**
 * @author Samuel Rowe
 * @since JTK 1.1
 */
struct jtk_ByteArray_t {
    int8_t* m_values;
    int32_t m_size;
};

typedef struct jtk_ByteArray_t jtk_ByteArray_t;

jtk_ByteArray_t* jtk_ByteArray_fromRawArray(int8_t* array, int32_t size) {
    jtk_Assert_assertObject(array, "The specified array is null.");

    jtk_ByteArray_t* result = jtk_Memory_allocate(jtk_ByteArray_t, 1);
    result->m_values = jtk_Arrays_clone_b(array, size);
    result->m_size = size;

    return result;
}

void jtk_ByteArray_delete(jtk_ByteArray_t* array) {
    jtk_Assert_assertObject(array, "The specified byte array is null.");

    jtk_Memory_deallocate(array->m_values);
    jtk_Memory_deallocate(array);
}

jtk_ByteArray_t* jtk_InputStreamHelper_toArray(jtk_InputStream_t* stream) {
    uint8_t* result = jtk_Memory_allocate(uint8_t, 1024);
    int32_t size = 1024;
    int32_t index = 0;
    while (true) {
        int32_t count = jtk_InputStream_readBytesEx(stream, result, size,
            index, size);

        if (count <= 0) {
            break;
        }

        index += count;

        if (index >= (int32_t)(size * 0.85f)) {
            uint8_t* temporary = result;
            int32_t newSize = size * 2;
            result = jtk_Arrays_copyOfEx_b(result, size, newSize, 0, false);
            jtk_Memory_deallocate(temporary);

            size = newSize;
        }
    }
    jtk_Array_t* array = (index == 0)? NULL : jtk_ByteArray_fromRawArray(result, index);

    jtk_Memory_deallocate(result);

    return array;
}

zen_Class_t* zen_ClassLoader_loadClass(zen_ClassLoader_t* classLoader,
    const uint8_t* descriptor, int32_t descriptorSize) {
    jtk_Assert_assertObject(classLoader, "The specified entity loader is null.");
    jtk_Assert_assertObject(descriptor, "The specified descriptor is null.");

    zen_Class_t* result = NULL;

    int32_t entityNameSize;
    uint8_t* entityName = jtk_CString_joinEx(descriptor, descriptorSize, ".feb",
        4, &entityNameSize);
    jtk_Path_t* entityFile = jtk_Path_newFromStringEx(entityName, entityNameSize);
    jtk_CString_delete(entityName);

    /* Retrieve an iterator over the list of registered entity directories. */
    jtk_Iterator_t* iterator = jtk_DoublyLinkedList_getIterator(classLoader->m_directories);
    while (jtk_Iterator_hasNext(iterator)) {
        /* Retrieve the next directory for searching the entity file. */
        jtk_Path_t* directoryPath = (jtk_Path_t*)jtk_Iterator_getNext(iterator);
        jtk_PathHandle_t* directoryHandle = jtk_PathHandle_newFromPath(directoryPath);
        /* The specified path may point to a regular file. Therefore, make sure
         * the path is a directory to provide a warning.
         *
         * NOTE: The entity file path can be directly constructed without checking
         * if the entity directory exists. This makes the lookup process faster.
         * Right now, the original algorithm is maintained to print debugging
         * information. Probably should use conditional directives to enable and
         * disable the check at compile time.
         */
        if ((directoryHandle != NULL) && jtk_PathHandle_isDirectory(directoryHandle)) {
            /* Construct a hypothetical path to the entity file. */
            jtk_Path_t* entityPath = jtk_Path_newWithParentAndChild_oo(directoryPath, entityFile);
            jtk_PathHandle_t* entityPathHandle = jtk_PathHandle_newFromPath(entityPath);
            if (entityPathHandle != NULL) {
                if (jtk_PathHandle_isRegularFile(entityPathHandle)) {
                    // NOTE: The loader should not maintain any reference to entity path.
                    jtk_FileInputStream_t* fileInputStream = jtk_FileInputStream_newFromHandle(entityPathHandle);
                    if (fileInputStream != NULL) {
                        jtk_BufferedInputStream_t* bufferedInputStream = jtk_BufferedInputStream_newEx(
                            fileInputStream->m_inputStream, ZEN_ENTITY_LOADER_BUFFER_SIZE);
                        jtk_InputStream_t* inputStream = bufferedInputStream->m_inputStream;

                        jtk_ByteArray_t* input = jtk_InputStreamHelper_toArray(inputStream);

                        zen_EntityFile_t* entityFile = zen_BinaryEntityParser_parse(
                            classLoader->m_parser, input->m_values, input->m_size);

                        result = zen_Class_new(classLoader->m_virtualMachine,
                                entityFile);
                        if (result == NULL) {
                            /* At this point, the entity loader found an entity file. Unfortunately, the
                             * entity file is corrupted. The entity loader may continue to look for entities
                             * in different files. It terminates here if the entity loader is not configured
                             * to ignore corrupt entity files.
                             */
                            if ((classLoader->m_flags & ZEN_ENTITY_LOADER_FLAG_IGNORE_CORRUPT_ENTITY) == 0) {
                                break;
                            }
                        }
                        else {
                            uint8_t* descriptorCopy = jtk_CString_newEx(descriptor, descriptorSize);
                            jtk_HashMap_put(classLoader->m_classes, descriptorCopy, result);
                        }

                        jtk_ByteArray_delete(input);
                        jtk_InputStream_destroy(inputStream);
                    }
                    else {
                        // Warning: Failed to load entity from handle.
                    }
                }
                /* Destroy the entity path handle created earlier. */
                jtk_PathHandle_delete(entityPathHandle);
            }
            else {
                // log("Could not find entity file here.");
            }
            jtk_Path_delete(entityPath);
        }
        else {
            fprintf(stderr, "[warning] Cannot find find directory.\n");
        }
    }
    jtk_Path_delete(entityFile);

    return result;
}

// Ignore Corrupt Entity

bool zen_ClassLoader_shouldIgnoreCorruptEntity(zen_ClassLoader_t* classLoader) {
    jtk_Assert_assertObject(classLoader, "The specified entity loader is null.");

    return (classLoader->m_flags & ZEN_ENTITY_LOADER_FLAG_IGNORE_CORRUPT_ENTITY) != 0;
}

void zen_ClassLoader_setIgnoreCorruptEntity(zen_ClassLoader_t* classLoader,
    bool ignoreCorruptEntity) {
    jtk_Assert_assertObject(classLoader, "The specified entity loader is null.");

    classLoader->m_flags = ignoreCorruptEntity?
        (classLoader->m_flags | ZEN_ENTITY_LOADER_FLAG_IGNORE_CORRUPT_ENTITY) :
        (classLoader->m_flags & ~ZEN_ENTITY_LOADER_FLAG_IGNORE_CORRUPT_ENTITY);
}