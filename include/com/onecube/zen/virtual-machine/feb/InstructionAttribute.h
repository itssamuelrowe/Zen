// Saturday, April 28, 2018

#ifndef ZEN_FEB_INSTRUCTION_ATTRIBUTE_H
#define ZEN_FEB_INSTRUCTION_ATTRIBUTE_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * InstructionAttribute                                                        *
 *******************************************************************************/

/**
 * @class InstructionAttribute
 * @ingroup zen_vm_feb
 * @author Samuel Rowe
 * @since zen 1.0 
 */
struct zen_InstructionAttribute_t {
    uint8_t* m_instructions;
};

/**
 * @memberof InstructionAttribute
 */
typedef struct zen_InstructionAttribute_t zen_InstructionAttribute_t;

#endif /* ZEN_FEB_INSTRUCTION_ATTRIBUTE_H */