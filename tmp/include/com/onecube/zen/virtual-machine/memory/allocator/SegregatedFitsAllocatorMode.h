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