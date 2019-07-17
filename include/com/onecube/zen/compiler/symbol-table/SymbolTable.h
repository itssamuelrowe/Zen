// Satuday, November 25, 2017

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_TABLE_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_TABLE_H

#include <com/onecube/zen/compiler/symbol-table/ClassScope.h>
#include <com/onecube/zen/compiler/symbol-table/Scope.h>
#include <com/onecube/zen/compiler/symbol-table/Symbol.h>

/*******************************************************************************
 * SymbolTable                                                                 *
 *******************************************************************************/

/**
 * @class SymbolTable
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_SymbolTable_t {
    zen_Scope_t* m_currentScope;
};

/**
 * @memberof SymbolTable
 */
typedef struct zen_SymbolTable_t zen_SymbolTable_t;

// Constructor

/**
 * @memberof SymbolTable
 */
zen_SymbolTable_t* zen_SymbolTable_new();

// Destructor

/**
 * @memberof SymbolTable
 */
void zen_SymbolTable_delete(zen_SymbolTable_t* symbolTable);

// Current Scope

/**
 * @memberof SymbolTable
 */
void zen_SymbolTable_setCurrentScope(zen_SymbolTable_t* symbolTable, zen_Scope_t* currentScope);

/**
 * @memberof SymbolTable
 */
zen_Scope_t* zen_SymbolTable_getCurrentScope(zen_SymbolTable_t* symbolTable);

/**
 * @memberof SymbolTable
 */
void zen_SymbolTable_invalidateCurrentScope(zen_SymbolTable_t* symbolTable);

// Define

/**
 * @memberof SymbolTable
 */
void zen_SymbolTable_define(zen_SymbolTable_t* symbolTable, zen_Symbol_t* symbol);

/**
 * @memberof SymbolTable
 */
void zen_SymbolTable_defineClassMember(zen_SymbolTable_t* symbolTable, zen_ClassScope_t* classScope, zen_Symbol_t* symbol);

// Resolve

/**
 * @memberof SymbolTable
 */
zen_Symbol_t* zen_SymbolTable_resolve(zen_SymbolTable_t* symbolTable, const uint8_t* identifier);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_TABLE_H */