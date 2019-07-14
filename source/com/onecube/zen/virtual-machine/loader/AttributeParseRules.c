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
        jtk_String_t* key = (jtk_String_t*)jtk_Iterator_getNext(keyIterator);
        jtk_String_delete(key);
    }
    jtk_Iterator_delete(keyIterator);

    jtk_HashMap_delete(rules->m_map);
    jtk_Memory_deallocate(rules);
}

// Initialize

void zen_AttributeParseRules_initialize(zen_AttributeParseRules_t* rules) {
    jtk_Assert_assertObject(rules, "The specified attribute parse rules is null.");
    
    jtk_String_t* instructionAttributeRuleKey = jtk_String_newEx(ZEN_PREDEFINED_ATTRIBUTE_INSTRUCTION, ZEN_PREDEFINED_ATTRIBUTE_INSTRUCTION_SIZE);
    jtk_HashMap_put(rules->m_map, instructionAttributeRuleKey, zen_BinaryEntityParser_parseInstructionAttribute);
}

// Rule

zen_AttributeParseRuleFunction_t zen_AttributeParseRules_getRuleEx(
    zen_AttributeParseRules_t* rules, uint8_t* name, int32_t size) {
    jtk_Assert_assertObject(rules, "The specified attribute parse rules is null.");

    jtk_String_t* key = jtk_String_newEx(name, size);
    zen_AttributeParseRuleFunction_t result = jtk_HashMap_getValue(rules->m_map, key);
    jtk_String_delete(key);

    return result;
}