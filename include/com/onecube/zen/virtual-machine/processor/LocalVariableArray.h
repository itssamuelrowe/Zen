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

// Monday, September 17, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_LOCAL_VARIABLE_ARRAY_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_LOCAL_VARIABLE_ARRAY_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/memory/MemoryManager.h>

/*******************************************************************************
 * LocalVariableArray                                                          *
 *******************************************************************************/

/**
 * @class LocalVariableArray
 * @ingroup zen_vm_processor
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_LocalVariableArray_t {
    int32_t* m_values;
    int32_t m_size;
};

/**
 * @memberof LocalVariableArray
 */
typedef struct zen_LocalVariableArray_t zen_LocalVariableArray_t;

/* Constructor */

/**
 * @memberof LocalVariableArray
 */
zen_LocalVariableArray_t* zen_LocalVariableArray_new(int32_t size);

/* Destructor */

/**
 * @memberof LocalVariableArray
 */
void zen_LocalVariableArray_delete(zen_LocalVariableArray_t* array);

/* Integer Value */

/**
 * @memberof LocalVariableArray
 */
void zen_LocalVariableArray_setInteger(zen_LocalVariableArray_t* array, int32_t index, int32_t value);

/**
 * @memberof LocalVariableArray
 */
int32_t zen_LocalVariableArray_getInteger(zen_LocalVariableArray_t* array, int32_t index);

/* Long Value */

/**
 * @memberof LocalVariableArray
 */
void zen_LocalVariableArray_setLong(zen_LocalVariableArray_t* array, int32_t index, int64_t value);

/**
 * @memberof LocalVariableArray
 */
int64_t zen_LocalVariableArray_getLong(zen_LocalVariableArray_t* array, int32_t index);

/* Float Value */

/**
 * @memberof LocalVariableArray
 */
void zen_LocalVariableArray_setFloat(zen_LocalVariableArray_t* array, int32_t index, float value);

/**
 * @memberof LocalVariableArray
 */
float zen_LocalVariableArray_getFloat(zen_LocalVariableArray_t* array, int32_t index);

/* Double Value */

/**
 * @memberof LocalVariableArray
 */
void zen_LocalVariableArray_setDouble(zen_LocalVariableArray_t* array, int32_t index, double value);

/**
 * @memberof LocalVariableArray
 */
double zen_LocalVariableArray_getDouble(zen_LocalVariableArray_t* array, int32_t index);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_LOCAL_VARIABLE_ARRAY_H */