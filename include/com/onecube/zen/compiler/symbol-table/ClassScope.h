// Monday, January 08, 2018

#ifndef ZEN_ST_CLASS_SCOPE_H
#define ZEN_ST_CLASS_SCOPE_H

#include <zen/collection/HashMap.h>
#include <zen/st/Scope.h>

typedef struct zen_ClassSymbol_t zen_ClassSymbol_t;

/*******************************************************************************
 * ClassScope                                                                  *
 *******************************************************************************/

struct zen_ClassScope_t {
    zen_Scope_t* m_scope;
    zen_HashMap_t* m_symbols;
    zen_Symbol_t* m_classSymbol;
};

typedef struct zen_ClassScope_t zen_ClassScope_t;

zen_ClassScope_t* zen_ClassScope_new(zen_Scope_t* enclosingScope);
void zen_ClassScope_delete(zen_ClassScope_t* classScope);
zen_Scope_t* zen_ClassScope_getScope(zen_ClassScope_t* scope);
zen_Symbol_t* zen_ClassScope_getClassSymbol(zen_ClassScope_t* classScope);
void zen_ClassScope_define(zen_ClassScope_t* scope, zen_Symbol_t* symbol);
zen_Symbol_t* zen_ClassScope_resolve(zen_ClassScope_t* scope, const uint8_t* identifier);

#endif /* ZEN_ST_CLASS_SCOPE_H */