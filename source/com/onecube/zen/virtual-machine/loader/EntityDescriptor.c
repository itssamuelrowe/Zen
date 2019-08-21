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

// Saturday, May 11, 2019

#include <com/onecube/virtual-machine/loader/EntityDescriptor.h>

// Constructor

zen_EntityDescriptor_t* zen_EntityDescriptor_new0() {
    zen_EntityDescriptor_t* descriptor = jtk_Memory_allocate(zen_EntityDescriptor_t, 1);
        
    return descriptor;
}

zen_EntityDescriptor_t* zen_EntityDescriptor_new(jtk_CString_t* value) {
    jtk_Assert_assertObject(value, "The specified descriptor is null.");
    
    jtk_EntityDescriptor_t* descriptor = jtk_EntityDescriptor_new0();
    descriptor->m_value = jtk_CString_clone(value);
}

zen_EntityDescriptor_t* zen_EntityDescriptor_newEx(const uint8_t* value, int32_t size) {
    jtk_Assert_assertObject(value, "The specified descriptor is null.");
    
    jtk_EntityDescriptor_t* descriptor = jtk_EntityDescriptor_new0();
    descriptor->m_value = jtk_CString_newEx(value, size);
}

// Destructor

void zen_EntityDescriptor_delete(zen_EntityDescriptor_t* descriptor) {
    jtk_Assert_assertObject(descriptor, "The specified descriptor is null.");
    
    jtk_CString_delete(descriptor->m_value);
    jtk_Memory_deallocate(descriptor);
}