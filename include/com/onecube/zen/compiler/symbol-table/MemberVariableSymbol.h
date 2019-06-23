// Sunday, January 07, 2018

#ifndef ZEN_ST_MEMBER_VARIABLE_SYMBOL_H
#define ZEN_ST_MEMBER_VARIABLE_SYMBOL_H

#include <zen/ASTNode.h>
#include <zen/st/Symbol.h>

/*******************************************************************************
 * MemberVariableSymbol                                                        *
 *******************************************************************************/

struct zen_MemberVariableSymbol_t {
    zen_Symbol_t* m_symbol;
    zen_ArrayList_t* m_modifiers; /* <zen_ASTNode_t*> */
};

typedef struct zen_MemberVariableSymbol_t zen_MemberVariableSymbol_t;

zen_MemberVariableSymbol_t* zen_AttributeVariableSymbol_new(zen_ASTNode_t* identifier,
    zen_ArrayList_t* m_modifiers);
void zen_AttributeVariableSymbol_delete(zen_MemberVariableSymbol_t* symbol);
zen_Symbol_t* zen_MemberVariableSymbol_getSymbol(zen_MemberVariableSymbol_t* symbol);

#endif /* ZEN_ST_MEMBER_VARIABLE_SYMBOL_H */