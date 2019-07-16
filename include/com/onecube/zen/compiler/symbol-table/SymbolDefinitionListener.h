// Sunday, January 07, 2018

#ifndef ZEN_ST_SYMBOL_DEFINITION_LISTENER_H
#define ZEN_ST_SYMBOL_DEFINITION_LISTENER_H

#include <com/onecube/zen/compiler/ast/Listener.h>
#include <com/onecube/zen/Context.h>
#include <com/onecube/zen/st/Scope.h>
#include <com/onecube/zen/st/ScopeType.h>
#include <com/onecube/zen/st/SymbolTable.h>
#include <com/onecube/zen/compiler/ast//AstAnnotations.h>

/*******************************************************************************
 * SymbolDefinitionListener                                                    *
 *******************************************************************************/

struct zen_SymbolDefinitionListener_t {
    zen_ASTListener_t* m_astListener;
    zen_SymbolTable_t* m_symbolTable;
    zen_AstAnnotations_t* m_scopes;
};

typedef struct zen_SymbolDefinitionListener_t zen_SymbolDefinitionListener_t;

zen_SymbolDefinitionListener_t* zen_SymbolDefinitionListener_new(zen_SymbolTable_t* symbolTable, zen_AstAnnotations_t* scopes);
void zen_SymbolDefinitionListener_delete(zen_SymbolDefinitionListener_t* listener);

zen_ASTListener_t* zen_SymbolDefinitionListener_getAstListener(zen_SymbolDefinitionListener_t* listener);

void zen_SymbolDefinitionListener_onEnterCompilationUnit(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_SymbolDefinitionListener_onExitCompilationUnit(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_SymbolDefinitionListener_onEnterFunctionDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_SymbolDefinitionListener_onExitFunctionDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_SymbolDefinitionListener_onEnterStatementSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_SymbolDefinitionListener_onExitStatementSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_SymbolDefinitionListener_onEnterVariableDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_SymbolDefinitionListener_onEnterConstantDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_SymbolDefinitionListener_onEnterLabelClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_SymbolDefinitionListener_onEnterForParameters(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_SymbolDefinitionListener_onEnterClassDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_SymbolDefinitionListener_onExitClassDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_SymbolDefinitionListener_onEnterEnumerationDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_SymbolDefinitionListener_onExitEnumerationDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_SymbolDefinitionListener_onEnterEnumerate(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_SymbolDefinitionListener_onExitEnumerate(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

#endif /* ZEN_ST_SYMBOL_DEFINITION_LISTENER_H */