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

// Friday, January 25, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_OBJECT_TYPE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_OBJECT_TYPE_H

/*******************************************************************************
 * ObjectType                                                                  *
 *******************************************************************************/

/**
 * @class ObjectType
 * @ingroup zen_virtualMachine_memory
 * @author Samuel Rowe
 * @since zen 1.0
 */
enum zen_ObjectType_t {
    ZEN_OBJECT_TYPE_PRIMITIVE_ARRAY = 0,
    ZEN_OBJECT_TYPE_REFERENCE_ARRAY = 1,
    ZEN_OBJECT_TYPE_AGGREGATE_OBJECT = 2,
};

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_MEMORY_OBJECT_TYPE_H */