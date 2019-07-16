// Sunday, January 07, 2018

#ifndef ZEN_ST_MEMBER_CONSTANT_SYMBOL_H
#define ZEN_ST_MEMBER_CONSTANT_SYMBOL_H

#include <com/onecube/zen/st/Symbol.h>

/*******************************************************************************
 * MemberConstantSymbol                                                     *
 *******************************************************************************/

struct zen_MemberConstantSymbol_t {
    zen_Symbol_t* m_symbol;
    jtk_ArrayList_t* m_modifiers; /* <zen_ASTNode_t*> */
};

typedef struct zen_MemberConstantSymbol_t zen_MemberConstantSymbol_t;

zen_MemberConstantSymbol_t* zen_MemberConstantSymbol_new(zen_ASTNode_t* identifier,
    jtk_ArrayList_t* modifiers);
void zen_MemberConstantSymbol_delete(zen_MemberConstantSymbol_t* symbol);
zen_Symbol_t* zen_MemberConstantSymbol_getSymbol(zen_MemberConstantSymbol_t* symbol);

#endif /* ZEN_ST_MEMBER_CONSTANT_SYMBOL_H */