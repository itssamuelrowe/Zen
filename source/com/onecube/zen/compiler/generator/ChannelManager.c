// Saturday, April 28, 2018

#include <zen/bcg/ChannelManager.h>

zen_ChannelManager_t* zen_ChannelManager_new() {
    zen_ChannelManager_t* manager = zen_Memory_allocate(zen_ChannelManager_t, 1);
    manager->m_stack = zen_ArrayList_new();

    return manager;
}

void zen_ChannelManager_delete(zen_ChannelManager_t* manager) {
    jtk_Assert_assertObject(manager, "The specified channel manager is null.");

    int32_t size = zen_ArrayList_getSize(manager->m_stack);
    if (size != 0) {
        fprintf(stderr, "[internal error] An non-empty channel manager is is being destroyed. This may indicate unflushed byte codes.");

        /* TODO: Use an iterator for increasing efficiency. */
        int32_t i;
        for (i = 0; i < size; i++) {
            zen_ByteCodeChannel_t* channel = (zen_ByteCodeChannel_t*)zen_ArrayList_get(manager->m_stack, i);
            zen_ByteCodeChannel_delete(channel);
        }
    }
    zen_ArrayList_delete(manager->m_stack);
    zen_Memory_deallocate(manager);
}

void zen_ChannelManager_add(zen_ChannelManager_t* manager) {
    jtk_Assert_assertObject(manager, "The specified channel manager is null.");

    zen_ByteCodeChannel_t* channel = zen_ByteCodeChannel_new();
    zen_ArrayList_addFirst(manager->m_stack, channel);
}

void zen_ChannelManager_remove(zen_ChannelManager_t* manager, int32_t index) {
    jtk_Assert_assertObject(manager, "The specified channel manager is null.");

    zen_ByteCodeChannel_t* channel = (zen_ByteCodeChannel_t*)zen_ArrayList_get(manager->m_stack, index);
    zen_ArrayList_removeIndex(manager->m_stack, index);
    zen_ByteCodeChannel_delete(channel); // THIS IS A BUG!!!! CRITICAL!!
}

zen_ByteCodeChannel_t* zen_ChannelManager_get(zen_ChannelManager_t* manager, int32_t index) {
    jtk_Assert_assertObject(manager, "The specified channel manager is null.");

    return zen_ArrayList_get(manager->m_stack, index);
}

zen_ByteCodeChannel_t* zen_ChannelManager_peek(zen_ChannelManager_t* manager) {
    jtk_Assert_assertObject(manager, "The specified channel manager is null.");

    return zen_ArrayList_get(manager->m_stack, 0);
}

void zen_ChannelManager_merge(zen_ChannelManager_t* manager) {
    jtk_Assert_assertObject(manager, "The specified channel manager is null.");

    zen_ChannelManager_append(manager, 0, 1);
    zen_ChannelManager_remove(manager, 0);
}

void zen_ChannelManager_append(zen_ChannelManager_t* manager, int32_t sourceIndex,
    int32_t destinationIndex) {
    jtk_Assert_assertObject(manager, "The specified channel manager is null.");

    zen_ByteCodeChannel_t* sourceChannel = zen_ArrayList_get(manager->m_stack, sourceIndex);
    zen_ByteCodeChannel_t* destinationChannel = zen_ArrayList_get(manager->m_stack, destinationIndex);

    zen_ByteCodeChannel_appendChannel(destinationChannel, sourceChannel);
}

void zen_ChannelManager_swap(zen_ChannelManager_t* manager, int32_t xIndex,
    int32_t yIndex) {
    jtk_Assert_assertObject(manager, "The specified channel manager is null.");

    zen_ByteCodeChannel_t* xChannel = (zen_ByteCodeChannel_t*)zen_ArrayList_get(manager->m_stack, xIndex);
    zen_ByteCodeChannel_t* yChannel = (zen_ByteCodeChannel_t*)zen_ArrayList_get(manager->m_stack, yIndex);
    zen_ArrayList_set(manager->m_stack, xIndex, yChannel);
    zen_ArrayList_set(manager->m_stack, yIndex, xChannel);
}