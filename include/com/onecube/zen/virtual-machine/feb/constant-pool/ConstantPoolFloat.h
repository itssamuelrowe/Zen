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

// Saturday, April 28, 2018

#ifndef ZEN_FEB_CONSTANT_POOL_FLOAT_CONSTANT_POOL_H
#define ZEN_FEB_CONSTANT_POOL_FLOAT_CONSTANT_POOL_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ConstantPoolFloat                                                           *
 *******************************************************************************/

/**
 * Represents an floating value reference.
 *
 * @author Samuel Rowe
 * @since  STK 1.0
 */
struct zen_ConstantPoolFloat_t {

    /**
     * The type of the constant pool entry.
     */
    uint8_t m_tag;

    /**
     * The value of the float constant. The bytes are stored in the
     * big-endian order.
     *
     * The floating value is encoded in IEEE 754 floating-point single format.
     * The bits
     */
    uint32_t m_bytes;
};

/**
 * @memberof ConstantPoolFloat
 */
typedef struct zen_ConstantPoolFloat_t zen_ConstantPoolFloat_t;

/* Value */

/**
 * @memberof ConstantPoolFloat
 */
float zen_ConstantPoolFloat_getValue(zen_ConstantPoolFloat_t* constantPoolFloat);

#endif /* ZEN_FEB_CONSTANT_POOL_FLOAT_CONSTANT_POOL_H */