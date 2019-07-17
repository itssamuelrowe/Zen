// Tuesday, February 27, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_LOCAL_SCOPE_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_LOCAL_SCOPE_H

#include <jtk/collection/map/HashMap.h>
#include <com/onecube/zen/compiler/symbol-table/Scope.h>

/*******************************************************************************
 * LocalScope                                                                  *
 *******************************************************************************/

/**
 * @class LocalScope
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_LocalScope_t {
    zen_Scope_t* m_scope;
    zen_HashMap_t* m_symbols;
};

/**
 * @memberof LocalScope
 */
typedef struct zen_LocalScope_t zen_LocalScope_t;

// Constructor

/**
 * @memberof LocalScope
 */
zen_LocalScope_t* zen_LocalScope_new(zen_Scope_t* enclosingScope);

// Destructor

/**
 * @memberof LocalScope
 */
void zen_LocalScope_delete(zen_LocalScope_t* scope);

// Define

/**
 * @memberof LocalScope
 */
void zen_LocalScope_define(zen_LocalScope_t* scope, zen_Symbol_t* symbol);

// Resolve

/**
 * @memberof LocalScope
 */
zen_Symbol_t* zen_LocalScope_resolve(zen_LocalScope_t* scope, const uint8_t* identifier);

// Scope

/**
 * @memberof LocalScope
 */
zen_Scope_t* zen_LocalScope_getScope(zen_LocalScope_t* scope);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_LOCAL_SCOPE_H */