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

// Saturday, April 06, 2019

#include <jtk/collection/Iterator.h>
#include <jtk/collection/array/Array.h>
#include <jtk/collection/list/DoublyLinkedList.h>
#include <jtk/fs/Path.h>
#include <jtk/fs/PathHandle.h>
#include <jtk/fs/FileInputStream.h>
#include <jtk/io/BufferedInputStream.h>
#include <jtk/io/InputStream.h>

#include <com/onecube/zen/virtual-machine/loader/EntityLoader.h>

/*******************************************************************************
 * EntityLoader                                                                *
 *******************************************************************************/

zen_EntityLoader_t* zen_EntityLoader_new() {
    jtk_ObjectAdapter_t* stringObjectAdapter = jtk_StringObjectAdapter_getInstance();

    zen_EntityLoader_t* loader = jtk_Memory_allocate(zen_EntityLoader_t, 1);
    loader->m_directories = jtk_DoublyLinkedList_new();
    loader->m_flags = ZEN_ENTITY_LOADER_FLAG_PRIORITIZE_DIRECTORIES;
    loader->m_entities = jtk_HashMap_newEx(stringObjectAdapter, NULL,
        ZEN_ENTITY_LOADER_DEFAULT_ENTITIES_MAP_CAPCITY, JTK_HASH_MAP_DEFAULT_LOAD_FACTOR);
    // loader->m_parser = zen_BinaryEntityParser_new(attributeParseRules, );
    loader->m_attributeParseRules = zen_AttributeParseRules_new();

    return loader;
}

zen_EntityLoader_t* zen_EntityLoader_newWithEntityDirectories(jtk_Iterator_t* entityDirectoryIterator) {
    jtk_ObjectAdapter_t* stringObjectAdapter = jtk_StringObjectAdapter_getInstance();

    zen_EntityLoader_t* loader = zen_EntityLoader_new();
    while (jtk_Iterator_hasNext(entityDirectoryIterator)) {
        jtk_CString_t* directory = jtk_Iterator_getNext(entityDirectoryIterator);
        zen_EntityLoader_addDirectory_s(loader, directory);
    }

    return loader;
}

void zen_EntityLoader_delete(zen_EntityLoader_t* loader) {
    jtk_Assert_assertObject(loader, "The specified entity loader is null.");

    int32_t size = jtk_DoublyLinkedList_getSize(loader->m_directories);
    jtk_Iterator_t* iterator = jtk_DoublyLinkedList_getIterator(loader->m_directories);
    while (jtk_Iterator_hasNext(iterator)) {
        jtk_Path_t* path = (jtk_Path_t*)jtk_Iterator_getNext(iterator);
        jtk_Path_delete(path);
    }
    jtk_Iterator_delete(iterator);
    jtk_DoublyLinkedList_delete(loader->m_directories);

    jtk_Iterator_t* entryIterator = jtk_HashMap_getEntryIterator(loader->m_entities);
    while (jtk_Iterator_hasNext(entryIterator)) {
        jtk_HashMapEntry_t* entry = (jtk_HashMapEntry_t*)jtk_Iterator_getNext(entryIterator);

        jtk_CString_t* descriptor = (jtk_CString_t*)jtk_HashMapEntry_getKey(entry);
        jtk_CString_delete(descriptor);

#warning "Should the entity be destroyed this way?"
        zen_EntityFile_t* entityFile = (zen_EntityFile_t*)jtk_HashMapEntry_getValue(entry->m_value);
        // zen_EntityFile_delete(entityFile);
    }

    zen_AttributeParseRules_delete(loader->m_attributeParseRules);

    jtk_Memory_deallocate(loader);
}

/* The original algorithm ensured that a valid directory was added to the entity
 * directoires. Since the directories are validated when loading entities the
 * algorithm was modified to include all the directories without checking.
 */
bool zen_EntityLoader_addDirectory(zen_EntityLoader_t* loader, const uint8_t* directory) {
    jtk_Assert_assertObject(loader, "The specified entity loader is null.");
    jtk_Assert_assertObject(directory, "The specified directory is null.");

    jtk_Path_t* path = jtk_Path_newFromString(directory);
    jtk_DoublyLinkedList_add(loader->m_directories, path);

    // bool result = jtk_Path_isDirectory(path);
    // if (result) {
    //    jtk_DoublyLinkedList_add(loader->m_directories, path);
    // }
    // else {
    //    jtk_Path_delete(path);
    // }

    return true;
}

bool zen_EntityLoader_addDirectory_s(zen_EntityLoader_t* loader, jtk_CString_t* directory) {
    jtk_Assert_assertObject(loader, "The specified entity loader is null.");
    jtk_Assert_assertObject(directory, "The specified directory is null.");

    jtk_Path_t* path = jtk_Path_newFromString_s(directory);
    jtk_DoublyLinkedList_add(loader->m_directories, path);

    // bool result = jtk_Path_isDirectory(path);
    // if (result) {
    //    jtk_DoublyLinkedList_add(loader->m_directories, path);
    // }
    // else {
    //    jtk_Path_delete(path);
    // }

    return true;
}

// Find Entity

zen_EntityFile_t* zen_EntityLoader_findEntity(zen_EntityLoader_t* loader, const uint8_t* descriptor) {
    jtk_Assert_assertObject(loader, "The specified entity loader is null.");
    jtk_Assert_assertObject(descriptor, "The specified descriptor is null.");

    zen_EntityFile_t* result = (zen_EntityFile_t*)jtk_HashMap_getValue(loader->m_entities, descriptor);
    if (result == NULL) {
        result = zen_EntityLoader_loadEntity(loader, descriptor);
    }

    return result;
}

// Get Entity

zen_EntityFile_t* zen_EntityLoader_getEntity(zen_EntityLoader_t* loader,
    const uint8_t* descriptor) {
    jtk_Assert_assertObject(loader, "The specified entity loader is null.");
    jtk_Assert_assertObject(descriptor, "The specified descriptor is null.");

    jtk_CString_t* string = jtk_CString_new(descriptor);
    zen_EntityFile_t* entity = (zen_EntityFile_t*)jtk_HashMap_getValue(loader->m_entities, string);
    jtk_CString_delete(string);
    
    return entity;
}

// Load Entity

zen_EntityFile_t* zen_EntityLoader_loadEntity(zen_EntityLoader_t* loader,
    const uint8_t* descriptor) {
    jtk_Assert_assertObject(loader, "The specified entity loader is null.");
    jtk_Assert_assertObject(descriptor, "The specified descriptor is null.");

    zen_EntityFile_t* result = NULL;

    /* Question. Why is joining two strings so complicated?! */
    // const uint8_t* strings[] = { descriptor, ".feb" };
    // jtk_CString_t* entityName = jtk_CString_newFromJoinEx(strings, 2);
    jtk_CString_t* entityName = jtk_CString_newFromJoin(descriptor, ".feb");
    jtk_Path_t* entityFile = jtk_Path_newFromStringEx(entityName->m_value, entityName->m_size);
    jtk_CString_delete(entityName);

    /* Retrieve an iterator over the list of registered entity directories. */
    jtk_Iterator_t* iterator = jtk_DoublyLinkedList_getIterator(loader->m_directories);
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
                    result = zen_EntityLoader_loadEntityFromHandle(loader, entityPathHandle);
                    if (result != NULL) {
                        /* Honestly, there's no requirement of the `zen_EntityDescriptor_t` type.
                         * Unfortunately, at this moment JTK library does not implement an aggregate
                         * type for strings. It seems like a distraction to implement it right now.
                         *
                         * Also, it seems right for an entity descriptor to have its own type.
                         *
                         * ** After a few minutes **
                         * Damn it! Let's go implement that String class! -_-
                         */
                        jtk_CString_t* entityDescriptor = jtk_CString_new(descriptor);

                        jtk_HashMap_put(loader->m_entities, entityDescriptor, result);
                    }
                    else {
                        /* At this point, the entity loader found an entity file. Unfortunately, the
                         * entity file is corrupted. The entity loader may continue to look for entities
                         * in different files. It terminates here if the entity loader is not configured
                         * to ignore corrupt entity files.
                         */
                        if (!zen_EntityLoader_shouldIgnoreCorruptEntity(loader)) {
                            break;
                        }
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
            fprintf(stderr, "Warning: Cannot find lookup directory\n");
        }
    }
    jtk_Path_delete(entityFile);

    return result;
}

// Load Entity From File

// BUG: You cannot copy an array of bytes with the general purpose Array.
jtk_Array_t* jtk_InputStreamHelper_toArray(jtk_InputStream_t* stream) {
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
        }
    }
    jtk_Array_t* array = (index == 0)? NULL : jtk_Array_newFromRawArray(result, index);
    
    jtk_Memory_deallocate(result);
    
    return array;
}

zen_EntityFile_t* zen_EntityLoader_loadEntityFromHandle(zen_EntityLoader_t* loader,
    jtk_PathHandle_t* handle) {
    jtk_Assert_assertObject(loader, "The specified entity loader is null.");
    jtk_Assert_assertObject(handle, "The specified entity path handle is null.");

    zen_EntityFile_t* result = NULL;
    jtk_FileInputStream_t* fileInputStream = jtk_FileInputStream_newFromHandle(handle);
    if (fileInputStream != NULL) {
        jtk_BufferedInputStream_t* bufferedInputStream = jtk_BufferedInputStream_newEx(
            fileInputStream->m_inputStream, ZEN_ENTITY_LOADER_BUFFER_SIZE);
        jtk_InputStream_t* inputStream = bufferedInputStream->m_inputStream;

        jtk_Array_t* input = jtk_InputStreamHelper_toArray(inputStream);

        zen_BinaryEntityParser_t* parser = zen_BinaryEntityParser_new(
            loader->m_attributeParseRules, input->m_values, input->m_size);
        result = zen_BinaryEntityParser_parse(parser, 
inputStream);

        zen_BinaryEntityParser_delete(parser);
        jtk_Array_delete(input);
        jtk_InputStream_destroy(inputStream);
    }
    else {
        // Warning: Failed to load entity from handle.
    }

    return result;
}

// Ignore Corrupt Entity

bool zen_EntityLoader_shouldIgnoreCorruptEntity(zen_EntityLoader_t* loader) {
    jtk_Assert_assertObject(loader, "The specified entity loader is null.");

    return (loader->m_flags & ZEN_ENTITY_LOADER_FLAG_IGNORE_CORRUPT_ENTITY) != 0;
}

void zen_EntityLoader_setIgnoreCorruptEntity(zen_EntityLoader_t* loader,
    bool ignoreCorruptEntity) {
    jtk_Assert_assertObject(loader, "The specified entity loader is null.");

    loader->m_flags = ignoreCorruptEntity?
        (loader->m_flags | ZEN_ENTITY_LOADER_FLAG_IGNORE_CORRUPT_ENTITY) :
        (loader->m_flags & ~ZEN_ENTITY_LOADER_FLAG_IGNORE_CORRUPT_ENTITY);
}