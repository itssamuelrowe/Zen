// Saturday, April 28, 2018

#include <com/onecube/zen/compiler/generator/DataChannel.h>

/*******************************************************************************
 * DataChannel                                                                 *
 *******************************************************************************/

#define ZEN_BYTE_CODE_CHANNEL_DEFAULT_CAPACITY 16

// Constructor
 
zen_DataChannel_t* zen_DataChannel_new(int32_t identifier) {
    zen_DataChannel_t* channel = zen_Memory_allocate(zen_DataChannel_t, 1);
    channel->m_bytes = zen_Memory_allocate(uint8_t, ZEN_BYTE_CODE_CHANNEL_DEFAULT_CAPACITY);
    channel->m_capacity = ZEN_BYTE_CODE_CHANNEL_DEFAULT_CAPACITY;
    channel->m_size = 0;
    channel->m_identifier = identifier;

    return channel;
}

// Destructor

void zen_DataChannel_delete(zen_DataChannel_t* channel) {
    jtk_Assert_assertObject(channel, "The specified byte code channel is null.");

    zen_Memory_deallocate(channel->m_bytes);
    zen_Memory_deallocate(channel);
}

// Append

void zen_DataChannel_appendChannel(zen_DataChannel_t* channel,
    zen_DataChannel_t* other) {
    jtk_Assert_assertObject(channel, "The specified byte code channel is null.");

    zen_DataChannel_requestCapacity(channel, channel->m_size + other->m_size);
    int32_t i;
    for (i = 0; i < other->m_size; i++) {
        channel->m_bytes[channel->m_size + i] = other->m_bytes[i];
    }
    channel->m_size += other->m_size;
}

void zen_DataChannel_appendByte(zen_DataChannel_t* channel, uint8_t byte) {
    jtk_Assert_assertObject(channel, "The specified byte code channel is null.");

    zen_DataChannel_requestCapacity(channel, channel->m_size + 1);
    channel->m_bytes[channel->m_size] = byte;
    channel->m_size++;
}

void zen_DataChannel_appendBytes(zen_DataChannel_t* channel,
    uint8_t* bytes, int32_t size) {
    jtk_Assert_assertObject(channel, "The specified byte code channel is null.");

    zen_DataChannel_appendBytesRange(channel, bytes, size, 0, size);
}

void zen_DataChannel_appendBytesRange(zen_DataChannel_t* channel,
    uint8_t* bytes, int32_t size, int32_t startIndex, int32_t stopIndex) {
    jtk_Assert_assertObject(channel, "The specified byte code channel is null.");
    // jtk_Assert_assertTrue(...);

    int32_t byteCount = stopIndex - startIndex;
    zen_DataChannel_requestCapacity(channel, channel->m_size + byteCount);
    int32_t i;
    int32_t j;
    for (i = channel->m_size, j = startIndex; j < stopIndex; i++, j++) {
        channel->m_bytes[i] = bytes[j];
    }
    channel->m_size += byteCount;
}

// Capacity

void zen_DataChannel_requestCapacity(zen_DataChannel_t* channel, int32_t capacity) {
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

// Size

int32_t zen_DataChannel_getSize(zen_DataChannel_t* channel) {
    jtk_Assert_assertObject(channel, "The specified byte code channel is null.");
    return channel->m_size;
}