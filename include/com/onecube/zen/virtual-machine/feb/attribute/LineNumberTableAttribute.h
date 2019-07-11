// Thursday, July 11, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_LINE_NUMBER_TABLE_ATTRIBUTE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_LINE_NUMBER_TABLE_ATTRIBUTE_H

/*******************************************************************************
 * LineNumberTableAttribute                                                    *
 *******************************************************************************/

/**
 * The line number table attribute is a secondary attribute, with a variable
 * length. It is designed to accommodate external tools such as debuggers.
 * It can be associated with classes, enumerations, fields, and functions.
 *
 * @class LineNumberTableAttribute
 * @ingroup zen_virtualMachine_feb_attribute
 * @author Samuel Rowe
 * @since zen 1.0
*/ 
struct zen_LineNumberTableAttribute_t {
    uint16_t m_size;
    uint16_t* m_startIndexes;
    uint16_t* m_lineNumbers;
};

/**
 * @memberof LineNumberTableAttribute
 */
typedef struct zen_LineNumberTableAttribute_t zen_LineNumberTableAttribute_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_LINE_NUMBER_TABLE_ATTRIBUTE_H */