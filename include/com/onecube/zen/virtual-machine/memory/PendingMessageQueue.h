// Monday, September 03, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_PENDING_MESSAGE_QUEUE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_PENDING_MESSAGE_QUEUE_H

/*******************************************************************************
 * PendingMessageQueue                                                         *
 *******************************************************************************/

/**
 * Insertion of messages into the message queue of the garbage collector
 * should be carefully done.
 *
 * The garbage collector pauses all the threads when collecting. Chances
 * are a mutator thread may hold the lock of the message queue.
 * This may cause a deadlock.
 *
 * A solution to this problem is the *pending message queue*. Messages and
 * their corresponding handlers are inserted in a special queue. These
 * messages are flushed into the target message queue once all the mutator
 * threads are resumed.
 
 * @class PendingMessageQueue
 * @ingroup zen_mms
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_PendingMessageQueue_t {

    zen_SinglyLinkedList_t* m_messages;
};

/**
 * @memberof PendingMessageQueue
 */
typedef struct zen_PendingMessageQueue_t zen_PendingMessageQueue_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_PENDING_MESSAGE_QUEUE_H */