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

// Saturday, April 06, 2019

// TODO: Change this to single-linked list.
#include <jtk/collection/Iterator.h>
#include <jtk/collection/list/DoublyLinkedList.h>
#include <jtk/core/CStringObjectAdapter.h>
#include <jtk/core/CString.h>

#include <com/onecube/zen/virtual-machine/object/Class.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolClass.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolUtf8.h>

/*******************************************************************************
 * Class                                                                       *
 *******************************************************************************/

// Constructor

zen_Class_t* zen_Class_new(zen_EntityFile_t* entityFile) {
    jtk_ObjectAdapter_t* stringObjectAdapter = jtk_CStringObjectAdapter_getInstance();

    zen_Class_t* class0 = jtk_Memory_allocate(zen_Class_t, 1);
    class0->m_entityFile = entityFile;
    class0->m_functions = jtk_HashMap_newEx(stringObjectAdapter, NULL,
        JTK_HASH_MAP_DEFAULT_CAPACITY, JTK_HASH_MAP_DEFAULT_LOAD_FACTOR);
    class0->m_fields = jtk_HashMap_newEx(stringObjectAdapter, NULL,
        JTK_HASH_MAP_DEFAULT_CAPACITY, JTK_HASH_MAP_DEFAULT_LOAD_FACTOR);
    class0->m_memoryRequirement = 0;

    zen_Class_initialize(class0, entityFile);

    return class0;
}

// Destructor

void zen_Class_delete(zen_Class_t* class0) {
    // Destroy functions
    jtk_Iterator_t* functionIterator = jtk_HashMap_getEntryIterator(class0->m_functions);
    while (jtk_Iterator_hasNext(functionIterator)) {
        jtk_HashMapEntry_t* entry =
            (jtk_HashMapEntry_t*)jtk_Iterator_getNext(functionIterator);
        uint8_t* key = (uint8_t*)jtk_HashMapEntry_getKey(entry);
        zen_Function_t* value = (zen_Function_t*)jtk_HashMapEntry_getValue(entry);

        jtk_CString_delete(key);
        zen_Function_delete(value);
    }
    jtk_Iterator_delete(functionIterator);

    // Destroy fields
    jtk_Iterator_t* fieldIterator = jtk_HashMap_getEntryIterator(class0->m_fields);
    while (jtk_Iterator_hasNext(fieldIterator)) {
        jtk_HashMapEntry_t* entry =
            (jtk_HashMapEntry_t*)jtk_Iterator_getNext(fieldIterator);
        // uint8_t* key = (uint8_t*)jtk_HashMapEntry_getKey(entry);
        zen_Field_t* value = (zen_Field_t*)jtk_HashMapEntry_getValue(entry);

        // Do not delete field keys because they belong to the Field class.
        // jtk_CString_delete(key);
        zen_Field_delete(value);
    }
    jtk_Iterator_delete(fieldIterator);

    jtk_HashMap_delete(class0->m_functions);
    jtk_HashMap_delete(class0->m_fields);
    jtk_CString_delete(class0->m_descriptor);
    jtk_Memory_deallocate(class0);
}

// Entity File

zen_EntityFile_t* zen_Class_getEntityFile(zen_Class_t* class0) {
    return class0->m_entityFile;
}

// Field Index

int32_t zen_Class_findFieldOffset(zen_Class_t* class0, const uint8_t* name,
    int32_t nameSize) {
    jtk_Assert_assertObject(class0, "The specified class is null.");
    jtk_Assert_assertObject(name, "The specified field name is null.");

    // TODO: Implement this function to find fields in class hierarchies.
    // TODO: Wrap the byte string in a String object, instead of allocating.

    zen_Field_t* field = jtk_HashMap_getValue(class0->m_fields, name);
    
    return (field != NULL)? field->m_offset : -1;
}

// Function

zen_Function_t* zen_Class_getConstructor(zen_Class_t* class0,
    const uint8_t* descriptor, int32_t descriptorSize) {
    return zen_Class_getStaticFunction(class0, "<initialize>",
        12, descriptor, descriptorSize);
}

zen_Function_t* zen_Class_getStaticFunction(zen_Class_t* class0,
    const uint8_t* name, int32_t nameSize, const uint8_t* descriptor,
    int32_t descriptorSize) {
    int32_t keySize;
    uint8_t* key = jtk_CString_joinEx(name, nameSize, descriptor,
        descriptorSize, &keySize);
    zen_Function_t* function = jtk_HashMap_getValue(class0->m_functions, key);
    jtk_CString_delete(key);

    // TODO: Filter for static function.
    return function;
}

zen_Function_t* zen_Class_getInstanceFunction(zen_Class_t* class0,
    const uint8_t* name, int32_t nameSize, const uint8_t* descriptor,
    int32_t descriptorSize) {
    int32_t keySize;
    uint8_t* key = jtk_CString_joinEx(name, nameSize, descriptor,
        descriptorSize, &keySize);
    zen_Function_t* function = jtk_HashMap_getValue(class0->m_functions, key);
    jtk_CString_delete(key);

    // TODO: Filter for instance function.
    return function;
}

// Initialize

void zen_Class_initialize(zen_Class_t* class0, zen_EntityFile_t* entityFile) {
    jtk_Assert_assertObject(class0, "The specified class is null.");
    jtk_Assert_assertObject(entityFile, "The specified entity file is null.");

    zen_Entity_t* entity = &entityFile->m_entity;
    zen_ConstantPool_t* constantPool = &entityFile->m_constantPool;

    zen_ConstantPoolUtf8_t* descriptorEntry =
        (zen_ConstantPoolUtf8_t*)constantPool->m_entries[entity->m_reference];
    class0->m_descriptor = jtk_CString_newEx(descriptorEntry->m_bytes, descriptorEntry->m_length);

    int32_t i;
    int32_t fieldCount = entity->m_fieldCount;
    for (i = 0; i < fieldCount; i++) {
        zen_FieldEntity_t* fieldEntity = (zen_FieldEntity_t*)entity->m_fields[i];
        /* Memory requirement should be strictly used to calculate the space
         * required by the fields of the class. This constraint is placed due
         * to the usage of m_memoryRequirement in evaluating the offsets of the
         * fields.
         */
        zen_Field_t* field = zen_Field_new(class0, fieldEntity, class0->m_memoryRequirement);
        jtk_HashMap_put(class0->m_fields, field->m_name, field);

        if (field->m_descriptorSize == 1) {
            switch (field->m_descriptor[0]) {
                // TODO: What is the size of a character?

                case 'z':
                case 'b': {
                    class0->m_memoryRequirement += 1;
                    break;
                }

                case 's': {
                    class0->m_memoryRequirement += 2;
                    break;
                }

                case 'f':
                case 'i': {
                    class0->m_memoryRequirement += 4;
                    break;
                }

                case 'd':
                case 'l': {
                    class0->m_memoryRequirement += 8;
                    break;
                }

                default: {
                    printf("[internal error] Control should not reach here!\n");
                    break;
                }
            }
        }
        else {
            class0->m_memoryRequirement += sizeof (uintptr_t);
        }
    }

    int32_t j;
    int32_t functionCount = entity->m_functionCount;
    for (j = 0; j < functionCount; j++) {
        zen_FunctionEntity_t* functionEntity = (zen_FunctionEntity_t*)entity->m_functions[j];
        zen_Function_t* function = zen_Function_newFromFunctionEntity(class0, functionEntity);

        int32_t keySize;
        uint8_t* key = jtk_CString_joinEx(function->m_name, function->m_nameSize,
            function->m_descriptor, function->m_descriptorSize, &keySize);
        jtk_HashMap_put(class0->m_functions, key, function);
    }
}