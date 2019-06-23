// Sunday, January 07, 2018

#include <zen/collection/LinkedStack.h>
#include <zen/st/SymbolTable.h>
#include <zen/st/AnnotationSymbol.h>
#include <zen/st/ClassSymbol.h>
#include <zen/st/FunctionSymbol.h>
#include <zen/st/EnumerationSymbol.h>
#include <zen/st/MemberConstantSymbol.h>
#include <zen/st/MemberClassSymbol.h>
#include <zen/st/MemberEnumerationSymbol.h>
#include <zen/st/FunctionSymbol.h>
#include <zen/st/MemberVariableSymbol.h>

zen_SymbolTable_t* zen_SymbolTable_new() {
    zen_SymbolTable_t* symbolTable = zen_Memory_allocate(zen_SymbolTable_t, 1);
    symbolTable->m_currentScope = NULL;

    return symbolTable;
}

void zen_SymbolTable_delete(zen_SymbolTable_t* symbolTable) {
    jtk_Assert_assertObject(symbolTable, "The specified symbol table is null.");

    zen_Memory_deallocate(symbolTable);
}

void zen_SymbolTable_setCurrentScope(zen_SymbolTable_t* symbolTable, zen_Scope_t* currentScope) {
    jtk_Assert_assertObject(symbolTable, "The specified symbol table is null.");

    // printf("<enter> %s\n", zen_Scope_getName(currentScope));
    symbolTable->m_currentScope = currentScope;
}

zen_Scope_t* zen_SymbolTable_getCurrentScope(zen_SymbolTable_t* symbolTable) {
    jtk_Assert_assertObject(symbolTable, "The specified symbol table is null.");
    return symbolTable->m_currentScope;
}

void zen_SymbolTable_invalidateCurrentScope(zen_SymbolTable_t* symbolTable) {
    jtk_Assert_assertObject(symbolTable, "The specified symbol table is null.");
    
    // printf("<exit> %s\n", zen_Scope_getName(symbolTable->m_currentScope));
    symbolTable->m_currentScope = zen_Scope_getEnclosingScope(symbolTable->m_currentScope);
}

void zen_SymbolTable_define(zen_SymbolTable_t* symbolTable, zen_Symbol_t* symbol) {
    jtk_Assert_assertObject(symbolTable, "The specified symbol table is null.");
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    zen_Scope_define(symbolTable->m_currentScope, symbol);
}

/* Resolve */

zen_Symbol_t* zen_SymbolTable_resolve(zen_SymbolTable_t* symbolTable, const uint8_t* identifier) {
    jtk_Assert_assertObject(symbolTable, "The specified symbol table is null.");
    jtk_Assert_assertObject(identifier, "The specified identifier is null.");

    zen_Scope_t* scope = symbolTable->m_currentScope;
    zen_Symbol_t* symbol = NULL;
    while (scope != NULL) {
        symbol = zen_Scope_resolve(scope, identifier);
        if (symbol != NULL) {
            break;
        }
        else {
            scope = zen_Scope_getEnclosingScope(scope);
        }
    }
    return symbol;
}