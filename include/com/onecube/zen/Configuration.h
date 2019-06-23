// Wedensday, October 18, 2017

#ifndef ZEN_CONFIGURATION_H
#define ZEN_CONFIGURATION_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define zen_Memory_allocate(type, units) (type*)zen_Memory_allocate0(sizeof (type) * (units))
#define zen_Memory_deallocate(object) free(object)
#define zen_Assert_assertObject(object, message) assert((object) != NULL)
#define zen_Assert_assertTrue(expression, message) assert(expression)
#define zen_Assert_assertFalse(expression, message) assert(!(expression))
#define zen_Assert_assertIndex(index, size, message) assert(((index) < (size)) && ((index) >= 0))
#define zen_Assert_assertRange(startIndex, stopIndex, size, message) puts("TODO: zen_Assert_assertRange");
#define zen_Assert_assertAddIndex(index, size, message) assert(((index) <= (size)) && ((index) >= 0))

void* zen_Memory_allocate0(int32_t size);

#endif /* ZEN_CONFIGURATION_H */