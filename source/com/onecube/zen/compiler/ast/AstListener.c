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

// Saturday, January 06, 2018

#include <com/onecube/zen/compiler/ast/ASTListener.h>

/*******************************************************************************
 * ASTListener                                                                 *
 *******************************************************************************/

zen_ASTListener_t* zen_ASTListener_newWithContext(void* context) {
    zen_ASTListener_t* listener = zen_Memory_allocate(zen_ASTListener_t, 1);
    // listener->m_walkerState = ZEN_AST_WALKER_STATE_INVALID;
    listener->m_walkerState = ZEN_AST_WALKER_STATE_VISIT_CHILDREN;
    listener->m_context = context;

    listener->m_onVisitErrorNode = zen_ASTListener_onVisitErrorNode;
    listener->m_onVisitTerminal = zen_ASTListener_onVisitTerminal;

    listener->m_onEnterEveryRule = zen_ASTListener_onEnterEveryRule;
    listener->m_onExitEveryRule = zen_ASTListener_onExitEveryRule;

    listener->m_onEnterCompilationUnit = zen_ASTListener_onEnterCompilationUnit;
    listener->m_onExitCompilationUnit = zen_ASTListener_onExitCompilationUnit;

    listener->m_onEnterImportDeclaration = zen_ASTListener_onEnterImportDeclaration;
    listener->m_onExitImportDeclaration = zen_ASTListener_onExitImportDeclaration;

    listener->m_onEnterAnnotatedComponentDeclaration = zen_ASTListener_onEnterAnnotatedComponentDeclaration;
    listener->m_onExitAnnotatedComponentDeclaration = zen_ASTListener_onExitAnnotatedComponentDeclaration;

    listener->m_onEnterAnnotations = zen_ASTListener_onEnterAnnotations;
    listener->m_onExitAnnotations = zen_ASTListener_onExitAnnotations;

    listener->m_onEnterAnnotation = zen_ASTListener_onEnterAnnotation;
    listener->m_onExitAnnotation = zen_ASTListener_onExitAnnotation;

    listener->m_onEnterAnnotationType = zen_ASTListener_onEnterAnnotationType;
    listener->m_onExitAnnotationType = zen_ASTListener_onExitAnnotationType;

    listener->m_onEnterAnnotationAttribute = zen_ASTListener_onEnterAnnotationAttribute;
    listener->m_onExitAnnotationAttribute = zen_ASTListener_onExitAnnotationAttribute;

    listener->m_onEnterComponentDeclaration = zen_ASTListener_onEnterComponentDeclaration;
    listener->m_onExitComponentDeclaration = zen_ASTListener_onExitComponentDeclaration;

    listener->m_onEnterFunctionDeclaration = zen_ASTListener_onEnterFunctionDeclaration;
    listener->m_onExitFunctionDeclaration = zen_ASTListener_onExitFunctionDeclaration;

    listener->m_onEnterFunctionParameters = zen_ASTListener_onEnterFunctionParameters;
    listener->m_onExitFunctionParameters = zen_ASTListener_onExitFunctionParameters;

    listener->m_onEnterFunctionBody = zen_ASTListener_onEnterFunctionBody;
    listener->m_onExitFunctionBody = zen_ASTListener_onExitFunctionBody;

    listener->m_onEnterStatementSuite = zen_ASTListener_onEnterStatementSuite;
    listener->m_onExitStatementSuite = zen_ASTListener_onExitStatementSuite;

    listener->m_onEnterSimpleStatement = zen_ASTListener_onEnterSimpleStatement;
    listener->m_onExitSimpleStatement = zen_ASTListener_onExitSimpleStatement;

    listener->m_onEnterStatement = zen_ASTListener_onEnterStatement;
    listener->m_onExitStatement = zen_ASTListener_onExitStatement;

    listener->m_onEnterEmptyStatement = zen_ASTListener_onEnterEmptyStatement;
    listener->m_onExitEmptyStatement = zen_ASTListener_onExitEmptyStatement;

    listener->m_onEnterVariableDeclaration = zen_ASTListener_onEnterVariableDeclaration;
    listener->m_onExitVariableDeclaration = zen_ASTListener_onExitVariableDeclaration;

    listener->m_onEnterVariableDeclarator = zen_ASTListener_onEnterVariableDeclarator;
    listener->m_onExitVariableDeclarator = zen_ASTListener_onExitVariableDeclarator;

    listener->m_onEnterConstantDeclaration = zen_ASTListener_onEnterConstantDeclaration;
    listener->m_onExitConstantDeclaration = zen_ASTListener_onExitConstantDeclaration;

    listener->m_onEnterConstantDeclarator = zen_ASTListener_onEnterConstantDeclarator;
    listener->m_onExitConstantDeclarator = zen_ASTListener_onExitConstantDeclarator;

    listener->m_onEnterAssertStatement = zen_ASTListener_onEnterAssertStatement;
    listener->m_onExitAssertStatement = zen_ASTListener_onExitAssertStatement;

    listener->m_onEnterBreakStatement = zen_ASTListener_onEnterBreakStatement;
    listener->m_onExitBreakStatement = zen_ASTListener_onExitBreakStatement;

    listener->m_onEnterContinueStatement = zen_ASTListener_onEnterContinueStatement;
    listener->m_onExitContinueStatement = zen_ASTListener_onExitContinueStatement;

    listener->m_onEnterReturnStatement = zen_ASTListener_onEnterReturnStatement;
    listener->m_onExitReturnStatement = zen_ASTListener_onExitReturnStatement;

    listener->m_onEnterThrowStatement = zen_ASTListener_onEnterThrowStatement;
    listener->m_onExitThrowStatement = zen_ASTListener_onExitThrowStatement;

    listener->m_onEnterCompoundStatement = zen_ASTListener_onEnterCompoundStatement;
    listener->m_onExitCompoundStatement = zen_ASTListener_onExitCompoundStatement;

    listener->m_onEnterIfStatement = zen_ASTListener_onEnterIfStatement;
    listener->m_onExitIfStatement = zen_ASTListener_onExitIfStatement;

    listener->m_onEnterIfClause = zen_ASTListener_onEnterIfClause;
    listener->m_onExitIfClause = zen_ASTListener_onExitIfClause;

    listener->m_onEnterElseIfClause = zen_ASTListener_onEnterElseIfClause;
    listener->m_onExitElseIfClause = zen_ASTListener_onExitElseIfClause;

    listener->m_onEnterElseClause = zen_ASTListener_onEnterElseClause;
    listener->m_onExitElseClause = zen_ASTListener_onExitElseClause;

    listener->m_onEnterIterativeStatement = zen_ASTListener_onEnterIterativeStatement;
    listener->m_onExitIterativeStatement = zen_ASTListener_onExitIterativeStatement;

    listener->m_onEnterLabelClause = zen_ASTListener_onEnterLabel;
    listener->m_onExitLabelClause = zen_ASTListener_onExitLabel;

    listener->m_onEnterWhileStatement = zen_ASTListener_onEnterWhileStatement;
    listener->m_onExitWhileStatement = zen_ASTListener_onExitWhileStatement;

    listener->m_onEnterForStatement = zen_ASTListener_onEnterForStatement;
    listener->m_onExitForStatement = zen_ASTListener_onExitForStatement;

    listener->m_onEnterForParameter = zen_ASTListener_onEnterForParameters;
    listener->m_onExitForParameter = zen_ASTListener_onExitForParameters;

    listener->m_onEnterTryStatement = zen_ASTListener_onEnterTryStatement;
    listener->m_onExitTryStatement = zen_ASTListener_onExitTryStatement;

    listener->m_onEnterTryClause = zen_ASTListener_onEnterTryClause;
    listener->m_onExitTryClause = zen_ASTListener_onExitTryClause;

    listener->m_onEnterCatchClause = zen_ASTListener_onEnterCatchClause;
    listener->m_onExitCatchClause = zen_ASTListener_onExitCatchClause;

    listener->m_onEnterCatchFilter = zen_ASTListener_onEnterCatchFilter;
    listener->m_onExitCatchFilter = zen_ASTListener_onExitCatchFilter;

    listener->m_onEnterFinallyClause = zen_ASTListener_onEnterFinallyClause;
    listener->m_onExitFinallyClause = zen_ASTListener_onExitFinallyClause;

    listener->m_onEnterSynchronizeStatement = zen_ASTListener_onEnterSynchronizeStatement;
    listener->m_onExitSynchronizeStatement = zen_ASTListener_onExitSynchronizeStatement;

    listener->m_onEnterWithStatement = zen_ASTListener_onEnterWithStatement;
    listener->m_onExitWithStatement = zen_ASTListener_onExitWithStatement;

    listener->m_onEnterWithParameters = zen_ASTListener_onEnterWithParameters;
    listener->m_onExitWithParameters = zen_ASTListener_onExitWithParameters;

    listener->m_onEnterWithParameter = zen_ASTListener_onEnterWithParameter;
    listener->m_onExitWithParameter = zen_ASTListener_onExitWithParameter;

    listener->m_onEnterClassDeclaration = zen_ASTListener_onEnterClassDeclaration;
    listener->m_onExitClassDeclaration = zen_ASTListener_onExitClassDeclaration;

    listener->m_onEnterClassExtendsClause = zen_ASTListener_onEnterClassExtendsClause;
    listener->m_onExitClassExtendsClause = zen_ASTListener_onExitClassExtendsClause;

    listener->m_onEnterClassSuite = zen_ASTListener_onEnterClassSuite;
    listener->m_onExitClassSuite = zen_ASTListener_onExitClassSuite;

    listener->m_onEnterClassMember = zen_ASTListener_onEnterClassMember;
    listener->m_onExitClassMember = zen_ASTListener_onExitClassMember;

    // listener->m_onEnterConstructorDeclaration = zen_ASTListener_onEnterConstructorDeclaration;
    // listener->m_onExitConstructorDeclaration = zen_ASTListener_onExitConstructorDeclaration;

    listener->m_onEnterEnumerationDeclaration = zen_ASTListener_onEnterEnumerationDeclaration;
    listener->m_onExitEnumerationDeclaration = zen_ASTListener_onExitEnumerationDeclaration;

    listener->m_onEnterEnumerationBaseClause = zen_ASTListener_onEnterEnumrationBaseClass;
    listener->m_onExitEnumerationBaseClause = zen_ASTListener_onExitEnumerationBaseClause;

    listener->m_onEnterEnumerationSuite = zen_ASTListener_onEnterEnumerationSuite;
    listener->m_onExitEnumerationSuite = zen_ASTListener_onExitEnumerationSuite;

    listener->m_onEnterEnumerate = zen_ASTListener_onEnterEnumerate;
    listener->m_onExitEnumerate = zen_ASTListener_onExitEnumerate;

    listener->m_onEnterExpressions = zen_ASTListener_onEnterExpressions;
    listener->m_onExitExpressions = zen_ASTListener_onExitExpressions;

    listener->m_onEnterExpression = zen_ASTListener_onEnterExpression;
    listener->m_onExitExpression = zen_ASTListener_onExitExpression;

    listener->m_onEnterAssignmentExpression = zen_ASTListener_onEnterAssignmentExpression;
    listener->m_onExitAssignmentExpression = zen_ASTListener_onExitAssignmentExpression;

    listener->m_onEnterConditionalExpression = zen_ASTListener_onEnterConditionalExpression;
    listener->m_onExitConditionalExpression = zen_ASTListener_onExitConditionalExpression;

    listener->m_onEnterLogicalOrExpression = zen_ASTListener_onEnterLogicalOrExpression;
    listener->m_onExitLogicalOrExpression = zen_ASTListener_onExitLogicalOrExpression;

    listener->m_onEnterLogicalAndExpression = zen_ASTListener_onEnterLogicalAndExpression;
    listener->m_onExitLogicalAndExpression = zen_ASTListener_onExitLogicalAndExpression;

    listener->m_onEnterInclusiveOrExpression = zen_ASTListener_onEnterInclusiveOrExpression;
    listener->m_onExitInclusiveOrExpression = zen_ASTListener_onExitInclusiveOrExpression;

    listener->m_onEnterExclusiveOrExpression = zen_ASTListener_onEnterExclusiveOrExpression;
    listener->m_onExitExclusiveOrExpression = zen_ASTListener_onExitExclusiveOrExpression;

    listener->m_onEnterAndExpression = zen_ASTListener_onEnterAndExpression;
    listener->m_onExitAndExpression = zen_ASTListener_onExitAndExpression;

    listener->m_onEnterEqualityExpression = zen_ASTListener_onEnterEqualityExpression;
    listener->m_onExitEqualityExpression = zen_ASTListener_onExitEqualityExpression;

    listener->m_onEnterRelationalExpression = zen_ASTListener_onEnterRelationalExpression;
    listener->m_onExitRelationalExpression = zen_ASTListener_onExitRelationalExpression;

    listener->m_onEnterShiftExpression = zen_ASTListener_onEnterShiftExpression;
    listener->m_onExitShiftExpression = zen_ASTListener_onExitShiftExpression;

    listener->m_onEnterAdditiveExpression = zen_ASTListener_onEnterAdditiveExpression;
    listener->m_onExitAdditiveExpression = zen_ASTListener_onExitAdditiveExpression;

    listener->m_onEnterMultiplicativeExpression = zen_ASTListener_onEnterMultiplicativeExpression;
    listener->m_onExitMultiplicativeExpression = zen_ASTListener_onExitMultiplicativeExpression;

    listener->m_onEnterUnaryExpression = zen_ASTListener_onEnterUnaryExpression;
    listener->m_onExitUnaryExpression = zen_ASTListener_onExitUnaryExpression;

    listener->m_onEnterPostfixExpression = zen_ASTListener_onEnterPostfixExpression;
    listener->m_onExitPostfixExpression = zen_ASTListener_onExitPostfixExpression;

    listener->m_onEnterSubscript = zen_ASTListener_onEnterSubscript;
    listener->m_onExitSubscript = zen_ASTListener_onExitSubscript;

    listener->m_onEnterFunctionArguments = zen_ASTListener_onEnterFunctionArguments;
    listener->m_onExitFunctionArguments = zen_ASTListener_onExitFunctionArguments;

    listener->m_onEnterMemberAccess = zen_ASTListener_onEnterMemberAccess;
    listener->m_onExitMemberAccess = zen_ASTListener_onExitMemberAccess;

    listener->m_onEnterPostfixOperator = zen_ASTListener_onEnterPostfixOperator;
    listener->m_onExitPostfixOperator = zen_ASTListener_onExitPostfixOperator;

    listener->m_onEnterPrimaryExpression = zen_ASTListener_onEnterPrimaryExpression;
    listener->m_onExitPrimaryExpression = zen_ASTListener_onExitPrimaryExpression;

    listener->m_onEnterMapExpression = zen_ASTListener_onEnterMapExpression;
    listener->m_onExitMapExpression = zen_ASTListener_onExitMapExpression;

    listener->m_onEnterMapEntries = zen_ASTListener_onEnterMapEntries;
    listener->m_onExitMapEntries = zen_ASTListener_onExitMapEntries;

    listener->m_onEnterMapEntry = zen_ASTListener_onEnterMapEntry;
    listener->m_onExitMapEntry = zen_ASTListener_onExitMapEntry;

    listener->m_onEnterListExpression = zen_ASTListener_onEnterListExpression;
    listener->m_onExitListExpression = zen_ASTListener_onExitListExpression;

    listener->m_onEnterNewExpression = zen_ASTListener_onEnterNewExpression;
    listener->m_onExitNewExpression = zen_ASTListener_onExitNewExpression;

    return listener;
}

void zen_ASTListener_delete(zen_ASTListener_t* listener) {
    jtk_Assert_assertObject(listener, "The specified listener is null.");
    jtk_Memory_deallocate(listener);
}

void zen_ASTListener_onVisitErrorNode(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onVisitTerminal(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onEnterEveryRule(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitEveryRule(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// compilationUnit

void zen_ASTListener_onEnterCompilationUnit(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitCompilationUnit(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// importDeclaration

void zen_ASTListener_onEnterImportDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitImportDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// annotatedComponentDeclaration

void zen_ASTListener_onEnterAnnotatedComponentDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitAnnotatedComponentDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// annotations

void zen_ASTListener_onEnterAnnotations(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitAnnotations(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// annotation

void zen_ASTListener_onEnterAnnotation(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitAnnotation(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// annotationType

void zen_ASTListener_onEnterAnnotationType(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitAnnotationType(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// annotationAttribute

void zen_ASTListener_onEnterAnnotationAttribute(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitAnnotationAttribute(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// componentDeclaration

void zen_ASTListener_onEnterComponentDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitComponentDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// functionDeclaration

void zen_ASTListener_onEnterFunctionDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitFunctionDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// functionParameters

void zen_ASTListener_onEnterFunctionParameters(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitFunctionParameters(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// functionBody

void zen_ASTListener_onEnterFunctionBody(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitFunctionBody(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// statementSuite

void zen_ASTListener_onEnterStatementSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitStatementSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// simpleStatement

void zen_ASTListener_onEnterSimpleStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitSimpleStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// statement

void zen_ASTListener_onEnterStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// emptyStatement

void zen_ASTListener_onEnterEmptyStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitEmptyStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// variableDeclaration

void zen_ASTListener_onEnterVariableDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitVariableDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// variableDeclarator

void zen_ASTListener_onEnterVariableDeclarator(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitVariableDeclarator(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// constantDeclaration

void zen_ASTListener_onEnterConstantDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitConstantDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// constantDeclarator

void zen_ASTListener_onEnterConstantDeclarator(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitConstantDeclarator(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// assertStatement

void zen_ASTListener_onEnterAssertStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitAssertStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// breakStatement

void zen_ASTListener_onEnterBreakStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitBreakStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// continueStatement

void zen_ASTListener_onEnterContinueStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitContinueStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// returnStatement

void zen_ASTListener_onEnterReturnStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitReturnStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// throwStatement

void zen_ASTListener_onEnterThrowStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitThrowStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// compoundStatement

void zen_ASTListener_onEnterCompoundStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitCompoundStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// ifStatement

void zen_ASTListener_onEnterIfStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitIfStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// ifClause

void zen_ASTListener_onEnterIfClause(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitIfClause(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// elseIfClause

void zen_ASTListener_onEnterElseIfClause(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitElseIfClause(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// elseClause

void zen_ASTListener_onEnterElseClause(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitElseClause(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// iterativeStatement

void zen_ASTListener_onEnterIterativeStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitIterativeStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// label

void zen_ASTListener_onEnterLabel(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitLabel(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// whileStatement

void zen_ASTListener_onEnterWhileStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitWhileStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// forStatement

void zen_ASTListener_onEnterForStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitForStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// forParameters

void zen_ASTListener_onEnterForParameters(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitForParameters(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// tryStatement

void zen_ASTListener_onEnterTryStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitTryStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// tryClause

void zen_ASTListener_onEnterTryClause(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitTryClause(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// catchClause

void zen_ASTListener_onEnterCatchClause(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitCatchClause(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// catchFilter

void zen_ASTListener_onEnterCatchFilter(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitCatchFilter(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// finallyClause

void zen_ASTListener_onEnterFinallyClause(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitFinallyClause(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// synchronizeStatement

void zen_ASTListener_onEnterSynchronizeStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitSynchronizeStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// withStatement

void zen_ASTListener_onEnterWithStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitWithStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// withParameters

void zen_ASTListener_onEnterWithParameters(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitWithParameters(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// withParameter

void zen_ASTListener_onEnterWithParameter(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitWithParameter(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// classDeclaration

void zen_ASTListener_onEnterClassDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitClassDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// classExtends

void zen_ASTListener_onEnterClassExtendsClause(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitClassExtendsClause(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// classSuite

void zen_ASTListener_onEnterClassSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitClassSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// classMember

void zen_ASTListener_onEnterClassMember(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitClassMember(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// void zen_ASTListener_onEnterConstructorDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
// }

// void zen_ASTListener_onExitConstructorDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
// }


// enumerationDeclaration

void zen_ASTListener_onEnterEnumerationDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitEnumerationDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// enumerationBaseClass

void zen_ASTListener_onEnterEnumrationBaseClass(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitEnumerationBaseClause(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// enumerationSuite

void zen_ASTListener_onEnterEnumerationSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitEnumerationSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// enumerate

void zen_ASTListener_onEnterEnumerate(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitEnumerate(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// expressions

void zen_ASTListener_onEnterExpressions(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitExpressions(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// expression

void zen_ASTListener_onEnterExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// assignmentExpression

void zen_ASTListener_onEnterAssignmentExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitAssignmentExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// conditionalExpression

void zen_ASTListener_onEnterConditionalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitConditionalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// logicalOrExpression

void zen_ASTListener_onEnterLogicalOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitLogicalOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// logicalAndExpression

void zen_ASTListener_onEnterLogicalAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitLogicalAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// inclusiveOrExpression

void zen_ASTListener_onEnterInclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitInclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// exclusiveOrExpression

void zen_ASTListener_onEnterExclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitExclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// andExpression

void zen_ASTListener_onEnterAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// equalityExpression

void zen_ASTListener_onEnterEqualityExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitEqualityExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// relationalExpression

void zen_ASTListener_onEnterRelationalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitRelationalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// shiftExpression

void zen_ASTListener_onEnterShiftExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitShiftExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// additiveExpression

void zen_ASTListener_onEnterAdditiveExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitAdditiveExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// multiplicativeExpression

void zen_ASTListener_onEnterMultiplicativeExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitMultiplicativeExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// unaryExpression

void zen_ASTListener_onEnterUnaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitUnaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// postfixExpression

void zen_ASTListener_onEnterPostfixExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitPostfixExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// subscript

void zen_ASTListener_onEnterSubscript(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitSubscript(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// functionArguments

void zen_ASTListener_onEnterFunctionArguments(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitFunctionArguments(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// memberAccess

void zen_ASTListener_onEnterMemberAccess(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitMemberAccess(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// postfixOperator

void zen_ASTListener_onEnterPostfixOperator(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitPostfixOperator(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// primaryExpression

void zen_ASTListener_onEnterPrimaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitPrimaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// mapExpression

void zen_ASTListener_onEnterMapExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitMapExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// mapEntries

void zen_ASTListener_onEnterMapEntries(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitMapEntries(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// mapEntry

void zen_ASTListener_onEnterMapEntry(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitMapEntry(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// listExpression

void zen_ASTListener_onEnterListExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitListExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onEnterNewExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_ASTListener_onExitNewExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// Children

void zen_ASTListener_skipChildren(zen_ASTListener_t* listener) {
    jtk_Assert_assertObject(listener, "The specified listener is null.");

    listener->m_walkerState = ZEN_AST_WALKER_STATE_SKIP_CHILDREN;
}

void zen_ASTListener_visitChildren(zen_ASTListener_t* listener) {
    jtk_Assert_assertObject(listener, "The specified listener is null.");

    listener->m_walkerState = ZEN_AST_WALKER_STATE_VISIT_CHILDREN;
}

// First Child

void zen_ASTListener_visitFirstChild(zen_ASTListener_t* listener) {
    jtk_Assert_assertObject(listener, "The specified listener is null.");

    listener->m_walkerState = ZEN_AST_WALKER_STATE_VISIT_FIRST_CHILD;
}