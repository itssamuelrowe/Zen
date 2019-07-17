// Saturday, November 25, 2017

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_H

#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/symbol-table/Scope.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolCategory.h>

/*******************************************************************************
 * Symbol                                                                      *
 *******************************************************************************/

/**
 * @class Symbol
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_Symbol_t {
    zen_SymbolCategory_t m_category;
    zen_ASTNode_t* m_identifier;
    zen_Scope_t* m_enclosingScope;
    void* m_context;
};

/**
 * @memberof Symbol
 */
typedef struct zen_Symbol_t zen_Symbol_t;

// Constructor

/**
 * @memberof Symbol
 */
zen_Symbol_t* zen_Symbol_new(zen_SymbolCategory_t category,
    zen_ASTNode_t* identifier, zen_Scope_t* enclosingScope,
    void* context);

// Destructor

/**
 * @memberof Symbol
 */
void zen_Symbol_delete(zen_Symbol_t* symbol);

// Function

/**
 * @memberof Symbol
 */
bool zen_Symbol_isFunction(zen_Symbol_t* symbol);

// Constant

/**
 * @memberof Symbol
 */
bool zen_Symbol_isConstant(zen_Symbol_t* symbol);

// Enumerate

/**
 * @memberof Symbol
 */
bool zen_Symbol_isEnumerate(zen_Symbol_t* symbol);

// Variable

/**
 * @memberof Symbol
 */
bool zen_Symbol_isVariable(zen_Symbol_t* symbol);

// Enumeration

/**
 * @memberof Symbol
 */
bool zen_Symbol_isEnumeration(zen_Symbol_t* symbol);

// Enclosing Scope

/**
 * @memberof Symbol
 */
zen_Scope_t* zen_Symbol_getEnclosingScope(zen_Symbol_t* symbol);

// Identifier

/**
 * @memberof Symbol
 */
zen_ASTNode_t* zen_Symbol_getIdentifier(zen_Symbol_t* symbol);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_H */