// Saturday, November 25, 2017

#ifndef ZEN_SCOPE_H
#define ZEN_SCOPE_H

#include <com/onecube/zen/collection/HashMap.h>
#include <com/onecube/zen/st/ScopeType.h>

typedef struct zen_Symbol_t zen_Symbol_t;

typedef zen_Symbol_t* (*zen_Scope_ResolveSymbolFunction_t)(void* context, const uint8_t* identifier);
typedef zen_Symbol_t* (*zen_Scope_DefineSymbolFunction_t)(void* context, zen_Symbol_t* symbol);

/*******************************************************************************
 * Scope                                                                       *
 *******************************************************************************/

typedef struct zen_Scope_t zen_Scope_t;

struct zen_Scope_t {
    uint8_t* m_name;
    zen_ScopeType_t m_type;
    zen_Scope_t* m_enclosingScope;
    void* m_context;
    zen_Scope_ResolveSymbolFunction_t m_resolveSymbol;
    zen_Scope_DefineSymbolFunction_t m_defineSymbol;
};

zen_Scope_t* zen_Scope_new(const uint8_t* name, zen_ScopeType_t type, zen_Scope_t* enclosingScope, void* context);
void zen_Scope_delete(zen_Scope_t* scope);
void zen_Scope_define(zen_Scope_t* scope, zen_Symbol_t* symbol);
zen_Symbol_t* zen_Scope_resolve(zen_Scope_t* scope, const uint8_t* identifier);
const uint8_t* zen_Scope_getName(zen_Scope_t* scope);
bool zen_Scope_isClassScope(zen_Scope_t* scope);
bool zen_Scope_isCompilationUnitScope(zen_Scope_t* scope);
bool zen_Scope_isLocalScope(zen_Scope_t* scope);
zen_Scope_t* zen_Scope_getEnclosingScope(zen_Scope_t* scope);
zen_ScopeType_t zen_Scope_getType(zen_Scope_t* scope);

#endif /* ZEN_SCOPE_H */