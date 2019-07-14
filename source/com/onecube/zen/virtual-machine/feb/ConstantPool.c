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