// Saturday, April 28, 2018

#include <zen/bcg/ByteCodeChannel.h>

/*******************************************************************************
 * ByteCodeChannel                                                             *
 *******************************************************************************/

#define ZEN_BYTE_CODE_CHANNEL_DEFAULT_CAPACITY 16

void zen_ByteCodeChannel_setCapacity(zen_ByteCodeChannel_t* channel,
    int32_t capacity) {
    if (capacity > 0) {
        int32_t currentCapacity = channel->m_capacity;
        int32_t requireCapacity = capacity - currentCapacity;
        if (requireCapacity > 0) {
            int32_t newCapacity = (currentCapacity * 2) + 2;
            if ((newCapacity - capacity) < 0) {
                newCapacity = capacity;
            }
            if (newCapacity < 0) {
                jtk_Assert_assertFalse(capacity < 0, "An int32_t overflow occurred, the requested capacity is too big.");
                newCapacity = 0x7FFFFFFF;
            }
            uint8_t* temporary = channel->m_bytes;
            channel->m_bytes = zen_Memory_allocate(uint8_t, newCapacity);
            int32_t i;
            for (i = 0; i < channel->m_size; i++) {
                channel->m_bytes[i] = temporary[i];
            }
            zen_Memory_deallocate(temporary);
            channel->m_capacity = newCapacity;
        }
    }
}
 
zen_ByteCodeChannel_t* zen_ByteCodeChannel_new() {
    zen_ByteCodeChannel_t* channel = zen_Memory_allocate(zen_ByteCodeChannel_t, 1);
    channel->m_bytes = zen_Memory_allocate(uint8_t, ZEN_BYTE_CODE_CHANNEL_DEFAULT_CAPACITY);
    channel->m_capacity = ZEN_BYTE_CODE_CHANNEL_DEFAULT_CAPACITY;
    channel->m_size = 0;

    return channel;
}

void zen_ByteCodeChannel_delete(zen_ByteCodeChannel_t* channel) {
    jtk_Assert_assertObject(channel, "The specified byte code channel is null.");

    zen_Memory_deallocate(channel->m_bytes);
    zen_Memory_deallocate(channel);
}

int32_t zen_ByteCodeChannel_getSize(zen_ByteCodeChannel_t* channel) {
    jtk_Assert_assertObject(channel, "The specified byte code channel is null.");
    return channel->m_size;
}

void zen_ByteCodeChannel_appendChannel(zen_ByteCodeChannel_t* channel,
    zen_ByteCodeChannel_t* other) {
    jtk_Assert_assertObject(channel, "The specified byte code channel is null.");

    zen_ByteCodeChannel_setCapacity(channel, channel->m_size + other->m_size);
    int32_t i;
    for (i = 0; i < other->m_size; i++) {
        channel->m_bytes[channel->m_size + i] = other->m_bytes[i];
    }
    channel->m_size += other->m_size;
}

void zen_ByteCodeChannel_appendByte(zen_ByteCodeChannel_t* channel, uint8_t byte) {
    jtk_Assert_assertObject(channel, "The specified byte code channel is null.");

    zen_ByteCodeChannel_setCapacity(channel, channel->m_size + 1);
    channel->m_bytes[channel->m_size] = byte;
    channel->m_size++;
}

void zen_ByteCodeChannel_appendBytes(zen_ByteCodeChannel_t* channel,
    uint8_t* bytes, int32_t size) {
    jtk_Assert_assertObject(channel, "The specified byte code channel is null.");

    zen_ByteCodeChannel_appendBytesRange(channel, bytes, size, 0, size);
}

void zen_ByteCodeChannel_appendBytesRange(zen_ByteCodeChannel_t* channel,
    uint8_t* bytes, int32_t size, int32_t startIndex, int32_t stopIndex) {
    jtk_Assert_assertObject(channel, "The specified byte code channel is null.");
    // jtk_Assert_assertTrue(...);

    int32_t byteCount = stopIndex - startIndex;
    zen_ByteCodeChannel_setCapacity(channel, channel->m_size + byteCount);
    int32_t i;
    int32_t j;
    for (i = channel->m_size, j = startIndex; j < stopIndex; i++, j++) {
        channel->m_bytes[i] = bytes[j];
    }
    channel->m_size += byteCount;
}