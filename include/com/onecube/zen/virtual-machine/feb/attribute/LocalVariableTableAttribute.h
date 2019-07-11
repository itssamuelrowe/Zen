// Thursday, July 11, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_LOCAL_VARIABLE_TABLE_ATTRIBUTE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_LOCAL_VARIABLE_TABLE_ATTRIBUTE_H

/*******************************************************************************
 * LocalVariableTableAttribute                                                 *
 *******************************************************************************/

/**
 * The local variable table attribute is a secondary attribute, with variable
 * length. It is designed to accommodate external tools such as debuggers.
 *
 * @class LocalVariableTableAttribute
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_LocalVariableTableAttribute_t {
    uint16_t m_size;
    zen_LocalVariable_t** m_localVariables;
};

/**
 * @memberof LocalVariableTableAttribute
 */
typedef struct zen_LocalVariableTableAttribute_t zen_LocalVariableTableAttribute_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_LOCAL_VARIABLE_TABLE_ATTRIBUTE_H */