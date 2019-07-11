// Saturday, June 09, 2018

#include <com/onecube/zen/virtual-machine/feb/BinaryEntityFormat.h>
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

    if (jtk_Tape_isAvailable(parser->m_tape, 12)) {
        /* The magic number is a simple measure to identify corrupt streams.
         * It is represented with four bytes, that is, a 32-bit integer.
         *
         * Read the magic number and verify whether it is equal to 0xFEB72000.
         */
        uint32_t magicNumber = jtk_Tape_readUncheckedInteger(parser->m_tape);

        if (magicNumber == ZEN_BINARY_ENTITY_FORMAT_MAGIC_NUMBER) {
            uint16_t majorVersion = jtk_Tape_readUncheckedShort(parser->m_tape);
            uint16_t minorVersion = jtk_Tape_readUncheckedShort(parser->m_tape);

            /* Make sure that the major and minor version numbers are recognized by
             * the binary entity parser.
             */
            if ((majorVersion <= ZEN_BINARY_ENTITY_FORMAT_MAJOR_VERSION) &&
                (minorVersion <= ZEN_BINARY_ENTITY_FORMAT_MINOR_VERSION)) {
                uint32_t size = jtk_Tape_readUncheckedInteger(parser->m_tape);
                uint16_t flags = jtk_Tape_readUncheckedShort(parser->m_tape);

                zen_BinaryEntityParser_parseConstantPool(parser);

                // zen_BinaryEntityParser_parseEntity(parser);

                /*
                entityFile->m_magicNumber = magicNumber;
                entityFile->m_version.m_majorVersion = majorVersion;
                entityFile->m_version.m_minorVersion = minorVersion;
                entityFile->m_flags = flags;
                entityFile->m_constantPool = constantPool;
                entityFile->m_entity = entity;
                */
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

        uint16_t size = jtk_Tape_readUncheckedShort(parser->m_tape);

        // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsing constant pool entries...");

        int32_t index;
        for (index = 0; index < size; index++) {
            uint8_t tag = jtk_Tape_readUncheckedByte(parser->m_tape);

            switch (tag) {
                case ZEN_CONSTANT_POOL_TAG_INTEGER: {
                    uint32_t bytes = jtk_Tape_readUncheckedInteger(parser->m_tape);
                    
                    // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolInteger_t`, stored at index %d.", index);

                    break;
                }

                case ZEN_CONSTANT_POOL_TAG_LONG: {
                    uint32_t highBytes = jtk_Tape_readUncheckedInteger(parser->m_tape);
                    uint32_t lowBytes = jtk_Tape_readUncheckedInteger(parser->m_tape);

                    // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolLong_t`, stored at index %d.", index);

                    break;
                }

                case ZEN_CONSTANT_POOL_TAG_FLOAT: {
                    uint32_t bytes = jtk_Tape_readUncheckedInteger(parser->m_tape);

                    // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolFloat_t`, stored at index %d.", index);

                    break;
                }

                case ZEN_CONSTANT_POOL_TAG_DOUBLE: {
                    uint32_t highBytes = jtk_Tape_readUncheckedInteger(parser->m_tape);
                    uint32_t lowBytes = jtk_Tape_readUncheckedInteger(parser->m_tape);

                    // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolDouble_t`, stored at index %d.", index);

                    break;
                }

                case ZEN_CONSTANT_POOL_TAG_UTF8: {
                    uint16_t length = jtk_Tape_readUncheckedShort(parser->m_tape);
                    uint8_t* bytes = jtk_Memory_allocate(uint8_t, length);
                    jtk_Tape_readUncheckedBytes(parser->m_tape, bytes, length);

                    // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolUtf8_t`, stored at index %d.", index);

                    break;
                }

                case ZEN_CONSTANT_POOL_TAG_STRING: {
                    uint16_t stringIndex = jtk_Tape_readUncheckedShort(parser->m_tape);

                    // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolString_t`, stored at index %d.", index);

                    break;
                }

                case ZEN_CONSTANT_POOL_TAG_FUNCTION: {
                    uint16_t classIndex = jtk_Tape_readUncheckedShort(parser->m_tape);
                    uint16_t descriptorIndex = jtk_Tape_readUncheckedShort(parser->m_tape);
                    uint16_t nameIndex = jtk_Tape_readUncheckedShort(parser->m_tape);

                    // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolFunction_t`, stored at index %d.", index);

                    break;
                }

                case ZEN_CONSTANT_POOL_TAG_FIELD: {
                    uint16_t descriptorIndex = jtk_Tape_readUncheckedShort(parser->m_tape);
                    uint16_t nameIndex = jtk_Tape_readUncheckedShort(parser->m_tape);

                    // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolField_t`, stored at index %d.", index);

                    break;
                }

                case ZEN_CONSTANT_POOL_TAG_CLASS: {
                    uint16_t nameIndex = jtk_Tape_readUncheckedShort(parser->m_tape);

                    // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolClass_t`, stored at index %d.", index);

                    break;
                }

                case ZEN_CONSTANT_POOL_TAG_ENUMERATE: {
                    uint16_t enumerationIndex = jtk_Tape_readUncheckedShort(parser->m_tape);
                    uint16_t nameIndex = jtk_Tape_readUncheckedShort(parser->m_tape);

                    // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolEnumerate_t`, stored at index %d.", index);

                    break;
                }

                case ZEN_CONSTANT_POOL_TAG_ENUMERATION: {
                    uint16_t nameIndex = jtk_Tape_readUncheckedShort(parser->m_tape);

                    // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed constant pool entry `zen_ConstantPoolEnumeration_t`, stored at index %d.", index);

                    break;
                }
            }

            // constantPool->m_tags[index] = tag;
            // constantPool->m_entries[index] = (void*)entry;
        }

        // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed %d constant pool entries, stored at indexes [0, %d).",
            // size, size);
}

/* Parse Entity */

zen_Entity_t* zen_BinaryEntityParser_parseEntity(zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    // zen_Entity_t* entity = (zen_Entity_t*)zen_MemoryManager_allocateEx(parser->m_memoryManager,
    //    sizeof (zen_Entity_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
    uint8_t type = jtk_Tape_readUncheckedByte(parser->m_tape);
    uint16_t flags = jtk_Tape_readUncheckedShort(parser->m_tape);
    uint16_t reference = jtk_Tape_readUncheckedShort(parser->m_tape);
    zen_BinaryEntityParser_parseAttributeTable(parser);

    switch (type) {
        case ZEN_ENTITY_TYPE_FUNCTION: {
            zen_BinaryEntityParser_parseFunction(parser);

            // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed function entity, with type %d.", type);

            break;
        }

        case ZEN_ENTITY_TYPE_FIELD: {
            zen_BinaryEntityParser_parseField(parser);

            // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed field entity, with type %d.", type);

            break;
        }

        case ZEN_ENTITY_TYPE_CLASS: {
            zen_BinaryEntityParser_parseClass(parser);

            // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed class entity, with type %d.", type);

            break;
        }

        case ZEN_ENTITY_TYPE_ENUMERATE: {
            zen_BinaryEntityParser_parseEnumerate(parser);

            // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed enumerate entity, with type %d.", type);

            break;
        }

        case ZEN_ENTITY_TYPE_ENUMERATION: {
            zen_BinaryEntityParser_parseEnumeration(parser);

            // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed enumeration entity, with type %d.", type);

            break;
        }

        case ZEN_ENTITY_TYPE_ANNOTATION: {
            zen_BinaryEntityParser_parseAnnotation(parser);

            // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed annotation entity, with type %d.", type);

            break;
        }

        case ZEN_ENTITY_TYPE_PACKAGE: {
            zen_BinaryEntityParser_parsePackage(parser);

            // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Parsed package entity, with type %d.", type);

            break;
        }

        default: {
            /* error: Control should not reach here. */
            break;
        }
    }

    return NULL;
}

/* Parse Attribute Table */

zen_AttributeTable_t* zen_BinaryEntityParser_parseAttributeTable(
    zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    uint16_t size = jtk_Tape_readUncheckedShort(parser->m_tape);
    
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

            // jtk_Logger_info(parser->m_logger, ZEN_BINARY_ENTITY_PARSER_TAG, "Skipped unrecognized attribute `%s`. Null value stored at index %d.", name, i);
        }
    }

    return NULL;
}

/* Parse Attributes */

void zen_BinaryEntityParser_parseAttributes(zen_BinaryEntityParser_t* parser, zen_AttributeTable_t* attributeTable) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    
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

zen_FunctionEntity_t* zen_BinaryEntityParser_parseFunction(zen_BinaryEntityParser_t* parser) {
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

zen_FieldEntity_t* zen_BinaryEntityParser_parseField(zen_BinaryEntityParser_t* parser) {
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

zen_ClassEntity_t* zen_BinaryEntityParser_parseClass(zen_BinaryEntityParser_t* parser) {
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
        fields[j] = zen_BinaryEntityParser_parseEntity(parser);
    }

    uint16_t functionCount = jtk_Tape_readUncheckedShort(parser->m_tape);
    zen_Entity_t** functions = (zen_Entity_t**)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_Entity_t*) * functionCount, ZEN_ALIGNMENT_CONSTRAINT_DEFAULT,
        ZEN_ALLOCATION_FLAG_MANUAL);
    int32_t k;
    for (k = 0; k < fieldCount; k++) {
        functions[k] = zen_BinaryEntityParser_parseEntity(parser);
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

zen_EnumerateEntity_t* zen_BinaryEntityParser_parseEnumerate(zen_BinaryEntityParser_t* parser) {
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
    zen_BinaryEntityParser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified binary entity parser is null.");

    uint16_t superClass = jtk_Tape_readUncheckedShort(parser->m_tape);

    uint16_t enumerateCount = jtk_Tape_readUncheckedShort(parser->m_tape);
    zen_Entity_t** enumerates = (zen_Entity_t**)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_Entity_t*) * enumerateCount, ZEN_ALIGNMENT_CONSTRAINT_DEFAULT,
        ZEN_ALLOCATION_FLAG_MANUAL);
    int32_t i;
    for (i = 0; i < enumerateCount; i++) {
        enumerates[i] = zen_BinaryEntityParser_parseEntity(parser);
    }

    zen_Entity_t* initializer = zen_BinaryEntityParser_parseEntity(parser);

    zen_EnumerationEntity_t* entity = (zen_EnumerationEntity_t*)zen_MemoryManager_allocateEx(parser->m_memoryManager,
        sizeof (zen_EnumerationEntity_t), ZEN_ALIGNMENT_CONSTRAINT_DEFAULT, ZEN_ALLOCATION_FLAG_MANUAL);
    entity->m_superClass = superClass;
    entity->m_enumerateCount = enumerateCount;
    entity->m_enumerates = enumerates;
    entity->m_initializer = initializer;

    return entity;
}

zen_AnnotationEntity_t* zen_BinaryEntityParser_parseAnnotation(
    zen_BinaryEntityParser_t* parser) {
}

zen_PackageEntity_t* zen_BinaryEntityParser_parsePackage(
    zen_BinaryEntityParser_t* parser) {
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