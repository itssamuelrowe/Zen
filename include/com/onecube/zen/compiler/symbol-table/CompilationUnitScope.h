// Sunday, February 25, 2018

#ifndef ZEN_ST_COMPILATION_UNIT_SCOPE_H
#define ZEN_ST_COMPILATION_UNIT_SCOPE_H

#include <zen/st/Scope.h>
#include <zen/st/Symbol.h>
#include <zen/collection/HashMap.h>

/*******************************************************************************
 * CompilationUnitScope                                                        *
 *******************************************************************************/

struct zen_CompilationUnitScope_t {
    zen_Scope_t* m_scope;
    zen_HashMap_t* m_symbols;
};

typedef struct zen_CompilationUnitScope_t zen_CompilationUnitScope_t;

zen_CompilationUnitScope_t* zen_CompilationUnitScope_new();
void zen_CompilationUnitScope_delete(zen_CompilationUnitScope_t* scope);
zen_Scope_t* zen_CompilationUnitScope_getScope(zen_CompilationUnitScope_t* scope);
void zen_CompilationUnitScope_define(zen_CompilationUnitScope_t* scope, zen_Symbol_t* symbol);
zen_Symbol_t* zen_CompilationUnitScope_resolve(zen_CompilationUnitScope_t* scope, const uint8_t* identifier);

#endif /* ZEN_ST_COMPILATION_UNIT_SCOPE_H */