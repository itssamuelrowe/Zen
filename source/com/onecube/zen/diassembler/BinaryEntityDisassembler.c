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

void zen_BinaryEntityDisassembler_disassemble(zen_BinaryEntityDisassembler_t* disassembler) {
    disassembler->m_bytes = bytes;
    disassembler->m_size = size;

    zen_BinaryEntityDisassembler_disassembleEntity(disassembler);
}

/* Disassemble Entity File */

zen_EntityFile_t* zen_BinaryEntityDisassembler_disassembleEntityFile(zen_BinaryEntityDisassembler_t* disassembler) {
    jtk_Assert_assertObject(disassembler, "The specified binary entity disassembler is null.");

    zen_EntityFile_t* entityFile = jtk_Memory_allocate(zen_EntityFile_t, 1);
    disassembler->m_entityFile = entityFile;

    if (disassembler->m_index + 12 < disassembler->m_size) {
        /* The magic number is a simple measure to identify corrupt streams.
         * It is represented with four bytes, that is, a 32-bit integer.
         *
         * Read the magic number and verify whether it is equal to 0xFEB72000.
         */
        uint32_t magicNumber = ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 24) |
            ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 16) |
            ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
            (disassembler->m_bytes[disassembler->m_index++] & 0xFF);

        if (magicNumber == ZEN_BINARY_ENTITY_FORMAT_MAGIC_NUMBER) {
            entityFile->m_magicNumber = magicNumber;

            uint16_t majorVersion = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
                (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
            entityFile->m_version.m_majorVersion = majorVersion;

            uint16_t minorVersion = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
                (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
            entityFile->m_version.m_minorVersion = minorVersion;

            /* Make sure that the major and minor version numbers are recognized by
             * the binary entity disassembler.
             */
            if ((majorVersion < ZEN_BINARY_ENTITY_FORMAT_MAJOR_VERSION) ||
                ((majorVersion == ZEN_BINARY_ENTITY_FORMAT_MAJOR_VERSION) &&
                 (minorVersion <= ZEN_BINARY_ENTITY_FORMAT_MINOR_VERSION))) {
                uint16_t flags = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
                (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
                entityFile->m_flags = flags;

                zen_BinaryEntityDisassembler_disassembleConstantPool(disassembler);
                zen_BinaryEntityDisassembler_disassembleEntity(disassembler);
            }
            else {
                // Error: Virtual machine version is lesser than the binary entity file version.
            }
        }
        else {
            // Error: Unknown magic number
        }
    }

    return entityFile;
}

/* NOTE: The following functions have been sorted with respect to the specification
 * of the Binary Entity Format. This is simply a convenience for the developers.
 * It holds no special meaning or whatsoever.
 */

/* Disassemble Constant Pool */

void zen_BinaryEntityDisassembler_disassembleConstantPool(
    zen_BinaryEntityDisassembler_t* disassembler) {
    jtk_Assert_assertObject(disassembler, "The specified binary entity disassembler is null.");

    // jtk_Logger_info(disassembler->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsing constant pool entries...");

    uint16_t size = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
                (disassembler->m_bytes[disassembler->m_index++] & 0xFF));

    zen_ConstantPool_t* constantPool = &disassembler->m_entityFile->m_constantPool;
    constantPool->m_size = size;
    constantPool->m_entries = jtk_Memory_allocate(zen_ConstantPoolEntry_t*, size + 1);
    constantPool->m_entries[0] = NULL;

    int32_t index;
    for (index = 1; index <= size; index++) {
        uint8_t tag = disassembler->m_bytes[disassembler->m_index++];

        switch (tag) {
            case ZEN_CONSTANT_POOL_TAG_INTEGER: {
                uint32_t bytes = ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 24) |
                    ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 16) |
                    ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
                    (disassembler->m_bytes[disassembler->m_index++] & 0xFF);

                zen_ConstantPoolInteger_t* constantPoolInteger = jtk_Memory_allocate(zen_ConstantPoolInteger_t, 1);
                constantPoolInteger->m_tag = ZEN_CONSTANT_POOL_TAG_INTEGER;
                constantPoolInteger->m_bytes = bytes;

                constantPool->m_entries[index] = constantPoolInteger;

                // jtk_Logger_info(disassembler->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Disassembled constant pool entry `zen_ConstantPoolInteger_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_LONG: {
                uint32_t highBytes = ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 24) |
                    ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 16) |
                    ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
                    (disassembler->m_bytes[disassembler->m_index++] & 0xFF);
                uint32_t lowBytes = ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 24) |
                    ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 16) |
                    ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
                    (disassembler->m_bytes[disassembler->m_index++] & 0xFF);

                zen_ConstantPoolLong_t* constantPoolLong = jtk_Memory_allocate(zen_ConstantPoolLong_t, 1);
                constantPoolLong->m_tag = ZEN_CONSTANT_POOL_TAG_LONG;
                constantPoolLong->m_highBytes = highBytes;
                constantPoolLong->m_lowBytes = lowBytes;

                constantPool->m_entries[index] = constantPoolLong;

                // jtk_Logger_info(disassembler->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Disassembled constant pool entry `zen_ConstantPoolLong_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FLOAT: {
                uint32_t bytes = ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 24) |
                    ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 16) |
                    ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
                    (disassembler->m_bytes[disassembler->m_index++] & 0xFF);

                zen_ConstantPoolFloat_t* constantPoolFloat = jtk_Memory_allocate(zen_ConstantPoolFloat_t, 1);
                constantPoolFloat->m_tag = ZEN_CONSTANT_POOL_TAG_FLOAT;
                constantPoolFloat->m_bytes = bytes;

                constantPool->m_entries[index] = constantPoolFloat;

                // jtk_Logger_info(disassembler->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Disassembled constant pool entry `zen_ConstantPoolFloat_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_DOUBLE: {
                uint32_t highBytes = ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 24) |
                    ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 16) |
                    ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
                    (disassembler->m_bytes[disassembler->m_index++] & 0xFF);
                uint32_t lowBytes = ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 24) |
                    ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 16) |
                    ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
                    (disassembler->m_bytes[disassembler->m_index++] & 0xFF);

                zen_ConstantPoolDouble_t* constantPoolDouble = jtk_Memory_allocate(zen_ConstantPoolDouble_t, 1);
                constantPoolDouble->m_tag = ZEN_CONSTANT_POOL_TAG_DOUBLE;
                constantPoolDouble->m_highBytes = highBytes;
                constantPoolDouble->m_lowBytes = lowBytes;

                constantPool->m_entries[index] = constantPoolDouble;

                // jtk_Logger_info(disassembler->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Disassembled constant pool entry `zen_ConstantPoolDouble_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_UTF8: {
                uint16_t length = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
                (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
                /* The specification guarantees that an empty string is never stored in a constant pool.
                 *
                 * Although the speciication does not specify the bytes to be null-terminated,
                 * the reference implementation of the Zen Virtual Machine terminates
                 * UTF8 constant pool entries for performance.
                 */

                uint8_t* bytes = jtk_Memory_allocate(uint8_t, length + 1);
                bytes[length] = '\0';
                jtk_Arrays_copyEx_b(disassembler->m_bytes, disassembler->m_size, disassembler->m_index,
                    bytes, length, 0, length);
                disassembler->m_index += length;

                zen_ConstantPoolUtf8_t* constantPoolUtf8 = jtk_Memory_allocate(zen_ConstantPoolUtf8_t, 1);
                constantPoolUtf8->m_tag = ZEN_CONSTANT_POOL_TAG_UTF8;
                constantPoolUtf8->m_length = length;
                constantPoolUtf8->m_bytes = bytes;
                constantPoolUtf8->m_hashCode = -1;

                constantPool->m_entries[index] = constantPoolUtf8;

                // jtk_Logger_info(disassembler->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Disassembled constant pool entry `zen_ConstantPoolUtf8_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_STRING: {
                uint16_t stringIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
                    (disassembler->m_bytes[disassembler->m_index++] & 0xFF));

                zen_ConstantPoolString_t* constantPoolString = jtk_Memory_allocate(zen_ConstantPoolString_t, 1);
                constantPoolString->m_tag = ZEN_CONSTANT_POOL_TAG_STRING;
                constantPoolString->m_stringIndex = stringIndex;

                constantPool->m_entries[index] = constantPoolString;

                // jtk_Logger_info(disassembler->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Disassembled constant pool entry `zen_ConstantPoolString_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FUNCTION: {
                uint16_t classIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
                    (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
                uint16_t descriptorIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
                    (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
                uint16_t nameIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
                    (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
                uint16_t tableIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
                    (disassembler->m_bytes[disassembler->m_index++] & 0xFF));

                zen_ConstantPoolFunction_t* constantPoolFunction = jtk_Memory_allocate(zen_ConstantPoolFunction_t, 1);
                constantPoolFunction->m_tag = ZEN_CONSTANT_POOL_TAG_FUNCTION;
                constantPoolFunction->m_classIndex = classIndex;
                constantPoolFunction->m_descriptorIndex = descriptorIndex;
                constantPoolFunction->m_nameIndex = nameIndex;

                constantPool->m_entries[index] = constantPoolFunction;

                // jtk_Logger_info(disassembler->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Disassembled constant pool entry `zen_ConstantPoolFunction_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FIELD: {
                uint16_t descriptorIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
                    (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
                uint16_t nameIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
                    (disassembler->m_bytes[disassembler->m_index++] & 0xFF));

                zen_ConstantPoolField_t* constantPoolField = jtk_Memory_allocate(zen_ConstantPoolField_t, 1);
                constantPoolField->m_tag = ZEN_CONSTANT_POOL_TAG_FIELD;
                constantPoolField->m_descriptorIndex = descriptorIndex;
                constantPoolField->m_nameIndex = nameIndex;

                constantPool->m_entries[index] = constantPoolField;

                // jtk_Logger_info(disassembler->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Disassembled constant pool entry `zen_ConstantPoolField_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_CLASS: {
                uint16_t nameIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
                (disassembler->m_bytes[disassembler->m_index++] & 0xFF));

                zen_ConstantPoolClass_t* constantPoolClass = jtk_Memory_allocate(zen_ConstantPoolClass_t, 1);
                constantPoolClass->m_tag = ZEN_CONSTANT_POOL_TAG_CLASS;
                constantPoolClass->m_nameIndex = nameIndex;

                constantPool->m_entries[index] = constantPoolClass;

                // jtk_Logger_info(disassembler->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Disassembled constant pool entry `zen_ConstantPoolClass_t`, stored at index %d.", index);

                break;
            }
        }
    }

    // jtk_Logger_info(disassembler->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Disassembled %d constant pool entries, stored at indexes [0, %d].",
        // size, size);

    return constantPool;
}

/* Disassemble Entity */

void zen_BinaryEntityDisassembler_disassembleEntity(zen_BinaryEntityDisassembler_t* disassembler) {
    jtk_Assert_assertObject(disassembler, "The specified binary entity disassembler is null.");

    uint8_t type = disassembler->m_bytes[disassembler->m_index++];
    uint16_t flags = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    uint16_t reference = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    uint16_t superclassCount = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));

    printf("type = %d, flags = %d, reference = %d\nsuperclassCount = %d\n", type, flags, reference,
        superclassCount);

    int32_t i;
    for (i = 0; i < superclassCount; i++) {
        uint16_t superclass = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
            (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
        printf("     #%d %d\n", i, superclass);

    }

    zen_BinaryEntityDisassembler_disassembleAttributeTable(disassembler, &entity ->m_attributeTable);

    // fieldCount fieldEntity*

    uint16_t fieldCount = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    uint16_t fieldTableSize = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    printf("fieldCount = %d, fieldTableSize = %d\n", fieldCount, fieldTableSize);

    int32_t j;
    for (j = 0; j < fieldCount; j++) {
        zen_BinaryEntityDisassembler_disassembleField(disassembler);
    }

    // functionCount functionEntity*

    uint16_t functionCount = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    uint16_t functionTableSize = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    printf("functionCount = %d, functionTableSize = %d\n", functionCount,
        functionTableSize);

    int32_t k;
    for (k = 0; k < functionCount; k++) {
        zen_BinaryEntityDisassembler_disassembleFunction(disassembler);
    }
}

/* Disassemble Attribute Table */

void zen_BinaryEntityDisassembler_disassembleAttributeTable(
    zen_BinaryEntityDisassembler_t* disassembler, zen_AttributeTable_t* attributeTable) {
    jtk_Assert_assertObject(disassembler, "The specified binary entity disassembler is null.");

    uint16_t size = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    attributeTable->m_size = size;
    attributeTable->m_attributes = (size > 0)?
        jtk_Memory_allocate(zen_Attribute_t*, size) : size;

    for (int32_t i = 0; i < size; i++) {
        uint16_t nameIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
            (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
        uint32_t length = ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 24) |
            ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 16) |
            ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
            (disassembler->m_bytes[disassembler->m_index++] & 0xFF);

        zen_ConstantPoolUtf8_t* nameConstantPoolUtf8 = zen_ConstantPool_resolveUtf8(
            &disassembler->m_entityFile->m_constantPool, nameIndex);
        zen_AttributeDisassembleRuleFunction_t attributeDisassembleRule =
            zen_AttributeDisassembleRules_getRuleEx(disassembler->m_attributeDisassembleRules,
                nameConstantPoolUtf8->m_bytes, nameConstantPoolUtf8->m_length);

        if (attributeDisassembleRule != NULL) {
            zen_Attribute_t* attribute = attributeDisassembleRule(disassembler, nameIndex, length);
            attributeTable->m_attributes[i] = attribute;

            // jtk_Logger_info(disassembler->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Disassembled attribute `%s`, stored at index %d.", name, i);
        }
        else {
            /* Skip the bytes occupied by the unrecognized attribute. */
            disassembler->m_index += length;

            /* Do not waste memory representing an unrecognized attribute.
             * It is represented as null in the attributes table.
             */

            // jtk_Logger_info(disassembler->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Skipped unrecognized attribute `%s`. Null value stored at index %d.", name, i);
        }
    }
}

/* Disassemble Instruction Attribute */

zen_InstructionAttribute_t* zen_BinaryEntityDisassembler_disassembleInstructionAttribute(
    zen_BinaryEntityDisassembler_t* disassembler, uint16_t nameIndex, uint32_t length) {
    jtk_Assert_assertObject(disassembler, "The specified binary entity disassembler is null.");

    zen_InstructionAttribute_t* instructionAttribute = jtk_Memory_allocate(zen_InstructionAttribute_t, 1);
    instructionAttribute->m_nameIndex = nameIndex;
    instructionAttribute->m_length = length;

    uint16_t maxStackSize = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    instructionAttribute->m_maxStackSize = maxStackSize;

    uint16_t localVariableCount = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    instructionAttribute->m_localVariableCount = localVariableCount;

    uint32_t instructionLength = ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 24) |
        ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 16) |
        ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF);
    instructionAttribute->m_instructionLength = instructionLength;

    uint8_t* instructions = NULL;
    if (instructionLength > 0) {
        instructions = jtk_Memory_allocate(uint8_t, instructionLength);
        jtk_Arrays_copyEx_b(disassembler->m_bytes, disassembler->m_size, disassembler->m_index,
            instructions, instructionLength, 0, instructionLength);
        disassembler->m_index += instructionLength;
    }
    instructionAttribute->m_instructions = instructions;

    zen_BinaryEntityDisassembler_disassembleExceptionTable(disassembler, &(instructionAttribute->m_exceptionTable));

    return instructionAttribute;
}

/* Disassemble Exception Table */

void zen_BinaryEntityDisassembler_disassembleExceptionTable(zen_BinaryEntityDisassembler_t* disassembler,
    zen_ExceptionTable_t* exceptionTable) {
    jtk_Assert_assertObject(disassembler, "The specified binary entity disassembler is null.");
    jtk_Assert_assertObject(exceptionTable, "The specified exception table is null.");

    uint16_t size = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    exceptionTable->m_size = size;
    exceptionTable->m_exceptionHandlerSites = (size > 0)?
        jtk_Memory_allocate(zen_ExceptionHandlerSite_t*, size) : NULL;

    int32_t i;
    for (i = 0; i < size; i++) {
        zen_ExceptionHandlerSite_t* exceptionHandlerSite = zen_BinaryEntityDisassembler_disassembleExceptionHandlerSite(disassembler);
        exceptionTable->m_exceptionHandlerSites[i] = exceptionHandlerSite;
    }
}

/* Disassemble Exception Handler Site */

zen_ExceptionHandlerSite_t* zen_BinaryEntityDisassembler_disassembleExceptionHandlerSite(
    zen_BinaryEntityDisassembler_t* disassembler) {
    jtk_Assert_assertObject(disassembler, "The specified binary entity disassembler is null.");

    zen_ExceptionHandlerSite_t* exceptionHandlerSite = jtk_Memory_allocate(zen_ExceptionHandlerSite_t, 1);

    uint16_t startIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    exceptionHandlerSite->m_startIndex = startIndex;

    uint16_t stopIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    exceptionHandlerSite->m_stopIndex = stopIndex;

    uint16_t handlerIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    exceptionHandlerSite->m_handlerIndex = handlerIndex;

    uint16_t exceptionClassIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    exceptionHandlerSite->m_exceptionClassIndex = exceptionClassIndex;

    return exceptionHandlerSite;
}

/* Disassemble Function */

zen_FunctionEntity_t* zen_BinaryEntityDisassembler_disassembleFunction(zen_BinaryEntityDisassembler_t* disassembler) {
    jtk_Assert_assertObject(disassembler, "The specified binary entity disassembler is null.");

    zen_FunctionEntity_t* functionEntity = jtk_Memory_allocate(zen_FunctionEntity_t, 1);

    // Flags

    uint16_t flags = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    functionEntity->m_flags = flags;

    // Name Index

    uint16_t nameIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    functionEntity->m_nameIndex = nameIndex;

    // Descriptor Index
    uint16_t descriptorIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    functionEntity->m_descriptorIndex = descriptorIndex;

    // Table Index
    uint16_t tableIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    functionEntity->m_tableIndex = tableIndex;

    // Attribute Table
    zen_BinaryEntityDisassembler_disassembleAttributeTable(disassembler, &(functionEntity->m_attributeTable));

    return functionEntity;
}

/* Disassemble Field */

zen_FieldEntity_t* zen_BinaryEntityDisassembler_disassembleField(zen_BinaryEntityDisassembler_t* disassembler) {
    jtk_Assert_assertObject(disassembler, "The specified binary entity disassembler is null.");

    zen_FieldEntity_t* fieldEntity = jtk_Memory_allocate(zen_FieldEntity_t, 1);

    // Flags
    uint16_t flags = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    fieldEntity->m_flags = flags;

    // Name Index
    uint16_t nameIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    fieldEntity->m_nameIndex = nameIndex;

    // Descriptor Index
    uint16_t descriptorIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    fieldEntity->m_descriptorIndex = descriptorIndex;

    // Parameter Threshold
    uint16_t parameterThreshold = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    fieldEntity->m_parameterThreshold = parameterThreshold;

    // Table Index
    uint16_t tableIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    fieldEntity->m_tableIndex = tableIndex;

    zen_BinaryEntityDisassembler_disassembleAttributeTable(disassembler, &(fieldEntity->m_attributeTable));

    return fieldEntity;
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