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

// Tuesday, October 02, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_ATTRIBUTE_PARSE_RULES_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_ATTRIBUTE_PARSE_RULES_H

/* Happy Gandhi Jayanthi!
 * Happy birthday, Girish!
 */

#include <com/onecube/zen/Configuration.h>
#include <jtk/collection/map/HashMap.h>

/* Forward Reference */
typedef struct zen_BinaryEntityParser_t zen_BinaryEntityParser_t;
 
/*******************************************************************************
 * AttributeParseRules                                                         *
 *******************************************************************************/

typedef void* (*zen_AttributeParseRuleFunction_t)(zen_BinaryEntityParser_t* parser, uint16_t nameIndex, uint32_t length);

/**
 * @class AttributeParseRules
 * @ingroup zen_vm_loader
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AttributeParseRules_t {
    jtk_HashMap_t* m_map;
};

/**
 * @memberof AttributeParseRules
 */
typedef struct zen_AttributeParseRules_t zen_AttributeParseRules_t;

// Constructor

zen_AttributeParseRules_t* zen_AttributeParseRules_new();

// Destructor

void zen_AttributeParseRules_delete(zen_AttributeParseRules_t* rules);

// Rule

zen_AttributeParseRuleFunction_t zen_AttributeParseRules_getRuleEx(
    zen_AttributeParseRules_t* rules, uint8_t* name, int32_t size);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_ATTRIBUTE_PARSE_RULES_H */