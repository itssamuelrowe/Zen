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

// Friday, April 17, 2019

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

#include <com/onecube/zen/disassembler/disassembler.h>

/*******************************************************************************
 * disassembler                                                    *
 *******************************************************************************/

// Constructor

zen_BinaryEntityDisassembler_t* zen_BinaryEntityDisassembler_new(zen_VirtualMachine_t* virtualMachine,
    jtk_Iterator_t* entityDirectoryIterator) {
    jtk_ObjectAdapter_t* stringObjectAdapter = jtk_CStringObjectAdapter_getInstance();

    zen_BinaryEntityDisassembler_t* disassembler = jtk_Memory_allocate(zen_BinaryEntityDisassembler_t, 1);
    disassembler->m_directories = jtk_DoublyLinkedList_new();
    disassembler->m_flags = ZEN_ENTITY_LOADER_FLAG_PRIORITIZE_DIRECTORIES;

    while (jtk_Iterator_hasNext(entityDirectoryIterator)) {
        uint8_t* directory = (uint8_t*)jtk_Iterator_getNext(entityDirectoryIterator);
        zen_BinaryEntityDisassembler_addDirectory(disassembler, directory, -1);
    }

    return disassembler;
}

// Destructor

void zen_BinaryEntityDisassembler_delete(zen_BinaryEntityDisassembler_t* disassembler) {
    jtk_Assert_assertObject(disassembler, "The specified class loader is null.");

    int32_t size = jtk_DoublyLinkedList_getSize(disassembler->m_directories);
    jtk_Iterator_t* iterator = jtk_DoublyLinkedList_getIterator(disassembler->m_directories);
    while (jtk_Iterator_hasNext(iterator)) {
        jtk_Path_t* path = (jtk_Path_t*)jtk_Iterator_getNext(iterator);
        jtk_Path_delete(path);
    }
    jtk_Iterator_delete(iterator);
    jtk_DoublyLinkedList_delete(disassembler->m_directories);

    jtk_Memory_deallocate(disassembler);
}

// Directory

bool zen_BinaryEntityDisassembler_addDirectory(zen_BinaryEntityDisassembler_t* disassembler,
    const uint8_t* directory, int32_t directorySize) {
    jtk_Assert_assertObject(disassembler, "The specified entity loader is null.");
    jtk_Assert_assertObject(directory, "The specified directory is null.");

    jtk_Path_t* path = jtk_Path_newFromStringEx(directory, directorySize);
    jtk_DoublyLinkedList_add(disassembler->m_directories, path);

    // bool result = jtk_Path_isDirectory(path);
    // if (result) {
    //    jtk_DoublyLinkedList_add(disassembler->m_directories, path);
    // }
    // else {
    //    jtk_Path_delete(path);
    // }

    return true;
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

void zen_BinaryEntityDisassembler_disassembleClass(zen_BinaryEntityDisassembler_t* disassembler,
    const uint8_t* descriptor, int32_t descriptorSize) {
    jtk_Assert_assertObject(disassembler, "The specified entity loader is null.");
    jtk_Assert_assertObject(descriptor, "The specified descriptor is null.");

    int32_t entityNameSize;
    uint8_t* entityName = jtk_CString_joinEx(descriptor, descriptorSize, ".feb",
        4, &entityNameSize);
    jtk_Path_t* entityFile = jtk_Path_newFromStringEx(entityName, entityNameSize);
    jtk_CString_delete(entityName);

    bool found = false;

    /* Retrieve an iterator over the list of registered entity directories. */
    jtk_Iterator_t* iterator = jtk_DoublyLinkedList_getIterator(disassembler->m_directories);
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
                            fileInputStream->m_inputStream, 1024 * 3);
                        jtk_InputStream_t* inputStream = bufferedInputStream->m_inputStream;

                        jtk_ByteArray_t* input = jtk_InputStreamHelper_toArray(inputStream);

                        zen_BinaryEntityDisassembler_disassemble(
                            disassembler, input->m_values, input->m_size);
                        found = true;

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

    if (!found) {
        printf("[error] Could not find class '%s'.\n", descriptor);
    }
}

// Ignore Corrupt Entity

bool zen_BinaryEntityDisassembler_shouldIgnoreCorruptEntity(zen_BinaryEntityDisassembler_t* disassembler) {
    jtk_Assert_assertObject(disassembler, "The specified entity loader is null.");

    return (disassembler->m_flags & ZEN_ENTITY_LOADER_FLAG_IGNORE_CORRUPT_ENTITY) != 0;
}

void zen_BinaryEntityDisassembler_setIgnoreCorruptEntity(zen_BinaryEntityDisassembler_t* disassembler,
    bool ignoreCorruptEntity) {
    jtk_Assert_assertObject(disassembler, "The specified entity loader is null.");

    disassembler->m_flags = ignoreCorruptEntity?
        (disassembler->m_flags | ZEN_ENTITY_LOADER_FLAG_IGNORE_CORRUPT_ENTITY) :
        (disassembler->m_flags & ~ZEN_ENTITY_LOADER_FLAG_IGNORE_CORRUPT_ENTITY);
}