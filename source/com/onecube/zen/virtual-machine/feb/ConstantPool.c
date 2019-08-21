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

// Tuesday, March 26, 2019

#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPool.h>

/*******************************************************************************
 * ConstantPool                                                                *
 *******************************************************************************/
 
zen_ConstantPoolUtf8_t* zen_ConstantPool_resolveUtf8(zen_ConstantPool_t* constantPool,
    uint16_t index) {
    return constantPool->m_entries[index];
}

void* zen_ConstantPool_getEntry(zen_ConstantPool_t* constantPool, int32_t index) {
    return NULL;
}

int32_t zen_ConstantPoolEntry_getTag(void* x) {
    return -1;
}


#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolInteger.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolLong.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolFloat.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolDouble.h>

int32_t zen_ConstantPoolInteger_getValue(zen_ConstantPoolInteger_t* entry) {
    return entry->m_bytes;
}

int64_t zen_ConstantPoolLong_getValue(zen_ConstantPoolLong_t* entry) {
    return ((entry->m_highBytes << 32L) | entry->m_lowBytes);
}

float zen_ConstantPoolFloat_getValue(zen_ConstantPoolFloat_t* entry) {
    return jtk_Float_pack(entry->m_bytes);
}

double zen_ConstantPoolDouble_getValue(zen_ConstantPoolDouble_t* entry) {
    return jtk_Double_pack((entry->m_highBytes << 32L) | entry->m_lowBytes);
}