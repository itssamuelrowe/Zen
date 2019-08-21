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

// Saturday, April 28, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ENTITY_FILE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ENTITY_FILE_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPool.h>
#include <com/onecube/zen/virtual-machine/feb/Entity.h>
#include <com/onecube/zen/virtual-machine/feb/FebVersion.h>

/*******************************************************************************
 * EntityFile                                                                  *
 *******************************************************************************/

/**
 * Representation of a entity file, more appropriately a binary tape, encoded
 * in the Binary Entity Format (FEB). It consists of an array of bytes.
 * Muti-byte quantities which occupy 2 bytes, 4 bytes, and 8 bytes are
 * constructed by reading two, four, and eight consecutive bytes, respectively.
 * These bytes are always stored in big-endian order.
 *
 * Every entity file contains the definition of a single primary entity: class,
 * interface, annotation, or enumeration.
 *
 * @class EntityFile
 * @ingroup zen_vm_feb
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EntityFile_t {

    /**
     * Represents the magic number which identifies the Binary Entity Format (FEB).
     * It has the value 0xFEB72000.
     */
    uint32_t m_magicNumber;

    /**
     * Represents the version of the Binary Entity Format (FEB) with which
     * the binary file is encoded.
     */
    zen_FebVersion_t m_version;

    /**
     * The size of the stream followed by this attribute. This helps virtual
     * machines allocate a single buffer to represent the whole entity hierarchy.
     *
     * Further, writing an entity to a stream will simply require copying the bytes
     * of the buffer in a linear fashion.
     */
    uint32_t m_size;

    /**
     * A mask of flags which denote various properties of this entity file.
     */
    uint16_t m_flags;

    /**
     * The constant pool is a table where references to various constants,
     * identifiers, and other such entries are stored.
     */
    zen_ConstantPool_t* m_constantPool;

    /**
     * The primary binary entity of this binary file.
     *
     * Every binary file contains the definition of a single entity: class,
     * interface, annotation, or enumeration. It is known as the primary entity.
     */
    zen_Entity_t* m_entity;
};

/**
 * @memberof EntityFile
 */
typedef struct zen_EntityFile_t zen_EntityFile_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ENTITY_FILE_H */