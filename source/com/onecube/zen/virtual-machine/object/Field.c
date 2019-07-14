// Sunday, July 14, 2019

#include <com/onecube/zen/virtual-machine/object/Field.h>
#include <com/onecube/zen/virtual-machine/object/Class.h>

/*******************************************************************************
 * Field                                                                       *
 *******************************************************************************/

// Constructor

zen_Field_t* zen_Field_newFromFieldEntity(zen_Class_t* class0,
    zen_FieldEntity_t* fieldEntity) {
    zen_ConstantPool_t* constantPool = class0->m_classEntity->m_constantPool;
    zen_ConstantPoolUtf8_t* nameEntry = constantPool->m_entries[fieldEntity->m_nameIndex];
    zen_ConstantPoolUtf8_t* descriptorEntry = constantPool->m_entries[fieldEntity->m_descriptorIndex];

    zen_Field_t* field = jtk_Memory_allocate(zen_Field_t, 1);
    field->m_name = jtk_String_newEx(nameEntry->m_bytes, nameEntry->m_length);
    function->m_descriptor = jtk_String_newEx(descriptorEntry->m_bytes, descriptorEntry->m_length);
    function->m_class = class0;

    return field;
}

// Destructor

void zen_Field_delete(zen_Field_t* field) {
    jtk_String_delete(field->m_name);
    jtk_String_delete(field->m_descriptor);
    jtk_Memory_deallocate(field);

}

// Class

zen_Class_t* zen_Field_getClass(zen_Field_t* field) {
    return field->m_class;
}

// Descriptor

jtk_String_t* zen_Field_getDescriptor(zen_Field_t* field) {
    return field->m_descriptor;
}

// Name

jtk_String_t* zen_Field_getName(zen_Field_t* field) {
    return field->m_name;
}