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

#include <stdbool.h>

struct zen_Parameter_t {
};

typedef struct zen_Parameter_t zen_Parameter_t;

void printHelp() {
    printf(
        "--help\t\t
    );
}

uint32_t zen_FebParser_readMagicNumber(zen_FebParser_t* parser) {
    jtk_Assert_assertTrue((parser->m_readSections & ZEN_FEB_SECTION_MAGIC_NUMBER) == 0, "The magic number section was previously parsed.");
    uint32_t magicNumber = zen_DataInputStream_readUnsignedInteger(parser->m_stream, ZEN_ENDIAN_BIG);
    parser->m_readSections |= ZEN_FEB_SECTION_MAGIC_NUMBER;
    return magicNumber;
}

uint16_t zen_FebParser_readMajorVersion(zen_FebParser_t* parser) {
    jtk_Assert_assertTrue((parser->m_readSections & ZEN_FEB_SECTION_MAJOR_VERSION) == 0, "The major version section was previously parsed.");
    uint16_t majorVersion = zen_DataInputStream_readUnsignedShort(parser->m_stream, ZEN_ENDIAN_BIG);
    parser->m_readSections |= ZEN_FEB_SECTION_MAJOR_VERSION;
    return majorVersion;
}

uint16_t zen_FebParser_readMinorVersion(zen_FebParser_t* parser) {
    jtk_Assert_assertTrue((parser->m_readSections & ZEN_FEB_SECTION_MINOR_VERSION) == 0, "The minor version section was previously parsed.");
    uint16_t minorVersion = zen_DataInputStream_readUnsignedShort(parser->m_stream, ZEN_ENDIAN_BIG);
    parser->m_readSections |= ZEN_FEB_SECTION_MINOR_VERSION;
    return minorVersion;
}

zen_ConstantPool_t* zen_FebParser_readConstantPool(zen_FebParser_t* parser) {
    jtk_Assert_assertTrue((parser->m_readSections & ZEN_FEB_SECTION_CONSTANT_POOL) == 0, "The constant pool section was previously parsed.");

    uint16_t constantPoolCount = zen_DataInputStream_readUnsignedShort(parser->m_stream, ZEN_ENDIAN_BIG);
    zen_ConstantPoolElement_t** constantPoolElements = zen_Memory_allocate(zen_ConstantPoolElement_t, constantPoolCount);
    int16_t i = 0;
    for (i = 0; i < constantPoolCount; i++) {
        zen_ConstantPoolElement_t* constantPoolElement = zen_FebParser_readConstantPoolElement(parser);
        constantPoolElements[i] = constantPoolElement;
    }

    zen_ConstantPool_t* constantPool = zen_Memory_allocate(zen_ConstantPool_t, 1);
    constantPool->m_count = constantPoolCount;
    constantPool->m_elements = constantPoolElements;

    return constantPool;
}

void zen_FebParser_parse(zen_FebParser_t* parser) {
    uint32_t magicNumber = zen_FebParser_readMagicNumber(parser);
    uint16_t majorVersion = zen_FebParser_readMajorVersion(parser);
    uint16_t minorVersion = zen_FebParser_readMinorVersion(parser);
    zen_ConstantPool_t* constantPool = zen_FebParser_readConstantPool(parser);
    zen_Entity_t* entity = 

    zen_BinaryEntityFormat_t* format = zen_BinaryEntityFormat_new();
    format->m_magicNumber = magicNumber;
}

int32_t main(int32_t size, char** arguments) {
    bool help = false;

    zen_Parameter_t* helpParameter = zen_Parameter_new(ZEN_PARAMETER_TYPE_SIMPLE, "Help", "help", 'h', "Prints the help message", false);

    zen_Parameters_t* parameters = zen_Parameters_new();
    zen_Parameters_register(parameters, helpParameter);

    zen_InputParameters_t* inputParameters = zen_ParameterParser_parse(parameters, arguments, size);

    if (size == 1) {
        help = true;
    }
    else {
        int32_t i;
        for (i = 1; i < size; i++) {
            if (strcmp(arguments[i], "--help") == 0) {
                help = true;
            }
        }
    }

    if (help) {
        printHelp();
    }
    else {

    }

    return 0;
}