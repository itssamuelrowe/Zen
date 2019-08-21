/*
 * Copyright 2018-2019 OneCube
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