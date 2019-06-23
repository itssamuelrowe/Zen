// Saturday, November 25, 2017

#ifndef ZEN_ST_SYMBOL_CATEGORY_H
#define ZEN_ST_SYMBOL_CATEGORY_H

/*******************************************************************************
 * SymbolType                                                                  *
 *******************************************************************************/

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

typedef enum zen_SymbolCategory_t zen_SymbolCategory_t;

#endif /* ZEN_ST_SYMBOL_CATEGORY_H */