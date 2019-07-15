// Tuesday, March 26, 2019

#include <com/onecube/zen/virtual-machine/object/Function.h>
#include <com/onecube/zen/virtual-machine/object/Class.h>

/*******************************************************************************
 * Function                                                                    *
 *******************************************************************************/

// Constructor

zen_Function_t* zen_Function_newFromFunctionEntity(zen_Class_t* class0,
    zen_FunctionEntity_t* functionEntity) {
    zen_ConstantPool_t* constantPool = class0->m_entityFile->m_constantPool;
    zen_ConstantPoolUtf8_t* nameEntry = constantPool->m_entries[functionEntity->m_nameIndex];
    zen_ConstantPoolUtf8_t* descriptorEntry = constantPool->m_entries[functionEntity->m_descriptorIndex];

    zen_Function_t* function = jtk_Memory_allocate(zen_Function_t, 1);
    function->m_name = jtk_String_newEx(nameEntry->m_bytes, nameEntry->m_length);
    function->m_descriptor = jtk_String_newEx(descriptorEntry->m_bytes, descriptorEntry->m_length);
    function->m_class = class0;
    function->m_flags = 0;

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
    return false;
}