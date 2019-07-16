// Friday, March 09, 2018

#include <com/onecube/zen/st/EnumerateSymbol.h>

/*******************************************************************************
 * EnumerateSymbol                                                             *
 *******************************************************************************/

zen_EnumerateSymbol_t* zen_EnumerateSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope) {
    zen_EnumerateSymbol_t* enumerateSymbol = zen_Memory_allocate(zen_EnumerateSymbol_t, 1);

    zen_Symbol_t* symbol = zen_Symbol_new(ZEN_SYMBOL_CATEGORY_ENUMERATE, identifier, enclosingScope, enumerateSymbol);

    enumerateSymbol->m_symbol = symbol;

    return enumerateSymbol;
}

void zen_EnumerateSymbol_delete(zen_EnumerateSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    zen_Symbol_delete(symbol->m_symbol);
    zen_Memory_deallocate(symbol);
}

zen_Symbol_t* zen_EnumerateSymbol_getSymbol(zen_EnumerateSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    return symbol->m_symbol;
}