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

// Monday, July 15, 2019

#include <com/onecube/zen/virtual-machine/object/NativeFunction.h>

/*******************************************************************************
 * NativeFunction                                                              *
 *******************************************************************************/

// Constructor

zen_NativeFunction_t* zen_NativeFunction_new(
    zen_NativeFunction_InvokeFunction_t invoke) {
    zen_NativeFunction_t* nativeFunction = jtk_Memory_allocate(zen_NativeFunction_t, 1);
    nativeFunction->m_invoke = invoke;

    return nativeFunction;
}

// Destructor

void zen_NativeFunction_delete(zen_NativeFunction_t* function) {
    jtk_Assert_assertObject(function, "The specified native function is null.");

    jtk_Memory_deallocate(function);
}