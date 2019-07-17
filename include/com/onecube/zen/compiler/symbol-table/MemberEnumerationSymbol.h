// Monday, January 08, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_ENUMERATION_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_ENUMERATION_SYMBOL_H

#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/symbol-table/Symbol.h>

/*******************************************************************************
 * MemberEnumerationSymbol                                                     *
 *******************************************************************************/

/**
 * @class MemberEnumerationSymbol
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_MemberEnumerationSymbol_t {
    zen_Symbol_t* m_symbol;
};

/**
 * @memberof MemberEnumerationSymbol
 */
typedef struct zen_MemberEnumerationSymbol_t zen_MemberEnumerationSymbol_t;

// Constructor

/**
 * @memberof MemberEnumerationSymbol
 */
zen_MemberEnumerationSymbol_t* zen_MemberEnumerationSymbol_new();

// Destructor

/**
 * @memberof MemberEnumerationSymbol
 */
void zen_MemberEnumerationSymbol_delete(zen_MemberEnumerationSymbol_t* symbol);

// Symbol

/**
 * @memberof MemberEnumerationSymbol
 */
zen_Symbol_t* zen_MemberEnumerationSymbol_getSymbol(zen_MemberEnumerationSymbol_t* symbol);
 
#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_ENUMERATION_SYMBOL_H */