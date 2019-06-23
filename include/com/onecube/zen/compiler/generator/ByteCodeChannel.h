// Saturday, April 28, 2018

#ifndef ZEN_BCG_BYTE_CODE_CHANNEL_H
#define ZEN_BCG_BYTE_CODE_CHANNEL_H

#include <com/onecube/zen/Configuration.h>
#include <zen/collection/ArrayList.h>

/*******************************************************************************
 * ByteCodeChannel                                                             *
 *******************************************************************************/

struct zen_ByteCodeChannel_t {
    uint8_t* m_bytes;
    int32_t m_size;
    int32_t m_capacity;
};

typedef struct zen_ByteCodeChannel_t zen_ByteCodeChannel_t;

zen_ByteCodeChannel_t* zen_ByteCodeChannel_new();
void zen_ByteCodeChannel_delete(zen_ByteCodeChannel_t* channel);
int32_t zen_ByteCodeChannel_getSize(zen_ByteCodeChannel_t* channel);
void zen_ByteCodeChannel_appendChannel(zen_ByteCodeChannel_t* channel, zen_ByteCodeChannel_t* other);
void zen_ByteCodeChannel_appendByte(zen_ByteCodeChannel_t* channel, uint8_t byte);
void zen_ByteCodeChannel_appendBytes(zen_ByteCodeChannel_t* channel, uint8_t* bytes, int32_t size);
void zen_ByteCodeChannel_appendBytesRange(zen_ByteCodeChannel_t* channel, uint8_t* bytes, int32_t size, int32_t startIndex, int32_t stopIndex);

#endif /* ZEN_BCG_BYTE_CODE_CHANNEL_H */