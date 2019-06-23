#include <zen/bcg/ConstantPoolBuilder.h>

/*******************************************************************************
 * ConstantPoolBuilder                                                         *
 *******************************************************************************/

zen_ConstantPoolBuilder_t* zen_ConstantPoolBuilder_new() {
    zen_ConstantPoolBuilder_t* builder = zen_Memory_allocate(zen_ConstantPoolBuilder_t, 1);
    builder->m_constants = zen_ArrayList_new();
    builder->m_constantPool = NULL;

    return builder;
}

void zen_ConstantPoolBuilder_delete(zen_ConstantPoolBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");

    zen_ArrayList_delete(builder->m_constants);
    if (builder->m_constantPool != NULL) {
        zen_ConstantPool_delete(builder->m_constantPool);
    }
    zen_Memory_deallocate(builder);
}

