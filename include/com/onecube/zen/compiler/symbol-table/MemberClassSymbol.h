// Monday, January 08, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_CLASS_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_CLASS_SYMBOL_H

#include <com/onecube/zen/compiler/symbol-table/Symbol.h>

/*******************************************************************************
 * MemberClassSymbol                                                           *
 *******************************************************************************/

/**
 * @class MemberClassSymbol
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_MemberClassSymbol_t {
    zen_Symbol_t* m_symbol;
};

/**
 * @memberof MemberClassSymbol
 */
typedef struct zen_MemberClassSymbol_t zen_MemberClassSymbol_t;

// Constructor

/**
 * @memberof MemberClassSymbol
 */
zen_MemberClassSymbol_t* zen_MemberClassSymbol_new(zen_ASTNode_t* identifier);

// Destructor

/**
 * @memberof MemberClassSymbol
 */
void zen_MemberClassSymbol_delete(zen_MemberClassSymbol_t* symbol);

// Symbol

/**
 * @memberof MemberClassSymbol
 */
zen_Symbol_t* zen_MemberClassSymbol_getSymbol(zen_MemberClassSymbol_t* symbol);
 
#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_CLASS_SYMBOL_H */