// Sunday, July 14, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_CLASS_LOADER_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_CLASS_LOADER_H

#include <jtk/collection/map/HashMap.h>
#include <jtk/core/String.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/feb/EntityFile.h>
#include <com/onecube/zen/virtual-machine/loader/EntityLoader.h>
#include <com/onecube/zen/virtual-machine/object/Class.h>

/*******************************************************************************
 * ClassLoader                                                                 *
 *******************************************************************************/

#define ZEN_CLASS_LOADER_DEFAULT_CLASSES_MAP_CAPCITY 128

/**
 * Along with the entity loader, the loading mechanism of the virtual machine
 * makes use of two level cache.
 *
 * @class ClassLoader
 * @ingroup zen_virtualMachine_loader
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ClassLoader_t {
    zen_EntityLoader_t* m_entityLoader;
    jtk_HashMap_t* m_classes;
};

/**
 * @memberof ClassLoader
 */
typedef struct zen_ClassLoader_t zen_ClassLoader_t;

// Constructor

zen_ClassLoader_t* zen_ClassLoader_new(zen_EntityLoader_t* entityLoader);

// Destructor

void zen_ClassLoader_delete(zen_ClassLoader_t* classLoader);

// Class

zen_Class_t* zen_ClassLoader_findClass(zen_ClassLoader_t* classLoader,
    const uint8_t* descriptor);

// Load

/**
 * This function does not ensure that a class with the specified descriptor was
 * previously loaded. This behavior can cause memory leaks if used incorrectly.
 */
zen_Class_t* zen_ClassLoader_loadFromEntityFile(zen_ClassLoader_t* classLoader,
    jtk_String_t* descriptor, zen_EntityFile_t* entityFile);
    
#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_CLASS_LOADER_H */