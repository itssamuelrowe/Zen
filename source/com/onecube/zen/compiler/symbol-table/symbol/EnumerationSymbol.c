// Thursday, March 08, 2018

#include <com/onecube/zen/st/EnumerationSymbol.h>

/*******************************************************************************
 * EnumerationSymbol                                                                 *
 *******************************************************************************/

zen_EnumerationSymbol_t* zen_EnumerationSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope, zen_Scope_t* enumerationScope) {
    zen_EnumerationSymbol_t* enumerationSymbol = zen_Memory_allocate(zen_EnumerationSymbol_t, 1);

    zen_Symbol_t* symbol = zen_Symbol_new(ZEN_SYMBOL_CATEGORY_ENUMERATION, identifier, enclosingScope, enumerationSymbol);

    enumerationSymbol->m_symbol = symbol;
    enumerationSymbol->m_superclass = NULL;
    enumerationSymbol->m_explicitModifiers = zen_ArrayList_new();
    enumerationSymbol->m_modifiers = 0;
    enumerationSymbol->m_enumerationScope = enumerationScope;

    return enumerationSymbol;
}

void zen_EnumerationSymbol_delete(zen_EnumerationSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    zen_Symbol_delete(symbol->m_symbol);
    zen_ArrayList_delete(symbol->m_explicitModifiers);
    zen_Memory_deallocate(symbol);
}

zen_Scope_t* zen_EnumerationSymbol_getEnumerationScope(zen_EnumerationSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    return symbol->m_enumerationScope;
}

zen_ClassSymbol_t* zen_EnumerationSymbol_getSuperclass(zen_EnumerationSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    return symbol->m_superclass;
}

zen_Symbol_t* zen_EnumerationSymbol_getSymbol(zen_EnumerationSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    return symbol->m_symbol;
}

void zen_EnumerationSymbol_addModifier(zen_EnumerationSymbol_t* symbol, zen_Modifier_t modifier,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    if (node != NULL) {
        zen_ArrayList_add(symbol->m_explicitModifiers, node);
    }
    symbol->m_modifiers |= (int32_t)modifier;
}

bool zen_EnumerationSymbol_hasModifier(zen_EnumerationSymbol_t* symbol, zen_Modifier_t modifier) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    return (symbol->m_modifiers & (int32_t)modifier) != 0;
}