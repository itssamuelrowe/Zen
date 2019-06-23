// Saturday, November 25, 2017

#ifndef ZEN_ST_SYMBOL_H
#define ZEN_ST_SYMBOL_H

#include <zen/ASTNode.h>
#include <zen/st/Scope.h>
#include <zen/st/SymbolCategory.h>

/*******************************************************************************
 * Symbol                                                                      *
 *******************************************************************************/

struct zen_Symbol_t {
    zen_SymbolCategory_t m_category;
    zen_ASTNode_t* m_identifier;
    zen_Scope_t* m_enclosingScope;
    void* m_context;
};

typedef struct zen_Symbol_t zen_Symbol_t;

zen_Symbol_t* zen_Symbol_new(zen_SymbolCategory_t category,
    zen_ASTNode_t* identifier, zen_Scope_t* enclosingScope,
    void* context);
void zen_Symbol_delete(zen_Symbol_t* symbol);
bool zen_Symbol_isFunction(zen_Symbol_t* symbol);
bool zen_Symbol_isConstant(zen_Symbol_t* symbol);
bool zen_Symbol_isEnumerate(zen_Symbol_t* symbol);
bool zen_Symbol_isVariable(zen_Symbol_t* symbol);
bool zen_Symbol_isEnumeration(zen_Symbol_t* symbol);
zen_Scope_t* zen_Symbol_getEnclosingScope(zen_Symbol_t* symbol);
zen_ASTNode_t* zen_Symbol_getIdentifier(zen_Symbol_t* symbol);

#endif /* ZEN_ST_SYMBOL_H */