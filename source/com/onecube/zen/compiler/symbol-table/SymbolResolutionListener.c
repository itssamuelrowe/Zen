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

// Sunday, March 11, 2018

#include <jtk/collection/stack/LinkedStack.h>
#include <jtk/collection/array/Arrays.h>
#include <jtk/core/StringBuilder.h>

#include <com/onecube/zen/compiler/lexer/Token.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/ast/ASTWalker.h>
#include <com/onecube/zen/compiler/ast/context/Context.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolResolutionListener.h>
#include <com/onecube/zen/compiler/symbol-table/Symbol.h>

/*
 * The following text describes a naive algorithm that I developed to analyze
 * if the left-value evaluates to a placeholder. I am not sure if the algorithm
 * described here already exists. If it does not, I would like to call it
 * the "Placeholder-Value AST Annotation Method". Further, please let me know
 * if there is a better way to do this.
 *
 * A placeholder is a location where a reference can be stored, it may be a local
 * variable or a local class member, or an object's attribute. A placeholder
 * is commonly referred to as lvalue, a value that can be the target of an
 * assignment. The "l" stands for "left", as in the left hand side of the
 * assignment operator.
 *
 * A consequent is a consequence of any expression, such as, function invocation,
 * variable/constant reference, addition, subtraction, multiplication, and so on.
 * It may produce either a placeholder or a value. A consequent is generally
 * known as rvalue, a result that is produced by an expression that appears on
 * the right hand side of the assignment operator. The "r" stands for "right",
 * as in the right hand side of the assignment operator. Henceforth in this text,
 * I use consequent and rvalue interchangeably, unless specified
 * otherwise.
 *
 * The expression `x = (y = 5)` can be represented with the following abstract
 * syntax tree (AST).
 *
 *   =V
 *  / \
 * xP  =V
 *    / \
 *   yP  5V
 *
 * Here, V and P are annotations that imply consequent and placeholder, respectively.
 *
 * 1. Consider a variable `label`, which is initialized to 'unknown'.
 * 2. Walk down the abstract syntax tree with an inorder depth first
 *    fashion.
 * 3. At any given node, if the term produces a consequent, the label is marked
 *    as 'consequent'.
 * 4. At any given node, if the term produces a placholder, the label is marked
 *    as 'placholder'. Since the definition of consequent cleary indicates that
 *    a consequent is a superset of the placeholder, all the terms can be
 *    labelled as 'consequent'. However, this behavior is not desirable. Therefore,
 *    the 'placholder' label has more priority. For example, `x`, a variable
 *    reference is always labelled as placeholder, even if it appears on the
 *    right hand side of an equation.
 * 5. After every node in the expression's AST is visited, the label contained
 *    by the `label` variable determines the nature of the expression.
 *
 * It should be noted that, certain a priori information is needed to handle
 * special situations. For example, the algorithm cannot differentiate between
 * a member attribute access expression and a member function invocation, if the
 * design of the AST is inherently unfavorable to the algorithm. In such cases,
 * special supervision is required.
 *
 * The efficiency of the algorithm can be drastically improved without visiting
 * every node in the AST, provided that the annotation is required only to
 * determine if an expression evaluates to placeholder or a consequent.
 * Only the root nodes of the expressions need to be inspected!
 *
 * Consider the following AST, with all its nodes annotated.
 *   =V
 *  / \
 * xP  =V
 *    / \
 *   yP  5V
 *
 * To determine if the left hand expression is a placeholder and the right hand
 * side expression is a consequent, the following annotation is satisfactory.
 *   =
 *  / \
 * xP  =V
 *    / \
 *   y   5
 */

zen_SymbolResolutionListener_t* zen_SymbolResolutionListener_new(
    zen_Compiler_t* compiler) {
    zen_SymbolResolutionListener_t* listener = zen_Memory_allocate(zen_SymbolResolutionListener_t, 1);
    listener->m_compiler = compiler;
    listener->m_astListener = zen_ASTListener_newWithContext(listener);
    listener->m_symbolTable = NULL;
    listener->m_scopes = NULL;
    listener->m_label = ZEN_EXPRESSION_ANNOTATION_UNKNOWN;
    listener->m_functionIndex = 0;
    listener->m_fieldIndex = 0;

    zen_ASTListener_t* astListener = listener->m_astListener;

    astListener->m_onVisitErrorNode = zen_SymbolResolutionListener_onVisitErrorNode;
    astListener->m_onVisitTerminal = zen_SymbolResolutionListener_onVisitTerminal;

    astListener->m_onEnterEveryRule = zen_SymbolResolutionListener_onEnterEveryRule;
    astListener->m_onExitEveryRule = zen_SymbolResolutionListener_onExitEveryRule;

    astListener->m_onEnterCompilationUnit = zen_SymbolResolutionListener_onEnterCompilationUnit;
    astListener->m_onExitCompilationUnit = zen_SymbolResolutionListener_onExitCompilationUnit;

    astListener->m_onEnterImportDeclaration = zen_SymbolResolutionListener_onEnterImportDeclaration;
    astListener->m_onExitImportDeclaration = zen_SymbolResolutionListener_onExitImportDeclaration;

    astListener->m_onEnterAnnotatedComponentDeclaration = zen_SymbolResolutionListener_onEnterAnnotatedComponentDeclaration;
    astListener->m_onExitAnnotatedComponentDeclaration = zen_SymbolResolutionListener_onExitAnnotatedComponentDeclaration;

    astListener->m_onEnterAnnotations = zen_SymbolResolutionListener_onEnterAnnotations;
    astListener->m_onExitAnnotations = zen_SymbolResolutionListener_onExitAnnotations;

    astListener->m_onEnterAnnotation = zen_SymbolResolutionListener_onEnterAnnotation;
    astListener->m_onExitAnnotation = zen_SymbolResolutionListener_onExitAnnotation;

    astListener->m_onEnterAnnotationType = zen_SymbolResolutionListener_onEnterAnnotationType;
    astListener->m_onExitAnnotationType = zen_SymbolResolutionListener_onExitAnnotationType;

    astListener->m_onEnterAnnotationAttribute = zen_SymbolResolutionListener_onEnterAnnotationAttribute;
    astListener->m_onExitAnnotationAttribute = zen_SymbolResolutionListener_onExitAnnotationAttribute;

    astListener->m_onEnterComponentDeclaration = zen_SymbolResolutionListener_onEnterComponentDeclaration;
    astListener->m_onExitComponentDeclaration = zen_SymbolResolutionListener_onExitComponentDeclaration;

    astListener->m_onEnterFunctionDeclaration = zen_SymbolResolutionListener_onEnterFunctionDeclaration;
    astListener->m_onExitFunctionDeclaration = zen_SymbolResolutionListener_onExitFunctionDeclaration;

    astListener->m_onEnterFunctionParameters = zen_SymbolResolutionListener_onEnterFunctionParameters;
    astListener->m_onExitFunctionParameters = zen_SymbolResolutionListener_onExitFunctionParameters;

    astListener->m_onEnterFunctionBody = zen_SymbolResolutionListener_onEnterFunctionBody;
    astListener->m_onExitFunctionBody = zen_SymbolResolutionListener_onExitFunctionBody;

    astListener->m_onEnterStatementSuite = zen_SymbolResolutionListener_onEnterStatementSuite;
    astListener->m_onExitStatementSuite = zen_SymbolResolutionListener_onExitStatementSuite;

    astListener->m_onEnterSimpleStatement = zen_SymbolResolutionListener_onEnterSimpleStatement;
    astListener->m_onExitSimpleStatement = zen_SymbolResolutionListener_onExitSimpleStatement;

    astListener->m_onEnterStatement = zen_SymbolResolutionListener_onEnterStatement;
    astListener->m_onExitStatement = zen_SymbolResolutionListener_onExitStatement;

    astListener->m_onEnterEmptyStatement = zen_SymbolResolutionListener_onEnterEmptyStatement;
    astListener->m_onExitEmptyStatement = zen_SymbolResolutionListener_onExitEmptyStatement;

    astListener->m_onEnterVariableDeclaration = zen_SymbolResolutionListener_onEnterVariableDeclaration;
    astListener->m_onExitVariableDeclaration = zen_SymbolResolutionListener_onExitVariableDeclaration;

    astListener->m_onEnterVariableDeclarator = zen_SymbolResolutionListener_onEnterVariableDeclarator;
    astListener->m_onExitVariableDeclarator = zen_SymbolResolutionListener_onExitVariableDeclarator;

    astListener->m_onEnterConstantDeclaration = zen_SymbolResolutionListener_onEnterConstantDeclaration;
    astListener->m_onExitConstantDeclaration = zen_SymbolResolutionListener_onExitConstantDeclaration;

    astListener->m_onEnterConstantDeclarator = zen_SymbolResolutionListener_onEnterConstantDeclarator;
    astListener->m_onExitConstantDeclarator = zen_SymbolResolutionListener_onExitConstantDeclarator;

    astListener->m_onEnterAssertStatement = zen_SymbolResolutionListener_onEnterAssertStatement;
    astListener->m_onExitAssertStatement = zen_SymbolResolutionListener_onExitAssertStatement;

    astListener->m_onEnterBreakStatement = zen_SymbolResolutionListener_onEnterBreakStatement;
    astListener->m_onExitBreakStatement = zen_SymbolResolutionListener_onExitBreakStatement;

    astListener->m_onEnterContinueStatement = zen_SymbolResolutionListener_onEnterContinueStatement;
    astListener->m_onExitContinueStatement = zen_SymbolResolutionListener_onExitContinueStatement;

    astListener->m_onEnterReturnStatement = zen_SymbolResolutionListener_onEnterReturnStatement;
    astListener->m_onExitReturnStatement = zen_SymbolResolutionListener_onExitReturnStatement;

    astListener->m_onEnterThrowStatement = zen_SymbolResolutionListener_onEnterThrowStatement;
    astListener->m_onExitThrowStatement = zen_SymbolResolutionListener_onExitThrowStatement;

    astListener->m_onEnterCompoundStatement = zen_SymbolResolutionListener_onEnterCompoundStatement;
    astListener->m_onExitCompoundStatement = zen_SymbolResolutionListener_onExitCompoundStatement;

    astListener->m_onEnterIfStatement = zen_SymbolResolutionListener_onEnterIfStatement;
    astListener->m_onExitIfStatement = zen_SymbolResolutionListener_onExitIfStatement;

    astListener->m_onEnterIfClause = zen_SymbolResolutionListener_onEnterIfClause;
    astListener->m_onExitIfClause = zen_SymbolResolutionListener_onExitIfClause;

    astListener->m_onEnterElseIfClause = zen_SymbolResolutionListener_onEnterElseIfClause;
    astListener->m_onExitElseIfClause = zen_SymbolResolutionListener_onExitElseIfClause;

    astListener->m_onEnterElseClause = zen_SymbolResolutionListener_onEnterElseClause;
    astListener->m_onExitElseClause = zen_SymbolResolutionListener_onExitElseClause;

    astListener->m_onEnterIterativeStatement = zen_SymbolResolutionListener_onEnterIterativeStatement;
    astListener->m_onExitIterativeStatement = zen_SymbolResolutionListener_onExitIterativeStatement;

    astListener->m_onEnterLabelClause = zen_SymbolResolutionListener_onEnterLabel;
    astListener->m_onExitLabelClause = zen_SymbolResolutionListener_onExitLabel;

    astListener->m_onEnterWhileStatement = zen_SymbolResolutionListener_onEnterWhileStatement;
    astListener->m_onExitWhileStatement = zen_SymbolResolutionListener_onExitWhileStatement;

    astListener->m_onEnterForStatement = zen_SymbolResolutionListener_onEnterForStatement;
    astListener->m_onExitForStatement = zen_SymbolResolutionListener_onExitForStatement;

    astListener->m_onEnterForParameter = zen_SymbolResolutionListener_onEnterForParameters;
    astListener->m_onExitForParameter = zen_SymbolResolutionListener_onExitForParameters;

    astListener->m_onEnterTryStatement = zen_SymbolResolutionListener_onEnterTryStatement;
    astListener->m_onExitTryStatement = zen_SymbolResolutionListener_onExitTryStatement;

    astListener->m_onEnterTryClause = zen_SymbolResolutionListener_onEnterTryClause;
    astListener->m_onExitTryClause = zen_SymbolResolutionListener_onExitTryClause;

    astListener->m_onEnterCatchClause = zen_SymbolResolutionListener_onEnterCatchClause;
    astListener->m_onExitCatchClause = zen_SymbolResolutionListener_onExitCatchClause;

    astListener->m_onEnterCatchFilter = zen_SymbolResolutionListener_onEnterCatchFilter;
    astListener->m_onExitCatchFilter = zen_SymbolResolutionListener_onExitCatchFilter;

    astListener->m_onEnterFinallyClause = zen_SymbolResolutionListener_onEnterFinallyClause;
    astListener->m_onExitFinallyClause = zen_SymbolResolutionListener_onExitFinallyClause;

    astListener->m_onEnterSynchronizeStatement = zen_SymbolResolutionListener_onEnterSynchronizeStatement;
    astListener->m_onExitSynchronizeStatement = zen_SymbolResolutionListener_onExitSynchronizeStatement;

    astListener->m_onEnterWithStatement = zen_SymbolResolutionListener_onEnterWithStatement;
    astListener->m_onExitWithStatement = zen_SymbolResolutionListener_onExitWithStatement;

    astListener->m_onEnterClassDeclaration = zen_SymbolResolutionListener_onEnterClassDeclaration;
    astListener->m_onExitClassDeclaration = zen_SymbolResolutionListener_onExitClassDeclaration;

    astListener->m_onEnterClassExtendsClause = zen_SymbolResolutionListener_onEnterClassExtendsClause;
    astListener->m_onExitClassExtendsClause = zen_SymbolResolutionListener_onExitClassExtendsClause;

    astListener->m_onEnterClassSuite = zen_SymbolResolutionListener_onEnterClassSuite;
    astListener->m_onExitClassSuite = zen_SymbolResolutionListener_onExitClassSuite;

    astListener->m_onEnterClassMember = zen_SymbolResolutionListener_onEnterClassMember;
    astListener->m_onExitClassMember = zen_SymbolResolutionListener_onExitClassMember;

    // astListener->m_onEnterConstructorDeclaration = zen_SymbolResolutionListener_onEnterConstructorDeclaration;
    // astListener->m_onExitConstructorDeclaration = zen_SymbolResolutionListener_onExitConstructorDeclaration;

    astListener->m_onEnterEnumerationDeclaration = zen_SymbolResolutionListener_onEnterEnumerationDeclaration;
    astListener->m_onExitEnumerationDeclaration = zen_SymbolResolutionListener_onExitEnumerationDeclaration;

    astListener->m_onEnterEnumerationBaseClause = zen_SymbolResolutionListener_onEnterEnumrationBaseClass;
    astListener->m_onExitEnumerationBaseClause = zen_SymbolResolutionListener_onExitEnumerationBaseClause;

    astListener->m_onEnterEnumerationSuite = zen_SymbolResolutionListener_onEnterEnumerationSuite;
    astListener->m_onExitEnumerationSuite = zen_SymbolResolutionListener_onExitEnumerationSuite;

    astListener->m_onEnterEnumerate = zen_SymbolResolutionListener_onEnterEnumerate;
    astListener->m_onExitEnumerate = zen_SymbolResolutionListener_onExitEnumerate;

    astListener->m_onEnterExpressions = zen_SymbolResolutionListener_onEnterExpressions;
    astListener->m_onExitExpressions = zen_SymbolResolutionListener_onExitExpressions;

    astListener->m_onEnterExpression = zen_SymbolResolutionListener_onEnterExpression;
    astListener->m_onExitExpression = zen_SymbolResolutionListener_onExitExpression;

    astListener->m_onEnterAssignmentExpression = zen_SymbolResolutionListener_onEnterAssignmentExpression;
    astListener->m_onExitAssignmentExpression = zen_SymbolResolutionListener_onExitAssignmentExpression;

    astListener->m_onEnterConditionalExpression = zen_SymbolResolutionListener_onEnterConditionalExpression;
    astListener->m_onExitConditionalExpression = zen_SymbolResolutionListener_onExitConditionalExpression;

    astListener->m_onEnterLogicalOrExpression = zen_SymbolResolutionListener_onEnterLogicalOrExpression;
    astListener->m_onExitLogicalOrExpression = zen_SymbolResolutionListener_onExitLogicalOrExpression;

    astListener->m_onEnterLogicalAndExpression = zen_SymbolResolutionListener_onEnterLogicalAndExpression;
    astListener->m_onExitLogicalAndExpression = zen_SymbolResolutionListener_onExitLogicalAndExpression;

    astListener->m_onEnterInclusiveOrExpression = zen_SymbolResolutionListener_onEnterInclusiveOrExpression;
    astListener->m_onExitInclusiveOrExpression = zen_SymbolResolutionListener_onExitInclusiveOrExpression;

    astListener->m_onEnterExclusiveOrExpression = zen_SymbolResolutionListener_onEnterExclusiveOrExpression;
    astListener->m_onExitExclusiveOrExpression = zen_SymbolResolutionListener_onExitExclusiveOrExpression;

    astListener->m_onEnterAndExpression = zen_SymbolResolutionListener_onEnterAndExpression;
    astListener->m_onExitAndExpression = zen_SymbolResolutionListener_onExitAndExpression;

    astListener->m_onEnterEqualityExpression = zen_SymbolResolutionListener_onEnterEqualityExpression;
    astListener->m_onExitEqualityExpression = zen_SymbolResolutionListener_onExitEqualityExpression;

    astListener->m_onEnterRelationalExpression = zen_SymbolResolutionListener_onEnterRelationalExpression;
    astListener->m_onExitRelationalExpression = zen_SymbolResolutionListener_onExitRelationalExpression;

    astListener->m_onEnterShiftExpression = zen_SymbolResolutionListener_onEnterShiftExpression;
    astListener->m_onExitShiftExpression = zen_SymbolResolutionListener_onExitShiftExpression;

    astListener->m_onEnterAdditiveExpression = zen_SymbolResolutionListener_onEnterAdditiveExpression;
    astListener->m_onExitAdditiveExpression = zen_SymbolResolutionListener_onExitAdditiveExpression;

    astListener->m_onEnterMultiplicativeExpression = zen_SymbolResolutionListener_onEnterMultiplicativeExpression;
    astListener->m_onExitMultiplicativeExpression = zen_SymbolResolutionListener_onExitMultiplicativeExpression;

    astListener->m_onEnterUnaryExpression = zen_SymbolResolutionListener_onEnterUnaryExpression;
    astListener->m_onExitUnaryExpression = zen_SymbolResolutionListener_onExitUnaryExpression;

    astListener->m_onEnterPostfixExpression = zen_SymbolResolutionListener_onEnterPostfixExpression;
    astListener->m_onExitPostfixExpression = zen_SymbolResolutionListener_onExitPostfixExpression;

    astListener->m_onEnterSubscript = zen_SymbolResolutionListener_onEnterSubscript;
    astListener->m_onExitSubscript = zen_SymbolResolutionListener_onExitSubscript;

    astListener->m_onEnterFunctionArguments = zen_SymbolResolutionListener_onEnterFunctionArguments;
    astListener->m_onExitFunctionArguments = zen_SymbolResolutionListener_onExitFunctionArguments;

    astListener->m_onEnterMemberAccess = zen_SymbolResolutionListener_onEnterMemberAccess;
    astListener->m_onExitMemberAccess = zen_SymbolResolutionListener_onExitMemberAccess;

    astListener->m_onEnterPostfixOperator = zen_SymbolResolutionListener_onEnterPostfixOperator;
    astListener->m_onExitPostfixOperator = zen_SymbolResolutionListener_onExitPostfixOperator;

    astListener->m_onEnterPrimaryExpression = zen_SymbolResolutionListener_onEnterPrimaryExpression;
    astListener->m_onExitPrimaryExpression = zen_SymbolResolutionListener_onExitPrimaryExpression;

    astListener->m_onEnterMapExpression = zen_SymbolResolutionListener_onEnterMapExpression;
    astListener->m_onExitMapExpression = zen_SymbolResolutionListener_onExitMapExpression;

    astListener->m_onEnterMapEntries = zen_SymbolResolutionListener_onEnterMapEntries;
    astListener->m_onExitMapEntries = zen_SymbolResolutionListener_onExitMapEntries;

    astListener->m_onEnterMapEntry = zen_SymbolResolutionListener_onEnterMapEntry;
    astListener->m_onExitMapEntry = zen_SymbolResolutionListener_onExitMapEntry;

    astListener->m_onEnterListExpression = zen_SymbolResolutionListener_onEnterListExpression;
    astListener->m_onExitListExpression = zen_SymbolResolutionListener_onExitListExpression;

    astListener->m_onEnterNewExpression = zen_SymbolResolutionListener_onEnterNewExpression;
    astListener->m_onExitNewExpression = zen_SymbolResolutionListener_onExitNewExpression;

    return listener;
}

void zen_SymbolResolutionListener_delete(zen_SymbolResolutionListener_t* listener) {
    jtk_Assert_assertObject(listener, "The specified listener is null.");

    zen_ASTListener_delete(listener->m_astListener);
    jtk_Memory_deallocate(listener);
}

zen_ASTListener_t* zen_SymbolResolutionListener_getASTListener(zen_SymbolResolutionListener_t* listener) {
    jtk_Assert_assertObject(listener, "The specified listener is null.");
    return listener->m_astListener;
}

// Reset

void zen_SymbolResolutionListener_reset(zen_SymbolResolutionListener_t* listener,
    zen_SymbolTable_t* symbolTable, zen_ASTAnnotations_t* scopes) {

    listener->m_symbolTable = symbolTable;
    listener->m_scopes = scopes;
    listener->m_label = ZEN_EXPRESSION_ANNOTATION_UNKNOWN;
    listener->m_fieldIndex = 0;
    listener->m_functionIndex = 0;
}

// Event Handlers

void zen_SymbolResolutionListener_onVisitErrorNode(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    fprintf(stderr, "[warning] Cannot resolve symbols on erroneous AST.\n");
}

void zen_SymbolResolutionListener_onVisitTerminal(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onEnterEveryRule(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitEveryRule(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

/* compilationUnit */

void zen_SymbolResolutionListener_applyDefaultImports(zen_SymbolResolutionListener_t* listener) {
    zen_Compiler_t* compiler = listener->m_compiler;
    zen_Symbol_t* symbol = zen_Compiler_resolveSymbol(compiler, "zen.core.ZenKernel", 18);
    zen_ClassSymbol_t* classSymbol = &symbol->m_context.m_asClass;
    zen_Scope_t* currentScope = listener->m_symbolTable->m_currentScope;

    jtk_Iterator_t* iterator = jtk_HashMap_getEntryIterator(classSymbol->m_classScope->m_symbols);
    while (jtk_Iterator_hasNext(iterator)) {
        jtk_HashMapEntry_t* entry = (jtk_HashMapEntry_t*)jtk_Iterator_getNext(iterator);
        const uint8_t* name = (const uint8_t*)entry->m_key;
        zen_Symbol_t* functionSymbol = (zen_Symbol_t*)entry->m_value;
        zen_Scope_defineEx(currentScope, name, -1, functionSymbol);
    }
    jtk_Iterator_delete(iterator);
}

void zen_SymbolResolutionListener_onEnterCompilationUnit(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_Scope_t* scope = zen_ASTAnnotations_get(listener->m_scopes, node);
    zen_SymbolTable_setCurrentScope(listener->m_symbolTable, scope);

    if (!listener->m_compiler->m_coreApi) {
        zen_SymbolResolutionListener_applyDefaultImports(listener);
    }
}

void zen_SymbolResolutionListener_onExitCompilationUnit(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_SymbolTable_invalidateCurrentScope(listener->m_symbolTable);
}

// importDeclaration

zen_Symbol_t* zen_SymbolResolutionListener_createExternalSymbol(
    zen_SymbolResolutionListener_t* listener, zen_Symbol_t* symbol,
    zen_Scope_t* enclosingScope, zen_ASTNode_t* identifier) {
    zen_Symbol_t* externalSymbol = jtk_Memory_allocate(zen_Symbol_t, 1);
    jtk_Arrays_copyEx_b((const int8_t*)symbol, sizeof (zen_Symbol_t), 0,
        (const int8_t*)externalSymbol, sizeof (zen_Symbol_t), 0,
        sizeof (zen_Symbol_t));
    externalSymbol->m_enclosingScope = enclosingScope;
    externalSymbol->m_identifier = identifier;
    externalSymbol->m_flags |= ZEN_SYMBOL_FLAG_EXTERNAL;

    return externalSymbol;
}

void zen_SymbolResolutionListener_onEnterImportDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_SymbolResolutionListener_t* listener =
        (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_Compiler_t* compiler = listener->m_compiler;
    zen_ErrorHandler_t* errorHandler = compiler->m_errorHandler;
    zen_ImportDeclarationContext_t* context = (zen_ImportDeclarationContext_t*)node->m_context;

    if (context->m_wildcard) {
        // TODO
    }
    else {
        int32_t identifierCount = jtk_ArrayList_getSize(context->m_identifiers);
        zen_ASTNode_t* lastIdentifier = jtk_ArrayList_getValue(context->m_identifiers,
            identifierCount - 1);
        zen_Token_t* lastIdentifierToken = (zen_Token_t*)lastIdentifier->m_context;

        jtk_StringBuilder_t* builder = jtk_StringBuilder_new();
        int32_t i;
        for (i = 0; i < identifierCount; i++) {
            zen_ASTNode_t* identifier = (zen_ASTNode_t*)jtk_ArrayList_getValue(context->m_identifiers, i);
            zen_Token_t* identifierToken = (zen_Token_t*)identifier->m_context;
            jtk_StringBuilder_appendEx_z(builder, identifierToken->m_text, identifierToken->m_length);
            if (i + 1 < identifierCount) {
                jtk_StringBuilder_append_c(builder, '.');
            }
        }
        int32_t qualifiedNameSize;
        uint8_t* qualifiedName = jtk_StringBuilder_toCString(builder, &qualifiedNameSize);
        jtk_StringBuilder_delete(builder);

        zen_Symbol_t* symbol = zen_Compiler_resolveSymbol(compiler, qualifiedName,
            qualifiedNameSize);
        if (symbol == NULL) {
            zen_ErrorHandler_handleSemanticalError(errorHandler, listener,
                ZEN_ERROR_CODE_UNKNOWN_CLASS, lastIdentifierToken);
        }
        else {
            zen_Symbol_t* localSymbol = zen_SymbolTable_resolve(listener->m_symbolTable,
                lastIdentifierToken->m_text);
            if (localSymbol == NULL) {
                zen_Symbol_t* externalSymbol = zen_Symbol_forExternal(lastIdentifier,
                    listener->m_symbolTable->m_currentScope, symbol);
                zen_SymbolTable_define(listener->m_symbolTable, externalSymbol);
            }
            else {
                zen_ErrorHandler_handleSemanticalError(errorHandler,
                    listener, ZEN_ERROR_CODE_REDECLARATION_OF_SYMBOL_PREVIOUSLY_IMPORTED,
                    (zen_Token_t*)localSymbol->m_identifier->m_context);
            }
        }

        jtk_CString_delete(qualifiedName);
    }
}

void zen_SymbolResolutionListener_onExitImportDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// annotatedComponentDeclaration

void zen_SymbolResolutionListener_onEnterAnnotatedComponentDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitAnnotatedComponentDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// annotations

void zen_SymbolResolutionListener_onEnterAnnotations(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitAnnotations(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// annotation

void zen_SymbolResolutionListener_onEnterAnnotation(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitAnnotation(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// annotationType

void zen_SymbolResolutionListener_onEnterAnnotationType(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitAnnotationType(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// annotationAttribute

void zen_SymbolResolutionListener_onEnterAnnotationAttribute(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitAnnotationAttribute(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// componentDeclaration

void zen_SymbolResolutionListener_onEnterComponentDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitComponentDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onEnterFunctionDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_FunctionDeclarationContext_t* context = (zen_FunctionDeclarationContext_t*)node->m_context;

    zen_Scope_t* scope = zen_ASTAnnotations_get(listener->m_scopes, node);
    zen_SymbolTable_setCurrentScope(listener->m_symbolTable, scope);
    zen_Scope_t* enclosingScope = zen_Scope_getEnclosingScope(scope);

    /* Retrieve the identifier associated with the function declaration. */
    zen_ASTNode_t* identifier = context->m_identifier;
    zen_Token_t* identifierToken = (zen_Token_t*)identifier->m_context;
    /* Retrieve the text representation of the identifier. */
    const uint8_t* identifierText = ((zen_Token_t*)identifier->m_context)->m_text;

    zen_FunctionParametersContext_t* functionParametersContext =
        (zen_FunctionParametersContext_t*)context->m_functionParameters->m_context;
    /* Retrieve the fixed parameters associated with the context of the
     * function arguments.
     */
    jtk_ArrayList_t* fixedParameters = functionParametersContext->m_fixedParameters;
    int32_t fixedParameterCount = jtk_ArrayList_getSize(fixedParameters);

    /* Retrieve the symbol table associated with this listener. */
    zen_SymbolTable_t* symbolTable = listener->m_symbolTable;
    zen_Symbol_t* symbol = zen_SymbolTable_resolve(symbolTable, identifierText);
    zen_FunctionSignature_t* signature = zen_Symbol_getFunctionSignature(symbol, fixedParameterCount);
    signature->m_tableIndex = ++listener->m_functionIndex;

    // TODO: In inheritance, the index should be generated after following up
    // the hierarchy to address overriding.

    /* NOTE: Previously a different approach was taken to resolve clashes in
     *       inheritance. The original idea was to apply BFS algorithm against
     *       the inheritance graph. The generated graph tree could be used in
     *       a  matrix to resolve clashes by comparing each class in the
     *       cardinal set. This algorithm is active such that the compiler
     *       quickly detects clashes.
     *
     *       The design of the virtual machine allows interchangeable references
     *       of classes and other symbols without the language barrier. Therefore,
     *       another check should be applied during the resolution of functions
     *       and other symbols.
     *
     *      This introduces two disadvantages to the original algorithm.
     *          - The compiler checks multiple times to detect the same error.
     *          - The complexity of the algorithm is more when compared to latter
     *            algorithm.
     *
     *      Therefore, I am dropping the implementation of the previous algorithm
     *      as it is, which is incomplete.
     */
    /* Resolve member functions with reference to superclasses. */
//    if (zen_Scope_isClassScope(enclosingScope)) {
//        zen_ASTNode_t* identifier = context->m_identifier;
//        zen_Token_t* identifierToken = (zen_Token_t*)identifier->m_context;
//        const uint8_t* identifierText = zen_Token_getText(identifierToken);
//
//        zen_ClassScope_t* classScope = (zen_ClassSymbol_t*)zen_Scope_getContext(enclosingScope);
//        zen_ClassSymbol_t* classSymbol = (zen_ClassSymbol_t*)classScope->m_classSymbol->m_context;
//
//        zen_LinkedStack_t* stack = zen_LinkedStack_new();
//
//        jtk_ArrayList_t* superClasses = zen_ClassSymbol_getSuperClasses(classSymbol);
//        int32_t size = jtk_ArrayList_getSize(superClasses);
//        int32_t i;
//        for (i = 0; i < size; i++) {
//            zen_Symbol_t* superClassSymbol = (zen_Symbol_t*)jtk_ArrayList_getValue(superClasses, i);
//            zen_ClassSymbol_t* superClassSymbol_ = (zen_ClassSymbol_t*)superClassSymbol->m_context;
//            zen_LinkedStack_push(stack, superClassSymbol_);
//        }
//
//        /* The function is not resolved against the current scope as it is redundant.
//         * Remember, the declaration phase takes care of this.
//         */
//
//        zen_Symbol_t* currentScopeSymbol = (zen_Symbol_t*)jtk_HashMap_getValue(classScope->m_symbols, identifierText);
//
//        while (!zen_LinkedStack_isEmpty(stack)) {
//            zen_ClassSymbol_t* classSymbol = (zen_ClassSymbol_t*)zen_LinkedStack_pop(stack);
//            zen_Scope_t* ts = zen_ClassSymbol_getClassScope(classSymbol);
//            classScope = (zen_ClassScope_t*)ts->m_context;
//
//            zen_Symbol_t* superScopeSymbol = (zen_Symbol_t*)jtk_HashMap_getValue(classScope->m_symbols, identifierText);
//            if (superScopeSymbol != NULL) {
//                if (zen_Symbol_isFunction(superScopeSymbol)) {
//                    /* Thanks to the declaration phase, we can be sure that the current scope symbol
//                     * is a function.
//                     */
//                    jtk_Assert_assertTrue(zen_Symbol_isFunction(currentScopeSymbol), "The declaration phase declared a different symbols with the same identifier.");
//
//                    zen_FunctionSymbol_t* currentScopeFunctionSymbol = (zen_FunctionSymbol_t*)currentScopeSymbol->m_context;
//                    zen_FunctionSymbol_t* superScopeFunctionSymbol = (zen_FunctionSymbol_t*)superScopeSymbol->m_context;
//
//                    if ((currentScopeFunctionSymbol->m_parameterThreshold == -1) &&
//                        (superScopeFunctionSymbol->m_parameterThreshold == -1)) {
//                        printf("Either functions were overridden or overloaded.");
//                    }
//                    else if ((currentScopeFunctionSymbol->m_parameterThreshold != -1) &&
//                        (superScopeFunctionSymbol->m_parameterThreshold != -1)) {
//                        if (currentScopeFunctionSymbol->m_parameterThreshold != superScopeFunctionSymbol->m_parameterThreshold) {
//                            /* zen_Error_t* error = */ zen_ErrorHandler_reportError(NULL, "Multiple overloads with variable parameters across superclass and subclass.", (zen_Token_t*)currentScopeSymbol->m_identifier->m_context);
//                            // zen_Error_addExtra(error, "Previously valid here");
//                       }
//                    }
//                    else if ((currentScopeFunctionSymbol->m_parameterThreshold != -1) &&
//                        (superScopeFunctionSymbol->m_parameterThreshold == -1)) {
//                        jtk_ArrayList_t* superScopeSignatures = zen_FunctionSymbol_getSignatures(superScopeFunctionSymbol);
//                        int32_t superScopeSignatureCount = jtk_ArrayList_getSize(superScopeSignatures);
//                        int32_t j;
//                        for (j = 0; j < superScopeSignatureCount; j++) {
//                            zen_FunctionSignature_t* superScopeSignature = (zen_FunctionSignature_t*)jtk_ArrayList_getValue(superScopeSignatures, j);
//                            int32_t fixedParameterCount = jtk_ArrayList_getSize(superScopeSignature->m_fixedParameters);
//                            if (fixedParameterCount >= currentScopeFunctionSymbol->m_parameterThreshold) {
//                                zen_ErrorHandler_reportError(NULL, "Superclass function exceeds parameter threshold", (zen_Token_t*)superScopeSymbol->m_identifier->m_context);
//                            }
//                        }
//                    }
//                    else if ((currentScopeFunctionSymbol->m_parameterThreshold == -1) &&
//                        (superScopeFunctionSymbol->m_parameterThreshold != -1)) {
//                        jtk_ArrayList_t* currentScopeSignatures = zen_FunctionSymbol_getSignatures(currentScopeFunctionSymbol);
//                        int32_t currentScopeSignatureCount = jtk_ArrayList_getSize(currentScopeSignatures);
//                        int32_t j;
//                        for (j = 0; j < currentScopeSignatureCount; j++) {
//                            zen_FunctionSignature_t* currentScopeSignature = (zen_FunctionSignature_t*)jtk_ArrayList_getValue(currentScopeSignatures, j);
//                            int32_t fixedParameterCount = jtk_ArrayList_getSize(currentScopeSignature->m_fixedParameters);
//                            if (fixedParameterCount >= superScopeFunctionSymbol->m_parameterThreshold) {
//                                zen_ErrorHandler_reportError(NULL, "Subclass function exceeds parameter threshold", (zen_Token_t*)currentScopeSymbol->m_identifier->m_context);
//                            }
//                        }
//                    }
//                }
//            }
//            else {
//                superClasses = zen_ClassSymbol_getSuperClasses(classSymbol);
//                size = jtk_ArrayList_getSize(superClasses);
//                for (i = 0; i < size; i++) {
//                    zen_Symbol_t* rawSuperClassSymbol = jtk_ArrayList_getValue(superClasses, i);
//                    zen_ClassSymbol_t* superClassSymbol = (zen_ClassSymbol_t*)rawSuperClassSymbol->m_context;
//                    zen_LinkedStack_push(stack, superClassSymbol);
//                }
//            }
//        }
//    }
}

void zen_SymbolResolutionListener_onExitFunctionDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_SymbolTable_invalidateCurrentScope(listener->m_symbolTable);
}

// functionParameters

void zen_SymbolResolutionListener_onEnterFunctionParameters(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitFunctionParameters(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// functionBody

void zen_SymbolResolutionListener_onEnterFunctionBody(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitFunctionBody(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onEnterStatementSuite(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;

    zen_Scope_t* scope = zen_ASTAnnotations_get(listener->m_scopes, node);
    zen_SymbolTable_setCurrentScope(listener->m_symbolTable, scope);
}

void zen_SymbolResolutionListener_onExitStatementSuite(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;

    zen_SymbolTable_invalidateCurrentScope(listener->m_symbolTable);
}

// simpleStatement

void zen_SymbolResolutionListener_onEnterSimpleStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitSimpleStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// statement

void zen_SymbolResolutionListener_onEnterStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// emptyStatement

void zen_SymbolResolutionListener_onEnterEmptyStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitEmptyStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// variableDeclaration

void zen_SymbolResolutionListener_onEnterVariableDeclaration(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitVariableDeclaration(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// variableDeclarator

void zen_SymbolResolutionListener_onEnterVariableDeclarator(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitVariableDeclarator(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// constantDeclaration

void zen_SymbolResolutionListener_onEnterConstantDeclaration(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitConstantDeclaration(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// constantDeclarator

void zen_SymbolResolutionListener_onEnterConstantDeclarator(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitConstantDeclarator(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// assertStatement

void zen_SymbolResolutionListener_onEnterAssertStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitAssertStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onEnterBreakStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// breakStatement

void zen_SymbolResolutionListener_onExitBreakStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// continueStatement

void zen_SymbolResolutionListener_onEnterContinueStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitContinueStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// returnStatement

void zen_SymbolResolutionListener_onEnterReturnStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitReturnStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// throwStatement

void zen_SymbolResolutionListener_onEnterThrowStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitThrowStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// compoundStatement

void zen_SymbolResolutionListener_onEnterCompoundStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitCompoundStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// ifStatement

void zen_SymbolResolutionListener_onEnterIfStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitIfStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// ifClause

void zen_SymbolResolutionListener_onEnterIfClause(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitIfClause(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// elseIfClause

void zen_SymbolResolutionListener_onEnterElseIfClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitElseIfClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// elseClause

void zen_SymbolResolutionListener_onEnterElseClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitElseClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// iterativeStatement

void zen_SymbolResolutionListener_onEnterIterativeStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitIterativeStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// label

void zen_SymbolResolutionListener_onEnterLabel(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitLabel(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// whileStatement

void zen_SymbolResolutionListener_onEnterWhileStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitWhileStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// forStatement

void zen_SymbolResolutionListener_onEnterForStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitForStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// forParameters

void zen_SymbolResolutionListener_onEnterForParameters(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitForParameters(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// tryStatement

void zen_SymbolResolutionListener_onEnterTryStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_TryStatementContext_t* context = (zen_TryStatementContext_t*)node->m_context;

    int32_t catchClauseCount = jtk_ArrayList_getSize(context->m_catchClauses);
    int32_t i;
    for (i = 0; i < catchClauseCount; i++) {
        zen_ASTNode_t* catchClause = (zen_ASTNode_t*)jtk_ArrayList_getValue(context->m_catchClauses, i);
        zen_CatchClauseContext_t* catchClauseContext = (zen_CatchClauseContext_t*)catchClause->m_context;

        zen_Scope_t* scope = zen_ASTAnnotations_get(listener->m_scopes, catchClause);
        zen_SymbolTable_setCurrentScope(listener->m_symbolTable, scope);

        /* Visit the scopes of the statement suite specified to the
         * catch clause.
         */
        zen_ASTWalker_walk(astListener, catchClauseContext->m_statementSuite);

        zen_SymbolTable_invalidateCurrentScope(listener->m_symbolTable);
    }

    zen_ASTListener_skipChildren(astListener);
}

void zen_SymbolResolutionListener_onExitTryStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// tryClause

void zen_SymbolResolutionListener_onEnterTryClause(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitTryClause(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// catchClause

void zen_SymbolResolutionListener_onEnterCatchClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitCatchClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// catchFilter

void zen_SymbolResolutionListener_onEnterCatchFilter(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitCatchFilter(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// finallyClause

void zen_SymbolResolutionListener_onEnterFinallyClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitFinallyClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// synchronizeStatement

void zen_SymbolResolutionListener_onEnterSynchronizeStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitSynchronizeStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// withStatement

void zen_SymbolResolutionListener_onEnterWithStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");
}

void zen_SymbolResolutionListener_onExitWithStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");
}

// classDeclaration

void zen_SymbolResolutionListener_onEnterClassDeclaration(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    /* Retrieve the instance of the symbol resolution listener. */
    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    /* Retrieve the scope associated with the class being declared. */
    zen_Scope_t* scope = zen_ASTAnnotations_get(listener->m_scopes, node);
    /* If the definition of the class is duplicate, a scope is not associated with
     * it.
     */
    if (scope != NULL) {
        /* Update the current scope in the symbol table. */
        zen_SymbolTable_setCurrentScope(listener->m_symbolTable, scope);

        zen_ClassDeclarationContext_t* classDeclarationContext = (zen_ClassDeclarationContext_t*)node->m_context;
        if (classDeclarationContext->m_classExtendsClause != NULL) {
            zen_ClassExtendsClauseContext_t* classExtendsClauseContext = classExtendsClauseContext = (zen_ClassExtendsClauseContext_t*)classDeclarationContext->m_classExtendsClause->m_context;

            // TODO: Use a node annotation

            zen_Symbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, zen_Token_getText((zen_Token_t*)classDeclarationContext->m_identifier->m_context));
            zen_ClassSymbol_t* classSymbol = &symbol->m_context.m_asClass;
            jtk_ArrayList_t* superClasses = zen_ClassSymbol_getSuperClasses(classSymbol);

            int32_t superClassCount = jtk_ArrayList_getSize(classExtendsClauseContext->m_typeNames);
            int32_t i;
            for (i = 0; i < superClassCount; i++) {
                zen_ASTNode_t* typeName = (zen_ASTNode_t*)jtk_ArrayList_getValue(classExtendsClauseContext->m_typeNames, i);
                // const uint8_t* qualifiedTypeName = zen_SymbolResolutionListener_getQualifiedTypeName(typeName);
                // TODO!!
                zen_TypeNameContext_t* typeNameContext = (zen_TypeNameContext_t*)(typeName->m_context);
                jtk_ArrayList_t* identifiers = typeNameContext->m_identifiers;
                zen_Token_t* first = ((zen_ASTNode_t*)jtk_ArrayList_getValue(identifiers, 0))->m_context;
                const uint8_t* firstText = zen_Token_getText(first);
                zen_Symbol_t* superClassSymbol = zen_SymbolTable_resolve(listener->m_symbolTable, firstText);
                if (superClassSymbol == NULL) {
                    fprintf(stderr, "[semantic error] Unknown class\n");
                }
                else {
                    jtk_ArrayList_add(superClasses, superClassSymbol);
                }
            }
        }

        zen_ASTWalker_walk(astListener, classDeclarationContext->m_classSuite);

        zen_SymbolTable_invalidateCurrentScope(listener->m_symbolTable);
    }
    zen_ASTListener_skipChildren(astListener);
}

void zen_SymbolResolutionListener_onExitClassDeclaration(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");
}

// classExtends

void zen_SymbolResolutionListener_onEnterClassExtendsClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitClassExtendsClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// classSuite

void zen_SymbolResolutionListener_onEnterClassSuite(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitClassSuite(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// classMember

void zen_SymbolResolutionListener_onEnterClassMember(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitClassMember(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// void zen_SymbolResolutionListener_onEnterConstructorDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
// }

// void zen_SymbolResolutionListener_onExitConstructorDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
// }

// enumerationDeclaration

void zen_SymbolResolutionListener_onEnterEnumerationDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitEnumerationDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// enumerationBaseClass

void zen_SymbolResolutionListener_onEnterEnumrationBaseClass(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitEnumerationBaseClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// enumerationSuite

void zen_SymbolResolutionListener_onEnterEnumerationSuite(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitEnumerationSuite(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// enumerate

void zen_SymbolResolutionListener_onEnterEnumerate(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitEnumerate(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// expressions

void zen_SymbolResolutionListener_onEnterExpressions(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitExpressions(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// expression

void zen_SymbolResolutionListener_onEnterExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_ExpressionContext_t* context = (zen_ExpressionContext_t*)node->m_context;

    /* Use the function stack to save the previous label. This is
     * important in the case of nested assignment expressions. Consider
     * the expression `x = (y = z)`, if the previous label is unsaved,
     * the nested assignment expression can alter the label of the outer
     * assignment expression.
     */
    zen_ExpressionAnnotation_t previous = listener->m_label;

    zen_ASTWalker_walk(astListener, context->m_assignmentExpression);

    /* Restore the previous label saved on the function stack. */
    listener->m_label = previous;

    zen_ASTListener_skipChildren(astListener);
}

void zen_SymbolResolutionListener_onExitExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// assignmentExpression

void zen_SymbolResolutionListener_onExitAssignmentExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onEnterAssignmentExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_AssignmentExpressionContext_t* context = (zen_AssignmentExpressionContext_t*)node->m_context;
    zen_Compiler_t* compiler = listener->m_compiler;
    zen_ErrorHandler_t* errorHandler = compiler->m_errorHandler;

    zen_ASTNode_t* assignmentOperator = context->m_assignmentOperator;
    if (assignmentOperator != NULL) {
        zen_ASTWalker_walk(astListener, context->m_conditionalExpression);
        if (listener->m_label == ZEN_EXPRESSION_ANNOTATION_VALUE) {
            zen_Token_t* assignmentOperatorToken = (zen_Token_t*)assignmentOperator->m_context;
            zen_ErrorHandler_handleSemanticalError(errorHandler, listener,
            ZEN_ERROR_CODE_INVALID_LVALUE, assignmentOperatorToken);
        }
        else {
            /* Do not walk through the assignment expression when the left value
             * is invalid.
             */
            zen_ASTWalker_walk(astListener, context->m_assignmentExpression);
        }
        zen_ASTListener_skipChildren(astListener);
    }
}

// conditionalExpression

void zen_SymbolResolutionListener_onExitConditionalExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_ConditionalExpressionContext_t* context = (zen_ConditionalExpressionContext_t*)node->m_context;

    if (context->m_thenExpression != NULL) {
        listener->m_label = ZEN_EXPRESSION_ANNOTATION_VALUE;
    }
}

void zen_SymbolResolutionListener_onEnterConditionalExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// logicalOrExpression

void zen_SymbolResolutionListener_onExitLogicalOrExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_LogicalOrExpressionContext_t* context = (zen_LogicalOrExpressionContext_t*)node->m_context;

    if (!jtk_ArrayList_isEmpty(context->m_logicalAndExpressions)) {
        listener->m_label = ZEN_EXPRESSION_ANNOTATION_VALUE;
    }
}

void zen_SymbolResolutionListener_onEnterLogicalOrExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// logicalAndExpression

void zen_SymbolResolutionListener_onExitLogicalAndExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_LogicalAndExpressionContext_t* context = (zen_LogicalAndExpressionContext_t*)node->m_context;

    if (!jtk_ArrayList_isEmpty(context->m_inclusiveOrExpressions)) {
        listener->m_label = ZEN_EXPRESSION_ANNOTATION_VALUE;
    }
}

void zen_SymbolResolutionListener_onEnterLogicalAndExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// inclusiveOrExpression

void zen_SymbolResolutionListener_onExitInclusiveOrExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_InclusiveOrExpressionContext_t* context = (zen_InclusiveOrExpressionContext_t*)node->m_context;

    if (!jtk_ArrayList_isEmpty(context->m_exclusiveOrExpressions)) {
        listener->m_label = ZEN_EXPRESSION_ANNOTATION_VALUE;
    }
}

void zen_SymbolResolutionListener_onEnterInclusiveOrExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// exclusiveOrExpression

void zen_SymbolResolutionListener_onExitExclusiveOrExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_ExclusiveOrExpressionContext_t* context = (zen_ExclusiveOrExpressionContext_t*)node->m_context;

    if (!jtk_ArrayList_isEmpty(context->m_andExpressions)) {
        listener->m_label = ZEN_EXPRESSION_ANNOTATION_VALUE;
    }
}

void zen_SymbolResolutionListener_onEnterExclusiveOrExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// andExpression

void zen_SymbolResolutionListener_onExitAndExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_AndExpressionContext_t* context = (zen_AndExpressionContext_t*)node->m_context;

    if (!jtk_ArrayList_isEmpty(context->m_equalityExpressions)) {
        listener->m_label = ZEN_EXPRESSION_ANNOTATION_VALUE;
    }
}

void zen_SymbolResolutionListener_onEnterAndExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// equalityExpression

void zen_SymbolResolutionListener_onExitEqualityExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_EqualityExpressionContext_t* context = (zen_EqualityExpressionContext_t*)node->m_context;

    if (!jtk_ArrayList_isEmpty(context->m_relationalExpressions)) {
        listener->m_label = ZEN_EXPRESSION_ANNOTATION_VALUE;
    }
}

void zen_SymbolResolutionListener_onEnterEqualityExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// relationalExpression

void zen_SymbolResolutionListener_onExitRelationalExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_RelationalExpressionContext_t* context = (zen_RelationalExpressionContext_t*)node->m_context;

    if (!jtk_ArrayList_isEmpty(context->m_shiftExpressions)) {
        listener->m_label = ZEN_EXPRESSION_ANNOTATION_VALUE;
    }
}

void zen_SymbolResolutionListener_onEnterRelationalExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// shiftExpression

void zen_SymbolResolutionListener_onExitShiftExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_ShiftExpressionContext_t* context = (zen_ShiftExpressionContext_t*)node->m_context;

    if (!jtk_ArrayList_isEmpty(context->m_additiveExpressions)) {
        listener->m_label = ZEN_EXPRESSION_ANNOTATION_VALUE;
    }
}

void zen_SymbolResolutionListener_onEnterShiftExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// additiveExpression

void zen_SymbolResolutionListener_onExitAdditiveExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_AdditiveExpressionContext_t* context = (zen_AdditiveExpressionContext_t*)node->m_context;

    if (!jtk_ArrayList_isEmpty(context->m_multiplicativeExpressions)) {
        listener->m_label = ZEN_EXPRESSION_ANNOTATION_VALUE;
    }
}

void zen_SymbolResolutionListener_onEnterAdditiveExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// multiplicativeExpression

void zen_SymbolResolutionListener_onExitMultiplicativeExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_MultiplicativeExpressionContext_t* context = (zen_MultiplicativeExpressionContext_t*)node->m_context;

    if (!jtk_ArrayList_isEmpty(context->m_unaryExpressions)) {
        listener->m_label = ZEN_EXPRESSION_ANNOTATION_VALUE;
    }
}

void zen_SymbolResolutionListener_onEnterMultiplicativeExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// unaryExpression

void zen_SymbolResolutionListener_onExitUnaryExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_UnaryExpressionContext_t* context = (zen_UnaryExpressionContext_t*)node->m_context;

    if (context->m_unaryOperator != NULL) {
        listener->m_label = ZEN_EXPRESSION_ANNOTATION_VALUE;
    }
}

void zen_SymbolResolutionListener_onEnterUnaryExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// postfixExpression

void zen_SymbolResolutionListener_onEnterPostfixExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_PostfixExpressionContext_t* context = (zen_PostfixExpressionContext_t*)node->m_context;

    /* The normal behaviour of the AST walker causes the listener to
     * visit the AST in an undesirable fashion. Therefore, we partially
     * switch from the listener to visitor design pattern. The AST walker
     * can be guided to switch to this mode via zen_ASTListener_skipChildren()
     * function which causes the AST walker to skip iterating over the children
     * nodes.
     */
    zen_ASTListener_skipChildren(astListener);
}

void zen_SymbolResolutionListener_onExitPostfixExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_PostfixExpressionContext_t* context = (zen_PostfixExpressionContext_t*)node->m_context;
    zen_Compiler_t* compiler = listener->m_compiler;
    zen_ErrorHandler_t* errorHandler = compiler->m_errorHandler;
    zen_ASTNode_t* primaryExpression = context->m_primaryExpression;
    zen_PrimaryExpressionContext_t* primaryExpressionContext = (zen_PrimaryExpressionContext_t*)primaryExpression->m_context;

    zen_Symbol_t* primarySymbol = NULL;
    int32_t postfixPartCount = jtk_ArrayList_getSize(context->m_postfixParts);

    zen_ASTNode_t* expression = primaryExpressionContext->m_expression;
    zen_Token_t* token = NULL;
    /* Check if the primary expression is a literal or an identifier. */
    if (zen_ASTNode_isTerminal(expression)) {
        /* Retrieve the token that the primary expression represents. */
        token = (zen_Token_t*)expression->m_context;

        switch (zen_Token_getType(token)) {
            case ZEN_TOKEN_IDENTIFIER: {
                /* Retrieve the string equivalent to the identifier node. */
                const uint8_t* identifierText = zen_Token_getText(token);
                /* Resolve the symbol in the symbol table. */
                zen_Symbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, identifierText);

                if (symbol != NULL) {
                    zen_Scope_t* enclosingScope = zen_Symbol_getEnclosingScope(symbol);
                    if (zen_Symbol_isVariable(symbol) || zen_Symbol_isConstant(symbol)) {
                        /* Annotate the AST node as placeholder. */
                        listener->m_label = ZEN_EXPRESSION_ANNOTATION_PLACEHOLDER;
                    }
                    else {
                        /* Pass the reference to the symbol to the next phase. */
                        primarySymbol = symbol;
                    }

                    if ((enclosingScope != NULL) && zen_Scope_isLocalScope(enclosingScope)) {
                        zen_Token_t* symbolToken = (zen_Token_t*)symbol->m_identifier->m_context;
                        if (token->m_startIndex <= symbolToken->m_startIndex) {
                            zen_ErrorHandler_handleSemanticalError(errorHandler, listener,
                                    ZEN_ERROR_CODE_UNDECLARED_IDENTIFIER, token);
                        }
                    }
                }
                else {
                    zen_ErrorHandler_handleSemanticalError(errorHandler, listener,
                            ZEN_ERROR_CODE_UNDECLARED_IDENTIFIER, token);
                }

                break;
            }

            case ZEN_TOKEN_INTEGER_LITERAL:
            case ZEN_TOKEN_STRING_LITERAL:
            case ZEN_TOKEN_KEYWORD_NULL:
            case ZEN_TOKEN_KEYWORD_FALSE:
            case ZEN_TOKEN_KEYWORD_THIS:
            case ZEN_TOKEN_KEYWORD_TRUE: {
                /* Annotate the AST node as value. */
                listener->m_label = ZEN_EXPRESSION_ANNOTATION_VALUE;

                break;
            }
        }
    }
    else if ((expression->m_type == ZEN_AST_NODE_TYPE_MAP_EXPRESSION) ||
        (expression->m_type == ZEN_AST_NODE_TYPE_LIST_EXPRESSION) ||
        (expression->m_type == ZEN_AST_NODE_TYPE_EXPRESSION) ||
        (expression->m_type == ZEN_AST_NODE_TYPE_NEW_EXPRESSION)) {
        zen_ASTWalker_walk(astListener, expression);

        /* Annotate the AST node as value. */
        listener->m_label = ZEN_EXPRESSION_ANNOTATION_VALUE;
    }

    int32_t i;
    for (i = 0; i < postfixPartCount; i++) {
        zen_ASTNode_t* postfixPart = (zen_ASTNode_t*)jtk_ArrayList_getValue(
            context->m_postfixParts, i);
        zen_ASTNodeType_t type = zen_ASTNode_getType(postfixPart);

        switch (type) {
            case ZEN_AST_NODE_TYPE_SUBSCRIPT: {
                /* Annotate the AST node as placeholder. */
                listener->m_label = ZEN_EXPRESSION_ANNOTATION_PLACEHOLDER;

                zen_SubscriptContext_t* subscriptContext = (zen_SubscriptContext_t*)postfixPart->m_context;

                /* Visit the index expression node and analyze. */
                zen_ASTWalker_walk(astListener, subscriptContext->m_expression);

                /* The normal behaviour of the AST walker causes the listener to
                 * visit the index in an undesirable fashion. Therefore, we partially
                 * switch from the listener to visitor design pattern. The AST walker
                 * can be guided to switch to this mode via zen_ASTListener_skipChildren()
                 * function which causes the AST walker to skip iterating over the children
                 * nodes.
                 */
                zen_ASTListener_skipChildren(astListener);

                break;
            }

            case ZEN_AST_NODE_TYPE_FUNCTION_ARGUMENTS: {
                /* Annotate the AST node as value. */
                listener->m_label = ZEN_EXPRESSION_ANNOTATION_VALUE;

                zen_FunctionArgumentsContext_t* functionArgumentsContext =
                    (zen_FunctionArgumentsContext_t*)postfixPart->m_context;

                if (i == 0) {
                    if (primarySymbol == NULL) {
                        zen_ErrorHandler_handleSemanticalError(errorHandler, listener,
                            ZEN_ERROR_CODE_VARIABLE_TREATED_AS_FUNCTION, token);
                    }
                    else if (zen_Symbol_isFunction(primarySymbol)) {
                        zen_FunctionSymbol_t* functionSymbol = &primarySymbol->m_context.m_asFunction;
                        zen_ASTNode_t* expressions = functionArgumentsContext->m_expressions;
                        if (expressions != NULL) {
                            zen_ExpressionsContext_t* expressionsContext = (zen_ExpressionsContext_t*)expressions->m_context;
                            int32_t argumentCount = jtk_ArrayList_getSize(expressionsContext->m_expressions);
                            if (argumentCount > 0) {
                                int32_t argumentIndex;
                                for (argumentIndex = 0; argumentIndex < argumentCount; argumentIndex++) {
                                    /* Retrieve the expression for the current argument. */
                                    zen_ASTNode_t* argument = (zen_ASTNode_t*)jtk_ArrayList_getValue(
                                        expressionsContext->m_expressions, argumentIndex);

                                    /* Visit the expression node and analyze. */
                                    zen_ASTWalker_walk(astListener, argument);
                                }
                            }
                        }
                    }
                    else {
                        printf("[error] Trying to invoke a non-function entity.\n");
                    }
                }
                else {
                    printf("[internal error] Control should not reach here.\n");
                }

                break;
            }

            case ZEN_AST_NODE_TYPE_MEMBER_ACCESS: {
                zen_MemberAccessContext_t* memberAccessContext =
                    (zen_MemberAccessContext_t*)postfixPart->m_context;
                zen_ASTNode_t* identifier = memberAccessContext->m_identifier;
                zen_Token_t* identifierToken = (zen_Token_t*)identifier->m_context;

                zen_ASTNode_t* functionArguments = NULL;

                /* Annotate the AST node as placeholder. */
                listener->m_label = ZEN_EXPRESSION_ANNOTATION_PLACEHOLDER;

                if ((i + 1) < postfixPartCount) {
                    zen_ASTNode_t* nextPostfixPart = (zen_ASTNode_t*)jtk_ArrayList_getValue(
                        context->m_postfixParts, i + 1);
                    zen_ASTNodeType_t nextPostfixPartType = zen_ASTNode_getType(nextPostfixPart);
                    if (nextPostfixPartType == ZEN_AST_NODE_TYPE_FUNCTION_ARGUMENTS) {
                        functionArguments = nextPostfixPart;
                        i++;
                    }
                }

                if (functionArguments != NULL) {
                    /* Annotate the AST node as placeholder. */
                    listener->m_label = ZEN_EXPRESSION_ANNOTATION_VALUE;

                    zen_FunctionArgumentsContext_t* functionArgumentsContext =
                        (zen_FunctionArgumentsContext_t*)functionArguments->m_context;

                    zen_ASTNode_t* expressions = functionArgumentsContext->m_expressions;
                    if (expressions != NULL) {
                        zen_ExpressionsContext_t* expressionsContext = (zen_ExpressionsContext_t*)expressions->m_context;
                        int32_t argumentCount = jtk_ArrayList_getSize(expressionsContext->m_expressions);
                        if (argumentCount > 0) {
                            int32_t argumentIndex;
                            for (argumentIndex = 0; argumentIndex < argumentCount; argumentIndex++) {
                                /* Retrieve the expression for the current argument. */
                                zen_ASTNode_t* argument = (zen_ASTNode_t*)jtk_ArrayList_getValue(
                                    expressionsContext->m_expressions, argumentIndex);

                                /* Visit the expression node and generate the relevant instructions. */
                                zen_ASTWalker_walk(astListener, argument);
                            }
                        }
                    }
                }
                break;
            }

            /*
            case ZEN_AST_NODE_TYPE_POSTFIX_OPERATOR: {

                break;
            }
            */

            default: {
                printf("[error] Invalid AST node type %d encountered.\n", type);
            }
        }
    }
}

// subscript

void zen_SymbolResolutionListener_onEnterSubscript(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_ExpressionContext_t* context = (zen_ExpressionContext_t*)node->m_context;

    // listener->m_label = ZEN_EXPRESSION_ANNOTATION_PLACEHOLDER;
}

void zen_SymbolResolutionListener_onExitSubscript(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// functionArguments

void zen_SymbolResolutionListener_onEnterFunctionArguments(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitFunctionArguments(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// memberAccess

void zen_SymbolResolutionListener_onEnterMemberAccess(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitMemberAccess(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// postfixOperator

void zen_SymbolResolutionListener_onEnterPostfixOperator(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitPostfixOperator(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// Primary Expression

void zen_SymbolResolutionListener_onEnterPrimaryExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitPrimaryExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onEnterMapExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitMapExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// mapEntries

void zen_SymbolResolutionListener_onEnterMapEntries(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitMapEntries(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// mapEntry

void zen_SymbolResolutionListener_onEnterMapEntry(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitMapEntry(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// listExpression

void zen_SymbolResolutionListener_onEnterListExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_SymbolResolutionListener_onExitListExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// New Expression

void zen_SymbolResolutionListener_onEnterNewExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    // TODO: Debug this function when superclasses are implemented!

    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_Compiler_t* compiler = listener->m_compiler;
    zen_ErrorHandler_t* errorHandler = compiler->m_errorHandler;
    /* Retrieve the logger from the compiler. */
    jtk_Logger_t* logger = compiler->m_logger;

    zen_NewExpressionContext_t* context = (zen_NewExpressionContext_t*)node->m_context;

    /* Retrieve the scope within which the new expression appears. */
    // zen_Scope_t* scope = zen_SymbolTable_getCurrentScope(generator->m_symbolTable);

    /* Retrieve the string equivalent of the type name node. */
    int32_t typeNameSize;
    uint8_t* typeNameText = zen_ASTNode_toCString(context->m_typeName, &typeNameSize);
    zen_TypeNameContext_t* typeName = (zen_TypeNameContext_t*)context->m_typeName->m_context;
    int32_t identifierCount = jtk_ArrayList_getSize(typeName->m_identifiers);
    zen_ASTNode_t* lastIdentifier = jtk_ArrayList_getValue(typeName->m_identifiers, identifierCount - 1);
    zen_Token_t* lastIdentifierToken = (zen_Token_t*)lastIdentifier->m_context;

    /* Resolve the class symbol for the type name. */
    zen_Symbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, typeNameText);

    if (symbol == NULL) {
        zen_ErrorHandler_handleSemanticalError(errorHandler, listener,
            ZEN_ERROR_CODE_UNDECLARED_CLASS, lastIdentifierToken);
    }
    else {
        if (zen_Symbol_isExternal(symbol)) {
            symbol = symbol->m_context.m_asExternal;
        }

        if (zen_Symbol_isClass(symbol)) {
            zen_ClassSymbol_t* classSymbol = &symbol->m_context.m_asClass;
            /* Retrieve the scope corresponding to the class symbol. */
            zen_Scope_t* scope = classSymbol->m_classScope;
            if (zen_Scope_isClassScope(scope)) {
                /* Retrieve the constructor declared in this class. */
                zen_Symbol_t* constructorSymbol = zen_Scope_resolve(scope, "new");

                if ((constructorSymbol == NULL) ||
                    (zen_Symbol_getEnclosingScope(constructorSymbol) != scope)) {
                    zen_ErrorHandler_handleSemanticalError(errorHandler, listener,
                        ZEN_ERROR_CODE_NO_SUITABLE_CONSTRUCTOR, lastIdentifierToken);
                }
                else {
                    if (!zen_Symbol_isFunction(constructorSymbol)) {
                        printf("[internal error] 'new' declared as non-constructor symbol in class %s.\n", typeNameText);
                        printf("[warning] Looks like the syntactical phase failed.\n");
                    }
                }
            }
            else {
                printf("[internal error] %s is a non-class scope\n", typeNameText);
                printf("[warning] Looks like the syntactical phase failed.\n");
            }
        }
        else {
            zen_ErrorHandler_handleSemanticalError(errorHandler, listener,
                ZEN_ERROR_CODE_INSTANTIATION_OF_NON_CLASS_SYMBOL, lastIdentifierToken);
        }
    }

    /* Delete the type name text. The typeNameText is used to generate the constructor
     * entry in the constant pool. Therefore, we delay its destruction.
     */
    jtk_CString_delete(typeNameText);
}

void zen_SymbolResolutionListener_onExitNewExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}