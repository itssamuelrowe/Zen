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

#include <jtk/collection/list/ArrayList.h>
#include <com/onecube/zen/compiler/ast/ASTWalker.h>
#include <com/onecube/zen/compiler/parser/Parser.h>

void zen_ASTWalker_walk(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(listener, "The specified listener is null.");
    // jtk_Assert_assertObject(node, "The specified root is null.");

    /*printf("[node enter] %s\n", zen_Parser_getRuleName(node->m_type));
    fflush(stdout);
    */

    if (node != NULL) {
        if (zen_ASTNode_isErroneous(node)) {
            listener->m_onVisitErrorNode(listener, node);
        }
        else if (zen_ASTNode_isTerminal(node)) {
            listener->m_onVisitTerminal(listener, node);
        }
        else {
            zen_ASTWalker_enterRule(listener, node);

            jtk_ArrayList_t* children = zen_ASTNode_getChildren(node);
            int32_t size = jtk_ArrayList_getSize(children);
            switch (listener->m_walkerState) {
                case ZEN_AST_WALKER_STATE_VISIT_CHILDREN: {
                    // TODO: Use a stack based traversal.
                    int32_t i;
                    for (i = 0; i < size; i++) {
                        zen_ASTNode_t* child = (zen_ASTNode_t*)jtk_ArrayList_getValue(children, i);
                        zen_ASTWalker_walk(listener, child);
                    }

                    break;
                }

                case ZEN_AST_WALKER_STATE_SKIP_CHILDREN: {
                    // Skip the children! Nothing to do here.

                    break;
                }

                case ZEN_AST_WALKER_STATE_VISIT_FIRST_CHILD: {
                    if (size > 0) {
                        zen_ASTNode_t* firstChild = (zen_ASTNode_t*)jtk_ArrayList_getValue(children, 0);
                        zen_ASTWalker_walk(listener, firstChild);
                    }

                    break;
                }

                case ZEN_AST_WALKER_STATE_VISIT_LAST_CHILD: {
                    if (size > 0) {
                        zen_ASTNode_t* lastChild = (zen_ASTNode_t*)jtk_ArrayList_getValue(
                            children, size - 1);
                        zen_ASTWalker_walk(listener, lastChild);
                    }

                    break;
                }

                default: {
                    printf("[warning] Ignored a walker state in ASTWalker.\n");
                }
            }

            // TODO: If the walker allows a node to prevent its siblings from being visited
            // please process it here. Further, do not reset to ZEN_AST_WALKER_STATE_VISIT_CHILDREN.

            zen_ASTListener_visitChildren(listener);

            zen_ASTWalker_exitRule(listener, node);
        }
    }

    // printf("[node exit] %s\n", zen_Parser_getRuleName(node->m_type));
}

void zen_ASTWalker_enterRule(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
    // printf("[enter node]: %s\n", zen_Parser_getRuleName(node->m_type));

    listener->m_onEnterEveryRule(listener, node);

    switch (node->m_type) {
        case ZEN_AST_NODE_TYPE_COMPILATION_UNIT: {
            listener->m_onEnterCompilationUnit(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_IMPORT_DECLARATION: {
            listener->m_onEnterImportDeclaration(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ANNOTATED_COMPONENT_DECLARATION: {
            listener->m_onEnterAnnotatedComponentDeclaration(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ANNOTATIONS: {
            listener->m_onEnterAnnotations(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ANNOTATION: {
            listener->m_onEnterAnnotation(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ANNOTATION_TYPE: {
            listener->m_onEnterAnnotationType(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ANNOTATION_ATTRIBUTE: {
            listener->m_onEnterAnnotationAttribute(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_COMPONENT_DECLARATION: {
            listener->m_onEnterComponentDeclaration(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_FUNCTION_DECLARATION: {
            listener->m_onEnterFunctionDeclaration(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_FUNCTION_PARAMETERS: {
            listener->m_onEnterFunctionParameters(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_FUNCTION_BODY: {
            listener->m_onEnterFunctionBody(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_STATEMENT_SUITE: {
            listener->m_onEnterStatementSuite(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_SIMPLE_STATEMENT: {
            listener->m_onEnterSimpleStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_STATEMENT: {
            listener->m_onEnterStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_EMPTY_STATEMENT: {
            listener->m_onEnterEmptyStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_VARIABLE_DECLARATION: {
            listener->m_onEnterVariableDeclaration(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_VARIABLE_DECLARATOR: {
            listener->m_onEnterVariableDeclarator(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_CONSTANT_DECLARATION: {
            listener->m_onEnterConstantDeclaration(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_CONSTANT_DECLARATOR: {
            listener->m_onEnterConstantDeclarator(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ASSERT_STATEMENT: {
            listener->m_onEnterAssertStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_BREAK_STATEMENT: {
            listener->m_onEnterBreakStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_CONTINUE_STATEMENT: {
            listener->m_onEnterContinueStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_RETURN_STATEMENT: {
            listener->m_onEnterReturnStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_THROW_STATEMENT: {
            listener->m_onEnterThrowStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_COMPOUND_STATEMENT: {
            listener->m_onEnterCompoundStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_IF_STATEMENT: {
            listener->m_onEnterIfStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_IF_CLAUSE: {
            listener->m_onEnterIfClause(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ELSE_IF_CLAUSE: {
            listener->m_onEnterElseIfClause(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ELSE_CLAUSE: {
            listener->m_onEnterElseClause(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ITERATIVE_STATEMENT: {
            listener->m_onEnterIterativeStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_LABEL_CLAUSE: {
            listener->m_onEnterLabelClause(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_WHILE_STATEMENT: {
            listener->m_onEnterWhileStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_FOR_STATEMENT: {
            listener->m_onEnterForStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_FOR_PARAMETER: {
            listener->m_onEnterForParameter(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_TRY_STATEMENT: {
            listener->m_onEnterTryStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_TRY_CLAUSE: {
            listener->m_onEnterTryClause(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_CATCH_CLAUSE: {
            listener->m_onEnterCatchClause(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_CATCH_FILTER: {
            listener->m_onEnterCatchFilter(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_FINALLY_CLAUSE: {
            listener->m_onEnterFinallyClause(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_SYNCHRONIZE_STATEMENT: {
            listener->m_onEnterSynchronizeStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_WITH_STATEMENT: {
            listener->m_onEnterWithStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_WITH_PARAMETERS: {
            listener->m_onEnterWithParameters(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_WITH_PARAMETER: {
            listener->m_onEnterWithParameter(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_CLASS_DECLARATION: {
            listener->m_onEnterClassDeclaration(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_EXTENDS_CLAUSE: {
            listener->m_onEnterClassExtendsClause(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_CLASS_SUITE: {
            listener->m_onEnterClassSuite(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_CLASS_MEMBER: {
            listener->m_onEnterClassMember(listener, node);
            break;
        }

        // case ZEN_AST_NODE_TYPE_CONSTRUCTOR_DECLARATION: {
            // listener->m_onEnterConstructorDeclaration(listener, node);
            // break;
        // }

        // case ZEN_AST_NODE_TYPE_ENUMERATION_DECLARATION: {
        //     listener->m_onEnterEnumerationDeclaration(listener, node);
        //     break;
        // }

        // case ZEN_AST_NODE_TYPE_ENUMERATION_BASE_CLAUSE: {
        //     listener->m_onEnterEnumerationBaseClause(listener, node);
        //     break;
        // }

        // case ZEN_AST_NODE_TYPE_ENUMERATION_SUITE: {
        //     listener->m_onEnterEnumerationSuite(listener, node);
        //     break;
        // }

        // case ZEN_AST_NODE_TYPE_ENUMERATE: {
        //     listener->m_onEnterEnumerate(listener, node);
        //     break;
        // }

        case ZEN_AST_NODE_TYPE_EXPRESSIONS: {
            listener->m_onEnterExpressions(listener, node);
            break;
        }


        case ZEN_AST_NODE_TYPE_EXPRESSION: {
            listener->m_onEnterExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ASSIGNMENT_EXPRESSION: {
            listener->m_onEnterAssignmentExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_CONDITIONAL_EXPRESSION: {
            listener->m_onEnterConditionalExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_LOGICAL_OR_EXPRESSION: {
            listener->m_onEnterLogicalOrExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_LOGICAL_AND_EXPRESSION: {
            listener->m_onEnterLogicalAndExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_INCLUSIVE_OR_EXPRESSION: {
            listener->m_onEnterInclusiveOrExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_EXCLUSIVE_OR_EXPRESSION: {
            listener->m_onEnterExclusiveOrExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_AND_EXPRESSION: {
            listener->m_onEnterAndExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_EQUALITY_EXPRESSION: {
            listener->m_onEnterEqualityExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_RELATIONAL_EXPRESSION: {
            listener->m_onEnterRelationalExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_SHIFT_EXPRESSION: {
            listener->m_onEnterShiftExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ADDITIVE_EXPRESSION: {
            listener->m_onEnterAdditiveExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_MULTIPLICATIVE_EXPRESSION: {
            listener->m_onEnterMultiplicativeExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_UNARY_EXPRESSION: {
            listener->m_onEnterUnaryExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_POSTFIX_EXPRESSION: {
            listener->m_onEnterPostfixExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_SUBSCRIPT: {
            listener->m_onEnterSubscript(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_FUNCTION_ARGUMENTS: {
            listener->m_onEnterFunctionArguments(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_MEMBER_ACCESS: {
            listener->m_onEnterMemberAccess(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_POSTFIX_OPERATOR: {
            listener->m_onEnterPostfixOperator(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_PRIMARY_EXPRESSION: {
            listener->m_onEnterPrimaryExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_MAP_EXPRESSION: {
            listener->m_onEnterMapExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_MAP_ENTRIES: {
            listener->m_onEnterMapEntries(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_MAP_ENTRY: {
            listener->m_onEnterMapEntry(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_LIST_EXPRESSION: {
            listener->m_onEnterListExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_NEW_EXPRESSION: {
            listener->m_onEnterNewExpression(listener, node);
            break;
        }
    }
}

void zen_ASTWalker_exitRule(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
    // printf("[exit node]: %s\n", zen_Parser_getRuleName(node->m_type));

    switch (node->m_type) {
        case ZEN_AST_NODE_TYPE_COMPILATION_UNIT: {
            listener->m_onExitCompilationUnit(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_IMPORT_DECLARATION: {
            listener->m_onExitImportDeclaration(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ANNOTATED_COMPONENT_DECLARATION: {
            listener->m_onExitAnnotatedComponentDeclaration(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ANNOTATIONS: {
            listener->m_onExitAnnotations(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ANNOTATION: {
            listener->m_onExitAnnotation(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ANNOTATION_TYPE: {
            listener->m_onExitAnnotationType(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ANNOTATION_ATTRIBUTE: {
            listener->m_onExitAnnotationAttribute(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_COMPONENT_DECLARATION: {
            listener->m_onExitComponentDeclaration(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_FUNCTION_DECLARATION: {
            listener->m_onExitFunctionDeclaration(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_FUNCTION_PARAMETERS: {
            listener->m_onExitFunctionParameters(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_FUNCTION_BODY: {
            listener->m_onExitFunctionBody(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_STATEMENT_SUITE: {
            listener->m_onExitStatementSuite(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_SIMPLE_STATEMENT: {
            listener->m_onExitSimpleStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_STATEMENT: {
            listener->m_onExitStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_EMPTY_STATEMENT: {
            listener->m_onExitEmptyStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_VARIABLE_DECLARATION: {
            listener->m_onExitVariableDeclaration(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_VARIABLE_DECLARATOR: {
            listener->m_onExitVariableDeclarator(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_CONSTANT_DECLARATION: {
            listener->m_onExitConstantDeclaration(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_CONSTANT_DECLARATOR: {
            listener->m_onExitConstantDeclarator(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ASSERT_STATEMENT: {
            listener->m_onExitAssertStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_BREAK_STATEMENT: {
            listener->m_onExitBreakStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_CONTINUE_STATEMENT: {
            listener->m_onExitContinueStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_RETURN_STATEMENT: {
            listener->m_onExitReturnStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_THROW_STATEMENT: {
            listener->m_onExitThrowStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_COMPOUND_STATEMENT: {
            listener->m_onExitCompoundStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_IF_STATEMENT: {
            listener->m_onExitIfStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_IF_CLAUSE: {
            listener->m_onExitIfClause(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ELSE_IF_CLAUSE: {
            listener->m_onExitElseIfClause(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ELSE_CLAUSE: {
            listener->m_onExitElseClause(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ITERATIVE_STATEMENT: {
            listener->m_onExitIterativeStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_LABEL_CLAUSE: {
            listener->m_onExitLabelClause(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_WHILE_STATEMENT: {
            listener->m_onExitWhileStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_FOR_STATEMENT: {
            listener->m_onExitForStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_FOR_PARAMETER: {
            listener->m_onExitForParameter(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_TRY_STATEMENT: {
            listener->m_onExitTryStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_TRY_CLAUSE: {
            listener->m_onExitTryClause(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_CATCH_CLAUSE: {
            listener->m_onExitCatchClause(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_CATCH_FILTER: {
            listener->m_onExitCatchFilter(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_FINALLY_CLAUSE: {
            listener->m_onExitFinallyClause(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_SYNCHRONIZE_STATEMENT: {
            listener->m_onExitSynchronizeStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_WITH_STATEMENT: {
            listener->m_onExitWithStatement(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_WITH_PARAMETERS: {
            listener->m_onExitWithParameters(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_WITH_PARAMETER: {
            listener->m_onExitWithParameter(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_CLASS_DECLARATION: {
            listener->m_onExitClassDeclaration(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_EXTENDS_CLAUSE: {
            listener->m_onExitClassExtendsClause(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_CLASS_SUITE: {
            listener->m_onExitClassSuite(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_CLASS_MEMBER: {
            listener->m_onExitClassMember(listener, node);
            break;
        }

//        case ZEN_AST_NODE_TYPE_CONSTRUCTOR_DECLARATION: {
//            listener->m_onExitConstructorDeclaration(listener, node);
//            break;
//        }

        // case ZEN_AST_NODE_TYPE_ENUMERATION_DECLARATION: {
        //     listener->m_onExitEnumerationDeclaration(listener, node);
        //     break;
        // }

        // case ZEN_AST_NODE_TYPE_ENUMERATION_BASE_CLAUSE: {
        //     listener->m_onExitEnumerationBaseClause(listener, node);
        //     break;
        // }

        // case ZEN_AST_NODE_TYPE_ENUMERATION_SUITE: {
        //     listener->m_onExitEnumerationSuite(listener, node);
        //     break;
        // }

        // case ZEN_AST_NODE_TYPE_ENUMERATE: {
        //     listener->m_onExitEnumerate(listener, node);
        //     break;
        // }

        case ZEN_AST_NODE_TYPE_EXPRESSIONS: {
            listener->m_onExitExpressions(listener, node);
            break;
        }


        case ZEN_AST_NODE_TYPE_EXPRESSION: {
            listener->m_onExitExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ASSIGNMENT_EXPRESSION: {
            listener->m_onExitAssignmentExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_CONDITIONAL_EXPRESSION: {
            listener->m_onExitConditionalExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_LOGICAL_OR_EXPRESSION: {
            listener->m_onExitLogicalOrExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_LOGICAL_AND_EXPRESSION: {
            listener->m_onExitLogicalAndExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_INCLUSIVE_OR_EXPRESSION: {
            listener->m_onExitInclusiveOrExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_EXCLUSIVE_OR_EXPRESSION: {
            listener->m_onExitExclusiveOrExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_AND_EXPRESSION: {
            listener->m_onExitAndExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_EQUALITY_EXPRESSION: {
            listener->m_onExitEqualityExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_RELATIONAL_EXPRESSION: {
            listener->m_onExitRelationalExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_SHIFT_EXPRESSION: {
            listener->m_onExitShiftExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_ADDITIVE_EXPRESSION: {
            listener->m_onExitAdditiveExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_MULTIPLICATIVE_EXPRESSION: {
            listener->m_onExitMultiplicativeExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_UNARY_EXPRESSION: {
            listener->m_onExitUnaryExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_POSTFIX_EXPRESSION: {
            listener->m_onExitPostfixExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_SUBSCRIPT: {
            listener->m_onExitSubscript(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_FUNCTION_ARGUMENTS: {
            listener->m_onExitFunctionArguments(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_MEMBER_ACCESS: {
            listener->m_onExitMemberAccess(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_POSTFIX_OPERATOR: {
            listener->m_onExitPostfixOperator(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_PRIMARY_EXPRESSION: {
            listener->m_onExitPrimaryExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_MAP_EXPRESSION: {
            listener->m_onExitMapExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_MAP_ENTRIES: {
            listener->m_onExitMapEntries(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_MAP_ENTRY: {
            listener->m_onExitMapEntry(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_LIST_EXPRESSION: {
            listener->m_onExitListExpression(listener, node);
            break;
        }

        case ZEN_AST_NODE_TYPE_NEW_EXPRESSION: {
            listener->m_onExitNewExpression(listener, node);
            break;
        }
    }
    listener->m_onExitEveryRule(listener, node);
}
