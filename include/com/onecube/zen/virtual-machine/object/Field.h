// Sunday, July 14, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_FIELD_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_FIELD_H

#include <jtk/core/String.h>
#include <com/onecube/zen/Configuration.h>

// Forward References

typedef struct zen_Class_t zen_Class_t;

/*******************************************************************************
 * Field                                                                       *
 *******************************************************************************/

/**
 * @class Field
 * @ingroup zen_virtualMachine_object
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_Field_t {
    jtk_String_t* m_name;
    jtk_String_t* m_descriptor;
    zen_Class_t* m_class;
};

/**
 * @memberof Field
 */
typedef struct zen_Field_t zen_Field_t;

// Constructor

zen_Field_t* zen_Field_newFromFieldEntity(zen_Class_t* class0, 
    zen_FieldnEntity_t* fieldEntity);

// Destructor

void zen_Field_delete(zen_Field_t* field);

// Descriptor

jtk_String_t* zen_Field_getDescriptor(zen_Field_t* field);

// Name

jtk_String_t* zen_Field_getName(zen_Field_t* field);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_FIELD_H */