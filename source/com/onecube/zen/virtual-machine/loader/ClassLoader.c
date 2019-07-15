// Sunday, July 14, 2019

#include <jtk/collection/Iterator.h>
#include <jtk/collection/map/HashMapEntry.h>
#include <jtk/core/StringObjectAdapter.h>

#include <com/onecube/zen/virtual-machine/loader/ClassLoader.h>

/*******************************************************************************
 * ClassLoader                                                                 *
 *******************************************************************************/

// Constructor

zen_ClassLoader_t* zen_ClassLoader_new(zen_EntityLoader_t* entityLoader) {
    jtk_ObjectAdapter_t* stringObjectAdapter = jtk_StringObjectAdapter_getInstance();
    
    zen_ClassLoader_t* classLoader = jtk_Memory_allocate(zen_ClassLoader_t, 1);
    classLoader->m_entityLoader = entityLoader;
    classLoader->m_classes = jtk_HashMap_newEx(stringObjectAdapter, NULL,
        ZEN_CLASS_LOADER_DEFAULT_CLASSES_MAP_CAPCITY, JTK_HASH_MAP_DEFAULT_LOAD_FACTOR);
    
    return classLoader;
}

// Destructor

void zen_ClassLoader_delete(zen_ClassLoader_t* classLoader) {
    jtk_Assert_assertObject(classLoader, "The specified class loader is null.");
    
    jtk_Iterator_t* entryIterator = jtk_HashMap_getEntryIterator(classLoader->m_classes);
    while (jtk_Iterator_hasNext(entryIterator)) {
        jtk_HashMapEntry_t* entry = (jtk_HashMapEntry_t*)jtk_Iterator_getNext(entryIterator);
        jtk_String_t* descriptor = (jtk_String_t*)jtk_HashMapEntry_getKey(entry);
        zen_Class_t* class0 = (zen_Class_t*)jtk_HashMapEntry_getValue(entry);
        
        jtk_String_delete(descriptor);
        zen_Class_delete(class0);
    }
    jtk_Iterator_delete(entryIterator);
    
    jtk_HashMap_delete(classLoader->m_classes);
    jtk_Memory_deallocate(classLoader);
}

// Class

zen_Class_t* zen_ClassLoader_findClass(zen_ClassLoader_t* classLoader,
    const uint8_t* descriptor) {
    jtk_Assert_assertObject(classLoader, "The specified class loader is null.");
    
    bool destroyDescriptorString = true;
    jtk_String_t* descriptorString = jtk_String_new(descriptor);
    zen_Class_t* class0 = (zen_Class_t*)jtk_HashMap_getValue(classLoader->m_classes,
        descriptorString);

    /* The class with the specified descriptor was not found. Try to load it from
     * the entity loader.
     */
    if (class0 == NULL) {
        zen_EntityFile_t* entityFile = zen_EntityLoader_findEntity(
            classLoader->m_entityLoader, descriptor);

        /* An entity file was found. Convert it to a class. */
        if (entityFile != NULL) {
            class0 = zen_ClassLoader_loadFromEntityFile(classLoader, descriptor,
                entityFile);
            destroyDescriptorString = false;
        }
    }
    
    if (destroyDescriptorString) {
        jtk_String_delete(descriptorString);
    }
    
    return class0;
}

// Load

zen_Class_t* zen_ClassLoader_loadFromEntityFile(zen_ClassLoader_t* classLoader,
    jtk_String_t* descriptor, zen_EntityFile_t* entityFile) {
    jtk_Assert_assertObject(classLoader, "The specified class loader is null.");
    jtk_Assert_assertObject(descriptor, "The specified class descriptor is null.");
    jtk_Assert_assertObject(entityFile, "The specified entity file is null.");
    
    zen_Class_t* class0 = zen_Class_newFromEntityFile(entityFile);
    jtk_HashMap_put(classLoader->m_classes, descriptor, class0);
    
    return class0;
}
