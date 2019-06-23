// Saturday, April 28, 2018

#ifndef ZEN_FEB_CODE_ATTRIBUTE_H
#define ZEN_FEB_CODE_ATTRIBUTE_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * CodeAttribute                                                               *
 *******************************************************************************/

/**
 * @class CodeAttribute
 * @ingroup zen_vm_feb
 * @author Samuel Rowe
 * @since zen 1.0 
 */
struct zen_CodeAttribute_t {
    uint8_t* m_instructions;
};

/**
 * @memberof CodeAttribute
 */
typedef struct zen_CodeAttribute_t zen_CodeAttribute_t;

#endif /* ZEN_FEB_CODE_ATTRIBUTE_H */