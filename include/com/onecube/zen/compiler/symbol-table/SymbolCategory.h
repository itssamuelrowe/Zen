// Saturday, November 25, 2017

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_CATEGORY_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_CATEGORY_H

/*******************************************************************************
 * SymbolCategory                                                              *
 *******************************************************************************/

/**
 * @class SymbolCategory
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
enum zen_SymbolCategory_t {
    ZEN_SYMBOL_CATEGORY_ANNOTATION,
    ZEN_SYMBOL_CATEGORY_CLASS,
    ZEN_SYMBOL_CATEGORY_ENUMERATE,
    ZEN_SYMBOL_CATEGORY_ENUMERATION,
    ZEN_SYMBOL_CATEGORY_FUNCTION,
    ZEN_SYMBOL_CATEGORY_CONSTANT,
    ZEN_SYMBOL_CATEGORY_VARIABLE,
    // ZEN_SYMBOL_CATEGORY_CONSTRUCTOR,
    ZEN_SYMBOL_CATEGORY_LABEL
};

/**
 * @memberof SymbolCategory
 */
typedef enum zen_SymbolCategory_t zen_SymbolCategory_t;

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_CATEGORY_H */