// Saturday, September 15, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_ENTITY_LOADER_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_ENTITY_LOADER_H

#include <jtk/collection/Iterator.h>
#include <jtk/collection/map/HashMap.h>
#include <jtk/collection/list/DoublyLinkedList.h>
#include <jtk/fs/Path.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/object/Class.h>
#include <com/onecube/zen/virtual-machine/feb/EntityFile.h>
#include <com/onecube/zen/virtual-machine/loader/BinaryEntityParser.h>
#include <com/onecube/zen/virtual-machine/loader/AttributeParseRules.h>

#define ZEN_ENTITY_LOADER_FLAG_PRIORITIZE_DIRECTORIES (1 << 0)
#define ZEN_ENTITY_LOADER_FLAG_IGNORE_CORRUPT_ENTITY (1 << 1)

/*******************************************************************************
 * EntityLoader                                                                *
 *******************************************************************************/

#define ZEN_ENTITY_LOADER_DEFAULT_ENTITIES_MAP_CAPCITY 128

/* A small experiment on a deployed project written in a certain virtual
 * machine powered language revealed that on an average most binary files
 * are 3 kilobytes in size. The buffer size was evaluated to reduce the cache
 * misses and increase the cache hits in the buffered input stream.
 */
#define ZEN_ENTITY_LOADER_BUFFER_SIZE (3 * 1024)

/**
 * @class EntityLoader
 * @ingroup zen_virtual_machine_loader
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EntityLoader_t {

    /**
     * The list of directories where the loader looks for the definitions
     * of entities.
     */
    jtk_DoublyLinkedList_t* m_directories;

    uint32_t m_flags;

    /**
     * The parser that contructs an entity from a binary input stream.
     */
    zen_BinaryEntityParser_t* m_parser;

    /**
     * Cache to store entities loaded previously.
     */
    jtk_HashMap_t* m_entities;
    
    zen_AttributeParseRules_t* m_attributeParseRules;
};

/**
 * @memberof EntityLoader
 */
typedef struct zen_EntityLoader_t zen_EntityLoader_t;

/* Constructor */

zen_EntityLoader_t* zen_EntityLoader_new();
zen_EntityLoader_t* zen_EntityLoader_newWithEntityDirectories(jtk_Iterator_t* iterator);

/* Destructor */

void zen_EntityLoader_delete(zen_EntityLoader_t* loader);

/* Class */

/**
 * First, it tries to find a previously loaded class with the specified
 * descriptor in the class registry. If not found, it tries to load it
 * from a physical description, i.e., a binary entity. It fails if a
 * corresponding binary entity is not found, without raising any exception.
 *
 * @memberof EntityLoader
 */
zen_EntityFile_t* zen_EntityLoader_findEntity(zen_EntityLoader_t* loader, const uint8_t* descriptor);

/**
 * It tries to find a previously loaded class with the specified descriptor in
 * the class registry. If not found, it fails without raising any exception.
 *
 * @memberof EntityLoader
 */
zen_EntityFile_t* zen_EntityLoader_getEntity(zen_EntityLoader_t* loader, const uint8_t* descriptor);
/**
 * It tries to load a class with the specified descriptor from a physical
 * description, i.e., a binary entity. It fails if a class was previously
 * loaded, or if a corresponding binary entity is not found.
 *
 * @memberof EntityLoader
 */
zen_EntityFile_t* zen_EntityLoader_loadEntity(zen_EntityLoader_t* loader, const uint8_t* descriptor);

// Load Class From

/**
 * It tries to load a class from the specified regular file path. If the file
 * does not exist or is corrupt, it fails without raising an exception.
 *
 * @memberof EntityLoader
 */
zen_EntityFile_t* zen_EntityLoader_loadEntityFromFile(zen_EntityLoader_t* loader,
    jtk_Path_t* path);

/**
 * It tries to load a class from the specified regular file handle. If the file
 * does not exist or is corrupt, it fails without raising an exception.
 *
 * @memberof EntityLoader
 */
zen_EntityFile_t* zen_EntityLoader_loadEntityFromHandle(zen_EntityLoader_t* loader,
    jtk_PathHandle_t* handle);

// Ignore Corrupt Entity

bool zen_EntityLoader_shouldIgnoreCorruptEntity(zen_EntityLoader_t* loader);
void zen_EntityLoader_setIgnoreCorruptEntity(zen_EntityLoader_t* loader, bool ignoreCorruptEntity);

bool zen_EntityLoader_addDirectory_s(zen_EntityLoader_t* loader, jtk_String_t* directory);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_ENTITY_LOADER_H */