// Sunday, January 07, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_VARIABLE_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_VARIABLE_SYMBOL_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/symbol-table/Symbol.h>

/*******************************************************************************
 * MemberVariableSymbol                                                        *
 *******************************************************************************/

/**
 * @class MemberVariableSymbol
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_MemberVariableSymbol_t {
    zen_Symbol_t* m_symbol;
    jtk_ArrayList_t* m_modifiers; /* <zen_ASTNode_t*> */
};

/**
 * @memberof MemberVariableSymbol
 */
typedef struct zen_MemberVariableSymbol_t zen_MemberVariableSymbol_t;

// Constructor

/**
 * @memberof MemberVariableSymbol
 */
zen_MemberVariableSymbol_t* zen_AttributeVariableSymbol_new(zen_ASTNode_t* identifier,
    jtk_ArrayList_t* m_modifiers);

// Destructor

/**
 * @memberof MemberVariableSymbol
 */
void zen_AttributeVariableSymbol_delete(zen_MemberVariableSymbol_t* symbol);

// Symbol

/**
 * @memberof MemberVariableSymbol
 */
zen_Symbol_t* zen_MemberVariableSymbol_getSymbol(zen_MemberVariableSymbol_t* symbol);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_MEMBER_VARIABLE_SYMBOL_H */