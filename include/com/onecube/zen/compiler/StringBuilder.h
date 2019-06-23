// Saturday, November 04, 2017

#ifndef ZEN_STRING_BUILDER_H
#define ZEN_STRING_BUILDER_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * StringBuilder                                                               *
 *******************************************************************************/

struct zen_StringBuilder_t {
    uint8_t* m_value;
    int32_t m_capacity;
    int32_t m_size;
};

typedef struct zen_StringBuilder_t zen_StringBuilder_t;

zen_StringBuilder_t* zen_StringBuilder_new();
zen_StringBuilder_t* zen_StringBuilder_newWithCapacity(int32_t capacity);
void zen_StringBuilder_delete(zen_StringBuilder_t* builder);
void zen_StringBuilder_appendCodePoint(zen_StringBuilder_t* builder, int32_t codePoint);
uint8_t* zen_StringBuilder_toString(zen_StringBuilder_t* builder);
int32_t zen_StringBuilder_getSize(zen_StringBuilder_t* builder);
void zen_StringBuilder_setCapacity(zen_StringBuilder_t* builder, int32_t capacity);
void zen_StringBuilder_clear(zen_StringBuilder_t* builder);

#endif /* ZEN_STRING_BUILDER_H */