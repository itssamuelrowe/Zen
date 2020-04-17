/*
 * Copyright 2017-2020 Samuel Rowe
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Saturday, March 10, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_RESOLUTION_LISTENER_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_RESOLUTION_LISTENER_H

#include <com/onecube/zen/compiler/Compiler.h>
#include <com/onecube/zen/compiler/ast/ASTAnnotations.h>
#include <com/onecube/zen/compiler/ast/ASTListener.h>
#include <com/onecube/zen/compiler/ast/ExpressionAnnotation.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolTable.h>

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
    zen_Compiler_t* m_compiler;
    zen_ASTListener_t* m_astListener;
    zen_SymbolTable_t* m_symbolTable;
    zen_ASTAnnotations_t* m_scopes;
    zen_ExpressionAnnotation_t m_label;
    int32_t m_functionIndex;
    int32_t m_fieldIndex;
};

/**
 * @memberof SymbolResolutionListener
 */
typedef struct zen_SymbolResolutionListener_t zen_SymbolResolutionListener_t;

// Constructor

/**
 * @memberof SymbolResolutionListener
 */
zen_SymbolResolutionListener_t* zen_SymbolResolutionListener_new(zen_Compiler_t* compiler);

// Destructor

/**
 * @memberof SymbolResolutionListener
 */
void zen_SymbolResolutionListener_delete(zen_SymbolResolutionListener_t* listener);

// AST Listener

zen_ASTListener_t* zen_SymbolResolutionListener_getASTListener(zen_SymbolResolutionListener_t* listener);

// Reset

void zen_SymbolResolutionListener_reset(zen_SymbolResolutionListener_t* listener,
    zen_SymbolTable_t* symbolTable, zen_ASTAnnotations_t* scopes);

zen_Symbol_t* zen_SymbolResolutionListener_createExternalSymbol(
    zen_SymbolResolutionListener_t* listener, zen_Symbol_t* symbol,
    zen_Scope_t* enclosingScope, zen_ASTNode_t* identifier);

// Event Handlers

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onVisitErrorNode(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onVisitTerminal(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterEveryRule(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitEveryRule(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterCompilationUnit(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitCompilationUnit(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterImportDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitImportDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterAnnotatedComponentDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitAnnotatedComponentDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterAnnotations(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitAnnotations(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterAnnotation(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitAnnotation(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterAnnotationType(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitAnnotationType(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterAnnotationAttribute(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitAnnotationAttribute(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterComponentDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitComponentDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterFunctionDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitFunctionDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterFunctionParameters(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitFunctionParameters(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterFunctionBody(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitFunctionBody(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterStatementSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitStatementSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterSimpleStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitSimpleStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterEmptyStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitEmptyStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterVariableDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitVariableDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterVariableDeclarator(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitVariableDeclarator(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterConstantDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitConstantDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterConstantDeclarator(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitConstantDeclarator(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterAssertStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitAssertStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterBreakStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitBreakStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterContinueStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitContinueStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterReturnStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitReturnStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterThrowStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitThrowStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterCompoundStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitCompoundStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterIfStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitIfStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterIfClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitIfClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterElseIfClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitElseIfClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterElseClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitElseClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterIterativeStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitIterativeStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterLabel(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitLabel(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterWhileStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitWhileStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterForStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitForStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterForParameters(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitForParameters(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterTryStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitTryStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterTryClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitTryClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterCatchClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitCatchClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterCatchFilter(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitCatchFilter(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterFinallyClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitFinallyClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterSynchronizeStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitSynchronizeStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterWithStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitWithStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterClassDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitClassDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterClassExtendsClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitClassExtendsClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterClassSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitClassSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterClassMember(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitClassMember(zen_ASTListener_t* listener, zen_ASTNode_t* node);
// void zen_SymbolResolutionListener_onEnterConstructorDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);
// void zen_SymbolResolutionListener_onExitConstructorDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterEnumerationDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitEnumerationDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterEnumrationBaseClass(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitEnumerationBaseClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterEnumerationSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitEnumerationSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterEnumerate(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitEnumerate(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterExpressions(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitExpressions(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterAssignmentExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitAssignmentExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterConditionalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitConditionalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterLogicalOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitLogicalOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterLogicalAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitLogicalAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterInclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitInclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterExclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitExclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterEqualityExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitEqualityExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterRelationalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitRelationalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterShiftExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitShiftExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterAdditiveExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitAdditiveExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterMultiplicativeExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitMultiplicativeExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterUnaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitUnaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterPostfixExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitPostfixExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterSubscript(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitSubscript(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterFunctionArguments(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitFunctionArguments(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterMemberAccess(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitMemberAccess(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterPostfixOperator(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitPostfixOperator(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterPrimaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitPrimaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterMapExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitMapExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterMapEntries(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onExitMapEntries(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_SymbolResolutionListener_onEnterMapEntry(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof SymbolResolutionListener
 */
void zen_SymbolResolutionListener_onExitMapEntry(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof SymbolResolutionListener
 */
void zen_SymbolResolutionListener_onEnterListExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof SymbolResolutionListener
 */
void zen_SymbolResolutionListener_onExitListExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof SymbolResolutionListener
 */
void zen_SymbolResolutionListener_onEnterNewExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof SymbolResolutionListener
 */
void zen_SymbolResolutionListener_onExitNewExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SYMBOL_RESOLUTION_LISTENER_H */