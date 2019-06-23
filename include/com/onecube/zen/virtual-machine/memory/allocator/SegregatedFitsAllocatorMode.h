// Sunday, March 17, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_SEGREGATED_FITS_ALLOCATOR_MODE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_SEGREGATED_FITS_ALLOCATOR_MODE_H

/*******************************************************************************
 * SegregatedFitsAllocatorMode                                                 *
 *******************************************************************************/

/**
 * @class SegregatedFitsAllocatorMode
 * @ingroup zen_virtualMachine_memory_allocator
 * @author Samuel Rowe
 * @since zen 1.0
 */
enum zen_SegregatedFitsAllocatorMode_t {
    ZEN_SEGREGATED_FITS_ALLOCATOR_MODE_NEXT_FIT,
    ZEN_SEGREGATED_FITS_ALLOCATOR_MODE_BEST_FIT,
    ZEN_SEGREGATED_FITS_ALLOCATOR_MODE_FIRST_FIT
};

/**
 * @memberof SegregatedFitsAllocatorMode
 */
typedef enum zen_SegregatedFitsAllocatorMode_t zen_SegregatedFitsAllocatorMode_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_ALLOCATOR_SEGREGATED_FITS_ALLOCATOR_MODE_H */