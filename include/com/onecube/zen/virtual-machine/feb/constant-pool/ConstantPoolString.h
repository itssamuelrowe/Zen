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

// Saturday, April 28, 2018

#ifndef ZEN_FEB_CONSTANT_POOL_STRING_CONSTANT_POOL_H
#define ZEN_FEB_CONSTANT_POOL_STRING_CONSTANT_POOL_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ConstantPoolString                                                          *
 *******************************************************************************/

/**
 * Represents a string reference.
 *
 * @author Samuel Rowe
 * @since  zen 1.0
 */
struct zen_ConstantPoolString_t {

    /**
     * The type of the constant pool entry.
     */
    uint8_t m_tag;

    /**
     * A valid index into the constant pool. The entry at this index is an
     * UTF-8 element which represents the sequence of Unicode code points
     * contained in this string entry.
     */
    uint16_t m_stringIndex;
};

typedef struct zen_ConstantPoolString_t zen_ConstantPoolString_t;

#endif /* ZEN_FEB_CONSTANT_POOL_STRING_CONSTANT_POOL_H */