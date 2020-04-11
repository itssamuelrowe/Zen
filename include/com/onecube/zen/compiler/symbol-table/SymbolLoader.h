/*
 * Copyright 2017-2020 Samuel Rowe
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

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_LOADER_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_LOADER_H

/*******************************************************************************
 * SymbolLoader                                                                *
 *******************************************************************************/

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/symbol-table/Symbol.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPool.h>

#include <jtk/collection/Iterator.h>
#include <jtk/collection/map/HashMap.h>
#include <jtk/collection/list/DoublyLinkedList.h>
#include <jtk/fs/Path.h>

#define ZEN_ENTITY_LOADER_FLAG_PRIORITIZE_DIRECTORIES (1 << 0)
#define ZEN_ENTITY_LOADER_FLAG_IGNORE_CORRUPT_ENTITY (1 << 1)

/*******************************************************************************
 * SymbolLoader                                                                *
 *******************************************************************************/

#define ZEN_ENTITY_LOADER_DEFAULT_ENTITIES_MAP_CAPCITY 128

/* A small experiment on a deployed project written in a certain virtual
 * machine powered language revealed that on an average most binary files
 * are 3 kilobytes in size. The buffer size was evaluated to reduce the cache
 * misses and increase the cache hits in the buffered input stream.
 */
#define ZEN_ENTITY_LOADER_BUFFER_SIZE (3 * 1024)

typedef struct zen_Compiler_t zen_Compiler_t;

/**
 * @class SymbolLoader
 * @ingroup zen_virtual_machine_loader
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_SymbolLoader_t {

    /**
     * The list of directories where the loader looks for the definitions
     * of entities.
     */
    jtk_DoublyLinkedList_t* m_directories;

    uint32_t m_flags;

    /**
     * Cache to store entities loaded previously.
     */
    jtk_HashMap_t* m_symbols;

    zen_Compiler_t* m_compiler;

    int32_t m_index;
    uint8_t* m_bytes;
    int32_t m_size;
    zen_ConstantPool_t m_constantPool;
    zen_Symbol_t* m_symbol;
};

/**
 * @memberof SymbolLoader
 */
typedef struct zen_SymbolLoader_t zen_SymbolLoader_t;

/* Constructor */

zen_SymbolLoader_t* zen_SymbolLoader_new(zen_Compiler_t* compiler);
zen_SymbolLoader_t* zen_SymbolLoader_newWithEntityDirectories(zen_Compiler_t* compiler,
    jtk_Iterator_t* entityDirectoryIterator);

/* Destructor */

void zen_SymbolLoader_delete(zen_SymbolLoader_t* loader);

/* Directory */

bool zen_SymbolLoader_addDirectory(zen_SymbolLoader_t* loader, const uint8_t* directory);

// Find Symbol

/**
 * First, it tries to find a previously loaded class with the specified
 * descriptor in the class registry. If not found, it tries to load it
 * from a physical description, i.e., a binary entity. It fails if a
 * corresponding binary entity is not found, without raising any exception.
 *
 * @memberof SymbolLoader
 */

zen_Symbol_t* zen_SymbolLoader_findSymbol(zen_SymbolLoader_t* loader,
    const uint8_t* descriptor, int32_t descriptorSize);

/**
 * It tries to load a class with the specified descriptor from a physical
 * description, i.e., a binary entity. It fails if a class was previously
 * loaded, or if a corresponding binary entity is not found.
 *
 * @memberof SymbolLoader
 */
zen_Symbol_t* zen_SymbolLoader_loadSymbol(zen_SymbolLoader_t* loader,
    const uint8_t* descriptor, int32_t descriptorSize);

/**
 * It tries to load a class from the specified regular file path. If the file
 * does not exist or is corrupt, it fails without raising an exception.
 *
 * @memberof SymbolLoader
 */
zen_Symbol_t* zen_SymbolLoader_loadEntityFromHandle(zen_SymbolLoader_t* loader,
    jtk_PathHandle_t* handle);

/**
 * It tries to load a class from the specified regular file handle. If the file
 * does not exist or is corrupt, it fails without raising an exception.
 *
 * @memberof SymbolLoader
 */
zen_Symbol_t* zen_SymbolLoader_loadSymbolFromHandle(zen_SymbolLoader_t* loader,
    jtk_PathHandle_t* handle);

zen_Symbol_t* zen_SymbolLoader_parse(zen_SymbolLoader_t* symbolLoader, uint8_t* bytes,
    int32_t size);

// Ignore Corrupt Entity

bool zen_SymbolLoader_shouldIgnoreCorruptEntity(zen_SymbolLoader_t* loader);
void zen_SymbolLoader_setIgnoreCorruptEntity(zen_SymbolLoader_t* loader, bool ignoreCorruptEntity);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_LOADER_H */