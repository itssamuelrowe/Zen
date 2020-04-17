/*
 * Copyright 2018-2020 Samuel Rowe
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

// Friday, April 17, 2019

#ifndef COM_ONECUBE_ZEN_DISASSEMBLER_BINARY_ENTITY_DISASSEMBLER_H
#define COM_ONECUBE_ZEN_DISASSEMBLER_BINARY_ENTITY_DISASSEMBLER_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPool.h>

#include <jtk/collection/list/DoublyLinkedList.h>
#include <jtk/fs/Path.h>

/*******************************************************************************
 * BinaryEntityDisassembler                                                    *
 *******************************************************************************/

struct zen_BinaryEntityDisassembler_t {

    /**
     * The list of directories where the loader looks for the definitions
     * of entities.
     */
    jtk_DoublyLinkedList_t* m_directories;

    int32_t m_index;
    uint8_t* m_bytes;
    int32_t m_size;

    zen_ConstantPool_t m_constantPool;
};

typedef struct zen_BinaryEntityDisassembler_t zen_BinaryEntityDisassembler_t;

zen_BinaryEntityDisassembler_t* zen_BinaryEntityDisassembler_new(
    jtk_Iterator_t* entityDirectoryIterator);

void zen_BinaryEntityDisassembler_delete(zen_BinaryEntityDisassembler_t* disassembler);

// Directory

bool zen_BinaryEntityDisassembler_addDirectory(zen_BinaryEntityDisassembler_t* disassembler,
    const uint8_t* directory, int32_t directorySize);
void zen_BinaryEntityDisassembler_disassemble(zen_BinaryEntityDisassembler_t* disassembler,
    uint8_t* bytes, int32_t size);

/* Disassemble Constant Pool */

void zen_BinaryEntityDisassembler_disassembleConstantPool(
    zen_BinaryEntityDisassembler_t* disassembler);

/* Disassemble Entity */

void zen_BinaryEntityDisassembler_disassembleEntity(zen_BinaryEntityDisassembler_t* disassembler);

/* Disassemble Attribute Table */

void zen_BinaryEntityDisassembler_disassembleAttributeTable(
    zen_BinaryEntityDisassembler_t* disassembler);

/* Disassemble Instruction Attribute */

void zen_BinaryEntityDisassembler_disassembleInstructionAttribute(
    zen_BinaryEntityDisassembler_t* disassembler, uint16_t nameIndex, uint32_t length);

/* Disassemble Exception Table */

void zen_BinaryEntityDisassembler_disassembleExceptionTable(
        zen_BinaryEntityDisassembler_t* disassembler);

/* Disassemble Exception Handler Site */

void zen_BinaryEntityDisassembler_disassembleExceptionHandlerSite(
    zen_BinaryEntityDisassembler_t* disassembler);

/* Disassemble Function */

void zen_BinaryEntityDisassembler_disassembleFunction(zen_BinaryEntityDisassembler_t* disassembler);

/* Disassemble Field */

void zen_BinaryEntityDisassembler_disassembleField(zen_BinaryEntityDisassembler_t* disassembler);

void zen_BinaryEntityDisassembler_disassembleClass(zen_BinaryEntityDisassembler_t* disassembler,
    const uint8_t* descriptor, int32_t descriptorSize);

#endif /* COM_ONECUBE_ZEN_DISASSEMBLER_BINARY_ENTITY_DISASSEMBLER_H */