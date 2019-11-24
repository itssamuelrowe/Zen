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

#ifndef ZEN_FEB_CONSTANT_VALUE_ATTRIBUTE_H
#define ZEN_FEB_CONSTANT_VALUE_ATTRIBUTE_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ConstantValueAttribute                                                      *
 *******************************************************************************/

/**
 * Represents the value of a constant expression.
 *
 * A static field is assigned a constant value attribute. When a class is
 * loaded for the first time, the virtual machine initializes the field to
 * this constant value.
 */
struct zen_ConstantValueAttribute_t {

    /**
A valid index into the constant pool table. The entry at this index
is a constant value. The appropriate entry types are:
    ZEN_CONSTANT_POOL_TAG_LONG           long
    ZEN_CONSTANT_POOL_TAG_FLOAT          float
    ZEN_CONSTANT_POOL_TAG_DOUBLE         double
    ZEN_CONSTANT_POOL_TAG_INTEGER        int, short, char, byte, boolean
    ZEN_CONSTANT_POOL_TAG_STRING         String
     */
    uint16_t m_valueIndex;
};

typedef struct zen_ConstantValueAttribute_t zen_ConstantValueAttribute_t;

#endif /* ZEN_FEB_CONSTANT_VALUE_ATTRIBUTE_H */