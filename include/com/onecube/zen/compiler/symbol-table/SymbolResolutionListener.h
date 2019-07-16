// Saturday, March 10, 2018

#ifndef ZEN_ST_SYMBOL_RESOLUTION_LISTENER_H
#define ZEN_ST_SYMBOL_RESOLUTION_LISTENER_H

#include <com/onecube/zen/compiler/ast/Listener.h>
#include <com/onecube/zen/st/SymbolTable.h>
#include <com/onecube/zen/compiler/ast//AstAnnotations.h>

/*******************************************************************************
 * SymbolResolutionListener                                                    *
 *******************************************************************************/

struct zen_SymbolResolutionListener_t {
    zen_ASTListener_t* m_astListener;
    zen_SymbolTable_t* m_symbolTable;
    zen_AstAnnotations_t* m_scopes;
};

typedef struct zen_SymbolResolutionListener_t zen_SymbolResolutionListener_t;

zen_SymbolResolutionListener_t* zen_SymbolResolutionListener_new(zen_SymbolTable_t* symbolTable, zen_AstAnnotations_t* scopes);
void zen_SymbolResolutionListener_delete(zen_SymbolResolutionListener_t* listener);
zen_ASTListener_t* zen_SymbolResolutionListener_getAstListener(zen_SymbolResolutionListener_t* listener);

void zen_SymbolResolutionListener_onEnterCompilationUnit(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_SymbolResolutionListener_onExitCompilationUnit(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_SymbolResolutionListener_onEnterClassDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_SymbolResolutionListener_onExitClassDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_SymbolResolutionListener_onEnterFunctionDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_SymbolResolutionListener_onExitFunctionDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_SymbolResolutionListener_onEnterStatementSuite(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_SymbolResolutionListener_onExitStatementSuite(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_SymbolResolutionListener_onEnterPrimaryExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

#endif /* ZEN_ST_SYMBOL_RESOLUTION_LISTENER_H */