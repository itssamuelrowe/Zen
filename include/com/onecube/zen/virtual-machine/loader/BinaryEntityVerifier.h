/*
 * Copyright 2018-2020 Samuel Rowe
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

// Monday, June 11, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_BINARY_ENTITY_VERIFIER_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_BINARY_ENTITY_VERIFIER_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * BinaryEntityVerifier                                                        *
 *******************************************************************************/

struct zen_BinaryEntityVerifier_t {
};

typedef struct zen_BinaryEntityVerifier_t zen_BinaryEntityVerifier_t;

/* Constructor */

zen_BinaryEntityVerifier_t* zen_BinaryEntityVerifier_new();

/* Destructor */

void zen_BinaryEntityVerifier_delete(zen_BinaryEntityVerifier_t* verifier);
 
#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_BINARY_ENTITY_VERIFIER_H */