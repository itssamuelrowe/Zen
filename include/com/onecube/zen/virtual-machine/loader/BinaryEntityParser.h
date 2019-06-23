// Saturday, June 09, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_BINARY_ENTITY_PARSER_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_BINARY_ENTITY_PARSER_H

#include <jtk/io/InputStream.h>
#include <jtk/log/Logger.h>
#include <jtk/hokum/Tape.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/feb/AnnotationEntity.h>
#include <com/onecube/zen/virtual-machine/feb/AuxillaryAttribute.h>
#include <com/onecube/zen/virtual-machine/feb/ClassEntity.h>
#include <com/onecube/zen/virtual-machine/feb/CodeAttribute.h>
#include <com/onecube/zen/virtual-machine/feb/FieldEntity.h>
#include <com/onecube/zen/virtual-machine/feb/FunctionEntity.h>
#include <com/onecube/zen/virtual-machine/feb/EntityFile.h>
#include <com/onecube/zen/virtual-machine/feb/EnumerateEntity.h>
#include <com/onecube/zen/virtual-machine/feb/EnumerationEntity.h>
#include <com/onecube/zen/virtual-machine/feb/ExceptionTableAttribute.h>
#include <com/onecube/zen/virtual-machine/feb/PackageEntity.h>
#include <com/onecube/zen/virtual-machine/feb/SourceFileAttribute.h>
#include <com/onecube/zen/virtual-machine/loader/AttributeParseRuleMap.h>
#include <com/onecube/zen/virtual-machine/feb/ConstantPoolClass.h>
#include <com/onecube/zen/virtual-machine/feb/ConstantPoolDouble.h>
#include <com/onecube/zen/virtual-machine/feb/ConstantPoolEnumerate.h>
#include <com/onecube/zen/virtual-machine/feb/ConstantPoolEnumeration.h>
#include <com/onecube/zen/virtual-machine/feb/ConstantPoolField.h>
#include <com/onecube/zen/virtual-machine/feb/ConstantPoolFloat.h>
#include <com/onecube/zen/virtual-machine/feb/ConstantPoolFunction.h>
#include <com/onecube/zen/virtual-machine/feb/ConstantPoolInteger.h>
#include <com/onecube/zen/virtual-machine/feb/ConstantPoolLong.h>
#include <com/onecube/zen/virtual-machine/feb/ConstantPoolString.h>
#include <com/onecube/zen/virtual-machine/feb/ConstantPoolTag.h>
#include <com/onecube/zen/virtual-machine/feb/ConstantPoolUtf8.h>
#include <com/onecube/zen/virtual-machine/feb/EntityType.h>
#include <com/onecube/zen/virtual-machine/memory/MemoryManager.h>

/*******************************************************************************
 * BinaryEntityParser                                                          *
 *******************************************************************************/

struct zen_BinaryEntityParser_t {
    jtk_Tape_t* m_tape;
    zen_AttributeParseRuleMap_t* m_attributeParseRuleMap;
    zen_MemoryManager_t* m_memoryManager;
    jtk_Logger_t* m_logger;
};

typedef struct zen_BinaryEntityParser_t zen_BinaryEntityParser_t;

/* Constructor */

// zen_BinaryEntityParser_t* zen_BinaryEntityParser_new(zen_MemoryManager_t* memoryManager, zen_AttributeParseRuleMap_t* attributeParseRuleMap, uint8_t* bytes, int32_t size);
zen_BinaryEntityParser_t* zen_BinaryEntityParser_new(zen_AttributeParseRuleMap_t* attributeParseRuleMap, uint8_t* bytes, int32_t size);

/* Destructor */

void zen_BinaryEntityParser_delete(zen_BinaryEntityParser_t* parser);

/* Parse */

zen_EntityFile_t* zen_BinaryEntityParser_parse(zen_BinaryEntityParser_t* parser,
    jtk_InputStream_t* inputStream);
zen_EntityFile_t* zen_BinaryEntityParser_parseEntityFile(zen_BinaryEntityParser_t* parser);
zen_ConstantPool_t* zen_BinaryEntityParser_parseConstantPool(zen_BinaryEntityParser_t* parser);
void zen_BinaryEntityParser_parseConstantPoolEntries(zen_BinaryEntityParser_t* parser, zen_ConstantPool_t* constantPool);

zen_ConstantPoolInteger_t* zen_BinaryEntityParser_parseConstantPoolInteger(zen_BinaryEntityParser_t* parser);
zen_ConstantPoolLong_t* zen_BinaryEntityParser_parseConstantPoolLong(zen_BinaryEntityParser_t* parser);
zen_ConstantPoolFloat_t* zen_BinaryEntityParser_parseConstantPoolFloat(zen_BinaryEntityParser_t* parser);
zen_ConstantPoolDouble_t* zen_BinaryEntityParser_parseConstantPoolDouble(zen_BinaryEntityParser_t* parser);
zen_ConstantPoolUtf8_t* zen_BinaryEntityParser_parseConstantPoolUtf8(zen_BinaryEntityParser_t* parser);
zen_ConstantPoolString_t* zen_BinaryEntityParser_parseConstantPoolString(zen_BinaryEntityParser_t* parser);
zen_ConstantPoolFunction_t* zen_BinaryEntityParser_parseConstantPoolFunction(zen_BinaryEntityParser_t* parser);
zen_ConstantPoolClass_t* zen_BinaryEntityParser_parseConstantPoolClass(zen_BinaryEntityParser_t* parser);
zen_ConstantPoolEnumerate_t* zen_BinaryEntityParser_parseConstantPoolEnumerate(zen_BinaryEntityParser_t* parser);
zen_ConstantPoolEnumeration_t* zen_BinaryEntityParser_parseConstantPoolEnumeration(zen_BinaryEntityParser_t* parser);

zen_ConstantPoolField_t* zen_BinaryEntityParser_parseConstantPoolField(zen_BinaryEntityParser_t* parser);
zen_Entity_t* zen_BinaryEntityParser_parseEntity(zen_BinaryEntityParser_t* parser, zen_ConstantPool_t* constantPool);
zen_AttributeTable_t* zen_BinaryEntityParser_parseAttributeTable(zen_BinaryEntityParser_t* parser, zen_ConstantPool_t* constantPool);
void zen_BinaryEntityParser_parseAttributes(zen_BinaryEntityParser_t* parser, zen_ConstantPool_t* constantPool, zen_AttributeTable_t* attributeTable);
zen_AuxillaryAttribute_t* zen_BinaryEntityParser_parseAuxillaryAttribute(zen_BinaryEntityParser_t* parser, int32_t nameIndex, int32_t length);
zen_CodeAttribute_t* zen_BinaryEntityParser_parseCodeAttribute(zen_BinaryEntityParser_t* parser, int32_t nameIndex, int32_t length);
zen_ExceptionTableAttribute_t* zen_BinaryEntityParser_parseExceptionTableAttribute(zen_BinaryEntityParser_t* parser, int32_t nameIndex, int32_t length);
zen_SourceFileAttribute_t* zen_BinaryEntityParser_parseSourceFileAttribute(zen_BinaryEntityParser_t* parser, int32_t nameIndex, int32_t length);
zen_FieldEntity_t* zen_BinaryEntityParser_parseField(zen_BinaryEntityParser_t* parser, zen_ConstantPool_t* constantPool);
zen_ClassEntity_t* zen_BinaryEntityParser_parseClass(zen_BinaryEntityParser_t* parser, zen_ConstantPool_t* constantPool);
zen_FunctionEntity_t* zen_BinaryEntityParser_parseFunction(zen_BinaryEntityParser_t* parser, zen_ConstantPool_t* constantPool);
zen_EnumerateEntity_t* zen_BinaryEntityParser_parseEnumerate(zen_BinaryEntityParser_t* parser, zen_ConstantPool_t* constantPool);
zen_EnumerationEntity_t* zen_BinaryEntityParser_parseEnumeration(zen_BinaryEntityParser_t* parser, zen_ConstantPool_t* constantPool);
zen_AnnotationEntity_t* zen_BinaryEntityParser_parseAnnotation(zen_BinaryEntityParser_t* parser, zen_ConstantPool_t* constantPool);
zen_PackageEntity_t* zen_BinaryEntityParser_parsePackage(zen_BinaryEntityParser_t* parser, zen_ConstantPool_t* constantPool);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_BINARY_ENTITY_PARSER_H */