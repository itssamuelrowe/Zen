// Monday, March 05, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_LABEL_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_LABEL_SYMBOL_H

#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/st/Symbol.h>

/*******************************************************************************
 * LabelSymbol                                                                 *
 *******************************************************************************/

/**
 * @class LabelSymbol
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_LabelSymbol_t {
    zen_Symbol_t* m_symbol;
};

/**
 * @memberof LabelSymbol
 */
typedef struct zen_LabelSymbol_t zen_LabelSymbol_t;

// Constructor

/**
 * @memberof LabelSymbol
 */
zen_LabelSymbol_t* zen_LabelSymbol_new(zen_ASTNode_t* identifier, zen_Scope_t* enclosingScope);

// Destructor

/**
 * @memberof LabelSymbol
 */
void zen_LabelSymbol_delete(zen_LabelSymbol_t* symbol);

// Symbol

/**
 * @memberof LabelSymbol
 */
zen_Symbol_t* zen_LabelSymbol_getSymbol(zen_LabelSymbol_t* symbol);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_LABEL_SYMBOL_H */