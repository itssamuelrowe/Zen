// Friday, July 12, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_FUNCTION_PARAMETERS_ATTRIBUTE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_FUNCTION_PARAMETERS_ATTRIBUTE_H

/*******************************************************************************
 * FunctionParametersAttribute                                                 *
 *******************************************************************************/

/**
 * @class FunctionParametersAttribute
 * @ingroup zen_virtualMachine_feb_attribute
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_FunctionParametersAttribute_t {
    uint8_t m_size;
    zen_FunctionParameter_t** m_parameters;
};

/**
 * @memberof FunctionParametersAttribute
 */
typedef struct zen_FunctionParametersAttribute_t zen_FunctionParametersAttribute_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_FUNCTION_PARAMETERS_ATTRIBUTE_H */