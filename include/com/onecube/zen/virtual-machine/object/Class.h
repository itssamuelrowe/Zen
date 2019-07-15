// Tuesday, October 02, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_CLASS_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_CLASS_H

#include <jtk/collection/map/HashMap.h>
#include <jtk/core/String.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/feb/EntityFile.h>
#include <com/onecube/zen/virtual-machine/feb/ClassEntity.h>
#include <com/onecube/zen/virtual-machine/object/Function.h>
#include <com/onecube/zen/virtual-machine/object/Field.h>

/*******************************************************************************
 * Class                                                                       *
 *******************************************************************************/

/**
 * @class Class
 * @ingroup zen_virtualMachine_object
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_Class_t {
    zen_EntityFile_t* m_entityFile;
    jtk_HashMap_t* m_functions;
    jtk_HashMap_t* m_fields;
    jtk_String_t* m_descriptor;
};

/**
 * @memberof Class
 */
typedef struct zen_Class_t zen_Class_t;

// Constructor

/**
 * @memberof Class
 */
zen_Class_t* zen_Class_newFromEntityFile(zen_EntityFile_t* entityFile);

// Destructor

/**
 * @memberof Class
 */
void zen_Class_delete(zen_Class_t* class0);

// Entity File

zen_EntityFile_t* zen_Class_getEntityFile(zen_Class_t* class0);

// Function

zen_Function_t* zen_Class_getStaticFunction(zen_Class_t* class0, jtk_String_t* name,
    jtk_String_t* descriptor);

zen_Function_t* zen_Class_getInstanceFunction(zen_Class_t* class0, jtk_String_t* name,
    jtk_String_t* descriptor);

// Initialize

void zen_Class_initialize(zen_Class_t* class0, zen_EntityFile_t* entityFile);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_OBJECT_CLASS_H */