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