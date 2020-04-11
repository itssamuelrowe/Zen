/*
 * Copyright 2017-2020 Samuel Rowe
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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