// Tuesday, February 27, 2018

#include <com/onecube/zen/st/VariableSymbol.h>

/*******************************************************************************
 * VariableSymbol                                                              *
 *******************************************************************************/

zen_VariableSymbol_t* zen_VariableSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope) {
    zen_VariableSymbol_t* VariableSymbol = zen_Memory_allocate(zen_VariableSymbol_t, 1);

    zen_Symbol_t* symbol = zen_Symbol_new(ZEN_SYMBOL_CATEGORY_VARIABLE, identifier, enclosingScope, VariableSymbol);

    VariableSymbol->m_symbol = symbol;
    VariableSymbol->m_modifiers = zen_ArrayList_new();
    VariableSymbol->m_implicitModifiers = 0;

    return VariableSymbol;
}

void zen_VariableSymbol_delete(zen_VariableSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    zen_Symbol_delete(symbol->m_symbol);
    zen_ArrayList_delete(symbol->m_modifiers);
    zen_Memory_deallocate(symbol);
}

zen_Symbol_t* zen_VariableSymbol_getSymbol(zen_VariableSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    return symbol->m_symbol;
}