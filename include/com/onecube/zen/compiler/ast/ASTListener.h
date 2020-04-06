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

// Sunday, November 26, 2017

#ifndef COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_LISTENER_H
#define COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_LISTENER_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/ast/ASTWalkerState.h>

/*******************************************************************************
 * ASTListener                                                                 *
 *******************************************************************************/

/**
 * @memberof ASTListener
 */
typedef struct zen_ASTListener_t zen_ASTListener_t;

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnVisitErrorNodeFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnVisitTerminalFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterEveryRuleFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitEveryRuleFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterCompilationUnitFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitCompilationUnitFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterImportDeclarationFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitImportDeclarationFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterAnnotatedComponentDeclarationFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitAnnotatedComponentDeclarationFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterAnnotationsFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitAnnotationsFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterAnnotationFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitAnnotationFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterAnnotationTypeFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitAnnotationTypeFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterAnnotationAttributeFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitAnnotationAttributeFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterComponentDeclarationFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitComponentDeclarationFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterFunctionDeclarationFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitFunctionDeclarationFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterFunctionParametersFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitFunctionParametersFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterFunctionBodyFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitFunctionBodyFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterStatementSuiteFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitStatementSuiteFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterSimpleStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitSimpleStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterEmptyStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitEmptyStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterVariableDeclarationFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitVariableDeclarationFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterVariableDeclaratorFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitVariableDeclaratorFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterConstantDeclarationFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitConstantDeclarationFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterConstantDeclaratorFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitConstantDeclaratorFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterAssertStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitAssertStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterBreakStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitBreakStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterContinueStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitContinueStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterReturnStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitReturnStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterThrowStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitThrowStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterCompoundStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitCompoundStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterIfStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitIfStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterIfClauseFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitIfClauseFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterElseIfClauseFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitElseIfClauseFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterElseClauseFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitElseClauseFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterIterativeStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitIterativeStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterLabelClauseFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitLabelClauseFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterWhileStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitWhileStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterForStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitForStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterForParametersFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitForParametersFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterTryStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitTryStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterTryClauseFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitTryClauseFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterCatchClauseFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitCatchClauseFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterCatchFilterFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitCatchFilterFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterFinallyClauseFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitFinallyClauseFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterSynchronizeStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitSynchronizeStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterWithStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitWithStatementFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterWithParametersFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitWithParametersFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterWithParameterFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitWithParameterFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterClassDeclarationFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitClassDeclarationFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterClassExtendsClauseFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitClassExtendsClauseFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterClassSuiteFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitClassSuiteFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterClassMemberFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitClassMemberFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

// typedef void (*zen_OnEnterConstructorDeclarationFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);
// typedef void (*zen_OnExitConstructorDeclarationFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterEnumerationDeclarationFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitEnumerationDeclarationFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterEnumerationBaseClauseFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitEnumerationBaseClauseFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterEnumerationSuiteFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitEnumerationSuiteFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterEnumerateFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitEnumerateFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterExpressionsFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitExpressionsFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterAssignmentExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitAssignmentExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterConditionalExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitConditionalExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterLogicalOrExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitLogicalOrExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterLogialAndExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitLogicalAndExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterInclusiveOrExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitInclusiveOrExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterExclusiveOrExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitExclusiveOrExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterAndExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitAndExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterEqualityExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitEqualityExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterRelationalExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitRelationalExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterShiftExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitShiftExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterAdditiveExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitAdditiveExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterMultiplicativeExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitMultiplicativeExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterUnaryExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitUnaryExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterPostfixExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitPostfixExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterSubscriptFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitSubscriptFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterFunctionArgumentsFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitFunctionArgumentsFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterMemberAccessFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitMemberAccessFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterPostfixOperatorFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitPostfixOperatorFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterPrimaryExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitPrimaryExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterMapExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitMapExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterMapEntriesFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitMapEntriesFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterMapEntryFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitMapEntryFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterListExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitListExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnEnterNewExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
typedef void (*zen_OnExitNewExpressionFunction_t)(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @class ASTListener
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ASTListener_t {
    zen_OnVisitErrorNodeFunction_t m_onVisitErrorNode;
    zen_OnVisitTerminalFunction_t m_onVisitTerminal;

    zen_OnEnterEveryRuleFunction_t m_onEnterEveryRule;
    zen_OnExitEveryRuleFunction_t m_onExitEveryRule;

    zen_OnEnterCompilationUnitFunction_t m_onEnterCompilationUnit;
    zen_OnExitCompilationUnitFunction_t m_onExitCompilationUnit;

    zen_OnEnterImportDeclarationFunction_t m_onEnterImportDeclaration;
    zen_OnExitImportDeclarationFunction_t m_onExitImportDeclaration;

    zen_OnEnterAnnotatedComponentDeclarationFunction_t m_onEnterAnnotatedComponentDeclaration;
    zen_OnExitAnnotatedComponentDeclarationFunction_t m_onExitAnnotatedComponentDeclaration;

    zen_OnEnterAnnotationsFunction_t m_onEnterAnnotations;
    zen_OnExitAnnotationsFunction_t m_onExitAnnotations;

    zen_OnEnterAnnotationFunction_t m_onEnterAnnotation;
    zen_OnExitAnnotationFunction_t m_onExitAnnotation;

    zen_OnEnterAnnotationTypeFunction_t m_onEnterAnnotationType;
    zen_OnExitAnnotationTypeFunction_t m_onExitAnnotationType;

    zen_OnEnterAnnotationAttributeFunction_t m_onEnterAnnotationAttribute;
    zen_OnExitAnnotationAttributeFunction_t m_onExitAnnotationAttribute;

    zen_OnEnterComponentDeclarationFunction_t m_onEnterComponentDeclaration;
    zen_OnExitComponentDeclarationFunction_t m_onExitComponentDeclaration;

    zen_OnEnterFunctionDeclarationFunction_t m_onEnterFunctionDeclaration;
    zen_OnExitFunctionDeclarationFunction_t m_onExitFunctionDeclaration;

    zen_OnEnterFunctionParametersFunction_t m_onEnterFunctionParameters;
    zen_OnExitFunctionParametersFunction_t m_onExitFunctionParameters;

    zen_OnEnterFunctionBodyFunction_t m_onEnterFunctionBody;
    zen_OnExitFunctionBodyFunction_t m_onExitFunctionBody;

    zen_OnEnterStatementSuiteFunction_t m_onEnterStatementSuite;
    zen_OnExitStatementSuiteFunction_t m_onExitStatementSuite;

    zen_OnEnterSimpleStatementFunction_t m_onEnterSimpleStatement;
    zen_OnExitSimpleStatementFunction_t m_onExitSimpleStatement;

    zen_OnEnterStatementFunction_t m_onEnterStatement;
    zen_OnExitStatementFunction_t m_onExitStatement;

    zen_OnEnterEmptyStatementFunction_t m_onEnterEmptyStatement;
    zen_OnExitEmptyStatementFunction_t m_onExitEmptyStatement;

    zen_OnEnterVariableDeclarationFunction_t m_onEnterVariableDeclaration;
    zen_OnExitVariableDeclarationFunction_t m_onExitVariableDeclaration;

    zen_OnEnterVariableDeclaratorFunction_t m_onEnterVariableDeclarator;
    zen_OnExitVariableDeclaratorFunction_t m_onExitVariableDeclarator;

    zen_OnEnterConstantDeclarationFunction_t m_onEnterConstantDeclaration;
    zen_OnExitConstantDeclarationFunction_t m_onExitConstantDeclaration;

    zen_OnEnterConstantDeclaratorFunction_t m_onEnterConstantDeclarator;
    zen_OnExitConstantDeclaratorFunction_t m_onExitConstantDeclarator;

    zen_OnEnterAssertStatementFunction_t m_onEnterAssertStatement;
    zen_OnExitAssertStatementFunction_t m_onExitAssertStatement;

    zen_OnEnterBreakStatementFunction_t m_onEnterBreakStatement;
    zen_OnExitBreakStatementFunction_t m_onExitBreakStatement;

    zen_OnEnterContinueStatementFunction_t m_onEnterContinueStatement;
    zen_OnExitContinueStatementFunction_t m_onExitContinueStatement;

    zen_OnEnterReturnStatementFunction_t m_onEnterReturnStatement;
    zen_OnExitReturnStatementFunction_t m_onExitReturnStatement;

    zen_OnEnterThrowStatementFunction_t m_onEnterThrowStatement;
    zen_OnExitThrowStatementFunction_t m_onExitThrowStatement;

    zen_OnEnterCompoundStatementFunction_t m_onEnterCompoundStatement;
    zen_OnExitCompoundStatementFunction_t m_onExitCompoundStatement;

    zen_OnEnterIfStatementFunction_t m_onEnterIfStatement;
    zen_OnExitIfStatementFunction_t m_onExitIfStatement;

    zen_OnEnterIfClauseFunction_t m_onEnterIfClause;
    zen_OnExitIfClauseFunction_t m_onExitIfClause;

    zen_OnEnterElseIfClauseFunction_t m_onEnterElseIfClause;
    zen_OnExitElseIfClauseFunction_t m_onExitElseIfClause;

    zen_OnEnterElseClauseFunction_t m_onEnterElseClause;
    zen_OnExitElseClauseFunction_t m_onExitElseClause;

    zen_OnEnterIterativeStatementFunction_t m_onEnterIterativeStatement;
    zen_OnExitIterativeStatementFunction_t m_onExitIterativeStatement;

    zen_OnEnterLabelClauseFunction_t m_onEnterLabelClause;
    zen_OnExitLabelClauseFunction_t m_onExitLabelClause;

    zen_OnEnterWhileStatementFunction_t m_onEnterWhileStatement;
    zen_OnExitWhileStatementFunction_t m_onExitWhileStatement;

    zen_OnEnterForStatementFunction_t m_onEnterForStatement;
    zen_OnExitForStatementFunction_t m_onExitForStatement;

    zen_OnEnterForParametersFunction_t m_onEnterForParameter;
    zen_OnExitForParametersFunction_t m_onExitForParameter;

    zen_OnEnterTryStatementFunction_t m_onEnterTryStatement;
    zen_OnExitTryStatementFunction_t m_onExitTryStatement;

    zen_OnEnterTryClauseFunction_t m_onEnterTryClause;
    zen_OnExitTryClauseFunction_t m_onExitTryClause;

    zen_OnEnterCatchClauseFunction_t m_onEnterCatchClause;
    zen_OnExitCatchClauseFunction_t m_onExitCatchClause;

    zen_OnEnterCatchFilterFunction_t m_onEnterCatchFilter;
    zen_OnExitCatchFilterFunction_t m_onExitCatchFilter;

    zen_OnEnterFinallyClauseFunction_t m_onEnterFinallyClause;
    zen_OnExitFinallyClauseFunction_t m_onExitFinallyClause;

    zen_OnEnterSynchronizeStatementFunction_t m_onEnterSynchronizeStatement;
    zen_OnExitSynchronizeStatementFunction_t m_onExitSynchronizeStatement;

    zen_OnEnterWithStatementFunction_t m_onEnterWithStatement;
    zen_OnExitWithStatementFunction_t m_onExitWithStatement;

    zen_OnEnterWithParametersFunction_t m_onEnterWithParameters;
    zen_OnExitWithParametersFunction_t m_onExitWithParameters;

    zen_OnEnterWithParameterFunction_t m_onEnterWithParameter;
    zen_OnExitWithParameterFunction_t m_onExitWithParameter;

    zen_OnEnterClassDeclarationFunction_t m_onEnterClassDeclaration;
    zen_OnExitClassDeclarationFunction_t m_onExitClassDeclaration;

    zen_OnEnterClassExtendsClauseFunction_t m_onEnterClassExtendsClause;
    zen_OnExitClassExtendsClauseFunction_t m_onExitClassExtendsClause;

    zen_OnEnterClassSuiteFunction_t m_onEnterClassSuite;
    zen_OnExitClassSuiteFunction_t m_onExitClassSuite;

    zen_OnEnterClassMemberFunction_t m_onEnterClassMember;
    zen_OnExitClassMemberFunction_t m_onExitClassMember;

//     zen_OnEnterConstructorDeclarationFunction_t m_onEnterConstructorDeclaration;
//     zen_OnExitConstructorDeclarationFunction_t m_onExitConstructorDeclaration;

    zen_OnEnterEnumerationDeclarationFunction_t m_onEnterEnumerationDeclaration;
    zen_OnExitEnumerationDeclarationFunction_t m_onExitEnumerationDeclaration;

    zen_OnEnterEnumerationBaseClauseFunction_t m_onEnterEnumerationBaseClause;
    zen_OnExitEnumerationBaseClauseFunction_t m_onExitEnumerationBaseClause;

    zen_OnEnterEnumerationSuiteFunction_t m_onEnterEnumerationSuite;
    zen_OnExitEnumerationSuiteFunction_t m_onExitEnumerationSuite;

    zen_OnEnterEnumerateFunction_t m_onEnterEnumerate;
    zen_OnExitEnumerateFunction_t m_onExitEnumerate;

    zen_OnEnterExpressionsFunction_t m_onEnterExpressions;
    zen_OnExitExpressionsFunction_t m_onExitExpressions;

    zen_OnEnterExpressionFunction_t m_onEnterExpression;
    zen_OnExitExpressionFunction_t m_onExitExpression;

    zen_OnEnterAssignmentExpressionFunction_t m_onEnterAssignmentExpression;
    zen_OnExitAssignmentExpressionFunction_t m_onExitAssignmentExpression;

    zen_OnEnterConditionalExpressionFunction_t m_onEnterConditionalExpression;
    zen_OnExitConditionalExpressionFunction_t m_onExitConditionalExpression;

    zen_OnEnterLogicalOrExpressionFunction_t m_onEnterLogicalOrExpression;
    zen_OnExitLogicalOrExpressionFunction_t m_onExitLogicalOrExpression;

    zen_OnEnterLogialAndExpressionFunction_t m_onEnterLogicalAndExpression;
    zen_OnExitLogicalAndExpressionFunction_t m_onExitLogicalAndExpression;

    zen_OnEnterInclusiveOrExpressionFunction_t m_onEnterInclusiveOrExpression;
    zen_OnExitInclusiveOrExpressionFunction_t m_onExitInclusiveOrExpression;

    zen_OnEnterExclusiveOrExpressionFunction_t m_onEnterExclusiveOrExpression;
    zen_OnExitExclusiveOrExpressionFunction_t m_onExitExclusiveOrExpression;

    zen_OnEnterAndExpressionFunction_t m_onEnterAndExpression;
    zen_OnExitAndExpressionFunction_t m_onExitAndExpression;

    zen_OnEnterEqualityExpressionFunction_t m_onEnterEqualityExpression;
    zen_OnExitEqualityExpressionFunction_t m_onExitEqualityExpression;

    zen_OnEnterRelationalExpressionFunction_t m_onEnterRelationalExpression;
    zen_OnExitRelationalExpressionFunction_t m_onExitRelationalExpression;

    zen_OnEnterShiftExpressionFunction_t m_onEnterShiftExpression;
    zen_OnExitShiftExpressionFunction_t m_onExitShiftExpression;

    zen_OnEnterAdditiveExpressionFunction_t m_onEnterAdditiveExpression;
    zen_OnExitAdditiveExpressionFunction_t m_onExitAdditiveExpression;

    zen_OnEnterMultiplicativeExpressionFunction_t m_onEnterMultiplicativeExpression;
    zen_OnExitMultiplicativeExpressionFunction_t m_onExitMultiplicativeExpression;

    zen_OnEnterUnaryExpressionFunction_t m_onEnterUnaryExpression;
    zen_OnExitUnaryExpressionFunction_t m_onExitUnaryExpression;

    zen_OnEnterPostfixExpressionFunction_t m_onEnterPostfixExpression;
    zen_OnExitPostfixExpressionFunction_t m_onExitPostfixExpression;

    zen_OnEnterSubscriptFunction_t m_onEnterSubscript;
    zen_OnExitSubscriptFunction_t m_onExitSubscript;

    zen_OnEnterFunctionArgumentsFunction_t m_onEnterFunctionArguments;
    zen_OnExitFunctionArgumentsFunction_t m_onExitFunctionArguments;

    zen_OnEnterMemberAccessFunction_t m_onEnterMemberAccess;
    zen_OnExitMemberAccessFunction_t m_onExitMemberAccess;

    zen_OnEnterPostfixOperatorFunction_t m_onEnterPostfixOperator;
    zen_OnExitPostfixOperatorFunction_t m_onExitPostfixOperator;

    zen_OnEnterPrimaryExpressionFunction_t m_onEnterPrimaryExpression;
    zen_OnExitPrimaryExpressionFunction_t m_onExitPrimaryExpression;

    zen_OnEnterMapExpressionFunction_t m_onEnterMapExpression;
    zen_OnExitMapExpressionFunction_t m_onExitMapExpression;

    zen_OnEnterMapEntriesFunction_t m_onEnterMapEntries;
    zen_OnExitMapEntriesFunction_t m_onExitMapEntries;

    zen_OnEnterMapEntryFunction_t m_onEnterMapEntry;
    zen_OnExitMapEntryFunction_t m_onExitMapEntry;

    zen_OnEnterListExpressionFunction_t m_onEnterListExpression;
    zen_OnExitListExpressionFunction_t m_onExitListExpression;

    zen_OnEnterNewExpressionFunction_t m_onEnterNewExpression;
    zen_OnExitNewExpressionFunction_t m_onExitNewExpression;

    zen_ASTWalkerState_t m_walkerState;
    void* m_context;
};

/**
 * @memberof ASTListener
 */
typedef struct zen_ASTListener_t zen_ASTListener_t;

/**
 * @memberof ASTListener
 */
zen_ASTListener_t* zen_ASTListener_newWithContext(void* context);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_delete(zen_ASTListener_t* listener);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onVisitErrorNode(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onVisitTerminal(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterEveryRule(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitEveryRule(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterCompilationUnit(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitCompilationUnit(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterImportDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitImportDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterAnnotatedComponentDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitAnnotatedComponentDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterAnnotations(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitAnnotations(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterAnnotation(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitAnnotation(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterAnnotationType(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitAnnotationType(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterAnnotationAttribute(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitAnnotationAttribute(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterComponentDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitComponentDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterFunctionDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitFunctionDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterFunctionParameters(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitFunctionParameters(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterFunctionBody(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitFunctionBody(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterStatementSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitStatementSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterSimpleStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitSimpleStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterEmptyStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitEmptyStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterVariableDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitVariableDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterVariableDeclarator(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitVariableDeclarator(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterConstantDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitConstantDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterConstantDeclarator(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitConstantDeclarator(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterAssertStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitAssertStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterBreakStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitBreakStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterContinueStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitContinueStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterReturnStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitReturnStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterThrowStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitThrowStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterCompoundStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitCompoundStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterIfStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitIfStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterIfClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitIfClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterElseIfClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitElseIfClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterElseClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitElseClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterIterativeStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitIterativeStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterLabel(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitLabel(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterWhileStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitWhileStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterForStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitForStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterForParameters(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitForParameters(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterTryStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitTryStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterTryClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitTryClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterCatchClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitCatchClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterCatchFilter(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitCatchFilter(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterFinallyClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitFinallyClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterSynchronizeStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitSynchronizeStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterWithStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitWithStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterWithParameters(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitWithParameters(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterWithParameter(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitWithParameter(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterClassDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitClassDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterClassExtendsClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitClassExtendsClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterClassSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitClassSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterClassMember(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitClassMember(zen_ASTListener_t* listener, zen_ASTNode_t* node);
// void zen_ASTListener_onEnterConstructorDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);
// void zen_ASTListener_onExitConstructorDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterEnumerationDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitEnumerationDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterEnumrationBaseClass(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitEnumerationBaseClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterEnumerationSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitEnumerationSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterEnumerate(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitEnumerate(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterExpressions(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitExpressions(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterAssignmentExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitAssignmentExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterConditionalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitConditionalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterLogicalOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitLogicalOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterLogicalAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitLogicalAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterInclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitInclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterExclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitExclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterEqualityExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitEqualityExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterRelationalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitRelationalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterShiftExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitShiftExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterAdditiveExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitAdditiveExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterMultiplicativeExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitMultiplicativeExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterUnaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitUnaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterPostfixExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitPostfixExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterSubscript(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitSubscript(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterFunctionArguments(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitFunctionArguments(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterMemberAccess(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitMemberAccess(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterPostfixOperator(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitPostfixOperator(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterPrimaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitPrimaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterMapExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitMapExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterMapEntries(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitMapEntries(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterMapEntry(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitMapEntry(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterListExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitListExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onEnterNewExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_ASTListener_onExitNewExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

// Skip Children

void zen_ASTListener_skipChildren(zen_ASTListener_t* listener);

// Vist Children

void zen_ASTListener_visitChildren(zen_ASTListener_t* listener);

#endif /* COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_LISTENER_H */