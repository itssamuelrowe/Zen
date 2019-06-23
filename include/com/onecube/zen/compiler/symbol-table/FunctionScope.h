// Monday, January 08, 2018

#ifndef ZEN_ST_FUNCTION_SCOPE_H
#define ZEN_ST_FUNCTION_SCOPE_H

#include <zen/collection/ArrayList.h>
#include <zen/st/ConstantSymbol.h>
#include <zen/st/Scope.h>

/*******************************************************************************
 * FunctionScope                                                               *
 *******************************************************************************/

struct zen_FunctionScope_t {
    zen_Scope_t* m_scope;
    /* TODO: Use an OrderedMap here. */
    zen_ArrayList_t* m_fixedParameters; /* <zen_Symbol*> */
    zen_Symbol_t* m_variableParameter;
};

typedef struct zen_FunctionScope_t zen_FunctionScope_t;

zen_FunctionScope_t* zen_FunctionScope_new(zen_Scope_t* enclosingScope);
void zen_FunctionScope_delete(zen_FunctionScope_t* scope);
zen_Scope_t* zen_FunctionScope_getScope(zen_FunctionScope_t* scope);
void zen_FunctionScope_define(zen_FunctionScope_t* scope, zen_Symbol_t* symbol);
zen_Symbol_t* zen_FunctionScope_resolve(zen_FunctionScope_t* scope, const uint8_t* identifier);

#endif /* ZEN_ST_FUNCTION_SCOPE_H */