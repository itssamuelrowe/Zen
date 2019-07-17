// Sunday, January 07, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_CONSTANT_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_CONSTANT_SYMBOL_H

#include <com/onecube/zen/compiler/symbol-table/Symbol.h>

/*******************************************************************************
 * MemberConstantSymbol                                                     *
 *******************************************************************************/

/**
 * @class MemberConstantSymbol
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_MemberConstantSymbol_t {
    zen_Symbol_t* m_symbol;
    jtk_ArrayList_t* m_modifiers; /* <zen_ASTNode_t*> */
};

/**
 * @memberof MemberConstantSymbol
 */
typedef struct zen_MemberConstantSymbol_t zen_MemberConstantSymbol_t;

// Constructor

/**
 * @memberof MemberConstantSymbol
 */
zen_MemberConstantSymbol_t* zen_MemberConstantSymbol_new(zen_ASTNode_t* identifier,
    jtk_ArrayList_t* modifiers);

// Destructor

/**
 * @memberof MemberConstantSymbol
 */
void zen_MemberConstantSymbol_delete(zen_MemberConstantSymbol_t* symbol);

// Symbol

/**
 * @memberof MemberConstantSymbol
 */
zen_Symbol_t* zen_MemberConstantSymbol_getSymbol(zen_MemberConstantSymbol_t* symbol);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_CONSTANT_SYMBOL_H */