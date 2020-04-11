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

// Sunday, November 05, 2017

#ifndef COM_ONECUBE_ZEN_COMPILER_PARSER_PARSER_H
#define COM_ONECUBE_ZEN_COMPILER_PARSER_PARSER_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/lexer/TokenStream.h>

/*******************************************************************************
 * Parser                                                                      *
 *******************************************************************************/

/**
 * @class Parser
 * @ingroup zen_compiler_parser
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_Parser_t {
    zen_Compiler_t* m_compiler;
    zen_TokenStream_t* m_tokens;
    zen_TokenType_t* m_followSet;
    int32_t m_followSetSize;
    int32_t m_followSetCapacity;
    bool m_recovery;
    zen_ASTNodeType_t m_mainComponent;
};

/**
 * @memberof Parser
 */
typedef struct zen_Parser_t zen_Parser_t;

// Constructor

/**
 * @memberof Parser
 */
zen_Parser_t* zen_Parser_new(zen_Compiler_t* compiler, zen_TokenStream_t* tokens);

// Destructor

/**
 * @memberof Parser
 */
void zen_Parser_delete(zen_Parser_t* parser);

// Rule

/**
 * @memberof Parser
 */
const char* zen_Parser_getRuleName(zen_ASTNodeType_t type);

// Parse

/**
 * @memberof Parser
 */
void zen_Parser_parse(zen_Parser_t* parser);

// Match

/**
 * @memberof Parser
 */
void zen_Parser_match(zen_Parser_t* parser, zen_TokenType_t type);

/**
 * @memberof Parser
 */
zen_Token_t* zen_Parser_matchAndYield(zen_Parser_t* parser, zen_TokenType_t type);

// Recovery

void zen_Parser_pushFollowToken(zen_Parser_t* parser, zen_TokenType_t type);
void zen_Parser_popFollowToken(zen_Parser_t* parser);
void zen_Parser_recover(zen_Parser_t* parser);

// Reset

/**
 * @memberof Parser
 */
void zen_Parser_reset(zen_Parser_t* parser, zen_TokenStream_t* tokens);

// Rules

/**
 * @memberof Parser
 */
void zen_Parser_compilationUnit(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_importDeclaration(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
bool zen_Parser_isAnnotatedComponentDeclarationFollow(zen_TokenType_t token);

/**
 * @memberof Parser
 */
void zen_Parser_annotatedComponentDeclaration(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_annotations(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_annotation(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_annotationType(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_annotationAttribute(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_componentDeclaration(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_functionDeclaration(zen_Parser_t* parser, zen_ASTNode_t* node, uint32_t modifiers);

/**
 * @memberof Parser
 */
void zen_Parser_functionParameters(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_functionBody(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_statementSuite(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_simpleStatement(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
bool zen_Parser_isSimpleStatementFollow(zen_TokenType_t type);

/**
 * @memberof Parser
 */
void zen_Parser_statement(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
bool zen_Parser_isStatementFollow(zen_TokenType_t type);

/**
 * @memberof Parser
 */
void zen_Parser_emptyStatement(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_variableDeclaration(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_variableDeclarator(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_constantDeclaration(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_constantDeclarator(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_assertStatement(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_breakStatement(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_continueStatement(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_returnStatement(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_throwStatement(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_compoundStatement(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
bool zen_Parser_isCompoundStatementFollow(zen_TokenType_t type);

/**
 * @memberof Parser
 */
void zen_Parser_ifStatement(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_ifClause(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_elseIfClause(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_elseClause(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_iterativeStatement(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_labelClause(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_whileStatement(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_forStatement(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_tryStatement(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_tryClause(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_catchClause(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_catchFilter(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_typeName(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_finallyClause(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_synchronizeStatement(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_withStatement(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_classDeclaration(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_classExtendsClause(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_classSuite(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
bool zen_Parser_isClassMemberFollow(zen_TokenType_t type);

/**
 * @memberof Parser
 */
void zen_Parser_classMember(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
bool zen_Parser_isClassMemberModifier(zen_TokenType_t type);

/**
 * @memberof Parser
 */
// void zen_Parser_constructorDeclaration(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_enumerationDeclaration(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_enumerationBaseClause(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_enumerationSuite(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_enumerate(zen_Parser_t* paresr, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_expressions(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_expression(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
bool zen_Parser_isExpressionFollow(zen_TokenType_t type);

/**
 * @memberof Parser
 */
void zen_Parser_assignmentExpression(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
bool zen_Parser_isAssignmentOperator(zen_TokenType_t type);

/**
 * @memberof Parser
 */
void zen_Parser_conditionalExpression(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_logicalOrExpression(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_logicalAndExpression(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_inclusiveOrExpression(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_exclusiveOrExpression(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_andExpression(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_equalityExpression(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
bool zen_Parser_isEqualityOperator(zen_TokenType_t type);

/**
 * @memberof Parser
 */
void zen_Parser_relationalExpression(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
bool zen_Parser_isRelationalOperator(zen_TokenType_t type);

/**
 * @memberof Parser
 */
void zen_Parser_shiftExpression(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
bool zen_Parser_isShiftOperator(zen_TokenType_t type);

/**
 * @memberof Parser
 */
void zen_Parser_additiveExpression(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
bool zen_Parser_isAdditiveOperator(zen_TokenType_t type);

/**
 * @memberof Parser
 */
void zen_Parser_multiplicativeExpression(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
bool zen_Parser_isMultiplicativeOperator(zen_TokenType_t type);

/**
 * @memberof Parser
 */
void zen_Parser_unaryExpression(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
bool zen_Parser_isUnaryExpressionFollow(zen_TokenType_t type);

/**
 * @memberof Parser
 */
bool zen_Parser_isUnaryOperator(zen_TokenType_t type);

/**
 * @memberof Parser
 */
void zen_Parser_postfixExpression(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
bool zen_Parser_isPostfixExpressionFollow(zen_TokenType_t type);

/**
 * @memberof Parser
 */
bool zen_Parser_isPostfixPartFollow(zen_TokenType_t type);

/**
 * @memberof Parser
 */
void zen_Parser_subscript(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_functionArguments(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_memberAccess(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_postfixOperator(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_primaryExpression(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
bool zen_Parser_isPrimaryExpressionFollow(zen_TokenType_t type);

/**
 * @memberof Parser
 */
void zen_Parser_identifier(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_literal(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
bool zen_Parser_isLiteral(zen_TokenType_t type);

/**
 * @memberof Parser
 */
bool zen_Parser_isLiteralFollow(zen_TokenType_t type);

/**
 * @memberof Parser
 */
void zen_Parser_mapExpression(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_mapEntries(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_mapEntry(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_listExpression(zen_Parser_t* parser, zen_ASTNode_t* node);

/**
 * @memberof Parser
 */
void zen_Parser_newExpression(zen_Parser_t* parser, zen_ASTNode_t* node);

#endif /* COM_ONECUBE_ZEN_COMPILER_PARSER_PARSER_H */