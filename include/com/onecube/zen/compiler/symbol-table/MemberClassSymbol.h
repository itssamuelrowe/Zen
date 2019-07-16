// Monday, January 08, 2018

#ifndef ZEN_ST_MEMBER_CLASS_SYMBOL_H
#define ZEN_ST_MEMBER_CLASS_SYMBOL_H

#include <com/onecube/zen/st/Symbol.h>

/*******************************************************************************
 * MemberClassSymbol                                                           *
 *******************************************************************************/

struct zen_MemberClassSymbol_t {
    zen_Symbol_t* m_symbol;
};

typedef struct zen_MemberClassSymbol_t zen_MemberClassSymbol_t;

zen_MemberClassSymbol_t* zen_MemberClassSymbol_new(zen_ASTNode_t* identifier);
void zen_MemberClassSymbol_delete(zen_MemberClassSymbol_t* symbol);
zen_Symbol_t* zen_MemberClassSymbol_getSymbol(zen_MemberClassSymbol_t* symbol);
 
#endif /* ZEN_ST_MEMBER_CLASS_SYMBOL_H */