// Saturday, January 26, 2019 (Happy Republic Day, India!)

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_ALIGNMENT_CONSTRAINT_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_ALIGNMENT_CONSTRAINT_H

/*******************************************************************************
 * AlignmentConstraint                                                         *
 *******************************************************************************/

enum zen_AlignmentConstraint_t {
    ZEN_ALIGNMENT_CONSTRAINT_DEFAULT,
    ZEN_ALIGNMENT_CONSTRAINT_16_BITS,
    ZEN_ALIGNMENT_CONSTRAINT_32_BITS,
    ZEN_ALIGNMENT_CONSTRAINT_64_BITS,
    ZEN_ALIGNMENT_CONSTRAINT_128_BITS
};

typedef enum zen_AlignmentConstraint_t zen_AlignmentConstraint_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_ALIGNMENT_CONSTRAINT_H */