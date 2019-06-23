// Saturday, June 09, 2018

#include <com/onecube/zen/virtual-machine/loader/BinaryEntityParser.h>

/*******************************************************************************
 * BinaryEntityParser                                                          *
 *******************************************************************************/

#define ZEN_BINARY_ENTITY_PARSER_FLAG_DIRTY (1 << 0)
#define ZEN_BINARY_ENTITY_PARSER_FLAG_READY (1 << 1)
#define ZEN_BINARY_ENTITY_PARSER_FLAG_HIT_EOS (1 << 2)

const uint8_t* ZEN_BINARY_ENTITY_PARSER_TAG = "com.onecube.zen.virtualmachine.loader.BinaryEntityParser";

/* Constructor */

zen_BinaryEntityParser_t* zen_BinaryEntityParser_new(
    zen_AttributeParseRuleMap_t* attributeParseRuleMap, uint8_t* bytes, int32_t size) {
    zen_BinaryEntityParser_t* parser = zen_Memory_allocate(zen_BinaryEntityParser_t, 1);
    parser->m_tape = jtk_Tape_wrap(bytes, size);
    parser->m_attributeParseRuleMap = attributeParseRuleMap;
    // parser->m_memoryManager = memoryManager;

    return parser;
}

/* Destructor */

void zen_BinaryEntityParser_delete(zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    zen_Memory_deallocate(parser);
}

/* Parse */

#warning "zen_MemoryManager_allocateEx is not defined."
#define zen_MemoryManager_allocateEx(a, b, c, d) NULL

/* TODO: Do not maintain a reference to the input stream. This restricts the
 * binary entity parser to a single thread. Therefore, make the state of the
 * binary entity parser independent of the object.
 */
zen_EntityFile_t* zen_BinaryEntityParser_parse(zen_BinaryEntityParser_t* parser,
    jtk_InputStream_t* inputStream) {
    // parser->m_tape = inputStream;
    return zen_BinaryEntityParser_parseEntityFile(parser);
}

zen_EntityFile_t* zen_BinaryEntityParser_parseEntityFile(zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    zen_EntityFile_t* entityFile = NULL;

    if (jtk_Tape_isAvailable(parser->m_tape, 8)) {
        /* The magic number is a simple measure to identify corrupt streams.
         * It is represented with four bytes, that is, a 32-bit integer.
         *
         * Read the magic number and verify whether it is equal to 0xFEB72000.
         */
        uint32_t magicNumber = jtk_Tape_readUncheckedInteger(parser->m_tape);

        uint8_t majorVersion = jtk_Tape_readUncheckedByte(parser->m_tape);
        uint8_t minorVersion = jtk_Tape_readUncheckedByte(parser->m_tape);

        uint16_t flags = jtk_Tape_readUncheckedShort(parser->m_tape);

        zen_ConstantPool_t* constantPool = zen_BinaryEntityParser_parseConstantPool(parser);

        zen_Entity_t* entity = zen_BinaryEntityParser_parseEntity(parser, constantPool);

        /*
        zen_EntityFile_t* entityFile = (zen_EntityFile_t*)zen_MemoryManager_allocateEx(
        parser->m_memoryManager, sizeof (zen_EntityFile_t),
            ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
            */
        entityFile = jtk_Memory_allocate(zen_EntityFile_t, 1);
        entityFile->m_magicNumber = magicNumber;
        entityFile->m_version.m_majorVersion = majorVersion;
        entityFile->m_version.m_minorVersion = minorVersion;
        entityFile->m_flags = flags;
        entityFile->m_constantPool = constantPool;
        entityFile->m_entity = entity;
    }

    return entityFile;
}

/* NOTE: The following functions have been sorted with respect to the specification
 * of the Binary Entity Format. This is simply a convenience for the developers.
 * It holds no special meaning or whatsoever.
 */

/* Parse Constant Pool */

zen_ConstantPool_t* zen_BinaryEntityParser_parseConstantPool(
    zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    uint16_t size = jtk_Tape_readUncheckedShort(parser->m_tape);
    void** entries = (void**)zen_MemoryManager_allocateEx(
        parser->m_memoryManager, sizeof (void*) * size,
            ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);

    zen_ConstantPool_t* constantPool = NULL;/*(zen_ConstantPool_t*)zen_MemoryManager_allocateEx(
        parser->m_memoryManager, sizeof (zen_ConstantPool_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);*/
    constantPool->m_size = size;
    constantPool->m_entries = entries;

    zen_BinaryEntityParser_parseConstantPoolEntries(parser, constantPool);

    return constantPool;
}

/* Parse Constant Pool Entries */

void zen_BinaryEntityParser_parseConstantPoolEntries(zen_BinaryEntityParser_t* parser,
    zen_ConstantPool_t* constantPool) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsing constant pool entries...");

    int32_t size = constantPool->m_size;
    int32_t index;
    for (index = 0; index < size; index++) {
        uint8_t tag = jtk_Tape_readUncheckedByte(parser->m_tape);
        void* entry = NULL;
        switch (tag) {
            case ZEN_CONSTANT_POOL_TAG_INTEGER: {
                entry = (void*)zen_BinaryEntityParser_parseConstantPoolInteger(parser);

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolInteger_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_LONG: {
                entry = (void*)zen_BinaryEntityParser_parseConstantPoolLong(parser);

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolLong_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FLOAT: {
                entry = (void*)zen_BinaryEntityParser_parseConstantPoolFloat(parser);

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolFloat_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_DOUBLE: {
                entry = (void*)zen_BinaryEntityParser_parseConstantPoolDouble(parser);

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolDouble_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_UTF8: {
                entry = (void*)zen_BinaryEntityParser_parseConstantPoolUtf8(parser);

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolUtf8_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_STRING: {
                entry = (void*)zen_BinaryEntityParser_parseConstantPoolString(parser);

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolString_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FUNCTION: {
                entry = (void*)zen_BinaryEntityParser_parseConstantPoolFunction(parser);

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolFunction_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FIELD: {
                entry = (void*)zen_BinaryEntityParser_parseConstantPoolField(parser);

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolField_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_CLASS: {
                entry = (void*)zen_BinaryEntityParser_parseConstantPoolClass(parser);

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolClass_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_ENUMERATE: {
                entry = (void*)zen_BinaryEntityParser_parseConstantPoolEnumerate(parser);

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolEnumerate_t`, stored at index %d.", index);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_ENUMERATION: {
                entry = (void*)zen_BinaryEntityParser_parseConstantPoolEnumeration(parser);

                // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolEnumeration_t`, stored at index %d.", index);

                break;
            }
        }

        constantPool->m_tags[index] = tag;
        constantPool->m_entries[index] = (void*)entry;
    }

    // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed %d constant pool entries, stored at indexes [0, %d).",
        // size, size);
}

/* Parser Constant Pool Integer */

zen_ConstantPoolInteger_t* zen_BinaryEntityParser_parseConstantPoolInteger(zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    zen_ConstantPoolInteger_t* entry = (zen_ConstantPoolInteger_t*)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_ConstantPoolInteger_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT,
        ZEN_ALLOCATION_FLAG_MANUAL);
    entry->m_bytes = jtk_Tape_readUncheckedInteger(parser->m_tape);

    return entry;
}

/* Parser Constant Pool Long */

zen_ConstantPoolLong_t* zen_BinaryEntityParser_parseConstantPoolLong(
    zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    zen_ConstantPoolLong_t* entry = (zen_ConstantPoolLong_t*)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_ConstantPoolLong_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
    entry->m_highBytes = jtk_Tape_readUncheckedInteger(parser->m_tape);
    entry->m_lowBytes = jtk_Tape_readUncheckedInteger(parser->m_tape);

    return entry;
}

/* Parser Constant Pool Float */

zen_ConstantPoolFloat_t* zen_BinaryEntityParser_parseConstantPoolFloat(
    zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    zen_ConstantPoolFloat_t* entry = NULL;/*(zen_ConstantPoolFloat_t*)
    zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_ConstantPoolFloat_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);*/
    entry->m_bytes = jtk_Tape_readUncheckedInteger(parser->m_tape);

    return entry;
}

/* Parser Constant Pool Double */

zen_ConstantPoolDouble_t* zen_BinaryEntityParser_parseConstantPoolDouble(
    zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    zen_ConstantPoolDouble_t* entry = (zen_ConstantPoolDouble_t*)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_ConstantPoolDouble_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
    entry->m_highBytes = jtk_Tape_readUncheckedInteger(parser->m_tape);
    entry->m_lowBytes = jtk_Tape_readUncheckedInteger(parser->m_tape);

    return entry;
}

/* Parse Constant Pool UTF-8 */

zen_ConstantPoolUtf8_t* zen_BinaryEntityParser_parseConstantPoolUtf8(
    zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    zen_ConstantPoolUtf8_t* entry = NULL;/*(zen_ConstantPoolUtf8_t*)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_ConstantPoolUtf8_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);*/
    entry->m_length = jtk_Tape_readUncheckedShort(parser->m_tape);
    entry->m_bytes = zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (uint8_t) * entry->m_length, ZEN_ALIGNMENT_CONSTRAINT_DEFAULT,
        ZEN_ALLOCATION_FLAG_MANUAL);
    jtk_Tape_readUncheckedBytes(parser->m_tape, entry->m_bytes, entry->m_length);

    return entry;
}

/* Parser Constant Pool String */

zen_ConstantPoolString_t* zen_BinaryEntityParser_parseConstantPoolString(
    zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    zen_ConstantPoolString_t* entry = (zen_ConstantPoolString_t*)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_ConstantPoolString_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
    entry->m_stringIndex = jtk_Tape_readUncheckedShort(parser->m_tape);

    return entry;
}

/* Parser Constant Pool Class */

zen_ConstantPoolClass_t* zen_BinaryEntityParser_parseConstantPoolClass(
    zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    zen_ConstantPoolClass_t* entry = (zen_ConstantPoolClass_t*)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_ConstantPoolClass_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
    entry->m_nameIndex = jtk_Tape_readUncheckedShort(parser->m_tape);

    return entry;
}

/* Parser Constant Pool Function */

zen_ConstantPoolFunction_t* zen_BinaryEntityParser_parseConstantPoolFunction(
    zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    zen_ConstantPoolFunction_t* entry = (zen_ConstantPoolFunction_t*)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_ConstantPoolFunction_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
    entry->m_classIndex = jtk_Tape_readUncheckedShort(parser->m_tape);
    entry->m_descriptorIndex = jtk_Tape_readUncheckedShort(parser->m_tape);
    entry->m_nameIndex = jtk_Tape_readUncheckedShort(parser->m_tape);

    return entry;
}

/* Parse Constant Pool Field */

zen_ConstantPoolField_t* zen_BinaryEntityParser_parseConstantPoolField(
    zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    zen_ConstantPoolField_t* entry = (zen_ConstantPoolField_t*)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_ConstantPoolField_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
    entry->m_nameIndex = jtk_Tape_readUncheckedShort(parser->m_tape);
    entry->m_descriptorIndex = jtk_Tape_readUncheckedShort(parser->m_tape);

    return entry;
}

/* Parse Constant Pool Enumerate */

zen_ConstantPoolEnumerate_t* zen_BinaryEntityParser_parseConstantPoolEnumerate(
    zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

#warning "zen_BinaryEntityParser_parseConstantPoolEnumerate not defined"

    return NULL;
}

/* Parse Constant Pool Enumeration */

zen_ConstantPoolEnumeration_t* zen_BinaryEntityParser_parseConstantPoolEnumeration(
    zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

#warning "zen_BinaryEntityParser_parseConstantPoolEnumeration not defined"

    return NULL;
}

/* Parse Entity */

zen_Entity_t* zen_BinaryEntityParser_parseEntity(zen_BinaryEntityParser_t* parser,
    zen_ConstantPool_t* constantPool) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    zen_Entity_t* entity = (zen_Entity_t*)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_Entity_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
    entity->m_type = jtk_Tape_readUncheckedByte(parser->m_tape);
    entity->m_flags = jtk_Tape_readUncheckedShort(parser->m_tape);
    entity->m_reference = jtk_Tape_readUncheckedShort(parser->m_tape);
    entity->m_attributeTable = zen_BinaryEntityParser_parseAttributeTable(parser, constantPool);

    zen_EntityType_t type = entity->m_type;
    switch (type) {
        case ZEN_ENTITY_TYPE_FUNCTION: {
            entity->m_body = zen_BinaryEntityParser_parseFunction(parser, constantPool);

            // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed function entity, with type %d.", type);

            break;
        }

        case ZEN_ENTITY_TYPE_FIELD: {
            entity->m_body = zen_BinaryEntityParser_parseField(parser, constantPool);

            // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed field entity, with type %d.", type);

            break;
        }

        case ZEN_ENTITY_TYPE_CLASS: {
            entity->m_body = zen_BinaryEntityParser_parseClass(parser, constantPool);

            // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed class entity, with type %d.", type);

            break;
        }

        case ZEN_ENTITY_TYPE_ENUMERATE: {
            entity->m_body = zen_BinaryEntityParser_parseEnumerate(parser, constantPool);

            // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed enumerate entity, with type %d.", type);

            break;
        }

        case ZEN_ENTITY_TYPE_ENUMERATION: {
            entity->m_body = zen_BinaryEntityParser_parseEnumeration(parser, constantPool);

            // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed enumeration entity, with type %d.", type);

            break;
        }

        case ZEN_ENTITY_TYPE_ANNOTATION: {
            entity->m_body = zen_BinaryEntityParser_parseAnnotation(parser, constantPool);

            // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed annotation entity, with type %d.", type);

            break;
        }

        case ZEN_ENTITY_TYPE_PACKAGE: {
            entity->m_body = zen_BinaryEntityParser_parsePackage(parser, constantPool);

            // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed package entity, with type %d.", type);

            break;
        }

        default: {
            entity->m_body = NULL;

            /* error: Control should not reach here. */
            break;
        }
    }

    return entity;
}

/* Parse Attribute Table */

zen_AttributeTable_t* zen_BinaryEntityParser_parseAttributeTable(
    zen_BinaryEntityParser_t* parser, zen_ConstantPool_t* constantPool) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    zen_AttributeTable_t* attributeTable =
        (zen_AttributeTable_t*)zen_MemoryManager_allocateEx(parser->m_memoryManager,
            sizeof (zen_AttributeTable_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT,
            ZEN_ALLOCATION_FLAG_MANUAL);

    attributeTable->m_size = jtk_Tape_readUncheckedShort(parser->m_tape);
    attributeTable->m_attributes = zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_Attribute_t*) * attributeTable->m_size, ZEN_ALIGNMENT_CONSTRAINT_DEFAULT,
        ZEN_ALLOCATION_FLAG_MANUAL);

    zen_BinaryEntityParser_parseAttributes(parser, constantPool, attributeTable);

    return attributeTable;
}

/* Parse Attributes */

void zen_BinaryEntityParser_parseAttributes(zen_BinaryEntityParser_t* parser,
    zen_ConstantPool_t* constantPool, zen_AttributeTable_t* attributeTable) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    int32_t size = attributeTable->m_size;
    for (int32_t i = 0; i < size; i++) {
        uint16_t nameIndex = jtk_Tape_readUncheckedShort(parser->m_tape);
        uint32_t length = jtk_Tape_readUncheckedInteger(parser->m_tape);

        uint8_t* name = zen_ConstantPool_resolveUtf8(constantPool, nameIndex);
        zen_AttributeParseRuleFunction_t attributeParseRule = NULL; // zen_AttributeParseRuleMap_getValue(parser->m_attributeParseRuleMap, name);

        if (attributeParseRule != NULL) {
            void* body = attributeParseRule(parser, nameIndex, length);

            zen_Attribute_t* attribute = (zen_Attribute_t*)zen_MemoryManager_allocateEx(parser->m_memoryManager,
                sizeof (zen_Attribute_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
            attribute->m_nameIndex = nameIndex;
            attribute->m_length = length;
            attribute->m_body = body;

            // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed attribute `%s`, stored at index %d.", name, i);
        }
        else {
            /* Skip the bytes occupied by the unrecognized attribute. */
            jtk_Tape_skipUnchecked(parser->m_tape, length);

            /* Do not waste memory representing an unrecognized attribute.
             * It is represented as null in the attributes table.
             */
            attributeTable->m_attributes[i] = NULL;

            // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Skipped unrecognized attribute `%s`. Null value stored at index %d.", name, i);
        }
    }
}

/* Parse Auxillary Attribute */

zen_AuxillaryAttribute_t* zen_BinaryEntityParser_parseAuxillaryAttribute(
    zen_BinaryEntityParser_t* parser, int32_t nameIndex, int32_t length) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    uint16_t operandStackSize = jtk_Tape_readUncheckedShort(parser->m_tape);
    uint16_t localVariableArraySize = jtk_Tape_readUncheckedShort(parser->m_tape);

    zen_AuxillaryAttribute_t* attribute = (zen_AuxillaryAttribute_t*)zen_MemoryManager_allocateEx(
        parser->m_memoryManager, sizeof (zen_AuxillaryAttribute_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
    attribute->m_operandStackSize = operandStackSize;
    attribute->m_localVariableArraySize = localVariableArraySize;

    return attribute;
}

/* Parse Code Attribute */

zen_CodeAttribute_t* zen_BinaryEntityParser_parseCodeAttribute(
    zen_BinaryEntityParser_t* parser, int32_t nameIndex, int32_t length) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    uint8_t* instructions = (uint8_t*)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (uint8_t) * length, ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
    jtk_Tape_readUncheckedBytes(parser->m_tape, instructions, length);

    zen_CodeAttribute_t* attribute = (zen_CodeAttribute_t*)zen_MemoryManager_allocateEx(
        parser->m_memoryManager, sizeof (zen_CodeAttribute_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
    attribute->m_instructions = instructions;

    return attribute;
}

/* Parse Exception Table Attribute */

zen_ExceptionTableAttribute_t* zen_BinaryEntityParser_parseExceptionTableAttribute(
    zen_BinaryEntityParser_t* parser, int32_t nameIndex, int32_t length) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    /* The virtual machine specification clearly states that
     * "An exception table attribute appears for a function entity only when
     * the function handles one or more exceptions. Which means this attribute
     * may not appear if the function handles no exceptions."
     *
     * Therefore, we can safely allocate the exception handler site array with
     * the specified number of elements.
     */
    uint16_t size = jtk_Tape_readUncheckedShort(parser->m_tape);
    zen_ExceptionHandlerSite_t** exceptionHanderSites = (zen_ExceptionHandlerSite_t**)zen_MemoryManager_allocateEx(
        parser->m_memoryManager, sizeof (zen_ExceptionHandlerSite_t*) * size,
        ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
    int32_t i;
    for (i = 0; i < size; i++) {
        uint16_t startIndex = jtk_Tape_readUncheckedShort(parser->m_tape);
        uint16_t stopIndex = jtk_Tape_readUncheckedShort(parser->m_tape);
        uint16_t handlerIndex = jtk_Tape_readUncheckedShort(parser->m_tape);
        uint16_t exceptionClass = jtk_Tape_readUncheckedShort(parser->m_tape);

        zen_ExceptionHandlerSite_t* exceptionHandlerSite = (zen_ExceptionHandlerSite_t*)zen_MemoryManager_allocateEx(
            parser->m_memoryManager, sizeof (zen_ExceptionHandlerSite_t),
            ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
        exceptionHandlerSite->m_startIndex = startIndex; /* inclusive */
        exceptionHandlerSite->m_stopIndex = stopIndex; /* inclusive */
        exceptionHandlerSite->m_handlerIndex = handlerIndex;
        exceptionHandlerSite->m_exceptionClass = exceptionClass;

        exceptionHanderSites[i] = exceptionHandlerSite;
    }

    zen_ExceptionTableAttribute_t* attribute = (zen_ExceptionTableAttribute_t*)zen_MemoryManager_allocateEx(
        parser->m_memoryManager, sizeof (zen_ExceptionTableAttribute_t),
        ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
    attribute->m_size = size;
    attribute->m_exceptionHandlerSites = exceptionHanderSites;

    return attribute;
}

/* Parse Source File Attribute */

zen_SourceFileAttribute_t* zen_BinaryEntityParser_parseSourceFileAttribute(
    zen_BinaryEntityParser_t* parser, int32_t nameIndex, int32_t length) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    uint16_t pathIndex = jtk_Tape_readUncheckedShort(parser->m_tape);

    zen_SourceFileAttribute_t* attribute = (zen_SourceFileAttribute_t*)zen_MemoryManager_allocateEx(
        parser->m_memoryManager, sizeof (zen_SourceFileAttribute_t),
        ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
    attribute->m_pathIndex = pathIndex;

    return attribute;
}

/* Parse Function */

zen_FunctionEntity_t* zen_BinaryEntityParser_parseFunction(zen_BinaryEntityParser_t* parser,
    zen_ConstantPool_t* constantPool) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    uint16_t nameIndex = jtk_Tape_readUncheckedShort(parser->m_tape);
    uint16_t descriptorIndex = jtk_Tape_readUncheckedShort(parser->m_tape);

    zen_FunctionEntity_t* entity = (zen_FunctionEntity_t*)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_FunctionEntity_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
    entity->m_nameIndex = nameIndex;
    entity->m_descriptorIndex = descriptorIndex;

    return entity;
}

/* Parse Field */

zen_FieldEntity_t* zen_BinaryEntityParser_parseField(zen_BinaryEntityParser_t* parser,
    zen_ConstantPool_t* constantPool) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    uint16_t nameIndex = jtk_Tape_readUncheckedShort(parser->m_tape);
    uint16_t descriptorIndex = jtk_Tape_readUncheckedShort(parser->m_tape);

    zen_FieldEntity_t* entity = (zen_FieldEntity_t*)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_FieldEntity_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
    entity->m_nameIndex = nameIndex;
    entity->m_descriptorIndex = descriptorIndex;

    return entity;
}

/* Parse Class */

zen_ClassEntity_t* zen_BinaryEntityParser_parseClass(zen_BinaryEntityParser_t* parser,
    zen_ConstantPool_t* constantPool) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    uint16_t superClassCount = jtk_Tape_readUncheckedShort(parser->m_tape);
    uint16_t* superClasses = (uint16_t*)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (uint16_t) * superClassCount, ZEN_ALIGNMENT_CONSTRAINT_DEFAULT,
        ZEN_ALLOCATION_FLAG_MANUAL);
    int32_t i;
    for (i = 0; i < superClassCount; i++) {
        superClasses[i] = jtk_Tape_readUncheckedShort(parser->m_tape);
    }

    uint16_t fieldCount = jtk_Tape_readUncheckedShort(parser->m_tape);
    zen_Entity_t** fields = (zen_Entity_t**)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_Entity_t*) * fieldCount, ZEN_ALIGNMENT_CONSTRAINT_DEFAULT,
        ZEN_ALLOCATION_FLAG_MANUAL);
    int32_t j;
    for (j = 0; j < fieldCount; j++) {
        fields[j] = zen_BinaryEntityParser_parseEntity(parser, constantPool);
    }

    uint16_t functionCount = jtk_Tape_readUncheckedShort(parser->m_tape);
    zen_Entity_t** functions = (zen_Entity_t**)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_Entity_t*) * functionCount, ZEN_ALIGNMENT_CONSTRAINT_DEFAULT,
        ZEN_ALLOCATION_FLAG_MANUAL);
    int32_t k;
    for (k = 0; k < fieldCount; k++) {
        functions[k] = zen_BinaryEntityParser_parseEntity(parser, constantPool);
    }

    zen_ClassEntity_t* entity = (zen_ClassEntity_t*)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_ClassEntity_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
    entity->m_superClassCount = superClassCount;
    entity->m_superClasses = superClasses;
    entity->m_fieldCount = fieldCount;
    entity->m_fields = fields;
    entity->m_functionCount = functionCount;
    entity->m_functions = functions;

    return entity;
}

/* Parse Enumerate */

zen_EnumerateEntity_t* zen_BinaryEntityParser_parseEnumerate(zen_BinaryEntityParser_t* parser,
    zen_ConstantPool_t* constantPool) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    uint16_t nameIndex = jtk_Tape_readUncheckedShort(parser->m_tape);
    uint16_t descriptorIndex = jtk_Tape_readUncheckedShort(parser->m_tape);

    zen_EnumerateEntity_t* entity = (zen_EnumerateEntity_t*)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_EnumerateEntity_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT,
        ZEN_ALLOCATION_FLAG_MANUAL);
    entity->m_nameIndex = nameIndex;
    entity->m_descriptorIndex = descriptorIndex;

    return entity;
}

/* Parse Enumeration */

zen_EnumerationEntity_t* zen_BinaryEntityParser_parseEnumeration(
    zen_BinaryEntityParser_t* parser, zen_ConstantPool_t* constantPool) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    uint16_t superClass = jtk_Tape_readUncheckedShort(parser->m_tape);

    uint16_t enumerateCount = jtk_Tape_readUncheckedShort(parser->m_tape);
    zen_Entity_t** enumerates = (zen_Entity_t**)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_Entity_t*) * enumerateCount, ZEN_ALIGNMENT_CONSTRAINT_DEFAULT,
        ZEN_ALLOCATION_FLAG_MANUAL);
    int32_t i;
    for (i = 0; i < enumerateCount; i++) {
        enumerates[i] = zen_BinaryEntityParser_parseEntity(parser, constantPool);
    }

    zen_Entity_t* initializer = zen_BinaryEntityParser_parseEntity(parser, constantPool);

    zen_EnumerationEntity_t* entity = (zen_EnumerationEntity_t*)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_EnumerationEntity_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
    entity->m_superClass = superClass;
    entity->m_enumerateCount = enumerateCount;
    entity->m_enumerates = enumerates;
    entity->m_initializer = initializer;

    return entity;
}

zen_AnnotationEntity_t* zen_BinaryEntityParser_parseAnnotation(
    zen_BinaryEntityParser_t* parser, zen_ConstantPool_t* constantPool) {
}

zen_PackageEntity_t* zen_BinaryEntityParser_parsePackage(
    zen_BinaryEntityParser_t* parser, zen_ConstantPool_t* constantPool) {
}

/*

import zen.fs.FileHelper as Files
import zen.net.HttpConnectionHelper as HttpConnections
import zen.core.ExceptionHelper.print as printStackTrace

function main
    try
        var url = new Url()
        url.setContent('https://www.onecube.in')
        with connection = HttpConnections.open(url), writer = path.openWriter()
            connection.readTo(writer)
    catch Exception exception
        exception.printStackTrace()
*/