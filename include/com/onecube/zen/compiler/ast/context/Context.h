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

// Friday, November 24, 2017

#ifndef COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_CONTEXT_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_CONTEXT_CONTEXT_H

/*
 * According to my knowledge, I had two options to implement the
 * contexts.
 *
 * 1. A generic class which represents all contexts. Each subrule
 *    or token in the rule is stored in a list. Accessing would
 *    require a linear search. Usually, children of a context do
 *    not exceed more than, say, 10. Thus, the performance would
 *    be affected in a negligible way. Further, each rule is
 *    associated with an helper classes which helps us in the
 *    linear search.
 *    This technique provides a common ground for destruction of
 *    nodes with error checking, abstract syntax tree (AST) walking,
 *    and other such generic manipulative tasks.
 *
 * 2. A custom class represents a particular rule. Each subrule
 *    or token in the rule is stored in its class. Accessing would
 *    not require any special methods. You just access with the
 *    member access operator.
 *
 * Here, I have implemented the latter technique. Yes, I understand the
 * former technique is more robust. Unfortunately, the code base is
 * too big to modify now without hours of typing, compiling, and
 * debugging.
 */

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/context/CompilationUnitContext.h>
#include <com/onecube/zen/compiler/ast/context/ImportDeclarationContext.h>
#include <com/onecube/zen/compiler/ast/context/AnnotatedComponentDeclarationContext.h>
#include <com/onecube/zen/compiler/ast/context/AnnotationContext.h>
#include <com/onecube/zen/compiler/ast/context/AnnotationsContext.h>
#include <com/onecube/zen/compiler/ast/context/AnnotationTypeContext.h>
#include <com/onecube/zen/compiler/ast/context/AnnotationAttributeContext.h>
#include <com/onecube/zen/compiler/ast/context/ComponentDeclarationContext.h>
#include <com/onecube/zen/compiler/ast/context/FunctionDeclarationContext.h>
#include <com/onecube/zen/compiler/ast/context/FunctionParametersContext.h>
#include <com/onecube/zen/compiler/ast/context/FunctionBodyContext.h>
#include <com/onecube/zen/compiler/ast/context/StatementSuiteContext.h>
#include <com/onecube/zen/compiler/ast/context/StatementContext.h>
#include <com/onecube/zen/compiler/ast/context/SimpleStatementContext.h>
#include <com/onecube/zen/compiler/ast/context/EmptyStatementContext.h>
#include <com/onecube/zen/compiler/ast/context/VariableDeclarationContext.h>
#include <com/onecube/zen/compiler/ast/context/VariableDeclaratorContext.h>
#include <com/onecube/zen/compiler/ast/context/ConstantDeclarationContext.h>
#include <com/onecube/zen/compiler/ast/context/ConstantDeclaratorContext.h>
#include <com/onecube/zen/compiler/ast/context/AssertStatementContext.h>
#include <com/onecube/zen/compiler/ast/context/BreakStatementContext.h>
#include <com/onecube/zen/compiler/ast/context/ContinueStatementContext.h>
#include <com/onecube/zen/compiler/ast/context/ReturnStatementContext.h>
#include <com/onecube/zen/compiler/ast/context/ThrowStatementContext.h>
#include <com/onecube/zen/compiler/ast/context/CompoundStatementContext.h>
#include <com/onecube/zen/compiler/ast/context/IfStatementContext.h>
#include <com/onecube/zen/compiler/ast/context/IfClauseContext.h>
#include <com/onecube/zen/compiler/ast/context/ElseIfClauseContext.h>
#include <com/onecube/zen/compiler/ast/context/ElseClauseContext.h>
#include <com/onecube/zen/compiler/ast/context/IterativeStatementContext.h>
#include <com/onecube/zen/compiler/ast/context/LabelClauseContext.h>
#include <com/onecube/zen/compiler/ast/context/WhileStatementContext.h>
#include <com/onecube/zen/compiler/ast/context/ForStatementContext.h>
#include <com/onecube/zen/compiler/ast/context/ForParameterContext.h>
#include <com/onecube/zen/compiler/ast/context/TryStatementContext.h>
#include <com/onecube/zen/compiler/ast/context/TryClauseContext.h>
#include <com/onecube/zen/compiler/ast/context/CatchClauseContext.h>
#include <com/onecube/zen/compiler/ast/context/CatchFilterContext.h>
#include <com/onecube/zen/compiler/ast/context/FinallyClauseContext.h>
#include <com/onecube/zen/compiler/ast/context/SynchronizeStatementContext.h>
#include <com/onecube/zen/compiler/ast/context/WithStatementContext.h>
#include <com/onecube/zen/compiler/ast/context/WithParametersContext.h>
#include <com/onecube/zen/compiler/ast/context/WithParameterContext.h>
#include <com/onecube/zen/compiler/ast/context/ClassDeclarationContext.h>
#include <com/onecube/zen/compiler/ast/context/ClassExtendsClauseContext.h>
#include <com/onecube/zen/compiler/ast/context/ClassSuiteContext.h>
#include <com/onecube/zen/compiler/ast/context/ClassMemberContext.h>
#include <com/onecube/zen/compiler/ast/context/EnumerationDeclarationContext.h>
#include <com/onecube/zen/compiler/ast/context/EnumerationBaseClauseContext.h>
#include <com/onecube/zen/compiler/ast/context/EnumerationSuiteContext.h>
#include <com/onecube/zen/compiler/ast/context/EnumerateContext.h>
#include <com/onecube/zen/compiler/ast/context/ExpressionsContext.h>
#include <com/onecube/zen/compiler/ast/context/ExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/AssignmentExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/ConditionalExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/LogicalAndExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/LogicalOrExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/InclusiveOrExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/ExclusiveOrExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/AndExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/EqualityExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/RelationalExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/ShiftExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/AdditiveExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/MultiplicativeExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/UnaryExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/PostfixExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/SubscriptContext.h>
#include <com/onecube/zen/compiler/ast/context/FunctionArgumentsContext.h>
#include <com/onecube/zen/compiler/ast/context/MemberAccessContext.h>
#include <com/onecube/zen/compiler/ast/context/PostfixOperatorContext.h>
#include <com/onecube/zen/compiler/ast/context/PrimaryExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/MapExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/MapEntriesContext.h>
#include <com/onecube/zen/compiler/ast/context/MapEntryContext.h>
#include <com/onecube/zen/compiler/ast/context/ListExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/NewExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/TypeNameContext.h>

#endif /* COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_CONTEXT_CONTEXT_H */

/*******************************************************************************
 * ConstructorDeclarationContext                                               *
 *******************************************************************************/

// struct zen_ConstructorDeclarationContext_t {
//     zen_ASTNode_t* m_node;
//     zen_ASTNode_t* m_identifier;
//     zen_ASTNode_t* m_functionParameters;
//     zen_ASTNode_t* m_statementSuite;
// };

// typedef struct zen_ConstructorDeclarationContext_t zen_ConstructorDeclarationContext_t;

// zen_ConstructorDeclarationContext_t* zen_ConstructorDeclarationContext_new(zen_ASTNode_t* node);
// void zen_ConstructorDeclarationContext_delete(zen_ConstructorDeclarationContext_t* context);
// void zen_ConstructorDeclarationContext_getChildren(zen_ConstructorDeclarationContext_t* context, jtk_ArrayList_t* children);
