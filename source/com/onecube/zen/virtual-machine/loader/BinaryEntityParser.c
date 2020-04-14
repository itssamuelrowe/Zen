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

// Saturday, June 09, 2018

#include <com/onecube/zen/virtual-machine/feb/BinaryEntityFormat.h>
#include <com/onecube/zen/virtual-machine/loader/BinaryEntityParser.h>

/*******************************************************************************
 * BinaryEntityParser                                                          *
 *******************************************************************************/

#define ZEN_BINARY_ENTITY_PARSER_FLAG_DIRTY (1 << 0)
#define ZEN_BINARY_ENTITY_PARSER_FLAG_READY (1 << 1)
#define ZEN_BINARY_ENTITY_PARSER_FLAG_HIT_EOS (1 << 2)

const uint8_t* ZEN_BINARY_ENTITY_PARSER_TAG = "com.onecube.zen.virtualmachine.parser.BinaryEntityParser";

/* Constructor */

zen_BinaryEntityParser_t* zen_BinaryEntityParser_new(
    zen_AttributeParseRules_t* attributeParseRules) {
    zen_BinaryEntityParser_t* parser = zen_Memory_allocate(zen_BinaryEntityParser_t, 1);
    parser->m_attributeParseRules = attributeParseRules;
    parser->m_logger = NULL;
    parser->m_entityFile = NULL;
    parser->m_index = 0;
    parser->m_bytes = NULL;
    parser->m_size = -1;

    return parser;
}

/* Destructor */

void zen_BinaryEntityParser_delete(zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    jtk_Memory_deallocate(parser);
}

/* Parse */

/* TODO: Do not maintain a reference to the input stream. This restricts the
 * binary entity parser to a single thread. Therefore, make the state of the
 * binary entity parser independent of the object.
 */
zen_EntityFile_t* zen_BinaryEntityParser_parse(zen_BinaryEntityParser_t* parser,
    uint8_t* bytes, int32_t size) {
    parser->m_bytes = bytes;
    parser->m_size = size;

    zen_EntityFile_t* result = zen_BinaryEntityParser_parseEntityFile(parser);

    parser->m_index = 0;
    parser->m_bytes = NULL;
    parser->m_size = 0;
    parser->m_entityFile = NULL;

    return result;
}

/* Parse Entity File */

zen_EntityFile_t* zen_BinaryEntityParser_parseEntityFile(zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    zen_EntityFile_t* entityFile = jtk_Memory_allocate(zen_EntityFile_t, 1);
    parser->m_entityFile = entityFile;

    if (parser->m_index + 12 < parser->m_size) {
        /* The magic number is a simple measure to identify corrupt streams.
         * It is represented with four bytes, that is, a 32-bit integer.
         *
         * Read the magic number and verify whether it is equal to 0xFEB72000.
         */
        uint32_t magicNumber = ((parser->m_bytes[parser->m_index++] & 0xFF) << 24) |
            ((parser->m_bytes[parser->m_index++] & 0xFF) << 16) |
            ((parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
            (parser->m_bytes[parser->m_index++] & 0xFF);

        if (magicNumber == ZEN_BINARY_ENTITY_FORMAT_MAGIC_NUMBER) {
            entityFile->m_magicNumber = magicNumber;

            uint16_t majorVersion = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
                (parser->m_bytes[parser->m_index++] & 0xFF));
            entityFile->m_version.m_majorVersion = majorVersion;

            uint16_t minorVersion = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
                (parser->m_bytes[parser->m_index++] & 0xFF));
            entityFile->m_version.m_minorVersion = minorVersion;

            /* Make sure that the major and minor version numbers are recognized by
             * the binary entity parser.
             */
            if ((majorVersion < ZEN_BINARY_ENTITY_FORMAT_MAJOR_VERSION) ||
                ((majorVersion == ZEN_BINARY_ENTITY_FORMAT_MAJOR_VERSION) &&
                 (minorVersion <= ZEN_BINARY_ENTITY_FORMAT_MINOR_VERSION))) {
                // uint32_t size = ((parser->m_bytes[parser->m_index++] & 0xFF) << 24) |
                    ((parser->m_bytes[parser->m_index++] & 0xFF) << 16) |
                    ((parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
                    (parser->m_bytes[parser->m_index++] & 0xFF);
                // entityFile->m_size = size;

                uint16_t flags = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
                (parser->m_bytes[parser->m_index++] & 0xFF));
                entityFile->m_flags = flags;

                zen_BinaryEntityParser_parseConstantPool(parser);
                zen_BinaryEntityParser_parseEntity(parser);
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

/* Parse Constant Pool */

void zen_BinaryEntityParser_parseConstantPool(
    zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsing constant pool entries...");

    uint16_t size = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
                (parser->m_bytes[parser->m_index++] & 0xFF));

    zen_ConstantPool_t* constantPool = &parser->m_entityFile->m_constantPool;
    constantPool->m_size = size;
    constantPool->m_entries = jtk_Memory_allocate(zen_ConstantPoolEntry_t*, size + 1);
    constantPool->m_entries[0] = NULL;

    int32_t index;
    for (index = 1; index <= size; index++) {
        uint8_t tag = parser->m_bytes[parser->m_index++];

        switch (tag) {
            case ZEN_CONSTANT_POOL_TAG_INTEGER: {
                uint32_t bytes = ((parser->m_bytes[parser->m_index++] & 0xFF) << 24) |
                    ((parser->m_bytes[parser->m_index++] & 0xFF) << 16) |
                    ((parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
                    (parser->m_bytes[parser->m_index++] & 0xFF);

                zen_ConstantPoolInteger_t* constantPoolInteger = jtk_Memory_allocate(zen_ConstantPoolInteger_t, 1);
                constantPoolInteger->m_tag = ZEN_CONSTANT_POOL_TAG_INTEGER;
                constantPoolInteger->m_bytes = bytes;

                constantPool->m_entries[index] = constantPoolInteger;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolInteger_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_LONG: {
                uint32_t highBytes = ((parser->m_bytes[parser->m_index++] & 0xFF) << 24) |
                    ((parser->m_bytes[parser->m_index++] & 0xFF) << 16) |
                    ((parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
                    (parser->m_bytes[parser->m_index++] & 0xFF);
                uint32_t lowBytes = ((parser->m_bytes[parser->m_index++] & 0xFF) << 24) |
                    ((parser->m_bytes[parser->m_index++] & 0xFF) << 16) |
                    ((parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
                    (parser->m_bytes[parser->m_index++] & 0xFF);

                zen_ConstantPoolLong_t* constantPoolLong = jtk_Memory_allocate(zen_ConstantPoolLong_t, 1);
                constantPoolLong->m_tag = ZEN_CONSTANT_POOL_TAG_LONG;
                constantPoolLong->m_highBytes = highBytes;
                constantPoolLong->m_lowBytes = lowBytes;

                constantPool->m_entries[index] = constantPoolLong;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolLong_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FLOAT: {
                uint32_t bytes = ((parser->m_bytes[parser->m_index++] & 0xFF) << 24) |
                    ((parser->m_bytes[parser->m_index++] & 0xFF) << 16) |
                    ((parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
                    (parser->m_bytes[parser->m_index++] & 0xFF);

                zen_ConstantPoolFloat_t* constantPoolFloat = jtk_Memory_allocate(zen_ConstantPoolFloat_t, 1);
                constantPoolFloat->m_tag = ZEN_CONSTANT_POOL_TAG_FLOAT;
                constantPoolFloat->m_bytes = bytes;

                constantPool->m_entries[index] = constantPoolFloat;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolFloat_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_DOUBLE: {
                uint32_t highBytes = ((parser->m_bytes[parser->m_index++] & 0xFF) << 24) |
                    ((parser->m_bytes[parser->m_index++] & 0xFF) << 16) |
                    ((parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
                    (parser->m_bytes[parser->m_index++] & 0xFF);
                uint32_t lowBytes = ((parser->m_bytes[parser->m_index++] & 0xFF) << 24) |
                    ((parser->m_bytes[parser->m_index++] & 0xFF) << 16) |
                    ((parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
                    (parser->m_bytes[parser->m_index++] & 0xFF);

                zen_ConstantPoolDouble_t* constantPoolDouble = jtk_Memory_allocate(zen_ConstantPoolDouble_t, 1);
                constantPoolDouble->m_tag = ZEN_CONSTANT_POOL_TAG_DOUBLE;
                constantPoolDouble->m_highBytes = highBytes;
                constantPoolDouble->m_lowBytes = lowBytes;

                constantPool->m_entries[index] = constantPoolDouble;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolDouble_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_UTF8: {
                uint16_t length = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
                (parser->m_bytes[parser->m_index++] & 0xFF));
                /* The specification guarantees that an empty string is never stored in a constant pool.
                 *
                 * Although the speciication does not specify the bytes to be null-terminated,
                 * the reference implementation of the Zen Virtual Machine terminates
                 * UTF8 constant pool entries for performance.
                 */

                uint8_t* bytes = jtk_Memory_allocate(uint8_t, length + 1);
                bytes[length] = '\0';
                jtk_Arrays_copyEx_b(parser->m_bytes, parser->m_size, parser->m_index,
                    bytes, length, 0, length);
                parser->m_index += length;

                zen_ConstantPoolUtf8_t* constantPoolUtf8 = jtk_Memory_allocate(zen_ConstantPoolUtf8_t, 1);
                constantPoolUtf8->m_tag = ZEN_CONSTANT_POOL_TAG_UTF8;
                constantPoolUtf8->m_length = length;
                constantPoolUtf8->m_bytes = bytes;

                constantPool->m_entries[index] = constantPoolUtf8;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolUtf8_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_STRING: {
                uint16_t stringIndex = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
                (parser->m_bytes[parser->m_index++] & 0xFF));

                zen_ConstantPoolString_t* constantPoolString = jtk_Memory_allocate(zen_ConstantPoolString_t, 1);
                constantPoolString->m_tag = ZEN_CONSTANT_POOL_TAG_STRING;
                constantPoolString->m_stringIndex = stringIndex;

                constantPool->m_entries[index] = constantPoolString;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolString_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FUNCTION: {
                uint16_t classIndex = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
                (parser->m_bytes[parser->m_index++] & 0xFF));
                uint16_t descriptorIndex = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
                (parser->m_bytes[parser->m_index++] & 0xFF));
                uint16_t nameIndex = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
                (parser->m_bytes[parser->m_index++] & 0xFF));

                zen_ConstantPoolFunction_t* constantPoolFunction = jtk_Memory_allocate(zen_ConstantPoolFunction_t, 1);
                constantPoolFunction->m_tag = ZEN_CONSTANT_POOL_TAG_FUNCTION;
                constantPoolFunction->m_classIndex = classIndex;
                constantPoolFunction->m_descriptorIndex = descriptorIndex;
                constantPoolFunction->m_nameIndex = nameIndex;

                constantPool->m_entries[index] = constantPoolFunction;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolFunction_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FIELD: {
                uint16_t descriptorIndex = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
                (parser->m_bytes[parser->m_index++] & 0xFF));
                uint16_t nameIndex = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
                (parser->m_bytes[parser->m_index++] & 0xFF));

                zen_ConstantPoolField_t* constantPoolField = jtk_Memory_allocate(zen_ConstantPoolField_t, 1);
                constantPoolField->m_tag = ZEN_CONSTANT_POOL_TAG_FIELD;
                constantPoolField->m_descriptorIndex = descriptorIndex;
                constantPoolField->m_nameIndex = nameIndex;

                constantPool->m_entries[index] = constantPoolField;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolField_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_CLASS: {
                uint16_t nameIndex = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
                (parser->m_bytes[parser->m_index++] & 0xFF));

                zen_ConstantPoolClass_t* constantPoolClass = jtk_Memory_allocate(zen_ConstantPoolClass_t, 1);
                constantPoolClass->m_tag = ZEN_CONSTANT_POOL_TAG_CLASS;
                constantPoolClass->m_nameIndex = nameIndex;

                constantPool->m_entries[index] = constantPoolClass;

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolClass_t`, stored at index %d.", index);

                break;
            }
        }
    }

    // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed %d constant pool entries, stored at indexes [0, %d].",
        // size, size);

    return constantPool;
}

/* Parse Entity */

void zen_BinaryEntityParser_parseEntity(zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    zen_Entity_t* entity = &parser->m_entityFile->m_entity;
    uint8_t type = parser->m_bytes[parser->m_index++];

    entity->m_type = type;

    uint16_t flags = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
        (parser->m_bytes[parser->m_index++] & 0xFF));
    entity->m_flags = flags;

    uint16_t reference = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
        (parser->m_bytes[parser->m_index++] & 0xFF));
    entity->m_reference = reference;

    uint16_t superclassCount = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
        (parser->m_bytes[parser->m_index++] & 0xFF));
    entity->m_superclassCount = superclassCount;

    uint16_t* superclasses = zen_Memory_allocate(uint16_t, superclassCount);
    int32_t i;
    for (i = 0; i < superclassCount; i++) {
        superclasses[i] = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
            (parser->m_bytes[parser->m_index++] & 0xFF));
    }
    entity->m_superclasses = superclasses;

    zen_BinaryEntityParser_parseAttributeTable(parser, &entity ->m_attributeTable);

    // fieldCount fieldEntity*

    uint16_t fieldCount = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
        (parser->m_bytes[parser->m_index++] & 0xFF));
    entity->m_fieldCount = fieldCount;
    entity->m_fields = (fieldCount > 0)?
        jtk_Memory_allocate(zen_FieldEntity_t*, fieldCount) : NULL;

    int32_t j;
    for (j = 0; j < fieldCount; j++) {
        zen_FieldEntity_t* fieldEntity = zen_BinaryEntityParser_parseField(parser);
        entity->m_fields[j] = fieldEntity;
    }

    // functionCount functionEntity*

    uint16_t functionCount = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
        (parser->m_bytes[parser->m_index++] & 0xFF));
    entity->m_functionCount = functionCount;
    entity->m_functions = (functionCount > 0)?
        jtk_Memory_allocate(zen_FunctionEntity_t*, functionCount) : NULL;

    int32_t k;
    for (k = 0; k < functionCount; k++) {
        zen_FunctionEntity_t* functionEntity = zen_BinaryEntityParser_parseFunction(parser);
        entity->m_functions[k] = functionEntity;
    }

    return entity;
}

/* Parse Attribute Table */

void zen_BinaryEntityParser_parseAttributeTable(
    zen_BinaryEntityParser_t* parser, zen_AttributeTable_t* attributeTable) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    uint16_t size = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
        (parser->m_bytes[parser->m_index++] & 0xFF));
    attributeTable->m_size = size;
    attributeTable->m_attributes = (size > 0)?
        jtk_Memory_allocate(zen_Attribute_t*, size) : size;

    for (int32_t i = 0; i < size; i++) {
        uint16_t nameIndex = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
            (parser->m_bytes[parser->m_index++] & 0xFF));
        uint32_t length = ((parser->m_bytes[parser->m_index++] & 0xFF) << 24) |
            ((parser->m_bytes[parser->m_index++] & 0xFF) << 16) |
            ((parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
            (parser->m_bytes[parser->m_index++] & 0xFF);

        zen_ConstantPoolUtf8_t* nameConstantPoolUtf8 = zen_ConstantPool_resolveUtf8(
            &parser->m_entityFile->m_constantPool, nameIndex);
        zen_AttributeParseRuleFunction_t attributeParseRule =
            zen_AttributeParseRules_getRuleEx(parser->m_attributeParseRules,
                nameConstantPoolUtf8->m_bytes, nameConstantPoolUtf8->m_length);

        if (attributeParseRule != NULL) {
            zen_Attribute_t* attribute = attributeParseRule(parser, nameIndex, length);
            attributeTable->m_attributes[i] = attribute;

            // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed attribute `%s`, stored at index %d.", name, i);
        }
        else {
            /* Skip the bytes occupied by the unrecognized attribute. */
            parser->m_index += length;

            /* Do not waste memory representing an unrecognized attribute.
             * It is represented as null in the attributes table.
             */

            // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Skipped unrecognized attribute `%s`. Null value stored at index %d.", name, i);
        }
    }
}

/* Parse Instruction Attribute */

zen_InstructionAttribute_t* zen_BinaryEntityParser_parseInstructionAttribute(
    zen_BinaryEntityParser_t* parser, uint16_t nameIndex, uint32_t length) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    zen_InstructionAttribute_t* instructionAttribute = jtk_Memory_allocate(zen_InstructionAttribute_t, 1);
    instructionAttribute->m_nameIndex = nameIndex;
    instructionAttribute->m_length = length;

    uint16_t maxStackSize = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
        (parser->m_bytes[parser->m_index++] & 0xFF));
    instructionAttribute->m_maxStackSize = maxStackSize;

    uint16_t localVariableCount = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
        (parser->m_bytes[parser->m_index++] & 0xFF));
    instructionAttribute->m_localVariableCount = localVariableCount;

    uint32_t instructionLength = ((parser->m_bytes[parser->m_index++] & 0xFF) << 24) |
        ((parser->m_bytes[parser->m_index++] & 0xFF) << 16) |
        ((parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
        (parser->m_bytes[parser->m_index++] & 0xFF);
    instructionAttribute->m_instructionLength = instructionLength;

    uint8_t* instructions = NULL;
    if (instructionLength > 0) {
        instructions = jtk_Memory_allocate(uint8_t, instructionLength);
        jtk_Arrays_copyEx_b(parser->m_bytes, parser->m_size, parser->m_index,
            instructions, instructionLength, 0, instructionLength);
        parser->m_index += instructionLength;
    }
    instructionAttribute->m_instructions = instructions;

    zen_BinaryEntityParser_parseExceptionTable(parser, &(instructionAttribute->m_exceptionTable));

    return instructionAttribute;
}

/* Parse Exception Table */

void zen_BinaryEntityParser_parseExceptionTable(zen_BinaryEntityParser_t* parser,
    zen_ExceptionTable_t* exceptionTable) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");
    jtk_Assert_assertObject(exceptionTable, "The specified exception table is null.");

    uint16_t size = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
        (parser->m_bytes[parser->m_index++] & 0xFF));
    exceptionTable->m_size = size;
    exceptionTable->m_exceptionHandlerSites = (size > 0)?
        jtk_Memory_allocate(zen_ExceptionHandlerSite_t*, size) : NULL;

    int32_t i;
    for (i = 0; i < size; i++) {
        zen_ExceptionHandlerSite_t* exceptionHandlerSite = zen_BinaryEntityParser_parseExceptionHandlerSite(parser);
        exceptionTable->m_exceptionHandlerSites[i] = exceptionHandlerSite;
    }
}

/* Parse Exception Handler Site */

zen_ExceptionHandlerSite_t* zen_BinaryEntityParser_parseExceptionHandlerSite(
    zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    zen_ExceptionHandlerSite_t* exceptionHandlerSite = jtk_Memory_allocate(zen_ExceptionHandlerSite_t, 1);

    uint16_t startIndex = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
        (parser->m_bytes[parser->m_index++] & 0xFF));
    exceptionHandlerSite->m_startIndex = startIndex;

    uint16_t stopIndex = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
        (parser->m_bytes[parser->m_index++] & 0xFF));
    exceptionHandlerSite->m_stopIndex = stopIndex;

    uint16_t handlerIndex = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
        (parser->m_bytes[parser->m_index++] & 0xFF));
    exceptionHandlerSite->m_handlerIndex = handlerIndex;

    uint16_t exceptionClassIndex = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
        (parser->m_bytes[parser->m_index++] & 0xFF));
    exceptionHandlerSite->m_exceptionClassIndex = exceptionClassIndex;

    return exceptionHandlerSite;
}

/* Parse Function */

zen_FunctionEntity_t* zen_BinaryEntityParser_parseFunction(zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    zen_FunctionEntity_t* functionEntity = jtk_Memory_allocate(zen_FunctionEntity_t, 1);

    // Flags

    uint16_t flags = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
        (parser->m_bytes[parser->m_index++] & 0xFF));
    functionEntity->m_flags = flags;

    // Name Index

    uint16_t nameIndex = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
        (parser->m_bytes[parser->m_index++] & 0xFF));
    functionEntity->m_nameIndex = nameIndex;

    uint16_t descriptorIndex = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
        (parser->m_bytes[parser->m_index++] & 0xFF));
    functionEntity->m_descriptorIndex = descriptorIndex;

    // Attribute Table

    zen_BinaryEntityParser_parseAttributeTable(parser, &(functionEntity->m_attributeTable));

    return functionEntity;
}

/* Parse Field */

zen_FieldEntity_t* zen_BinaryEntityParser_parseField(zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    zen_FieldEntity_t* fieldEntity = jtk_Memory_allocate(zen_FieldEntity_t, 1);

    // Flags

    uint16_t flags = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
        (parser->m_bytes[parser->m_index++] & 0xFF));
    fieldEntity->m_flags = flags;

    // Name Index

    uint16_t nameIndex = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
        (parser->m_bytes[parser->m_index++] & 0xFF));
    fieldEntity->m_nameIndex = nameIndex;

    // Descriptor Index

    uint16_t descriptorIndex = (uint16_t)(((uint32_t)(parser->m_bytes[parser->m_index++] & 0xFF) << 8) |
        (parser->m_bytes[parser->m_index++] & 0xFF));
    fieldEntity->m_descriptorIndex = descriptorIndex;

    zen_BinaryEntityParser_parseAttributeTable(parser, &(fieldEntity->m_attributeTable));

    return fieldEntity;
}

/* Parse Class */

/*zen_ClassEntity_t* zen_BinaryEntityParser_parseClass(zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");


    return classEntity;
}
*/
/*
zen_AnnotationEntity_t* zen_BinaryEntityParser_parseAnnotation(
    zen_BinaryEntityParser_t* parser) {
    return NULL;
}

zen_PackageEntity_t* zen_BinaryEntityParser_parsePackage(
    zen_BinaryEntityParser_t* parser) {
    return NULL;
}
*/

/*

import zen.fs.*
import zen.net.*

function main
    try
        with var connection = Url.open('https://www.onecube.in'), \
             var writer = File.getWriter('output')
            connection.readTo(writer)
    catch Exception exception
        exception.print()
*/