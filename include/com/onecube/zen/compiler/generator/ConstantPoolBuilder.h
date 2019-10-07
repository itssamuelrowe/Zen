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

#ifndef COM_ONECUBE_ZEN_COMPILER_GENERATOR_CONSTANT_POOL_BUILDER_H
#define COM_ONECUBE_ZEN_COMPILER_GENERATOR_CONSTANT_POOL_BUILDER_H

/*******************************************************************************
 * ConstantPoolBuilder                                                         *
 *******************************************************************************/

/**
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ConstantPoolBuilder_t {
};

/**
 * @memberof ConstantPoolBuilder
 */
typedef struct zen_ConstantPoolBuilder_t zen_ConstantPoolBuilder_t;

// Constructor

zen_ConstantPoolBuilder_t* zen_ConstantPoolBuilder_new();

// Destructor

void zen_ConstantPoolBuilder_delete(zen_ConstantPoolBuilder_t* builder);

#endif /* COM_ONECUBE_ZEN_COMPILER_GENERATOR_CONSTANT_POOL_BUILDER_H */