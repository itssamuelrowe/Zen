// Monday, January 08, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_CLASS_SCOPE_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_CLASS_SCOPE_H

#include <jtk/collection/map/HashMap.h>
#include <com/onecube/zen/compiler/symbol-table/Scope.h>

// Forward reference
typedef struct zen_ClassSymbol_t zen_ClassSymbol_t;

/*******************************************************************************
 * ClassScope                                                                  *
 *******************************************************************************/

/**
 * @class ClassScope
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ClassScope_t {
    zen_Scope_t* m_scope;
    zen_HashMap_t* m_symbols;
    zen_Symbol_t* m_classSymbol;
};

/**
 * @memberof ClassScope
 */
typedef struct zen_ClassScope_t zen_ClassScope_t;

// Constructor

/**
 * @memberof ClassScope
 */
zen_ClassScope_t* zen_ClassScope_new(zen_Scope_t* enclosingScope);

// Destructor

/**
 * @memberof ClassScope
 */
void zen_ClassScope_delete(zen_ClassScope_t* classScope);

// Class Symbol

/**
 * @memberof ClassScope
 */
zen_Symbol_t* zen_ClassScope_getClassSymbol(zen_ClassScope_t* classScope);

// Define

/**
 * @memberof ClassScope
 */
void zen_ClassScope_define(zen_ClassScope_t* scope, zen_Symbol_t* symbol);

// Resolve

/**
 * @memberof ClassScope
 */
zen_Symbol_t* zen_ClassScope_resolve(zen_ClassScope_t* scope, const uint8_t* identifier);

// Scope

/**
 * @memberof ClassScope
 */
zen_Scope_t* zen_ClassScope_getScope(zen_ClassScope_t* scope);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_CLASS_SCOPE_H */