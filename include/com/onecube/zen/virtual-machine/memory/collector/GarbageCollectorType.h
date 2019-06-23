// Saturday, March 16, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTOR_TYPE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTOR_TYPE_H

/*******************************************************************************
 * GarbageCollectorType                                                        *
 *******************************************************************************/

enum zen_GarbageCollectorType_t {
    ZEN_GARBAGE_COLLECTOR_TYPE_MARK_SWEEP,
    ZEN_GARBAGE_COLLECTOR_TYPE_COPYING
};

typedef enum zen_GarbageCollectorType_t zen_GarbageCollectorType_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_COLLECTOR_GARBAGE_COLLECTOR_TYPE_H */