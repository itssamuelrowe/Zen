// Sunday, January 07, 2018

#ifndef ZEN_ST_VARIABLE_SYMBOL_H
#define ZEN_ST_VARIABLE_SYMBOL_H

#include <zen/ASTNode.h>
#include <zen/st/Symbol.h>

/*******************************************************************************
 * VariableSymbol                                                              *
 *******************************************************************************/

struct zen_VariableSymbol_t {
    zen_Symbol_t* m_symbol;
    zen_ArrayList_t* m_modifiers;
    int32_t m_implicitModifiers;
};

typedef struct zen_VariableSymbol_t zen_VariableSymbol_t;

zen_VariableSymbol_t* zen_VariableSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope);
void zen_VariableSymbol_delete(zen_VariableSymbol_t* symbol);
zen_Symbol_t* zen_VariableSymbol_getSymbol(zen_VariableSymbol_t* symbol);

#endif /* ZEN_ST_VARIABLE_SYMBOL_H */