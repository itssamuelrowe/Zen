// Thursday, March 08, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_ENUMERATION_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_ENUMERATION_SYMBOL_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/compiler/symbol-table/Symbol.h>
#include <com/onecube/zen/compiler/symbol-table/Modifier.h>
#include <com/onecube/zen/compiler/symbol-table/ClassSymbol.h>

/*******************************************************************************
 * EnumerationSymbol                                                           *
 *******************************************************************************/

/**
 * @class EnumerationSymbol
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EnumerationSymbol_t {
    zen_Symbol_t* m_symbol;

    /* The superclass attribute is updated during the resolution phase. */
    zen_ClassSymbol_t* m_superclass;

    jtk_ArrayList_t* m_explicitModifiers;

    int32_t m_modifiers;

    zen_Scope_t* m_enumerationScope;
};

/**
 * @memberof EnumerationSymbol
 */
typedef struct zen_EnumerationSymbol_t zen_EnumerationSymbol_t;

// Constructor

/**
 * @memberof EnumerationSymbol
 */
zen_EnumerationSymbol_t* zen_EnumerationSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope, zen_Scope_t* m_enumerationScope);

// Destructor

/**
 * @memberof EnumerationSymbol
 */
void zen_EnumerationSymbol_delete(zen_EnumerationSymbol_t* symbol);

// Modifier

/**
 * @memberof EnumerationSymbol
 */
void zen_EnumerationSymbol_addModifier(zen_EnumerationSymbol_t* symbol,
    zen_Modifier_t modifier, zen_ASTNode_t* node);

/**
 * @memberof EnumerationSymbol
 */
bool zen_EnumerationSymbol_hasModifier(zen_EnumerationSymbol_t* symbol,
    zen_Modifier_t modifier);

// Scope

/**
 * @memberof EnumerationSymbol
 */
zen_Scope_t* zen_EnumerationSymbol_getEnumerationScope(zen_EnumerationSymbol_t* symbol);

// Superclass

/**
 * @memberof EnumerationSymbol
 */
zen_ClassSymbol_t* zen_EnumerationSymbol_getSuperclass(zen_EnumerationSymbol_t* symbol);

// Symbol

/**
 * @memberof EnumerationSymbol
 */
zen_Symbol_t* zen_EnumerationSymbol_getSymbol(zen_EnumerationSymbol_t* symbol);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_ENUMERATION_SYMBOL_H */