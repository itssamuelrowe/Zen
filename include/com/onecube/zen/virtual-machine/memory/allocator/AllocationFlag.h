// Wednesday, January 23, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_ALLOCATION_FLAG_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_ALLOCATION_FLAG_H

/*******************************************************************************
 * AllocationFlag                                                              *
 *******************************************************************************/

enum zen_AllocationFlag_t {
    ZEN_ALLOCATION_FLAG_MANUAL = (1 << 0),
    ZEN_ALLOCATION_FLAG_AUTOMATIC = (1 << 1),
    ZEN_ALLOCATION_FLAG_IMMORTAL = (1 << 2)
};

typedef enum zen_AllocationFlag_t zen_AllocationFlag_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_ALLOCATION_FLAG_H */