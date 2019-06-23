// Tuesday, February 27, 2018

#include <zen/st/ConstantSymbol.h>

/*******************************************************************************
 * ConstantSymbol                                                              *
 *******************************************************************************/

zen_ConstantSymbol_t* zen_ConstantSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope) {
    zen_ConstantSymbol_t* constantSymbol = zen_Memory_allocate(zen_ConstantSymbol_t, 1);

    zen_Symbol_t* symbol = zen_Symbol_new(ZEN_SYMBOL_CATEGORY_CONSTANT, identifier, enclosingScope, constantSymbol);

    constantSymbol->m_symbol = symbol;
    constantSymbol->m_explicitModifiers = zen_ArrayList_new();
    constantSymbol->m_modifiers = 0;

    return constantSymbol;
}

void zen_ConstantSymbol_delete(zen_ConstantSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    zen_Symbol_delete(symbol->m_symbol);
    zen_ArrayList_delete(symbol->m_explicitModifiers);
    zen_Memory_deallocate(symbol);
}

zen_Symbol_t* zen_ConstantSymbol_getSymbol(zen_ConstantSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    return symbol->m_symbol;
}

void zen_ConstantSymbol_addModifier(zen_ConstantSymbol_t* symbol, zen_Modifier_t modifier, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");
    
    if (node != NULL) {
        zen_ArrayList_add(symbol->m_explicitModifiers, node);
    }
    symbol->m_modifiers |= (int32_t)modifier;
}

bool zen_ConstantSymbol_hasModifier(zen_ConstantSymbol_t* symbol, zen_Modifier_t modifier) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");
    
    return (symbol->m_modifiers & (int32_t)modifier) != 0;
}
