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

// Monday, October 07, 2019

/*******************************************************************************
 * ConstantPoolBuilder                                                         *
 *******************************************************************************/

// Constructor

zen_ConstantPoolBuilder_t* zen_ConstantPoolBuilder_new() {
    zen_ConstantPoolBuilder_t* builder = zen_Memory_allocate(zen_ConstantPoolBuilder_t, 1);
    builder->m_entries = jtk_ArrayList_new();

    return builder;
}

// Destructor

void zen_ConstantPoolBuilder_delete(zen_ConstantPoolBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");

    jtk_ArrayList_delete(builder->m_entries);
    zen_Memory_deallocate(builder);
}

// Entries

int32_t zen_ConstantPoolBuilder_countEntries(zen_ConstantPoolBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");
    
    return jtk_ArrayList_getSize(builder->m_entries);
}

zen_ConstantPoolEntry_t* zen_ConstantPoolBuilder_getEntry(zen_ConstantPoolBuilder_t* builder,
    int32_t index) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");
    
    return jtk_ArrayList_getValue(builder->m_entries, index);
}