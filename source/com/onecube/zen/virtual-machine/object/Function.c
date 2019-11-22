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

// Tuesday, March 26, 2019

#include <com/onecube/zen/virtual-machine/feb/EntityFlag.h>
#include <com/onecube/zen/virtual-machine/object/Function.h>
#include <com/onecube/zen/virtual-machine/object/Class.h>

/*******************************************************************************
 * Function                                                                    *
 *******************************************************************************/

// Constructor

zen_Function_t* zen_Function_newFromFunctionEntity(zen_Class_t* class0,
    zen_FunctionEntity_t* functionEntity) {
    zen_ConstantPool_t* constantPool = &class0->m_entityFile->m_constantPool;
    zen_ConstantPoolUtf8_t* nameEntry = constantPool->m_entries[functionEntity->m_nameIndex];
    zen_ConstantPoolUtf8_t* descriptorEntry = constantPool->m_entries[functionEntity->m_descriptorIndex];

    zen_Function_t* function = jtk_Memory_allocate(zen_Function_t, 1);
    function->m_name = jtk_String_newEx(nameEntry->m_bytes, nameEntry->m_length);
    function->m_descriptor = jtk_String_newEx(descriptorEntry->m_bytes, descriptorEntry->m_length);
    function->m_class = class0;
    function->m_flags = 0;
    function->m_functionEntity = functionEntity;

    return function;
}

// Destructor

void zen_Function_delete(zen_Function_t* function) {
    jtk_String_delete(function->m_name);
    jtk_String_delete(function->m_descriptor);
    jtk_Memory_deallocate(function);
}

// Class

zen_Class_t* zen_Function_getClass(zen_Function_t* function) {
    return function->m_class;
}

// Descriptor

jtk_String_t* zen_Function_getDescriptor(zen_Function_t* function) {
    return function->m_descriptor;
}

// Name

jtk_String_t* zen_Function_getName(zen_Function_t* function) {
    return function->m_name;
}

// Native

bool zen_Function_isNative(zen_Function_t* function) {
    return (function->m_functionEntity->m_flags & ZEN_ENTITY_FLAG_NATIVE) != 0;
}