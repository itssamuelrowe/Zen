/*
 * Copyright 2018-2019 OneCube
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

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_BINARY_ENTITY_PARSER_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_BINARY_ENTITY_PARSER_H

#include <jtk/io/InputStream.h>
#include <jtk/log/Logger.h>
#include <jtk/hokum/Tape.h>

#include <com/onecube/zen/Configuration.h>

#include <com/onecube/zen/virtual-machine/feb/AnnotationEntity.h>
#include <com/onecube/zen/virtual-machine/feb/ClassEntity.h>
#include <com/onecube/zen/virtual-machine/feb/FieldEntity.h>
#include <com/onecube/zen/virtual-machine/feb/FunctionEntity.h>
#include <com/onecube/zen/virtual-machine/feb/EntityFile.h>
#include <com/onecube/zen/virtual-machine/feb/PackageEntity.h>
#include <com/onecube/zen/virtual-machine/feb/EntityType.h>

#include <com/onecube/zen/virtual-machine/feb/attribute/InstructionAttribute.h>
#include <com/onecube/zen/virtual-machine/feb/attribute/ExceptionTable.h>
#include <com/onecube/zen/virtual-machine/feb/attribute/SourceFileAttribute.h>

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

#include <com/onecube/zen/virtual-machine/loader/AttributeParseRules.h>
#include <com/onecube/zen/virtual-machine/memory/MemoryManager.h>

/*******************************************************************************
 * BinaryEntityParser                                                          *
 *******************************************************************************/

struct zen_BinaryEntityParser_t {
    jtk_Tape_t* m_tape;
    zen_AttributeParseRules_t* m_attributeParseRules;
    zen_MemoryManager_t* m_memoryManager;
    jtk_Logger_t* m_logger;
    zen_EntityFile_t* m_entityFile;
};

typedef struct zen_BinaryEntityParser_t zen_BinaryEntityParser_t;

/* Constructor */

// zen_BinaryEntityParser_t* zen_BinaryEntityParser_new(zen_MemoryManager_t* memoryManager, zen_AttributeParseRules_t* AttributeParseRules, uint8_t* bytes, int32_t size);

zen_BinaryEntityParser_t* zen_BinaryEntityParser_new(zen_AttributeParseRules_t* AttributeParseRules, uint8_t* bytes, int32_t size);

/* Destructor */

void zen_BinaryEntityParser_delete(zen_BinaryEntityParser_t* parser);

/* Parse */

zen_EntityFile_t* zen_BinaryEntityParser_parse(zen_BinaryEntityParser_t* parser,
    jtk_InputStream_t* inputStream);

/* Parse Entity File */

zen_EntityFile_t* zen_BinaryEntityParser_parseEntityFile(zen_BinaryEntityParser_t* parser);

/* Parse Constant Pool */

zen_ConstantPool_t* zen_BinaryEntityParser_parseConstantPool(zen_BinaryEntityParser_t* parser);

/* Parse Entity */

zen_Entity_t* zen_BinaryEntityParser_parseEntity(zen_BinaryEntityParser_t* parser);

/* Parse Attribute Table */

void zen_BinaryEntityParser_parseAttributeTable(zen_BinaryEntityParser_t* parser, zen_AttributeTable_t* attributeTable);
    
/* Parse Instruction Attribute */

zen_InstructionAttribute_t* zen_BinaryEntityParser_parseInstructionAttribute(
    zen_BinaryEntityParser_t* parser, uint16_t nameIndex, uint32_t length);
    
/* Parse Exception Table */

void zen_BinaryEntityParser_parseExceptionTable(zen_BinaryEntityParser_t* parser,
    zen_ExceptionTable_t* exceptionTable);

/* Parse Exception Handler Site */

zen_ExceptionHandlerSite_t* zen_BinaryEntityParser_parseExceptionHandlerSite(zen_BinaryEntityParser_t* parser);

/* Parse Function */

zen_FunctionEntity_t* zen_BinaryEntityParser_parseFunction(zen_BinaryEntityParser_t* parser);

/* Parse Field */

zen_FieldEntity_t* zen_BinaryEntityParser_parseField(zen_BinaryEntityParser_t* parser);

/* Parse Class */

zen_ClassEntity_t* zen_BinaryEntityParser_parseClass(zen_BinaryEntityParser_t* parser);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_BINARY_ENTITY_PARSER_H */