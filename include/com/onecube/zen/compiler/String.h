// Saturday, November 04, 2017

#ifndef ZEN_STRING_H
#define ZEN_STRING_H

#include <com/onecube/zen/Configuration.h>

uint8_t* zen_String_new(const uint8_t* string);
uint8_t* zen_String_newWithSize(const uint8_t* string, int32_t size);
void zen_String_delete(uint8_t* string);
bool zen_String_equals(const uint8_t* string1, int32_t size1, const uint8_t* string2, int32_t size2);
int32_t zen_String_getLength(const uint8_t* string);
int32_t zen_String_hash(const uint8_t* string);

#endif /* ZEN_STRING_H */