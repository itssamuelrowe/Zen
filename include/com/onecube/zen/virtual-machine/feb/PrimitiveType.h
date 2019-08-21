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

// Wednesday, January 23, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_PRIMITIVE_TYPE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_PRIMITIVE_TYPE_H

/*******************************************************************************
 * PrimitiveType                                                               *
 *******************************************************************************/

/**
 * @class PrimitiveType
 * @ingroup zen_vm_memory_allocator
 * @author Samuel Rowe
 * @since zen 1.0
 */
enum zen_PrimitiveType_t {
    ZEN_PRIMITIVE_TYPE_BOOLEAN = 0,
    ZEN_PRIMITIVE_TYPE_CHARACTER = 1,
    ZEN_PRIMITIVE_TYPE_INTEGER_8 = 2,
    ZEN_PRIMITIVE_TYPE_INTEGER_16 = 3,
    ZEN_PRIMITIVE_TYPE_INTEGER_32 = 4,
    ZEN_PRIMITIVE_TYPE_INTEGER_64 = 5,
    ZEN_PRIMITIVE_TYPE_DECIMAL_32 = 6,
    ZEN_PRIMITIVE_TYPE_DECIMAL_64 = 7
};

/**
 * @memberof PrimitiveType
 */
typedef enum zen_PrimitiveType_t zen_PrimitiveType_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_PRIMITIVE_TYPE_H */