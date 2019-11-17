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

// Saturday, April 06, 2019

// TODO: Change this to single-linked list.
#include <jtk/collection/Iterator.h>
#include <jtk/collection/list/DoublyLinkedList.h>

#include <com/onecube/zen/virtual-machine/object/Class.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolClass.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolUtf8.h>

#warning "Move this function to String.c"

jtk_String_t* jtk_String_append(jtk_String_t* string1, jtk_String_t* string2) {
    return jtk_String_newFromJoinEx(string1->m_value, string1->m_size,
        string2->m_value, string2->m_size);
}



/*******************************************************************************
 * Class                                                                       *
 *******************************************************************************/

// Constructor

zen_Class_t* zen_Class_newFromEntityFile(zen_EntityFile_t* entityFile) {
    jtk_ObjectAdapter_t* stringObjectAdapter = jtk_StringObjectAdapter_getInstance();

    zen_Class_t* class0 = jtk_Memory_allocate(zen_Class_t, 1);
    class0->m_entityFile = entityFile;
    class0->m_functions = jtk_HashMap_newEx(stringObjectAdapter, NULL,
        JTK_HASH_MAP_DEFAULT_CAPACITY, JTK_HASH_MAP_DEFAULT_LOAD_FACTOR);
    class0->m_fields = jtk_HashMap_newEx(stringObjectAdapter, NULL,
        JTK_HASH_MAP_DEFAULT_CAPACITY, JTK_HASH_MAP_DEFAULT_LOAD_FACTOR);

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
        jtk_String_t* key = (jtk_String_t*)jtk_HashMapEntry_getKey(entry);
        zen_Function_t* value = (zen_Function_t*)jtk_HashMapEntry_getValue(entry);
        
        jtk_String_delete(key);
        zen_Function_delete(value);
    }
    jtk_Iterator_delete(functionIterator);
    
    // Destroy fields
    jtk_Iterator_t* fieldIterator = jtk_HashMap_getEntryIterator(class0->m_fields);
    while (jtk_Iterator_hasNext(fieldIterator)) {
        jtk_HashMapEntry_t* entry =
            (jtk_HashMapEntry_t*)jtk_Iterator_getNext(fieldIterator);
        // jtk_String_t* key = (jtk_String_t*)jtk_HashMapEntry_getKey(entry);
        zen_Field_t* value = (zen_Field_t*)jtk_HashMapEntry_getValue(entry);
        
        // Do not delete field keys because they belong to the Field class.
        // jtk_String_delete(key);
        zen_Field_delete(value);
    }
    jtk_Iterator_delete(fieldIterator);
    
    jtk_HashMap_delete(class0->m_functions);
    jtk_HashMap_delete(class0->m_fields);
    jtk_String_delete(class0->m_descriptor);
    jtk_Memory_deallocate(class0);
}

// Entity File

zen_EntityFile_t* zen_Class_getEntityFile(zen_Class_t* class0) {
    return class0->m_entityFile;
}

// Function

zen_Function_t* zen_Class_getStaticFunction(zen_Class_t* class0, jtk_String_t* name,
    jtk_String_t* descriptor) {
    jtk_String_t* key = jtk_String_append(name, descriptor);
    zen_Function_t* function = jtk_HashMap_getValue(class0->m_functions, key);
    jtk_String_delete(key);
    
    // TODO: Filter for static function.
    return function;
}

zen_Function_t* zen_Class_getInstanceFunction(zen_Class_t* class0, jtk_String_t* name,
    jtk_String_t* descriptor) {
    jtk_String_t* key = jtk_String_append(name, descriptor);
    zen_Function_t* function = jtk_HashMap_getValue(class0->m_functions, key);
    jtk_String_delete(key);
    
    // TODO: Filter for instance function.
    return function;
}

// Initialize

void zen_Class_initialize(zen_Class_t* class0, zen_EntityFile_t* entityFile) {
    jtk_Assert_assertObject(class0, "The specified class is null.");
    jtk_Assert_assertObject(entityFile, "The specified entity file is null.");

    zen_Entity_t* entity = &entityFile->m_entity;
    zen_ConstantPool_t* constantPool = entityFile->m_constantPool;

    zen_ConstantPoolClass_t* classEntry =
        (zen_ConstantPoolClass_t*)constantPool->m_entries[entity->m_reference];
    zen_ConstantPoolUtf8_t* descriptorEntry =
        (zen_ConstantPoolUtf8_t*)constantPool->m_entries[classEntry->m_nameIndex];

    class0->m_descriptor = jtk_String_newEx(descriptorEntry->m_bytes, descriptorEntry->m_length);

    int32_t i;
    int32_t fieldCount = entity->m_fieldCount;
    for (i = 0; i < fieldCount; i++) {
        zen_FieldEntity_t* fieldEntity = (zen_FieldEntity_t*)entity->m_fields[i];
        zen_Field_t* field = zen_Field_newFromFieldEntity(class0, fieldEntity);
        jtk_HashMap_put(class0->m_fields, field->m_name, field);
    }

    int32_t j;
    int32_t functionCount = entity->m_functionCount;
    for (j = 0; j < functionCount; j++) {
        zen_FunctionEntity_t* functionEntity = (zen_FunctionEntity_t*)entity->m_functions[j];
        zen_Function_t* function = zen_Function_newFromFunctionEntity(class0, functionEntity);

        jtk_String_t* key = jtk_String_append(function->m_name, function->m_descriptor);
        jtk_HashMap_put(class0->m_functions, key, function);
    }
}