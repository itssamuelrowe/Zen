// Friday, March 30, 2018

#include <com/onecube/zen/bcg/BinaryEntityGenerator.h>
#include <com/onecube/zen/feb/Attribute.h>
#include <com/onecube/zen/feb/BinaryEntityFormat.h>
#include <com/onecube/zen/feb/BinaryFile.h>
#include <com/onecube/zen/feb/ClassEntity.h>
#include <com/onecube/zen/feb/CodeAttribute.h>
#include <com/onecube/zen/feb/ConstantPoolClass.h>
#include <com/onecube/zen/feb/ConstantPoolDouble.h>
#include <com/onecube/zen/feb/ConstantPoolEntry.h>
#include <com/onecube/zen/feb/ConstantPoolField.h>
#include <com/onecube/zen/feb/ConstantPoolFloat.h>
#include <com/onecube/zen/feb/ConstantPoolFunction.h>
#include <com/onecube/zen/feb/ConstantPoolInteger.h>
#include <com/onecube/zen/feb/ConstantPoolLong.h>
#include <com/onecube/zen/feb/ConstantPoolString.h>
#include <com/onecube/zen/feb/ConstantPoolTag.h>
#include <com/onecube/zen/feb/ConstantPoolUtf8.h>
#include <com/onecube/zen/feb/ConstantValueAttribute.h>
#include <com/onecube/zen/feb/Entity.h>
#include <com/onecube/zen/feb/EntityFlag.h>
#include <com/onecube/zen/feb/EntityType.h>
#include <com/onecube/zen/feb/FieldEntity.h>
#include <com/onecube/zen/feb/FunctionEntity.h>
#include <com/onecube/zen/feb/PredefinedAttribute.h>

zen_BinaryEntityGenerator_t* zen_BinaryEntityGenerator_new() {
    return NULL;
}

void zen_BinaryEntityGenerator_generate(zen_BinaryEntityGenerator_t* generator) {
}

void zen_BinaryEntityGenerator_delete(zen_BinaryEntityGenerator_t* generator) {
}

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    FILE* output = fopen("hello.feb", "w");
    fwrite(ptr, size, 1, output);
    /*
    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            fprintf(output, "%u", byte);
        }
    }*/
}

/*
void zen_BinaryEntityGenerator_onEnterCompilationUnit(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;

    zen_BinaryEntityGenerator_writeMagicNumber(generator);
    zen_BinaryEntityGenerator_writeVersion(generator);
    zen_BinaryEntityGenerator_writeConstantPool(generator);
    zen_BinaryEntityGenerator_writeEntity(generator);
}

void zen_BinaryEntityGenerator_writeMagicNumber(zen_BinaryEntityGenerator_t* generator) {
    zen_DataOutputStream_writeInteger(generator->m_output, ZEN_BINARY_ENTITY_FORMAT_MAGIC_NUMBER);
}

void zen_BinaryEntityGenerator_writeVersion(zen_BinaryEntityGenerator_t* generator) {
    zen_DataOutputStream_writeShort(generator->m_output, ZEN_BINARY_ENTITY_FORMAT_MAJOR_VERSION);
    zen_DataOutputStream_writeShort(generator->m_output, ZEN_BINARY_ENTITY_FORMAT_MINOR_VERSION);
}

void zen_BinaryEntityGenerator_writeConstantPool(zen_BinaryEntityGenerator_t* generator) {
    zen_ConstantPool_t* constantPool = generator->m_constantPool;

    zen_DataOutputStream_writeInteger(generator->m_output, constantPool->m_size);

    int32_t i;
    for (i = 0; i < constantPool->m_size; i++) {
        zen_ConstantPoolEntry_t* entry = generator->m_entries[i];
        zen_BinaryEntityGenerator_writeConstantPoolEntry(generator, entry);
    }
}

void zen_BinaryEntityGenerator_writeConstantPoolEntry(
    zen_BinaryEntityGenerator_t* generator, zen_ConstantPoolEntry_t* entry) {
    zen_DataOutputStream_writeByte(generator->m_output, entry->m_tag);
    switch (entry->m_tag) {
        case ZEN_CONSTANT_POOL_TAG_UTF8: {
            zen_BinaryEntityGenerator_writeConstantPoolUtf8(generator, (zen_ConstantPoolUtf8_t*)entry->m_body);
            break;
        }

        case ZEN_CONSTANT_POOL_TAG_INTEGER: {
            zen_BinaryEntityGenerator_writeConstantPoolInteger(generator, (zen_ConstantPoolInteger_t*)entry->m_body);
            break;
        }

        case ZEN_CONSTANT_POOL_TAG_LONG: {
            zen_BinaryEntityGenerator_writeConstantPoolLong(generator, (zen_ConstantPoolLong_t*)entry->m_body);
            break;
        }

        case ZEN_CONSTANT_POOL_TAG_FLOAT: {
            zen_BinaryEntityGenerator_writeConstantPoolFloat(generator, (zen_ConstantPoolFloat_t*)entry->m_body);
            break;
        }

        case ZEN_CONSTANT_POOL_TAG_DOUBLE: {
            zen_BinaryEntityGenerator_writeConstantPoolDouble(generator, (zen_ConstantPoolDouble_t*)entry->m_body);
            break;
        }

        case ZEN_CONSTANT_POOL_TAG_STRING: {
            zen_BinaryEntityGenerator_writeConstantPoolString(generator, (zen_ConstantPoolString_t*)entry->m_body);
            break;
        }

        case ZEN_CONSTANT_POOL_TAG_CLASS: {
            zen_BinaryEntityGenerator_writeConstantPoolClass(generator, (zen_ConstantPoolClass_t*)entry->m_body);
            break;
        }

        case ZEN_CONSTANT_POOL_TAG_FUNCTION: {
            zen_BinaryEntityGenerator_writeConstantPoolFunction(generator, (zen_ConstantPoolFunction_t*)entry->m_body);
            break;
        }

        case ZEN_CONSTANT_POOL_TAG_FIELD: {
            zen_BinaryEntityGenerator_writeConstantPoolField(generator, (zen_ConstantPoolField_t*)entry->m_body);
            break;
        }
    }
}

void zen_BinaryEntityGenerator_writeConstantPoolUtf8(zen_BinaryEntityGenerator_t* generator, zen_ConstantPoolUtf8_t* utf8) {
    zen_DataOutputStream_writeShort(generator->m_output, utf8->m_length);
    int32_t i;
    for (i = 0; i < utf8->m_length; i++) {
        zen_DataOutputStream_write(generator->m_output, utf8->m_bytes[i]);
    }
}

void zen_BinaryEntityGenerator_writeConstantPoolInteger(zen_BinaryEntityGenerator_t* generator, zen_ConstantPoolInteger_t* cpi) {
    zen_DataOutputStream_writeInteger(generator->m_output, cpi->m_bytes);
}

void zen_BinaryEntityGenerator_writeConstantPoolLong(zen_BinaryEntityGenerator_t* generator, zen_ConstantPoolLong_t* cpl) {
    zen_DataOutputStream_writeLong(generator->m_output, cpl->m_bytes);
}

void zen_BinaryEntityGenerator_writeConstantPoolFloat(zen_BinaryEntityGenerator_t* generator, zen_ConstantPoolFloat_t* cpf) {
    zen_DataOutputStream_writeInteger(generator->m_output, cpf->m_bytes);
}

void zen_BinaryEntityGenerator_writeConstantPoolDouble(zen_BinaryEntityGenerator_t* generator, zen_ConstantPoolDouble_t* cpd) {
    zen_DataOutputStream_writeLong(generator->m_output, cpd->m_bytes);
}

void zen_BinaryEntityGenerator_writeConstantPoolString(zen_BinaryEntityGenerator_t* generator, zen_ConstantPoolString_t* cps) {
    zen_DataOutputStream_writeShort(generator->m_output, field->m_stringIndex);
}

void zen_BinaryEntityGenerator_writeConstantPoolClass(zen_BinaryEntityGenerator_t* generator, zen_ConstantPoolClass_t* cpc) {
    zen_DataOutputStream_writeShort(generator->m_output, cpc->m_nameIndex);
}

void zen_BinaryEntityGenerator_writeConstantPoolFunction(zen_BinaryEntityGenerator_t* generator, zen_ConstantPoolFunction_t* function) {
    zen_DataOutputStream_writeShort(generator->m_output, function->m_classIndex);
    zen_DataOutputStream_writeShort(generator->m_output, function->m_descriptorIndex);
    zen_DataOutputStream_writeShort(generator->m_output, function->m_nameIndex);
}

void zen_BinaryEntityGenerator_writeConstantPoolField(zen_BinaryEntityGenerator_t* generator, zen_ConstantPoolField_t* field) {
    zen_DataOutputStream_writeShort(generator->m_output, field->m_classIndex);
    zen_DataOutputStream_writeShort(generator->m_output, field->m_descriptorIndex);
    zen_DataOutputStream_writeShort(generator->m_output, field->m_nameIndex);
}

void zen_BinaryEntityGenerator_writeEntity(
    zen_BinaryEntityGenerator_t* generator, zen_Entity_t* entity) {
    zen_DataOutputStream_writeByte(generator->m_output, entity->m_type);
    zen_DataOutputStream_writeShort(generator->m_output, entity->m_flags);
    zen_DataOutputStream_writeShort(generator->m_output, entity->m_reference);
    zen_BinaryEntityGenerator_writeAttributes(generator, entity->m_attributes, entity->m_attributeCount);

    switch (entity->m_type) {
        case ZEN_ENTITY_TYPE_CLASS: {
            zen_BinaryEntityGenerator_writeClassEntity(generator, (zen_ClassEntity_t*)(entity->m_body));
            break;
        }

        case ZEN_ENTITY_TYPE_ENUMERATION: {
            zen_BinaryEntityGenerator_writeEnumerationEntity(generator, (zen_EnumerationEntity_t*)(entity->m_body));
            break;
        }

        case ZEN_ENTITY_TYPE_ANNOTATION: {
            zen_BinaryEntityGenerator_writeAnnotationEntity(generator, (zen_AnnotationEntity_t*)(entity->m_body));
            break;
        }
        
        default: {
            // [internal error] Invalid top-level entity
            break;
        }
    }
}

void zen_BinaryEntityGenerator_writeClassEntity(zen_BinaryEntityGenerator_t* generator,
    zen_ClassEntity_t* entity) {
    zen_BinaryEntityGenerator_writeSuperClasses(generator->m_output, entity->m_superClasses, entity->m_superClassCount);
    zen_BinaryEntityGenerator_writeFieldEntities(generator->m_output, entity->m_fields, entity->m_fieldCount);
    zen_BinaryEntityGenerator_writeFunctionEntities(generator->m_output, entity->m_functions, entity->m_functionCount);
    zen_BinaryEntityGenerator_writeAttributes(generator->m_output, entity->m_attributes, entity->m_attributeCount);
}

void zen_BinaryEntityGenerator_writeFieldEntity(zen_BinaryEntityGenerator_t* generator,
    zen_FieldEntity_t* entity) {
    zen_DataOutputStream_writeShort(generator->m_output, entity->m_nameIndex);
    zen_DataOutputStream_writeShort(generator->m_output, entity->m_descriptorIndex);
}

void zen_BinaryEntityGenerator_writeFunctionEntity(zen_BinaryEntityGenerator_t* generator,
    zen_FunctionEntity_t* entity) {
    zen_DataOutputStream_writeShort(generator->m_output, entity->m_nameIndex);
    zen_DataOutputStream_writeShort(generator->m_output, entity->m_descriptorIndex);
}

void zen_BinaryEntityGenerator_writeAttributes(
    zen_BinaryEntityGenerator_t* generator, zen_Attribute_t** attributes,
    int32_t attributeCount) {
    int32_t i;
    for (i = 0; i < attributeCount; i++) {
        zen_Attribute_t* attribute = attributes[i];
        zen_BinaryEntityGenerator_writeAttribute(generator, attribute);
    }
}

/*void zen_BinaryEntityGenerator_writeAttribute(zen_BinaryEntityGenerator_t* generator,
    uint8_t* byteCodes, uint32_t length

    */