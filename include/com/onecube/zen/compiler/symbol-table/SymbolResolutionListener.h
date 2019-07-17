// Saturday, March 10, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_RESOLUTION_LISTENER_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_RESOLUTION_LISTENER_H

#include <com/onecube/zen/compiler/ast/Listener.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolTable.h>
#include <com/onecube/zen/compiler/ast//AstAnnotations.h>

/*******************************************************************************
 * SymbolResolutionListener                                                    *
 *******************************************************************************/

/**
 * @class SymbolResolutionListener
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_SymbolResolutionListener_t {
    zen_ASTListener_t* m_astListener;
    zen_SymbolTable_t* m_symbolTable;
    zen_AstAnnotations_t* m_scopes;
};

/**
 * @memberof SymbolResolutionListener
 */
typedef struct zen_SymbolResolutionListener_t zen_SymbolResolutionListener_t;

// Constructor

/**
 * @memberof SymbolResolutionListener
 */
zen_SymbolResolutionListener_t* zen_SymbolResolutionListener_new(zen_SymbolTable_t* symbolTable, zen_AstAnnotations_t* scopes);

// Destructor

/**
 * @memberof SymbolResolutionListener
 */
void zen_SymbolResolutionListener_delete(zen_SymbolResolutionListener_t* listener);

// AST Listener

zen_ASTListener_t* zen_SymbolResolutionListener_getASTListener(zen_SymbolResolutionListener_t* listener);

// Event Handlers

/**
 * @memberof SymbolResolutionListener
 */
void zen_SymbolResolutionListener_onEnterCompilationUnit(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

/**
 * @memberof SymbolResolutionListener
 */
void zen_SymbolResolutionListener_onExitCompilationUnit(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

/**
 * @memberof SymbolResolutionListener
 */
void zen_SymbolResolutionListener_onEnterClassDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

/**
 * @memberof SymbolResolutionListener
 */
void zen_SymbolResolutionListener_onExitClassDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

/**
 * @memberof SymbolResolutionListener
 */
void zen_SymbolResolutionListener_onEnterFunctionDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

/**
 * @memberof SymbolResolutionListener
 */
void zen_SymbolResolutionListener_onExitFunctionDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

/**
 * @memberof SymbolResolutionListener
 */
void zen_SymbolResolutionListener_onEnterStatementSuite(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

/**
 * @memberof SymbolResolutionListener
 */
void zen_SymbolResolutionListener_onExitStatementSuite(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

/**
 * @memberof SymbolResolutionListener
 */
void zen_SymbolResolutionListener_onEnterPrimaryExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_RESOLUTION_LISTENER_H */