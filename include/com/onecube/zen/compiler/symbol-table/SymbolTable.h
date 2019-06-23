// Satuday, November 25, 2017

#ifndef ZEN_ST_SYMBOL_TABLE_H
#define ZEN_ST_SYMBOL_TABLE_H

#include <zen/st/ClassScope.h>
#include <zen/st/Scope.h>
#include <zen/st/Symbol.h>

/*******************************************************************************
 * SymbolTable                                                                 *
 *******************************************************************************/

struct zen_SymbolTable_t {
    zen_Scope_t* m_currentScope;
};

typedef struct zen_SymbolTable_t zen_SymbolTable_t;

zen_SymbolTable_t* zen_SymbolTable_new();
void zen_SymbolTable_delete(zen_SymbolTable_t* symbolTable);
void zen_SymbolTable_setCurrentScope(zen_SymbolTable_t* symbolTable, zen_Scope_t* currentScope);
zen_Scope_t* zen_SymbolTable_getCurrentScope(zen_SymbolTable_t* symbolTable);
void zen_SymbolTable_invalidateCurrentScope(zen_SymbolTable_t* symbolTable);
void zen_SymbolTable_defineClassMember(zen_SymbolTable_t* symbolTable, zen_ClassScope_t* classScope, zen_Symbol_t* symbol);
zen_Symbol_t* zen_SymbolTable_resolve(zen_SymbolTable_t* symbolTable, const uint8_t* identifier);
void zen_SymbolTable_define(zen_SymbolTable_t* symbolTable, zen_Symbol_t* symbol);

#endif /* ZEN_ST_SYMBOL_TABLE_H */