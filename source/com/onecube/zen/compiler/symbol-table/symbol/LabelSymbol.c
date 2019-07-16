// Monday, March 05, 2018

#include <com/onecube/zen/st/LabelSymbol.h>

/*******************************************************************************
 * LabelSymbol                                                                 *
 *******************************************************************************/

zen_LabelSymbol_t* zen_LabelSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope) {
    zen_LabelSymbol_t* labelSymbol = zen_Memory_allocate(zen_LabelSymbol_t, 1);

    zen_Symbol_t* symbol = zen_Symbol_new(ZEN_SYMBOL_CATEGORY_LABEL, identifier, enclosingScope, labelSymbol);

    labelSymbol->m_symbol = symbol;

    return labelSymbol;
}

void zen_LabelSymbol_delete(zen_LabelSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    zen_Symbol_delete(symbol->m_symbol);
    zen_Memory_deallocate(symbol);
}

zen_Symbol_t* zen_LabelSymbol_getSymbol(zen_LabelSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    return symbol->m_symbol;
}
