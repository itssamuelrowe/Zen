// Friday, March 20, 2018

#ifndef ZEN_BEG_BINARY_ENTITY_GENERATOR_H
#define ZEN_BEG_BINARY_ENTITY_GENERATOR_H

#include <com/onecube/zen/Configuration.h>
#include <zen/feb/ConstantPool.h>

/*******************************************************************************
 * BinaryEntityGenerator                                                       *
 *******************************************************************************/

struct zen_BinaryEntityGenerator_t {
    zen_ConstantPool_t* m_constantPool;
    void* m_output;
};

typedef struct zen_BinaryEntityGenerator_t zen_BinaryEntityGenerator_t;

zen_BinaryEntityGenerator_t* zen_BinaryEntityGenerator_new();
void zen_BinaryEntityGenerator_delete(zen_BinaryEntityGenerator_t* generator);
void zen_BinaryEntityGenerator_generate(zen_BinaryEntityGenerator_t* generator);

#endif /* ZEN_BEG_BINARY_ENTITY_GENERATOR_H */