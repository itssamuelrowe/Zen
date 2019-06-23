// Tuesday, February 27, 2018

#ifndef ZEN_ST_LOCAL_SCOPE_H
#define ZEN_ST_LOCAL_SCOPE_H

#include <zen/collection/HashMap.h>
#include <zen/st/Scope.h>

/*******************************************************************************
 * LocalScope                                                                  *
 *******************************************************************************/

struct zen_LocalScope_t {
    zen_Scope_t* m_scope;
    zen_HashMap_t* m_symbols;
};

typedef struct zen_LocalScope_t zen_LocalScope_t;

zen_LocalScope_t* zen_LocalScope_new(zen_Scope_t* enclosingScope);
void zen_LocalScope_delete(zen_LocalScope_t* scope);
zen_Scope_t* zen_LocalScope_getScope(zen_LocalScope_t* scope);
void zen_LocalScope_define(zen_LocalScope_t* scope, zen_Symbol_t* symbol);
zen_Symbol_t* zen_LocalScope_resolve(zen_LocalScope_t* scope, const uint8_t* identifier);
 
#endif /* ZEN_ST_LOCAL_SCOPE_H */