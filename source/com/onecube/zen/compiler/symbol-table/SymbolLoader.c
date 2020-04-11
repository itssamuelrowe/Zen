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
#include <jtk/collection/array/Arrays.h>
#include <jtk/collection/list/DoublyLinkedList.h>
#include <jtk/fs/Path.h>
#include <jtk/fs/PathHandle.h>
#include <jtk/fs/FileInputStream.h>
#include <jtk/io/BufferedInputStream.h>
#include <jtk/io/InputStream.h>
#include <jtk/core/CString.h>
#include <jtk/core/CStringObjectAdapter.h>

#include <com/onecube/zen/virtual-machine/feb/BinaryEntityFormat.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/constantPool->.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/constantPool->Class.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/constantPool->Double.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/constantPool->Field.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/constantPool->Float.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/constantPool->Function.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/constantPool->Integer.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/constantPool->Long.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/constantPool->String.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/constantPool->Tag.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/constantPool->Utf8.h>

#include <com/onecube/zen/compiler/symbol-table/SymbolLoader.h>
#include <com/onecube/zen/compiler/Compiler.h>

/*******************************************************************************
 * SymbolLoader                                                                *
 *******************************************************************************/

zen_SymbolLoader_t* zen_SymbolLoader_new(zen_Compiler_t* compiler) {
    jtk_ObjectAdapter_t* stringObjectAdapter = jtk_CStringObjectAdapter_getInstance();

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
    jtk_ObjectAdapter_t* stringObjectAdapter = jtk_CStringObjectAdapter_getInstance();

    zen_SymbolLoader_t* loader = zen_SymbolLoader_new(compiler);
    while (jtk_Iterator_hasNext(entityDirectoryIterator)) {
        uint8_t* directory = (uint8_t*)jtk_Iterator_getNext(entityDirectoryIterator);
        zen_SymbolLoader_addDirectory(loader, directory);
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

        zen_Symbol_t* symbol = (zen_Symbol_t*)jtk_HashMapEntry_getValue(entry->m_value);
        zen_Symbol_delete(symbol);
    }
    jtk_Iterator_delete(entryIterator);
    jtk_HashMap_delete(loader->m_symbols);

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
    const uint8_t* descriptor, int32_t descriptorSize) {
    jtk_Assert_assertObject(loader, "The specified entity loader is null.");
    jtk_Assert_assertObject(descriptor, "The specified descriptor is null.");

    zen_Symbol_t* result = (zen_Symbol_t*)jtk_HashMap_getValue(loader->m_symbols, descriptor);
    if (result == NULL) {
        result = zen_SymbolLoader_loadSymbol(loader, descriptor,
            descriptorSize);
    }

    return result;
}

// Load Symbol

zen_Symbol_t* zen_SymbolLoader_loadSymbol(zen_SymbolLoader_t* loader,
    const uint8_t* descriptor, int32_t descriptorSize) {
    jtk_Assert_assertObject(loader, "The specified entity loader is null.");
    jtk_Assert_assertObject(descriptor, "The specified descriptor is null.");

    zen_Symbol_t* result = NULL;

    int32_t fileNameSize;
    uint8_t* fileName = jtk_CString_join(descriptor, ".feb", &fileNameSize);
    jtk_Arrays_replaceEx_b(fileName, fileNameSize, '.', '/', 0, fileNameSize - 4);
    jtk_Path_t* entityFile = jtk_Path_newFromStringEx(fileName, fileNameSize);
    jtk_CString_delete(fileName);

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
                        uint8_t* identifier = jtk_CString_newEx(descriptor, descriptorSize);
                        jtk_HashMap_put(loader->m_symbols, identifier, result);
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
    jtk_ByteArray_t* array = (index == 0)? NULL : jtk_ByteArray_fromRawArray(result, index);

    jtk_Memory_deallocate(result);

    return array;
}

zen_Symbol_t* zen_SymbolLoader_loadSymbolFromHandle(zen_SymbolLoader_t* loader,
    jtk_PathHandle_t* handle) {
    jtk_Assert_assertObject(loader, "The specified entity loader is null.");
    jtk_Assert_assertObject(handle, "The specified entity path handle is null.");

    zen_Symbol_t* result = NULL;
    jtk_FileInputStream_t* fileInputStream = jtk_FileInputStream_newFromHandle(handle);
    if (fileInputStream != NULL) {
        jtk_BufferedInputStream_t* bufferedInputStream = jtk_BufferedInputStream_newEx(
            fileInputStream->m_inputStream, ZEN_ENTITY_LOADER_BUFFER_SIZE);
        jtk_InputStream_t* inputStream = bufferedInputStream->m_inputStream;

        jtk_ByteArray_t* input = jtk_InputStreamHelper_toArray(inputStream);

        result = zen_SymbolLoader_parse(loader, input->m_values, input->m_size);

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

// Parse

#define ZEN_FEB_HEADER_SIZE 12

void zen_SymbolLoader_parseConstantPool(zen_SymbolLoader_t* loader) {
    zen_constantPool->_t* constantPool = &loader->m_constantPool;
    constantPool->->m_size = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
        (loader->m_bytes[loader->m_index++] & 0xFF);
    constantPool->->m_entries = jtk_Memory_allocate(zen_constantPool->Entry_t*, size + 1);
    constantPool->->m_entries[0] = NULL;

    int32_t i;
    for (i = 1; i <= size; i++) {
        uint8_t tag = loader->m_bytes[loader->m_index++];

        switch (tag) {
            case ZEN_CONSTANT_POOL_TAG_INTEGER: {
                uint32_t value = ((loader->m_bytes[loader->m_index++] & 0xFF) << 24) |
                    ((loader->m_bytes[loader->m_index++] & 0xFF) << 16) |
                    ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);

                zen_constantPool->Integer_t* constantPool->Integer =
                    jtk_Memory_allocate(zen_constantPool->Integer_t, 1);
                constantPool->Integer->m_tag = ZEN_CONSTANT_POOL_TAG_INTEGER;
                constantPool->Integer->m_bytes = value;

                constantPool->.m_entries[i] = constantPool->Integer;

                jtk_Logger_debug(logger, "Parsed constant pool entry `zen_constantPool->Integer_t`, stored at index %d.", i);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_LONG: {
                uint32_t highBytes = ((loader->m_bytes[loader->m_index++] & 0xFF) << 24) |
                    ((loader->m_bytes[loader->m_index++] & 0xFF) << 16) |
                    ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);
                uint32_t lowBytes = ((loader->m_bytes[loader->m_index++] & 0xFF) << 24) |
                    ((loader->m_bytes[loader->m_index++] & 0xFF) << 16) |
                    ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);

                zen_constantPool->Long_t* constantPool->Long = jtk_Memory_allocate(zen_constantPool->Long_t, 1);
                constantPool->Long->m_tag = ZEN_CONSTANT_POOL_TAG_LONG;
                constantPool->Long->m_highBytes = highBytes;
                constantPool->Long->m_lowBytes = lowBytes;

                constantPool->.m_entries[i] = constantPool->Long;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_constantPool->Long_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FLOAT: {
                uint32_t value = ((loader->m_bytes[loader->m_index++] & 0xFF) << 24) |
                    ((loader->m_bytes[loader->m_index++] & 0xFF) << 16) |
                    ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);

                zen_constantPool->Float_t* constantPool->Float = jtk_Memory_allocate(zen_constantPool->Float_t, 1);
                constantPool->Float->m_tag = ZEN_CONSTANT_POOL_TAG_FLOAT;
                constantPool->Float->m_bytes = value;

                constantPool->.m_entries[i] = constantPool->Float;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_constantPool->Float_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_DOUBLE: {
                uint32_t highBytes = ((loader->m_bytes[loader->m_index++] & 0xFF) << 24) |
                    ((loader->m_bytes[loader->m_index++] & 0xFF) << 16) |
                    ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);
                uint32_t lowBytes = ((loader->m_bytes[loader->m_index++] & 0xFF) << 24) |
                    ((loader->m_bytes[loader->m_index++] & 0xFF) << 16) |
                    ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);

                zen_constantPool->Double_t* constantPool->Double = jtk_Memory_allocate(zen_constantPool->Double_t, 1);
                constantPool->Double->m_tag = ZEN_CONSTANT_POOL_TAG_DOUBLE;
                constantPool->Double->m_highBytes = highBytes;
                constantPool->Double->m_lowBytes = lowBytes;

                constantPool->.m_entries[i] = constantPool->Double;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_constantPool->Double_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_UTF8: {
                uint16_t length = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);
                /* The specification guarantees that an empty string is never stored in a constant pool.
                    *
                    * Although the speciication does not specify the bytes to be null-terminated,
                    * the reference implementation of the compiler terminates UTF8 constant pool
                    * entries for performance.
                    */

                uint8_t* value = jtk_Memory_allocate(uint8_t, length + 1);
                value[length] = '\0';
                jtk_Arrays_copyEx_b(bytes, size, index, value, length, 0, length);

                zen_constantPool->Utf8_t* constantPool->Utf8 = jtk_Memory_allocate(zen_constantPool->Utf8_t, 1);
                constantPool->Utf8->m_tag = ZEN_CONSTANT_POOL_TAG_UTF8;
                constantPool->Utf8->m_length = length;
                constantPool->Utf8->m_bytes = value;

                constantPool->.m_entries[i] = constantPool->Utf8;

                jtk_Logger_debug(logger, "Parsed constant pool entry `zen_constantPool->Utf8_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_STRING: {
                uint16_t stringIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);

                zen_constantPool->String_t* constantPool->String = jtk_Memory_allocate(zen_constantPool->String_t, 1);
                constantPool->String->m_tag = ZEN_CONSTANT_POOL_TAG_STRING;
                constantPool->String->m_stringIndex = stringIndex;

                constantPool->.m_entries[i] = constantPool->String;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_constantPool->String_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FUNCTION: {
                uint16_t classIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);
                uint16_t descriptorIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);
                uint16_t nameIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);

                zen_constantPool->Function_t* constantPool->Function = jtk_Memory_allocate(zen_constantPool->Function_t, 1);
                constantPool->Function->m_tag = ZEN_CONSTANT_POOL_TAG_FUNCTION;
                constantPool->Function->m_classIndex = classIndex;
                constantPool->Function->m_descriptorIndex = descriptorIndex;
                constantPool->Function->m_nameIndex = nameIndex;

                constantPool->.m_entries[i] = constantPool->Function;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_constantPool->Function_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FIELD: {
                uint16_t descriptorIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);
                uint16_t nameIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);

                zen_constantPool->Field_t* constantPool->Field = jtk_Memory_allocate(zen_constantPool->Field_t, 1);
                constantPool->Field->m_tag = ZEN_CONSTANT_POOL_TAG_FIELD;
                constantPool->Field->m_descriptorIndex = descriptorIndex;
                constantPool->Field->m_nameIndex = nameIndex;

                constantPool->.m_entries[i] = constantPool->Field;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_constantPool->Field_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_CLASS: {
                uint16_t nameIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);

                zen_constantPool->Class_t* constantPool->Class = jtk_Memory_allocate(zen_constantPool->Class_t, 1);
                constantPool->Class->m_tag = ZEN_CONSTANT_POOL_TAG_CLASS;
                constantPool->Class->m_nameIndex = nameIndex;

                constantPool->.m_entries[i] = constantPool->Class;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_constantPool->Class_t`, stored at index %d.", index);

                break;
            }
        }
    }
}

void zen_SymbolLoader_skipAttributeTable(zen_SymbolLoader_t* loader) {
    uint16_t size = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
        (loader->m_bytes[loader->m_index++] & 0xFF);
    int32_t i;
    for (i = 0; i < size; i++) {
        uint16_t nameIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
        (loader->m_bytes[loader->m_index++] & 0xFF);
        uint32_t length = ((loader->m_bytes[loader->m_index++] & 0xFF) << 24) |
            ((loader->m_bytes[loader->m_index++] & 0xFF) << 16) |
            ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
            (loader->m_bytes[loader->m_index++] & 0xFF);

        /* Skip the bytes occupied by the unrecognized attribute. */
        loader->m_index += length;
    }
}

void zen_SymbolLoader_parseField(zen_SymbolLoader_t* loader) {
    // Flags
    uint16_t flags = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
        (loader->m_bytes[loader->m_index++] & 0xFF);

    // Name Index
    uint16_t nameIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
        (loader->m_bytes[loader->m_index++] & 0xFF);

    // Descriptor Index
    uint16_t descriptorIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
        (loader->m_bytes[loader->m_index++] & 0xFF);

    // Define field

    // Skip attribute table
    zen_SymbolLoader_skipAttributeTable(loader);
}

zen_Symbol_t* zen_SymbolLoader_parse(zen_SymbolLoader_t* loader, uint8_t* bytes,
    int32_t size) {
    zen_Compiler_t* compiler = loader->m_compiler;
    zen_ErrorHandler_t* errorHandler = compiler->m_errorHandler;
    jtk_Logger_t* logger = compiler->m_logger;

    if (loader->m_index + ZEN_FEB_HEADER_SIZE < size) {
        uint32_t magicNumber = ((loader->m_bytes[loader->m_index++] & 0xFF) << 24) |
                               ((loader->m_bytes[loader->m_index++] & 0xFF) << 16) |
                               ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                               (loader->m_bytes[loader->m_index++] & 0xFF);
        if (magicNumber == ZEN_BINARY_ENTITY_FORMAT_MAGIC_NUMBER) {
            uint16_t majorVersion = (uint16_t)(((uint32_t)(loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                (loader->m_bytes[loader->m_index++] & 0xFF));
            uint16_t minorVersion = (uint16_t)(((uint32_t)(loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                (loader->m_bytes[loader->m_index++] & 0xFF));

            if ((majorVersion < ZEN_BINARY_ENTITY_FORMAT_MAJOR_VERSION) ||
                ((majorVersion == ZEN_BINARY_ENTITY_FORMAT_MAJOR_VERSION) &&
                (minorVersion <= ZEN_BINARY_ENTITY_FORMAT_MINOR_VERSION))) {
                uint16_t flags = (uint16_t)(((uint32_t)(loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF));

                // Constant Pool
                zen_SymbolLoader_parseConstantPool(loader);

                /* Parse the entity */

                uint8_t type = (loader->m_bytes[loader->m_index++] & 0xFF);
                uint16_t flags = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                                (loader->m_bytes[loader->m_index++] & 0xFF);
                uint16_t reference = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                                (loader->m_bytes[loader->m_index++] & 0xFF);
                uint16_t superclassCount = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);
                uint16_t* superclasses = zen_Memory_allocate(uint16_t, superclassCount);
                int32_t i;
                for (i = 0; i < superclassCount; i++) {
                    superclasses[i] = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                        (loader->m_bytes[loader->m_index++] & 0xFF);
                }

                /* Parse attribute table */
                zen_SymbolLoader_skipAttributeTable(loader);

                /* Parse fields
                 * fieldCount fieldEntity*
                 */

                uint16_t fieldCount = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);
                fields = (fieldCount > 0)?
                    jtk_Memory_allocate(zen_FieldEntity_t*, fieldCount) : NULL;

                int32_t j;
                for (j = 0; j < fieldCount; j++) {
                    zen_SymbolLoader_parseField(parser);
                }

                // functionCount functionEntity*

                uint16_t functionCount = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);

                functions = (functionCount > 0)?
                    jtk_Memory_allocate(zen_FunctionEntity_t*, functionCount) : NULL;

                int32_t k;
                for (k = 0; k < functionCount; k++) {
                    zen_FunctionEntity_t* functionEntity = zen_BinaryEntityParser_parseFunction(parser);
                    entity->m_functions[k] = functionEntity;
                }
            }
            else {
                zen_ErrorHandler_handleGeneralError(errorHandler, ZEN_ERROR_CODE_INVALID_FEB_VERSION);
            }
        }
        else {
            zen_ErrorHandler_handleGeneralError(errorHandler, ZEN_ERROR_CODE_CORRUPTED_BINARY_ENTITY);
        }
    }

    return NULL;
}
