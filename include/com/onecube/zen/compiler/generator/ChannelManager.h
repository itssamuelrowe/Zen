// Saturday, April 28, 2018

#ifndef ZEN_BCG_CHANNEL_MANAGER_H
#define ZEN_BCG_CHANNEL_MANAGER_H

#include <com/onecube/zen/Configuration.h>
#include <zen/collection/DoublyLinkedList.h>
#include <zen/bcg/ByteCodeChannel.h>
#include <zen/collection/ArrayList.h>

/*******************************************************************************
 * ChannelManager                                                      *
 *******************************************************************************/

struct zen_ChannelManager_t {
    zen_ArrayList_t* m_stack;
};

typedef struct zen_ChannelManager_t zen_ChannelManager_t;

zen_ChannelManager_t* zen_ChannelManager_new();
void zen_ChannelManager_delete(zen_ChannelManager_t* manager);
void zen_ChannelManager_add(zen_ChannelManager_t* manager);
void zen_ChannelManager_remove(zen_ChannelManager_t* manager, int32_t index);
zen_ByteCodeChannel_t* zen_ChannelManager_get(zen_ChannelManager_t* manager, int32_t index);
zen_ByteCodeChannel_t* zen_ChannelManager_peek(zen_ChannelManager_t* manager);
void zen_ChannelManager_merge(zen_ChannelManager_t* manager);
void zen_ChannelManager_append(zen_ChannelManager_t* manager, int32_t sourceIndex, int32_t destinationIndex);

#endif /* ZEN_BCG_CHANNEL_MANAGER_H */