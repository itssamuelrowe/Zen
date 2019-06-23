// Saturday, April 28, 2018

#ifndef ZEN_BCG_BYTE_CODE_CHANNEL_STACK_H
#define ZEN_BCG_BYTE_CODE_CHANNEL_STACK_H

#include <zen/bcg/ByteCodeChannel.h>

/*******************************************************************************
 * ByteCodeChannelStack                                                        *
 *******************************************************************************/

struct zen_ByteCodeChannelStack_t {
};

typedef struct zen_ByteCodeChannelStack_t zen_ByteCodeChannelStack_t;

void zen_ByteCodeChannelStack_push(zen_ByteCodeChannelStack_t* stack, zen_ByteCodeChannel_t* channel);

#endif /* ZEN_BCG_BYTE_CODE_CHANNEL_STACK_H */