// Thursday, July 11, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_PARENT_FUNCTION_ATTRIBUTE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_PARENT_FUNCTION_ATTRIBUTE_H

/*******************************************************************************
 * ParentFunctionAttribute                                                     *
 *******************************************************************************/

/**
 * The parent function attribute is a secondary attribute, with fixed length.
 * It represents the function enclosing another function.
 *
 * @class ParentFunctionAttribute
 * @ingroup zen_virtualMachine_feb_attribute
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ParentFunctionAttribute_t {
    
    /**
     * A valid index into the constant pool. The entry at the specified index
     * should be a `ConstantPoolFunction` instance.
     */
    uint16_t m_functionIndex;
};

typedef struct zen_ParentFunctionAttribute_t zen_ParentFunctionAttribute_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_PARENT_FUNCTION_ATTRIBUTE_H */