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

// Saturday, April 10, 2020

#include <jtk/collection/Iterator.h>
#include <jtk/collection/array/Array.h>
#include <jtk/collection/list/DoublyLinkedList.h>
#include <jtk/fs/Path.h>
#include <jtk/fs/PathHandle.h>
#include <jtk/fs/FileInputStream.h>
#include <jtk/io/BufferedInputStream.h>
#include <jtk/io/InputStream.h>
#include <jtk/core/CString.h>

#include <com/onecube/zen/compiler/symbol-table/SymbolLoader.h>

/*******************************************************************************
 * SymbolLoader                                                                *
 *******************************************************************************/

zen_SymbolLoader_t* zen_SymbolLoader_new(zen_Compiler_t* compiler) {
    jtk_ObjectAdapter_t* stringObjectAdapter = jtk_StringObjectAdapter_getInstance();

    zen_SymbolLoader_t* loader = jtk_Memory_allocate(zen_SymbolLoader_t, 1);
    loader->m_directories = jtk_DoublyLinkedList_new();
    loader->m_flags = ZEN_ENTITY_LOADER_FLAG_PRIORITIZE_DIRECTORIES;
    loader->m_symbols = jtk_HashMap_newEx(stringObjectAdapter, NULL,
        ZEN_ENTITY_LOADER_DEFAULT_ENTITIES_MAP_CAPCITY, JTK_HASH_MAP_DEFAULT_LOAD_FACTOR);
    loader->m_compiler = compiler;

    return loader;
}

zen_SymbolLoader_t* zen_SymbolLoader_newWithEntityDirectories(zen_Compiler_t* compiler,
    jtk_Iterator_t* entityDirectoryIterator) {
    jtk_ObjectAdapter_t* stringObjectAdapter = jtk_StringObjectAdapter_getInstance();

    zen_SymbolLoader_t* loader = zen_SymbolLoader_new(compiler);
    while (jtk_Iterator_hasNext(entityDirectoryIterator)) {
        uint8_t* directory = (uint8_t*)jtk_Iterator_getNext(entityDirectoryIterator);
        zen_SymbolLoader_addDirectory_s(loader, directory);
    }

    return loader;
}

void zen_SymbolLoader_delete(zen_SymbolLoader_t* loader) {
    jtk_Assert_assertObject(loader, "The specified entity loader is null.");

    int32_t size = jtk_DoublyLinkedList_getSize(loader->m_directories);
    jtk_Iterator_t* iterator = jtk_DoublyLinkedList_getIterator(loader->m_directories);
    while (jtk_Iterator_hasNext(iterator)) {
        jtk_Path_t* path = (jtk_Path_t*)jtk_Iterator_getNext(iterator);
        jtk_Path_delete(path);
    }
    jtk_Iterator_delete(iterator);
    jtk_DoublyLinkedList_delete(loader->m_directories);

    jtk_Iterator_t* entryIterator = jtk_HashMap_getEntryIterator(loader->m_symbols);
    while (jtk_Iterator_hasNext(entryIterator)) {
        jtk_HashMapEntry_t* entry = (jtk_HashMapEntry_t*)jtk_Iterator_getNext(entryIterator);

        uint8_t* descriptor = (uint8_t*)jtk_HashMapEntry_getKey(entry);
        jtk_CString_delete(descriptor);

#warning "Should the entity be destroyed this way?"
        zen_EntityFile_t* entityFile = (zen_EntityFile_t*)jtk_HashMapEntry_getValue(entry->m_value);
        // zen_EntityFile_delete(entityFile);
    }

    jtk_Memory_deallocate(loader);
}

/* The original algorithm ensured that a valid directory was added to the entity
 * directries. Since the directories are validated when loading entities the
 * algorithm was modified to include all the directories without checking.
 */
bool zen_SymbolLoader_addDirectory(zen_SymbolLoader_t* loader, const uint8_t* directory) {
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

// Find Symbol

zen_Symbol_t* zen_SymbolLoader_findSymbol(zen_SymbolLoader_t* loader,
    const uint8_t* descriptor) {
    jtk_Assert_assertObject(loader, "The specified entity loader is null.");
    jtk_Assert_assertObject(descriptor, "The specified descriptor is null.");

    zen_Symbol_t* result = (zen_Symbol_t*)jtk_HashMap_getValue(loader->m_symbols, descriptor);
    if (result == NULL) {
        result = zen_SymbolLoader_loadSymbol(loader, descriptor);
    }

    return result;
}

// Load Symbol

zen_Symbol_t* zen_SymbolLoader_loadSymbol(zen_SymbolLoader_t* loader,
    const uint8_t* descriptor) {
    jtk_Assert_assertObject(loader, "The specified entity loader is null.");
    jtk_Assert_assertObject(descriptor, "The specified descriptor is null.");

    zen_Symbol_t* result = NULL;

    uint8_t* entityName = jtk_CString_newFromJoin(descriptor, ".feb");
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
                    result = zen_SymbolLoader_loadSymbolFromHandle(loader, entityPathHandle);
                    if (result != NULL) {
                        uint8_t* entityDescriptor = jtk_CString_new(descriptor);
                        jtk_HashMap_put(loader->m_symbols, entityDescriptor, result);
                    }
                    else {
                        /* At this point, the entity loader found an entity file. Unfortunately, the
                         * entity file is corrupted. The entity loader may continue to look for entities
                         * in different files. It terminates here if the entity loader is not configured
                         * to ignore corrupt entity files.
                         */
                        if (!zen_SymbolLoader_shouldIgnoreCorruptEntity(loader)) {
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
            fprintf(stderr, "[warning] Cannot find lookup directory '%s'\n",
                directoryPath->m_value);
        }
    }
    jtk_Path_delete(entityFile);

    return result;
}

// Load Entity From File
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

zen_EntityFile_t* zen_SymbolLoader_loadEntityFromHandle(zen_SymbolLoader_t* loader,
    jtk_PathHandle_t* handle) {
    jtk_Assert_assertObject(loader, "The specified entity loader is null.");
    jtk_Assert_assertObject(handle, "The specified entity path handle is null.");

    zen_EntityFile_t* result = NULL;
    jtk_FileInputStream_t* fileInputStream = jtk_FileInputStream_newFromHandle(handle);
    if (fileInputStream != NULL) {
        jtk_BufferedInputStream_t* bufferedInputStream = jtk_BufferedInputStream_newEx(
            fileInputStream->m_inputStream, ZEN_ENTITY_LOADER_BUFFER_SIZE);
        jtk_InputStream_t* inputStream = bufferedInputStream->m_inputStream;

        jtk_ByteArray_t* input = jtk_InputStreamHelper_toArray(inputStream);

        zen_BinaryEntityParser_t* parser = zen_BinaryEntityParser_new(
            loader->m_attributeParseRules, input->m_values, input->m_size);
        result = zen_BinaryEntityParser_parse(parser, inputStream);

        zen_BinaryEntityParser_delete(parser);
        jtk_ByteArray_delete(input);
        jtk_InputStream_destroy(inputStream);
    }
    else {
        // Warning: Failed to load entity from handle.
    }

    return result;
}

// Ignore Corrupt Entity

bool zen_SymbolLoader_shouldIgnoreCorruptEntity(zen_SymbolLoader_t* loader) {
    jtk_Assert_assertObject(loader, "The specified entity loader is null.");

    return (loader->m_flags & ZEN_ENTITY_LOADER_FLAG_IGNORE_CORRUPT_ENTITY) != 0;
}

void zen_SymbolLoader_setIgnoreCorruptEntity(zen_SymbolLoader_t* loader,
    bool ignoreCorruptEntity) {
    jtk_Assert_assertObject(loader, "The specified entity loader is null.");

    loader->m_flags = ignoreCorruptEntity?
        (loader->m_flags | ZEN_ENTITY_LOADER_FLAG_IGNORE_CORRUPT_ENTITY) :
        (loader->m_flags & ~ZEN_ENTITY_LOADER_FLAG_IGNORE_CORRUPT_ENTITY);
}

// Load

zen_Symbol_t* zen_SymbolLoader_loadFromEntityFile(zen_SymbolLoader_t* loader,
    const uint8_t* descriptor, int32_t descriptorSize,
    zen_EntityFile_t* entityFile) {
    jtk_Assert_assertObject(loader, "The specified class loader is null.");
    jtk_Assert_assertObject(descriptor, "The specified class descriptor is null.");
    jtk_Assert_assertObject(entityFile, "The specified entity file is null.");

    zen_Symbol_t* symbol = zen_SymbolLoader_makeClassSymbol(loader,
        entityFile);
    uint8_t* descriptorCopy = jtk_CString_newEx(descriptor, descriptorSize);
    jtk_HashMap_put(loader->m_classes, descriptorCopy, symbol);

    return symbol;
}