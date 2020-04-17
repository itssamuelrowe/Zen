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

#include <com/onecube/zen/disassembler/BinaryEntityDisassembler.h>

#include <com/onecube/zen/virtual-machine/feb/BinaryEntityFormat.h>
#include <com/onecube/zen/virtual-machine/feb/Instruction.h>
#include <com/onecube/zen/virtual-machine/feb/ByteCode.h>
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
#include <com/onecube/zen/virtual-machine/feb/attribute/PredefinedAttribute.h>

/*******************************************************************************
 * disassembler                                                    *
 *******************************************************************************/

// Constructor

zen_BinaryEntityDisassembler_t* zen_BinaryEntityDisassembler_new(
    jtk_Iterator_t* entityDirectoryIterator) {
    jtk_ObjectAdapter_t* stringObjectAdapter = jtk_CStringObjectAdapter_getInstance();

    zen_BinaryEntityDisassembler_t* disassembler = jtk_Memory_allocate(zen_BinaryEntityDisassembler_t, 1);
    disassembler->m_directories = jtk_DoublyLinkedList_new();
    disassembler->m_index = 0;
    disassembler->m_bytes = NULL;
    disassembler->m_constantPool.m_entries = NULL;
    disassembler->m_constantPool.m_size = 0;

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

void zen_BinaryEntityDisassembler_disassemble(zen_BinaryEntityDisassembler_t* disassembler,
    uint8_t* bytes, int32_t size) {
    disassembler->m_bytes = bytes;
    disassembler->m_size = size;

    zen_BinaryEntityDisassembler_disassembleEntity(disassembler);
}

/* Disassemble Constant Pool */

void zen_BinaryEntityDisassembler_disassembleConstantPool(
    zen_BinaryEntityDisassembler_t* disassembler) {
    jtk_Assert_assertObject(disassembler, "The specified binary entity disassembler is null.");

    // jtk_Logger_info(disassembler->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsing constant pool entries...");

    uint16_t size = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
                (disassembler->m_bytes[disassembler->m_index++] & 0xFF));

    zen_ConstantPool_t* constantPool = &disassembler->m_constantPool;
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
                disassembler->m_bytes[length] = '\0';
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

    printf("[Entity]\ntype=%d, flags=%d, reference=%d\nsuperclassCount=%d\n", type, flags, reference,
        superclassCount);

    int32_t i;
    for (i = 0; i < superclassCount; i++) {
        uint16_t superclass = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
            (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
        printf("     #%d %d\n", i, superclass);

    }

    zen_BinaryEntityDisassembler_disassembleAttributeTable(disassembler);

    // fieldCount fieldEntity*

    uint16_t fieldCount = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    uint16_t fieldTableSize = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    printf("[Fields]\nfieldCount=%d, fieldTableSize=%d\n", fieldCount, fieldTableSize);

    int32_t j;
    for (j = 0; j < fieldCount; j++) {
        zen_BinaryEntityDisassembler_disassembleField(disassembler);
    }

    // functionCount functionEntity*

    uint16_t functionCount = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    uint16_t functionTableSize = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    printf("[Functions]\nfunctionCount=%d, functionTableSize=%d\n", functionCount,
        functionTableSize);

    int32_t k;
    for (k = 0; k < functionCount; k++) {
        zen_BinaryEntityDisassembler_disassembleFunction(disassembler);
    }
}

/* Disassemble Attribute Table */

void zen_BinaryEntityDisassembler_disassembleAttributeTable(
    zen_BinaryEntityDisassembler_t* disassembler) {
    jtk_Assert_assertObject(disassembler, "The specified binary entity disassembler is null.");

    uint16_t size = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    printf("[Attributes]\nsize=%d\n", size);

    for (int32_t i = 0; i < size; i++) {
        uint16_t nameIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
            (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
        uint32_t length = ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 24) |
            ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 16) |
            ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
            (disassembler->m_bytes[disassembler->m_index++] & 0xFF);

        zen_ConstantPoolUtf8_t* nameConstantPoolUtf8 =
            (zen_ConstantPoolUtf8_t*)disassembler->m_constantPool.m_entries[nameIndex];

        printf("[Attribute] %s\nnameIndex=%d, length=%d\n",
            nameConstantPoolUtf8->m_bytes, nameIndex, length);

        if (jtk_CString_equals(nameConstantPoolUtf8->m_bytes,
            nameConstantPoolUtf8->m_length, ZEN_PREDEFINED_ATTRIBUTE_INSTRUCTION,
            ZEN_PREDEFINED_ATTRIBUTE_INSTRUCTION_SIZE)) {
            zen_BinaryEntityDisassembler_disassembleInstructionAttribute(disassembler,
                nameIndex, length);
        }
        else {
            /* Skip the bytes occupied by the unrecognized attribute. */
            disassembler->m_index += length;

            // TODO: Dump hex
        }
    }
}

/* Disassemble Instruction Attribute */

void zen_BinaryEntityDisassembler_disassembleInstructionAttribute(
    zen_BinaryEntityDisassembler_t* disassembler, uint16_t nameIndex, uint32_t length) {
    jtk_Assert_assertObject(disassembler, "The specified binary entity disassembler is null.");

    uint16_t maxStackSize = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    uint16_t localVariableCount = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    uint32_t instructionLength = ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 24) |
        ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 16) |
        ((disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF);
    printf("maxStackSize=%d, localVariableCount=%d, instructionLength=%d\n",
        maxStackSize, localVariableCount, instructionLength);

    int32_t i;
    for (i = 0; i < instructionLength; i++) {
        zen_Instruction_t* instruction = zen_Instruction_getInstance(disassembler->m_bytes[i]);
        printf("#%d %s", i, instruction->m_text);
        if (instruction->m_argumentCount != 0) {
            printf(" ");
            switch (disassembler->m_bytes[i])  {
                case ZEN_BYTE_CODE_JUMP_EQ0_I:
                case ZEN_BYTE_CODE_JUMP_NE0_I:
                case ZEN_BYTE_CODE_JUMP_LT0_I:
                case ZEN_BYTE_CODE_JUMP_GT0_I:
                case ZEN_BYTE_CODE_JUMP_GE0_I:
                case ZEN_BYTE_CODE_JUMP_LE0_I:
                case ZEN_BYTE_CODE_JUMP_EQ_I:
                case ZEN_BYTE_CODE_JUMP_NE_I:
                case ZEN_BYTE_CODE_JUMP_LT_I:
                case ZEN_BYTE_CODE_JUMP_GT_I:
                case ZEN_BYTE_CODE_JUMP_GE_I:
                case ZEN_BYTE_CODE_JUMP_LE_I:
                case ZEN_BYTE_CODE_JUMP_EQ_A:
                case ZEN_BYTE_CODE_JUMP_NE_A:
                case ZEN_BYTE_CODE_JUMP_EQN_A:
                case ZEN_BYTE_CODE_JUMP_NEN_A:
                case ZEN_BYTE_CODE_JUMP:
                {
                    uint16_t offset = (disassembler->m_bytes[++i] << 8) | disassembler->m_bytes[++i];
                    printf("offset=%d\n", offset);
                    break;
                }

                case ZEN_BYTE_CODE_INCREMENT_I: {
                    printf("index=%d, constant=%d", disassembler->m_bytes[++i], disassembler->m_bytes[++i]);
                    break;
                }

                case ZEN_BYTE_CODE_INVOKE_SPECIAL:
                case ZEN_BYTE_CODE_INVOKE_VIRTUAL:
                case ZEN_BYTE_CODE_INVOKE_DYNAMIC:
                case ZEN_BYTE_CODE_INVOKE_STATIC:
                case ZEN_BYTE_CODE_LOAD_INSTANCE_FIELD:
                case ZEN_BYTE_CODE_LOAD_STATIC_FIELD:
                case ZEN_BYTE_CODE_NEW:
                case ZEN_BYTE_CODE_NEW_ARRAY_A:
                case ZEN_BYTE_CODE_STORE_STATIC_FIELD:
                case ZEN_BYTE_CODE_STORE_INSTANCE_FIELD:
                {
                    uint16_t index = (disassembler->m_bytes[++i] << 8) | disassembler->m_bytes[++i];
                    printf("index=%d", index);
                    break;
                }

                /* Load */

                case ZEN_BYTE_CODE_LOAD_I:
                case ZEN_BYTE_CODE_LOAD_L:
                case ZEN_BYTE_CODE_LOAD_F:
                case ZEN_BYTE_CODE_LOAD_D:
                case ZEN_BYTE_CODE_LOAD_A:
                case ZEN_BYTE_CODE_LOAD_CPR:
                case ZEN_BYTE_CODE_NEW_ARRAY:
                case ZEN_BYTE_CODE_STORE_I:
                case ZEN_BYTE_CODE_STORE_L:
                case ZEN_BYTE_CODE_STORE_F:
                case ZEN_BYTE_CODE_STORE_D:
                case ZEN_BYTE_CODE_STORE_A:
                {
                    printf("index=%d", disassembler->m_bytes[++i]);
                    break;
                }

                case ZEN_BYTE_CODE_NEW_ARRAY_AN: {
                    uint16_t index = (disassembler->m_bytes[++i] << 8) | disassembler->m_bytes[++i];
                    uint8_t dimensions = disassembler->m_bytes[++i];
                    printf("index=%d, dimensions=%d\n", index, dimensions);
                    break;
                }


                case ZEN_BYTE_CODE_PUSH_B: {
                    printf("value=%d\n", disassembler->m_bytes[++i]);
                    break;
                }

                case ZEN_BYTE_CODE_PUSH_S: {
                    uint16_t value = (disassembler->m_bytes[++i] << 8) | disassembler->m_bytes[++i];
                    printf("value=%d\n", value);
                    break;
                }

                default: {
                    print("[internal error] Control should not reach here.\n");
                }
            }
        }
    }
    zen_BinaryEntityDisassembler_disassembleExceptionTable(disassembler);
}

/* Disassemble Exception Table */

void zen_BinaryEntityDisassembler_disassembleExceptionTable(
        zen_BinaryEntityDisassembler_t* disassembler) {
    jtk_Assert_assertObject(disassembler, "The specified binary entity disassembler is null.");

    uint16_t size = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    printf("[Exception Table]\nsize=%d\n", size);
    int32_t i;
    for (i = 0; i < size; i++) {
        printf("#%d ", i);
        zen_BinaryEntityDisassembler_disassembleExceptionHandlerSite(disassembler);
    }
}

/* Disassemble Exception Handler Site */

void zen_BinaryEntityDisassembler_disassembleExceptionHandlerSite(
    zen_BinaryEntityDisassembler_t* disassembler) {
    jtk_Assert_assertObject(disassembler, "The specified binary entity disassembler is null.");

    uint16_t startIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    uint16_t stopIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    uint16_t handlerIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    uint16_t exceptionClassIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));

    printf("startIndex=%d, stopIndex=%d, handlerIndex=%d, exceptionClassIndex=%d",
        startIndex, stopIndex, handlerIndex, exceptionClassIndex);
}

/* Disassemble Function */

void zen_BinaryEntityDisassembler_disassembleFunction(zen_BinaryEntityDisassembler_t* disassembler) {
    jtk_Assert_assertObject(disassembler, "The specified binary entity disassembler is null.");

    uint16_t flags = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    uint16_t nameIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    uint16_t descriptorIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    uint16_t tableIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));

    printf("flags=%d, nameIndex=%d, descriptorIndex=%d, tableIndex=%d\n",
        flags, nameIndex, descriptorIndex, tableIndex);

    zen_BinaryEntityDisassembler_disassembleAttributeTable(disassembler);
}

/* Disassemble Field */

void zen_BinaryEntityDisassembler_disassembleField(zen_BinaryEntityDisassembler_t* disassembler) {
    jtk_Assert_assertObject(disassembler, "The specified binary entity disassembler is null.");

    uint16_t flags = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    uint16_t nameIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    uint16_t descriptorIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    uint16_t parameterThreshold = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));
    uint16_t tableIndex = (uint16_t)(((uint32_t)(disassembler->m_bytes[disassembler->m_index++] & 0xFF) << 8) |
        (disassembler->m_bytes[disassembler->m_index++] & 0xFF));

    printf("flags=%d, nameIndex=%d, descriptorIndex=%d, parameterThreshold=%d, tableIndex=%d\n",
        flags, nameIndex, descriptorIndex, parameterThreshold, tableIndex);
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