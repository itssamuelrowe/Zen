// Tuesday, October 02, 2018

/* Happy Gandhi Jayanthi!
 * Happy birthday, Girish!
 */

#include <com/onecube/zen/Configuration.h>

/* Forward Reference */
typedef struct zen_BinaryEntityParser_t zen_BinaryEntityParser_t;
 
/*******************************************************************************
 * AttributeParseRuleMap                                                       *
 *******************************************************************************/

typedef void* (*zen_AttributeParseRuleFunction_t)(zen_BinaryEntityParser_t* parser, uint16_t nameIndex, uint32_t length);

/**
 * @class AttributeParseRuleMap
 * @ingroup zen_vm_loader
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AttributeParseRuleMap_t {
};

/**
 * @memberof AttributeParseRuleMap
 */
typedef struct zen_AttributeParseRuleMap_t zen_AttributeParseRuleMap_t;