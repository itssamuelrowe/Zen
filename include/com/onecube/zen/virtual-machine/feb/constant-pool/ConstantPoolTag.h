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

#ifndef ZEN_FEB_CONSTANT_POOL_TAG_CONSTANT_POOL_H
#define ZEN_FEB_CONSTANT_POOL_TAG_CONSTANT_POOL_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ConstantPoolTag                                                             *
 *******************************************************************************/

/**
 * @author Samuel Rowe
 * @since  zen 1.0
 */
enum zen_ConstantPoolTag_t {
    ZEN_CONSTANT_POOL_TAG_INTEGER = 0,
    ZEN_CONSTANT_POOL_TAG_LONG = 1,
    ZEN_CONSTANT_POOL_TAG_FLOAT = 2,
    ZEN_CONSTANT_POOL_TAG_DOUBLE = 3,
    ZEN_CONSTANT_POOL_TAG_UTF8 = 4,
    ZEN_CONSTANT_POOL_TAG_STRING = 5,
    ZEN_CONSTANT_POOL_TAG_FUNCTION = 6,
    ZEN_CONSTANT_POOL_TAG_FIELD = 7,
    ZEN_CONSTANT_POOL_TAG_CLASS = 8
};

typedef enum zen_ConstantPoolTag_t zen_ConstantPoolTag_t;

#endif /* ZEN_FEB_CONSTANT_POOL_TAG_CONSTANT_POOL_H */