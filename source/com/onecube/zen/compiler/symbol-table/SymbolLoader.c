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
#include <jtk/collection/array/ByteArray.h>
#include <jtk/collection/list/DoublyLinkedList.h>
#include <jtk/fs/Path.h>
#include <jtk/fs/PathHandle.h>
#include <jtk/fs/FileInputStream.h>
#include <jtk/io/BufferedInputStream.h>
#include <jtk/io/InputStream.h>
#include <jtk/io/InputStreamHelper.h>
#include <jtk/core/CString.h>
#include <jtk/core/CStringObjectAdapter.h>

#include <com/onecube/zen/virtual-machine/feb/BinaryEntityFormat.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPool.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolClass.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolDouble.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolField.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolFloat.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolFunction.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolInteger.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolLong.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolString.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolTag.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolUtf8.h>

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
    loader->m_index = 0;
    loader->m_bytes = NULL;
    loader->m_size = -1;
    loader->m_constantPool.m_size = 0;
    loader->m_constantPool.m_entries = NULL;
    loader->m_symbol = NULL;

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

    if (loader->m_constantPool.m_entries != NULL) {
        jtk_Memory_deallocate(loader->m_constantPool.m_entries);
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

        jtk_ByteArray_t* input = jtk_InputStreamHelper_toByteArray(inputStream);

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
    jtk_Logger_t* logger = loader->m_compiler->m_logger;
    zen_ConstantPool_t* constantPool = &loader->m_constantPool;
    constantPool->m_size = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
        (loader->m_bytes[loader->m_index++] & 0xFF);
    constantPool->m_entries = jtk_Memory_allocate(zen_ConstantPoolEntry_t*, loader->m_size + 1);
    constantPool->m_entries[0] = NULL;

    int32_t i;
    for (i = 1; i <= constantPool->m_size; i++) {
        uint8_t tag = loader->m_bytes[loader->m_index++];

        switch (tag) {
            case ZEN_CONSTANT_POOL_TAG_INTEGER: {
                uint32_t value = ((loader->m_bytes[loader->m_index++] & 0xFF) << 24) |
                    ((loader->m_bytes[loader->m_index++] & 0xFF) << 16) |
                    ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);

                zen_ConstantPoolInteger_t* constantPoolInteger =
                    jtk_Memory_allocate(zen_ConstantPoolInteger_t, 1);
                constantPoolInteger->m_tag = ZEN_CONSTANT_POOL_TAG_INTEGER;
                constantPoolInteger->m_bytes = value;

                constantPool->m_entries[i] = constantPoolInteger;

                jtk_Logger_debug(logger, "Parsed constant pool entry `zen_ConstantPoolInteger_t`, stored at index %d.", i);

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

                zen_ConstantPoolLong_t* constantPoolLong = jtk_Memory_allocate(zen_ConstantPoolLong_t, 1);
                constantPoolLong->m_tag = ZEN_CONSTANT_POOL_TAG_LONG;
                constantPoolLong->m_highBytes = highBytes;
                constantPoolLong->m_lowBytes = lowBytes;

                constantPool->m_entries[i] = constantPoolLong;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolLong_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FLOAT: {
                uint32_t value = ((loader->m_bytes[loader->m_index++] & 0xFF) << 24) |
                    ((loader->m_bytes[loader->m_index++] & 0xFF) << 16) |
                    ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);

                zen_ConstantPoolFloat_t* constantPoolFloat = jtk_Memory_allocate(zen_ConstantPoolFloat_t, 1);
                constantPoolFloat->m_tag = ZEN_CONSTANT_POOL_TAG_FLOAT;
                constantPoolFloat->m_bytes = value;

                constantPool->m_entries[i] = constantPoolFloat;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolFloat_t`, stored at index %d.", index);

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

                zen_ConstantPoolDouble_t* constantPoolDouble = jtk_Memory_allocate(zen_ConstantPoolDouble_t, 1);
                constantPoolDouble->m_tag = ZEN_CONSTANT_POOL_TAG_DOUBLE;
                constantPoolDouble->m_highBytes = highBytes;
                constantPoolDouble->m_lowBytes = lowBytes;

                constantPool->m_entries[i] = constantPoolDouble;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolDouble_t`, stored at index %d.", index);

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
                jtk_Arrays_copyEx_b(loader->m_bytes, loader->m_size, loader->m_index,
                    value, length, 0, length);
                loader->m_index += length;

                zen_ConstantPoolUtf8_t* constantPoolUtf8 = jtk_Memory_allocate(zen_ConstantPoolUtf8_t, 1);
                constantPoolUtf8->m_tag = ZEN_CONSTANT_POOL_TAG_UTF8;
                constantPoolUtf8->m_length = length;
                constantPoolUtf8->m_bytes = value;

                constantPool->m_entries[i] = constantPoolUtf8;

                // jtk_Logger_debug(logger, "Parsed constant pool entry `zen_ConstantPoolUtf8_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_STRING: {
                uint16_t stringIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);

                zen_ConstantPoolString_t* constantPoolString = jtk_Memory_allocate(zen_ConstantPoolString_t, 1);
                constantPoolString->m_tag = ZEN_CONSTANT_POOL_TAG_STRING;
                constantPoolString->m_stringIndex = stringIndex;

                constantPool->m_entries[i] = constantPoolString;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolString_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FUNCTION: {
                uint16_t classIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);
                uint16_t descriptorIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);
                uint16_t nameIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);
                uint16_t tableIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);

                zen_ConstantPoolFunction_t* constantPoolFunction = jtk_Memory_allocate(zen_ConstantPoolFunction_t, 1);
                constantPoolFunction->m_tag = ZEN_CONSTANT_POOL_TAG_FUNCTION;
                constantPoolFunction->m_classIndex = classIndex;
                constantPoolFunction->m_descriptorIndex = descriptorIndex;
                constantPoolFunction->m_nameIndex = nameIndex;
                constantPoolFunction->m_tableIndex = tableIndex;

                constantPool->m_entries[i] = constantPoolFunction;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolFunction_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FIELD: {
                uint16_t classIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);
                uint16_t descriptorIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);
                uint16_t nameIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);

                zen_ConstantPoolField_t* constantPoolField = jtk_Memory_allocate(zen_ConstantPoolField_t, 1);
                constantPoolField->m_tag = ZEN_CONSTANT_POOL_TAG_FIELD;
                constantPoolField->m_classIndex = classIndex;
                constantPoolField->m_descriptorIndex = descriptorIndex;
                constantPoolField->m_nameIndex = nameIndex;

                constantPool->m_entries[i] = constantPoolField;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolField_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_CLASS: {
                uint16_t nameIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);

                zen_ConstantPoolClass_t* constantPoolClass = jtk_Memory_allocate(zen_ConstantPoolClass_t, 1);
                constantPoolClass->m_tag = ZEN_CONSTANT_POOL_TAG_CLASS;
                constantPoolClass->m_nameIndex = nameIndex;

                constantPool->m_entries[i] = constantPoolClass;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolClass_t`, stored at index %d.", index);

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

    // Table Index
    uint16_t tableIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
        (loader->m_bytes[loader->m_index++] & 0xFF);

    // Define field

    // Skip attribute table
    zen_SymbolLoader_skipAttributeTable(loader);
}

void zen_SymbolLoader_declareFunction(zen_SymbolLoader_t* loader,
    zen_Symbol_t* symbol, const uint8_t* descriptor, int32_t descriptorSize,
    uint16_t modifiers, uint16_t tableIndex) {
    zen_FunctionSymbol_t* functionSymbol = &symbol->m_context.m_asFunction;
    zen_FunctionSignature_t* signature = zen_FunctionSignature_newEx(descriptor, descriptorSize,
        modifiers, tableIndex);
    zen_FunctionSymbol_addSignature(functionSymbol, signature);
}

static const uint8_t* newName = "new";

void zen_SymbolLoader_parseFunction(zen_SymbolLoader_t* loader) {
    // Flags
    uint16_t flags = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
        (loader->m_bytes[loader->m_index++] & 0xFF);

    // Name Index
    uint16_t nameIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
        (loader->m_bytes[loader->m_index++] & 0xFF);
    zen_ConstantPoolUtf8_t* name = loader->m_constantPool.m_entries[nameIndex];

    // Descriptor Index
    uint16_t descriptorIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
        (loader->m_bytes[loader->m_index++] & 0xFF);
    zen_ConstantPoolUtf8_t* descriptor = loader->m_constantPool.m_entries[descriptorIndex];

    /* NOTE: Parameter threshold set by a statically typed language prevents
     * a dynamically typed language such as Zen from declaring functions with
     * variable parameters.
     */
    // uint16_t parameterThreshold = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
    //     (loader->m_bytes[loader->m_index++] & 0xFF);

    uint16_t tableIndex = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
        (loader->m_bytes[loader->m_index++] & 0xFF);

    // Define function
    zen_Symbol_t* classSymbol = loader->m_symbol;
    zen_Scope_t* classScope = classSymbol->m_context.m_asClass.m_classScope;
    zen_Symbol_t* functionSymbol = zen_Scope_resolve(classScope, descriptor->m_bytes);
    if (functionSymbol == NULL) {
        functionSymbol = zen_Symbol_forFunction(NULL, classScope);
        const uint8_t* name0 = name->m_bytes;
        int32_t nameSize0 = name->m_length;
        if (jtk_CString_equals(name->m_bytes, name->m_length, "<initialize>", 12)) {
            name0 = newName;
            nameSize0 = 3;
        }
        functionSymbol->m_name = name0;
        functionSymbol->m_nameSize = nameSize0;
        functionSymbol->m_modifiers = flags;
        zen_Scope_defineEx(classScope, name0, nameSize0, functionSymbol);
    }
    zen_SymbolLoader_declareFunction(loader, functionSymbol, descriptor->m_bytes,
        descriptor->m_length, flags, tableIndex);

    // Skip attribute table
    zen_SymbolLoader_skipAttributeTable(loader);
}

void zen_SymbolLoader_destroyConstantPool(zen_SymbolLoader_t* loader) {
    zen_ConstantPool_t* constantPool = &loader->m_constantPool;
    int32_t i;
    for (i = 1; i <= constantPool->m_size; i++) {
        zen_ConstantPoolEntry_t* entry = (zen_ConstantPoolEntry_t*)constantPool->m_entries[i];
        if (entry->m_tag == ZEN_CONSTANT_POOL_TAG_UTF8) {
            zen_ConstantPoolUtf8_t* utf8Entry = (zen_ConstantPoolUtf8_t*)entry;
            jtk_Memory_deallocate(utf8Entry->m_bytes);
        }
        jtk_Memory_deallocate(entry);
    }
    jtk_Memory_deallocate(loader->m_constantPool.m_entries);
}

zen_Symbol_t* zen_SymbolLoader_parse(zen_SymbolLoader_t* loader, uint8_t* bytes,
    int32_t size) {
    zen_Compiler_t* compiler = loader->m_compiler;
    zen_ErrorHandler_t* errorHandler = compiler->m_errorHandler;
    jtk_Logger_t* logger = compiler->m_logger;

    loader->m_bytes = bytes;
    loader->m_size = size;

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
                uint16_t entityFlags = (uint16_t)(((uint32_t)(loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
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

                zen_ConstantPoolUtf8_t* descriptor = loader->m_constantPool.m_entries[reference];

                zen_Scope_t* classScope = zen_Scope_forClass(NULL);
                loader->m_symbol = zen_Symbol_forClassAlt( classScope, descriptor->m_bytes,
                    descriptor->m_length);
                classScope->m_symbol = loader->m_symbol;

                /* Skip attribute table */
                zen_SymbolLoader_skipAttributeTable(loader);

                /* Parse fields
                 * fieldCount fieldEntity*
                 */
                uint16_t fieldCount = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);
                uint16_t fieldTableSize = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);
                int32_t j;
                for (j = 0; j < fieldCount; j++) {
                    zen_SymbolLoader_parseField(loader);
                }

                /* Parse functions
                 * functionCount functionEntity*
                 */

                uint16_t functionCount = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);
                uint16_t functionTableSize = ((loader->m_bytes[loader->m_index++] & 0xFF) << 8) |
                    (loader->m_bytes[loader->m_index++] & 0xFF);
                int32_t k;
                for (k = 0; k < functionCount; k++) {
                    zen_SymbolLoader_parseFunction(loader);
                }
            }
            else {
                zen_ErrorHandler_handleGeneralError(errorHandler, loader, ZEN_ERROR_CODE_INVALID_FEB_VERSION);
            }
        }
        else {
            zen_ErrorHandler_handleGeneralError(errorHandler, loader, ZEN_ERROR_CODE_CORRUPTED_BINARY_ENTITY);
        }
    }

    zen_Symbol_t* result = loader->m_symbol;


    /* Reset the symbol loader. */
    loader->m_index = 0;
    loader->m_bytes = NULL;
    loader->m_size = 0;
    // TODO: The constant pool needs to be freed.
    /*if (loader->m_constantPool.m_entries != NULL) {
        zen_SymbolLoader_destroyConstantPool(loader);
    }*/
    loader->m_constantPool.m_size = 0;
    loader->m_constantPool.m_entries = NULL;
    loader->m_symbol = NULL;

    return result;
}
