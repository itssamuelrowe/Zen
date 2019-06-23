// Saturday, September 22, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_AUXILLARY_ATTRIBUTE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_AUXILLARY_ATTRIBUTE_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * AuxillaryAttribute                                                          *
 *******************************************************************************/

/**
 * @class AuxillaryAttribute
 * @ingroup zen_vm_feb
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AuxillaryAttribute_t {
    uint16_t m_operandStackSize;
    uint16_t m_localVariableArraySize;
};

/**
 * @memberof AuxillaryAttribute
 */
typedef struct zen_AuxillaryAttribute_t zen_AuxillaryAttribute_t;
    
#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_AUXILLARY_ATTRIBUTE_H */