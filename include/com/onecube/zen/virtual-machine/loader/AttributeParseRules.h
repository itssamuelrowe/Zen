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