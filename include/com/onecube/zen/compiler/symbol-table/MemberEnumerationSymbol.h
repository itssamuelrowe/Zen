// Monday, January 08, 2018

#ifndef ZEN_ST_MEMBER_ENUMERATION_SYMBOL_H
#define ZEN_ST_MEMBER_ENUMERATION_SYMBOL_H

#include <com/onecube/zen/compiler/ast/Node.h>
#include <com/onecube/zen/st/Symbol.h>

/*******************************************************************************
 * MemberEnumerationSymbol                                                     *
 *******************************************************************************/

struct zen_MemberEnumerationSymbol_t {
    zen_Symbol_t* m_symbol;
};

typedef struct zen_MemberEnumerationSymbol_t zen_MemberEnumerationSymbol_t;

zen_MemberEnumerationSymbol_t* zen_MemberEnumerationSymbol_new();
void zen_MemberEnumerationSymbol_delete(zen_MemberEnumerationSymbol_t* symbol);
zen_Symbol_t* zen_MemberEnumerationSymbol_getSymbol(zen_MemberEnumerationSymbol_t* symbol);
 
#endif /* ZEN_ST_MEMBER_ENUMERATION_SYMBOL_H */