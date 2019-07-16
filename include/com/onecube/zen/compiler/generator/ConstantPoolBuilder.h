// Tuesday, April 03, 2018

#ifndef ZEN_BCG_CONSTANT_POOL_BUILDER_H
#define ZEN_BCG_CONSTANT_POOL_BUILDER_H

#include <com/onecube/zen/collection/ArrayList.h>
#include <com/onecube/zen/feb/ConstantPool.h>

/*******************************************************************************
 * ConstantPoolBuilder                                                         *
 *******************************************************************************/

struct zen_ConstantPoolBuilder_t {
    jtk_ArrayList_t* m_constants;
    zen_ConstantPool_t* m_constantPool;
};

typedef struct zen_ConstantPoolBuilder_t zen_ConstantPoolBuilder_t;

zen_ConstantPoolBuilder_t* zen_ConstantPoolBuilder_new();
void zen_ConstantPoolBuilder_delete(zen_ConstantPoolBuilder_t* builder);
void zen_ConstantPoolBuilder_addConstant(zen_ConstantPoolBuilder_t* builder, zen_ConstantPoolEntry_t* entry);
void zen_ConstantPoolBuilder_hasConstant(zen_ConstantPoolBuilder_t* builder, uint8_t* tag, void* body);
void zen_ConstantPoolBuilder_build(zen_ConstantPoolBuilder_t* builder);
zen_ConstantPool_t* zen_ConstantPoolBuilder_getConstantPool(zen_ConstantPoolBuilder_t* builder);

#endif /* ZEN_BCG_CONSTANT_POOL_BUILDER_H */