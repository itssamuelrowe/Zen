// Saturday, February 24, 2018

#include <zen/st/ClassSymbol.h>

/*******************************************************************************
 * ClassSymbol                                                                 *
 *******************************************************************************/

zen_ClassSymbol_t* zen_ClassSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope, zen_Scope_t* classScope) {
    zen_ClassSymbol_t* classSymbol = zen_Memory_allocate(zen_ClassSymbol_t, 1);

    zen_Symbol_t* symbol = zen_Symbol_new(ZEN_SYMBOL_CATEGORY_CLASS, identifier, enclosingScope, classSymbol);

    classSymbol->m_symbol = symbol;
    classSymbol->m_superClasses = zen_ArrayList_new();
    classSymbol->m_explicitModifiers = zen_ArrayList_new();
    classSymbol->m_modifiers = 0;
    classSymbol->m_classScope = classScope;

    return classSymbol;
}

void zen_ClassSymbol_delete(zen_ClassSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    zen_Symbol_delete(symbol->m_symbol);
    zen_ArrayList_delete(symbol->m_superClasses);
    zen_ArrayList_delete(symbol->m_explicitModifiers);
    zen_Memory_deallocate(symbol);
}

zen_Scope_t* zen_ClassSymbol_getClassScope(zen_ClassSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    return symbol->m_classScope;
}

zen_ArrayList_t* zen_ClassSymbol_getSuperClasses(zen_ClassSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    return symbol->m_superClasses;
}

zen_Symbol_t* zen_ClassSymbol_getSymbol(zen_ClassSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    return symbol->m_symbol;
}

void zen_ClassSymbol_addModifier(zen_ClassSymbol_t* symbol, zen_Modifier_t modifier,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");
    
    if (node != NULL) {
        zen_ArrayList_add(symbol->m_explicitModifiers, node);
    }
    symbol->m_modifiers |= (int32_t)modifier;
}

bool zen_ClassSymbol_hasModifier(zen_ClassSymbol_t* symbol, zen_Modifier_t modifier) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");
    
    return (symbol->m_modifiers & (int32_t)modifier) != 0;
}