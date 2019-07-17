// Sunday, March 04, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MODIFIER_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MODIFIER_H

/*******************************************************************************
 * Modifier                                                                    *
 *******************************************************************************/

/**
 * @class Modifier
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
enum zen_Modifier_t {
    ZEN_MODIFIER_VARIABLE_PARAMETER = 1 << 0
};

/**
 * @memberof Modifier
 */
typedef enum zen_Modifier_t zen_Modifier_t;

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MODIFIER_H */