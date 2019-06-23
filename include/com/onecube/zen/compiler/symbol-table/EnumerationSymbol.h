// Thursday, March 08, 2018

#ifndef ZEN_ST_ENUMERATION_SYMBOL_H
#define ZEN_ST_ENUMERATION_SYMBOL_H

#include <zen/st/Symbol.h>
#include <zen/st/Modifier.h>
#include <zen/st/ClassSymbol.h>

/*******************************************************************************
 * EnumerationSymbol                                                           *
 *******************************************************************************/

struct zen_EnumerationSymbol_t {
    zen_Symbol_t* m_symbol;
    /* The superclass attribute is updated during the resolution phase. */
    zen_ClassSymbol_t* m_superclass;
    zen_ArrayList_t* m_explicitModifiers;
    int32_t m_modifiers;
    zen_Scope_t* m_enumerationScope;
};

typedef struct zen_EnumerationSymbol_t zen_EnumerationSymbol_t;

zen_EnumerationSymbol_t* zen_EnumerationSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope, zen_Scope_t* m_enumerationScope);
void zen_EnumerationSymbol_delete(zen_EnumerationSymbol_t* symbol);
zen_Scope_t* zen_EnumerationSymbol_getEnumerationScope(zen_EnumerationSymbol_t* symbol);
zen_ClassSymbol_t* zen_EnumerationSymbol_getSuperclass(zen_EnumerationSymbol_t* symbol);
zen_Symbol_t* zen_EnumerationSymbol_getSymbol(zen_EnumerationSymbol_t* symbol);
void zen_EnumerationSymbol_addModifier(zen_EnumerationSymbol_t* symbol, zen_Modifier_t modifier, zen_ASTNode_t* node);
bool zen_EnumerationSymbol_hasModifier(zen_EnumerationSymbol_t* symbol, zen_Modifier_t modifier);

#endif /* ZEN_ST_ENUMERATION_SYMBOL_H */