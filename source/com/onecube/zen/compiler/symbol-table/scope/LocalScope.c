// Tuesday, February 27, 2018

#include <com/onecube/zen/core/StringObjectAdapter.h>
#include <com/onecube/zen/st/LocalScope.h>
#include <com/onecube/zen/Token.h>

/*******************************************************************************
 * LocalScope                                                                  *
 *******************************************************************************/

static const uint8_t zen_LocalScope_name[] = { 'l',  'o',  'c',  'a',  'l', '\0' };

zen_LocalScope_t* zen_LocalScope_new(zen_Scope_t* enclosingScope) {
    zen_LocalScope_t* localScope = zen_Memory_allocate(zen_LocalScope_t, 1);

    zen_Scope_t* scope = zen_Scope_new(zen_LocalScope_name, ZEN_SCOPE_LOCAL, enclosingScope, localScope);
    scope->m_resolveSymbol = (zen_Scope_ResolveSymbolFunction_t)zen_LocalScope_resolve;
    scope->m_defineSymbol = (zen_Scope_DefineSymbolFunction_t)zen_LocalScope_define;

    localScope->m_scope = scope;
    /* The value adapter is null because the HashMap is not required to test
     * any values.
     */
    localScope->m_symbols = zen_HashMap_new(zen_StringObjectAdapter_getInstance(), NULL);

    return localScope;
}

void zen_LocalScope_delete(zen_LocalScope_t* scope) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    zen_Scope_delete(scope->m_scope);
    zen_HashMap_delete(scope->m_symbols);
    zen_Memory_deallocate(scope);
}

zen_Scope_t* zen_LocalScope_getScope(zen_LocalScope_t* scope) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    return scope->m_scope;
}

void zen_LocalScope_define(zen_LocalScope_t* scope, zen_Symbol_t* symbol) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    /* NOTE: The key string is owned by the ASTNode. */
    const uint8_t* text = zen_Token_getText((zen_Token_t*)zen_ASTNode_getContext(zen_Symbol_getIdentifier(symbol)));
    if (!jtk_HashMap_putStrictly(scope->m_symbols, (void*)text, symbol)) {
        fprintf(stderr, "[internal error] zen_CompilationUnitScope_define invoked to redefine a symbol.");
    }
}

zen_Symbol_t* zen_LocalScope_resolve(zen_LocalScope_t* scope, const uint8_t* identifier) {
    jtk_Assert_assertObject(scope, "The specified scope is null.");

    return zen_HashMap_getValue(scope->m_symbols, (void*)identifier);
}