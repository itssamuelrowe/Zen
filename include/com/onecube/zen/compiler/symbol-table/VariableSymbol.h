// Sunday, January 07, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_VARIABLE_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_VARIABLE_SYMBOL_H

#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/symbol-table/Symbol.h>

/*******************************************************************************
 * VariableSymbol                                                              *
 *******************************************************************************/

/**
 * @class ClassScope
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_VariableSymbol_t {
    zen_Symbol_t* m_symbol;
    jtk_ArrayList_t* m_modifiers;
    int32_t m_implicitModifiers;
};

/**
 * @memberof AnnotationSymbol
 */
typedef struct zen_VariableSymbol_t zen_VariableSymbol_t;

// Constructor

/**
 * @memberof AnnotationSymbol
 */
zen_VariableSymbol_t* zen_VariableSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope);

// Destructor

/**
 * @memberof AnnotationSymbol
 */
void zen_VariableSymbol_delete(zen_VariableSymbol_t* symbol);

// Symbol

/**
 * @memberof AnnotationSymbol
 */
zen_Symbol_t* zen_VariableSymbol_getSymbol(zen_VariableSymbol_t* symbol);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_VARIABLE_SYMBOL_H */