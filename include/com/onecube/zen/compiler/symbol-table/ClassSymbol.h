// Sunday, January 07, 2018

#ifndef ZEN_ST_CLASS_SYMBOL_H
#define ZEN_ST_CLASS_SYMBOL_H

#include <zen/st/Symbol.h>
#include <zen/st/Modifier.h>

typedef struct zen_ClassScope_t zen_ClassScope_t;

/*******************************************************************************
 * ClassSymbol                                                                 *
 *******************************************************************************/

struct zen_ClassSymbol_t {
    zen_Symbol_t* m_symbol;
    /* The superclasses are added in the resolution phase. */
    zen_ArrayList_t* m_superClasses; /* <zen_ClassSymbol_t*> */
    zen_ArrayList_t* m_explicitModifiers;
    int32_t m_modifiers;
    zen_Scope_t* m_classScope;
};

typedef struct zen_ClassSymbol_t zen_ClassSymbol_t;

zen_ClassSymbol_t* zen_ClassSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope, zen_Scope_t* m_classScope);
void zen_ClassSymbol_delete(zen_ClassSymbol_t* symbol);
zen_Scope_t* zen_ClassSymbol_getClassScope(zen_ClassSymbol_t* symbol);
zen_ArrayList_t* zen_ClassSymbol_getSuperClasses(zen_ClassSymbol_t* symbol);
zen_Symbol_t* zen_ClassSymbol_getSymbol(zen_ClassSymbol_t* symbol);
void zen_ClassSymbol_addModifier(zen_ClassSymbol_t* symbol, zen_Modifier_t modifier, zen_ASTNode_t* node);
bool zen_ClassSymbol_hasModifier(zen_ClassSymbol_t* symbol, zen_Modifier_t modifier);

#endif /* ZEN_ST_CLASS_SYMBOL_H */