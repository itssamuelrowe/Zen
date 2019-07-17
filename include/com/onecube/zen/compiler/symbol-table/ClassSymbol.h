// Sunday, January 07, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_CLASS_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_CLASS_SYMBOL_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/compiler/symbol-table/Symbol.h>
#include <com/onecube/zen/compiler/symbol-table/Modifier.h>

// Forward reference
typedef struct zen_ClassScope_t zen_ClassScope_t;

/*******************************************************************************
 * ClassSymbol                                                                 *
 *******************************************************************************/

/**
 * @class ClassSymbol
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ClassSymbol_t {
    zen_Symbol_t* m_symbol;
    
    /* The superclasses are added in the resolution phase. */
    jtk_ArrayList_t* m_superClasses; /* <zen_ClassSymbol_t*> */
    
    jtk_ArrayList_t* m_explicitModifiers;
    
    int32_t m_modifiers;
    
    zen_Scope_t* m_classScope;
};

/**
 * @memberof ClassSymbol
 */
typedef struct zen_ClassSymbol_t zen_ClassSymbol_t;

// Constructor

/**
 * @memberof ClassSymbol
 */
zen_ClassSymbol_t* zen_ClassSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope, zen_Scope_t* m_classScope);

// Destructor

/**
 * @memberof ClassSymbol
 */
void zen_ClassSymbol_delete(zen_ClassSymbol_t* symbol);

// Class Scope

/**
 * @memberof ClassSymbol
 */
zen_Scope_t* zen_ClassSymbol_getClassScope(zen_ClassSymbol_t* symbol);

// Modifier

/**
 * @memberof ClassSymbol
 */
void zen_ClassSymbol_addModifier(zen_ClassSymbol_t* symbol, zen_Modifier_t modifier,
    zen_ASTNode_t* node);

/**
 * @memberof ClassSymbol
 */
bool zen_ClassSymbol_hasModifier(zen_ClassSymbol_t* symbol, zen_Modifier_t modifier);

// Superclasses

/**
 * @memberof ClassSymbol
 */
jtk_ArrayList_t* zen_ClassSymbol_getSuperclasses(zen_ClassSymbol_t* symbol);

// Symbol

/**
 * @memberof ClassSymbol
 */
zen_Symbol_t* zen_ClassSymbol_getSymbol(zen_ClassSymbol_t* symbol);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_CLASS_SYMBOL_H */