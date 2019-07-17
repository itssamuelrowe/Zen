// Sunday, January 07, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_CONSTANT_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_CONSTANT_SYMBOL_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/symbol-table/Symbol.h>
#include <com/onecube/zen/compiler/symbol-table/Modifier.h>

/*******************************************************************************
 * ConstantSymbol                                                              *
 *******************************************************************************/

/**
 * @class ConstantSymbol
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ConstantSymbol_t {
    zen_Symbol_t* m_symbol;
    jtk_ArrayList_t* m_explicitModifiers; /* Useful for reporting errors. */
    int32_t m_modifiers;
};

/**
 * @memberof ConstantSymbol
 */
typedef struct zen_ConstantSymbol_t zen_ConstantSymbol_t;

// Constructor

/**
 * @memberof ConstantSymbol
 */
zen_ConstantSymbol_t* zen_ConstantSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope);

// Destructor

/**
 * @memberof ConstantSymbol
 */
void zen_ConstantSymbol_delete(zen_ConstantSymbol_t* symbol);

// Modifier

/**
 * @memberof ConstantSymbol
 */
void zen_ConstantSymbol_addModifier(zen_ConstantSymbol_t* symbol,
    zen_Modifier_t modifier, zen_ASTNode_t* node);

/**
 * @memberof ConstantSymbol
 */
bool zen_ConstantSymbol_hasModifier(zen_ConstantSymbol_t* symbol,
    zen_Modifier_t modifier);

// Symbol

/**
 * @memberof ConstantSymbol
 */
zen_Symbol_t* zen_ConstantSymbol_getSymbol(zen_ConstantSymbol_t* symbol);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_CONSTANT_SYMBOL_H */