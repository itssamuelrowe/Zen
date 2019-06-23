// Friday, March 09, 2018

#ifndef ZEN_ST_ENUMERATION_SCOPE_H
#define ZEN_ST_ENUMERATION_SCOPE_H

#include <zen/collection/HashMap.h>
#include <zen/st/Scope.h>

/*******************************************************************************
 * EnumerationScope                                                            *
 *******************************************************************************/

struct zen_EnumerationScope_t {
    zen_Scope_t* m_scope;
#warning "TODO: Use OrderedHashMap here!"
    zen_HashMap_t* m_enumerates;
    zen_Symbol_t* m_enumerationSymbol;
};

typedef struct zen_EnumerationScope_t zen_EnumerationScope_t;

zen_EnumerationScope_t* zen_EnumerationScope_new(zen_Scope_t* enclosingScope);
void zen_EnumerationScope_delete(zen_EnumerationScope_t* classScope);
zen_Scope_t* zen_EnumerationScope_getScope(zen_EnumerationScope_t* scope);
zen_Symbol_t* zen_EnumerationScope_getEnumerationSymbol(zen_EnumerationScope_t* classScope);
void zen_EnumerationScope_define(zen_EnumerationScope_t* scope, zen_Symbol_t* symbol);
zen_Symbol_t* zen_EnumerationScope_resolve(zen_EnumerationScope_t* scope, const uint8_t* identifier);

#endif /* ZEN_ST_ENUMERATION_SCOPE_H */