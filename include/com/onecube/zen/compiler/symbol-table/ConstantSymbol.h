// Sunday, January 07, 2018

#ifndef ZEN_ST_CONSTANT_SYMBOL_H
#define ZEN_ST_CONSTANT_SYMBOL_H

#include <com/onecube/zen/compiler/ast/Node.h>
#include <com/onecube/zen/st/Symbol.h>
#include <com/onecube/zen/st/Modifier.h>
#include <com/onecube/zen/collection/ArrayList.h>

/*******************************************************************************
 * ConstantSymbol                                                              *
 *******************************************************************************/

struct zen_ConstantSymbol_t {
    zen_Symbol_t* m_symbol;
    jtk_ArrayList_t* m_explicitModifiers; /* Useful for reporting errors. */
    int32_t m_modifiers;
};

typedef struct zen_ConstantSymbol_t zen_ConstantSymbol_t;

zen_ConstantSymbol_t* zen_ConstantSymbol_new(zen_ASTNode_t* identifier,
    zen_Scope_t* enclosingScope);
void zen_ConstantSymbol_delete(zen_ConstantSymbol_t* symbol);
zen_Symbol_t* zen_ConstantSymbol_getSymbol(zen_ConstantSymbol_t* symbol);
void zen_ConstantSymbol_addModifier(zen_ConstantSymbol_t* symbol, zen_Modifier_t modifier, zen_ASTNode_t* node);
bool zen_ConstantSymbol_hasModifier(zen_ConstantSymbol_t* symbol, zen_Modifier_t modifier);

#endif /* ZEN_ST_CONSTANT_SYMBOL_H */