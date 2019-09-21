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

// Sunday, July 14, 2019

#include <jtk/collection/Iterator.h>
#include <jtk/core/StringObjectAdapter.h>

#include <com/onecube/zen/virtual-machine/loader/AttributeParseRules.h>
#include <com/onecube/zen/virtual-machine/loader/BinaryEntityParser.h>
#include <com/onecube/zen/virtual-machine/feb/attribute/PredefinedAttribute.h>

/*******************************************************************************
 * AttributeParseRules                                                         *
 *******************************************************************************/

// Constructor

zen_AttributeParseRules_t* zen_AttributeParseRules_new() {
    jtk_ObjectAdapter_t* stringObjectAdapter = jtk_StringObjectAdapter_getInstance();
    
    zen_AttributeParseRules_t* rules = jtk_Memory_allocate(zen_AttributeParseRules_t, 1);
    rules->m_map = jtk_HashMap_newEx(stringObjectAdapter, NULL,
        JTK_HASH_MAP_DEFAULT_CAPACITY, JTK_HASH_MAP_DEFAULT_LOAD_FACTOR);

    /* The constructor does not initialize the list of known attribute parse
     * rules. This allows us to register all the rules in a function with
     * a specific purpose.
     */
    zen_AttributeParseRules_initialize(rules);

    return rules;
}

// Destructor

void zen_AttributeParseRules_delete(zen_AttributeParseRules_t* rules) {
    jtk_Assert_assertObject(rules, "The specified attribute parse rules is null.");

    // Delete keys!
    jtk_Iterator_t* keyIterator = jtk_HashMap_getKeyIterator(rules->m_map);
    while (jtk_Iterator_hasNext(keyIterator)) {
        jtk_CString_t* key = (jtk_CString_t*)jtk_Iterator_getNext(keyIterator);
        jtk_CString_delete(key);
    }
    jtk_Iterator_delete(keyIterator);

    jtk_HashMap_delete(rules->m_map);
    jtk_Memory_deallocate(rules);
}

// Initialize

void zen_AttributeParseRules_initialize(zen_AttributeParseRules_t* rules) {
    jtk_Assert_assertObject(rules, "The specified attribute parse rules is null.");
    
    jtk_CString_t* instructionAttributeRuleKey = jtk_CString_newEx(ZEN_PREDEFINED_ATTRIBUTE_INSTRUCTION, ZEN_PREDEFINED_ATTRIBUTE_INSTRUCTION_SIZE);
    jtk_HashMap_put(rules->m_map, instructionAttributeRuleKey, zen_BinaryEntityParser_parseInstructionAttribute);
}

// Rule

zen_AttributeParseRuleFunction_t zen_AttributeParseRules_getRuleEx(
    zen_AttributeParseRules_t* rules, uint8_t* name, int32_t size) {
    jtk_Assert_assertObject(rules, "The specified attribute parse rules is null.");

    jtk_CString_t* key = jtk_CString_newEx(name, size);
    zen_AttributeParseRuleFunction_t result = jtk_HashMap_getValue(rules->m_map, key);
    jtk_CString_delete(key);

    return result;
}