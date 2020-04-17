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

// Friday, March 20, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_GENERATOR_BINARY_ENTITY_GENERATOR_H
#define COM_ONECUBE_ZEN_COMPILER_GENERATOR_BINARY_ENTITY_GENERATOR_H

// #include <jtk/collection/map/DualHashMap.h>
#include <jtk/collection/list/ArrayList.h>
#include <jtk/io/OutputStream.h>

#include <com/onecube/zen/Configuration.h>

#include <com/onecube/zen/compiler/Compiler.h>
#include <com/onecube/zen/compiler/ast/ASTListener.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/ast/ASTAnnotations.h>
#include <com/onecube/zen/compiler/ast/ExpressionAnnotation.h>
#include <com/onecube/zen/compiler/generator/ConstantPoolBuilder.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolTable.h>

#include <com/onecube/zen/virtual-machine/feb/ByteCode.h>
#include <com/onecube/zen/virtual-machine/feb/EntityFile.h>

/*******************************************************************************
 * BinaryEntityGenerator                                                       *
 *******************************************************************************/

struct zen_FunctionRecord_t {
};

/**
 * @author Samuel Rowe
 * @since zen 1.0
 * @ingroup zen_compiler_generator
 */
struct zen_BinaryEntityGenerator_t {
    zen_Compiler_t* m_compiler;
    zen_ASTListener_t* m_astListener;
    zen_BinaryEntityBuilder_t* m_builder;
    zen_SymbolTable_t* m_symbolTable;
    zen_ASTAnnotations_t* m_scopes;
    zen_ASTNode_t* m_compilationUnit;

    /**
     * The output stream where the generated binary entity will be written to.
     */
    jtk_OutputStream_t* m_outputStream;

    zen_EntityFile_t* m_entityFile;
    zen_ConstantPoolBuilder_t* m_constantPoolBuilder;
    uint8_t* m_package;
    int32_t m_packageSize;
    jtk_ArrayList_t* m_fields;
    jtk_ArrayList_t* m_functions;

    /* The following attributes are used to generate the instruction attribute.
     * Unfortunately, these fields are overwritten when multiple functions are being
     * declared simultaneously. For example, this situation may arise in nested
     * functions such as lambdas. A simple solution would be to refactor these
     * fields to another structure, say FunctionContext. Then the structure can
     * be stored in a hash map or stack contingent on the design of the code
     * generator.
     */

    // TODO: Rename this class!
    /**
     * The builder holds the channels where the instructions of the function
     * are written as they are generated.
     */
    zen_BinaryEntityBuilder_t* m_instructions;

    /**
     * The maximum number of operands the operand stack can store.
     * A simple way to count this is to increment this field whenever
     * a push_* instruction is generated.
     *
     * TODO: In the future, please make sure that this field does not exceed
     * the bounds of `uint16_t`.
     *
     */
    uint16_t m_maxStackSize;

    /**
     * The total number of local variables declared within the function being
     * declared.
     *
     * A simple way to count this is to increment this field whenever
     * a variable is declared within a function scope.
     */
    int32_t m_localVariableCount;

    /**
     * The list of exception handler sites within the function being declared.
     */
    jtk_ArrayList_t* m_exceptionHandlerSites;

    /* The descriptor of the function for which instructions are being
     * generated.
     */
    uint8_t* m_descriptor;
    int32_t m_descriptorSize;

    int32_t* m_breakRecords;
    int32_t m_breakRecordsCount;
    int32_t m_breakRecordsCapacity;
    int32_t m_nextLoopLabel;
    int32_t m_currentLoopLabel;

    zen_ASTNodeType_t m_mainComponent;
    bool m_classPrepared;
    uint8_t* m_className;
    int32_t m_classNameSize;

    uint16_t* m_cpfIndexes;
};

/**
 * @memberof BinaryEntityGenerator
 */
typedef struct zen_BinaryEntityGenerator_t zen_BinaryEntityGenerator_t;

// Constructor

/**
 * @memberof BinaryEntityGenerator
 */
zen_BinaryEntityGenerator_t* zen_BinaryEntityGenerator_new(zen_Compiler_t* compiler);

// Destructor

/**
 * @memberof BinaryEntityGenerator
 */
void zen_BinaryEntityGenerator_delete(zen_BinaryEntityGenerator_t* generator);

// Initialize

void zen_BinaryEntityGenerator_initializeCPFCache(zen_BinaryEntityGenerator_t* generator);

// Generate

/**
 * @memberof BinaryEntityGenerator
 */
void zen_BinaryEntityGenerator_generate(zen_BinaryEntityGenerator_t* generator);

// Reset

void zen_BinaryEntityGenerator_reset(zen_BinaryEntityGenerator_t* generator,
    zen_SymbolTable_t* symbolTable, zen_ASTAnnotations_t* scopes,
    zen_ASTNode_t* compilationUnit, const uint8_t* package, int32_t packageSize,
    jtk_OutputStream_t* outputStream);

// Descriptor

uint8_t* zen_BinaryEntityGenerator_getDescriptorEx(zen_ASTNode_t* functionParameters,
    bool constructor, int32_t* descriptorSize);

// Event Handlers

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onVisitErrorNode(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onVisitTerminal(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterEveryRule(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitEveryRule(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterCompilationUnit(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitCompilationUnit(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterImportDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitImportDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterAnnotatedComponentDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitAnnotatedComponentDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterAnnotations(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitAnnotations(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterAnnotation(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitAnnotation(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterAnnotationType(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitAnnotationType(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterAnnotationAttribute(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitAnnotationAttribute(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterComponentDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitComponentDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterFunctionDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitFunctionDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterFunctionParameters(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitFunctionParameters(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterFunctionBody(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitFunctionBody(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterStatementSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitStatementSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterSimpleStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitSimpleStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterEmptyStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitEmptyStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterVariableDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitVariableDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterVariableDeclarator(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitVariableDeclarator(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterConstantDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitConstantDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterConstantDeclarator(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitConstantDeclarator(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterAssertStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitAssertStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterBreakStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitBreakStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterContinueStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitContinueStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterReturnStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitReturnStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterThrowStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitThrowStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterCompoundStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitCompoundStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterIfStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitIfStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterIfClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitIfClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterElseIfClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitElseIfClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterElseClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitElseClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterIterativeStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitIterativeStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterLabel(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitLabel(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterWhileStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitWhileStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterForStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitForStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterForParameters(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitForParameters(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterTryStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitTryStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterTryClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitTryClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterCatchClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitCatchClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterCatchFilter(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitCatchFilter(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterFinallyClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitFinallyClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterSynchronizeStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitSynchronizeStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterWithStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitWithStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterClassDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitClassDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterClassExtendsClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitClassExtendsClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterClassSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitClassSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterClassMember(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitClassMember(zen_ASTListener_t* listener, zen_ASTNode_t* node);
// void zen_BinaryEntityGenerator_onEnterConstructorDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);
// void zen_BinaryEntityGenerator_onExitConstructorDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterEnumerationDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitEnumerationDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterEnumrationBaseClass(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitEnumerationBaseClause(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterEnumerationSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitEnumerationSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterEnumerate(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitEnumerate(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterExpressions(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitExpressions(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterAssignmentExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitAssignmentExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterConditionalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitConditionalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterLogicalOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitLogicalOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterLogicalAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitLogicalAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterInclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitInclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterExclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitExclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterEqualityExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitEqualityExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterRelationalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitRelationalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterShiftExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitShiftExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterAdditiveExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitAdditiveExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterMultiplicativeExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitMultiplicativeExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterUnaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitUnaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterPostfixExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitPostfixExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterSubscript(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitSubscript(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterFunctionArguments(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitFunctionArguments(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterMemberAccess(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitMemberAccess(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterPostfixOperator(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitPostfixOperator(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterPrimaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitPrimaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterMapExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitMapExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterMapEntries(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onExitMapEntries(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTListener
 */
void zen_BinaryEntityGenerator_onEnterMapEntry(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof BinaryEntityGenerator
 */
void zen_BinaryEntityGenerator_onExitMapEntry(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof BinaryEntityGenerator
 */
void zen_BinaryEntityGenerator_onEnterListExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof BinaryEntityGenerator
 */
void zen_BinaryEntityGenerator_onExitListExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof BinaryEntityGenerator
 */
void zen_BinaryEntityGenerator_onEnterNewExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof BinaryEntityGenerator
 */
void zen_BinaryEntityGenerator_onExitNewExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

#endif /* COM_ONECUBE_ZEN_COMPILER_GENERATOR_BINARY_ENTITY_GENERATOR_H */