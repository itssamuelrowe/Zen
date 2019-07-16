// Monday, March 05, 2018

#ifndef ZEN_ST_LABEL_SYMBOL_H
#define ZEN_ST_LABEL_SYMBOL_H

#include <com/onecube/zen/compiler/ast/Node.h>
#include <com/onecube/zen/st/Symbol.h>

/*******************************************************************************
 * LabelSymbol                                                                 *
 *******************************************************************************/

struct zen_LabelSymbol_t {
    zen_Symbol_t* m_symbol;
};

typedef struct zen_LabelSymbol_t zen_LabelSymbol_t;

zen_LabelSymbol_t* zen_LabelSymbol_new(zen_ASTNode_t* identifier, zen_Scope_t* enclosingScope);
void zen_LabelSymbol_delete(zen_LabelSymbol_t* symbol);
zen_Symbol_t* zen_LabelSymbol_getSymbol(zen_LabelSymbol_t* symbol);

#endif /* ZEN_ST_LABEL_SYMBOL_H */