/*
 * Copyright 2018-2019 OneCube
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

// Friday, March 30, 2018

#include <jtk/core/Integer.h>
#include <jtk/core/Long.h>
#include <jtk/core/VariableArguments.h>
#include <jtk/core/StringBuilder.h>
#include <jtk/collection/Pair.h>

#include <com/onecube/zen/compiler/ast/ASTWalker.h>
#include <com/onecube/zen/compiler/ast/context/Context.h>
#include <com/onecube/zen/compiler/ast/ASTAnnotation.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/ast/ASTHelper.h>
#include <com/onecube/zen/compiler/lexer/Token.h>
#include <com/onecube/zen/compiler/lexer/TokenType.h>
#include <com/onecube/zen/compiler/generator/BinaryEntityBuilder.h>
#include <com/onecube/zen/compiler/symbol-table/Symbol.h>
#include <com/onecube/zen/compiler/symbol-table/FunctionSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/ConstantSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/VariableSymbol.h>
#include <com/onecube/zen/compiler/generator/BinaryEntityGenerator.h>
#include <com/onecube/zen/virtual-machine/feb/EntityType.h>
#include <com/onecube/zen/virtual-machine/feb/attribute/InstructionAttribute.h>
#include <com/onecube/zen/virtual-machine/feb/attribute/PredefinedAttribute.h>

// Constructor

zen_BinaryEntityGenerator_t* zen_BinaryEntityGenerator_newEx(
    zen_SymbolTable_t* symbolTable, zen_ASTAnnotations_t* scopes,
    zen_ASTNode_t* compilationUnit, jtk_OutputStream_t* outputStream) {
    zen_BinaryEntityGenerator_t* generator = zen_Memory_allocate(zen_BinaryEntityGenerator_t, 1);
    generator->m_astListener = zen_ASTListener_newWithContext(generator);
    generator->m_builder = zen_BinaryEntityBuilder_new();
    generator->m_symbolTable = symbolTable;
    generator->m_scopes = scopes;
    generator->m_compilationUnit = compilationUnit;
    generator->m_outputStream = outputStream;
    generator->m_entityFile = zen_Memory_allocate(zen_EntityFile_t, 1);
    generator->m_constantPoolBuilder = zen_ConstantPoolBuilder_new();
    generator->m_package = NULL;
    generator->m_fields = jtk_ArrayList_new();
    generator->m_functions = jtk_ArrayList_new();

    generator->m_maxStackSize = 0;
    generator->m_localVariableCount = 0;
    generator->m_exceptionHandlerSites = jtk_ArrayList_new();

    zen_ASTListener_t* astListener = generator->m_astListener;

    astListener->m_onVisitErrorNode = zen_BinaryEntityGenerator_onVisitErrorNode;
    astListener->m_onVisitTerminal = zen_BinaryEntityGenerator_onVisitTerminal;

    astListener->m_onEnterEveryRule = zen_BinaryEntityGenerator_onEnterEveryRule;
    astListener->m_onExitEveryRule = zen_BinaryEntityGenerator_onExitEveryRule;

    astListener->m_onEnterCompilationUnit = zen_BinaryEntityGenerator_onEnterCompilationUnit;
    astListener->m_onExitCompilationUnit = zen_BinaryEntityGenerator_onExitCompilationUnit;

    astListener->m_onEnterImportDeclaration = zen_BinaryEntityGenerator_onEnterImportDeclaration;
    astListener->m_onExitImportDeclaration = zen_BinaryEntityGenerator_onExitImportDeclaration;

    astListener->m_onEnterAnnotatedComponentDeclaration = zen_BinaryEntityGenerator_onEnterAnnotatedComponentDeclaration;
    astListener->m_onExitAnnotatedComponentDeclaration = zen_BinaryEntityGenerator_onExitAnnotatedComponentDeclaration;

    astListener->m_onEnterAnnotations = zen_BinaryEntityGenerator_onEnterAnnotations;
    astListener->m_onExitAnnotations = zen_BinaryEntityGenerator_onExitAnnotations;

    astListener->m_onEnterAnnotation = zen_BinaryEntityGenerator_onEnterAnnotation;
    astListener->m_onExitAnnotation = zen_BinaryEntityGenerator_onExitAnnotation;

    astListener->m_onEnterAnnotationType = zen_BinaryEntityGenerator_onEnterAnnotationType;
    astListener->m_onExitAnnotationType = zen_BinaryEntityGenerator_onExitAnnotationType;

    astListener->m_onEnterAnnotationAttribute = zen_BinaryEntityGenerator_onEnterAnnotationAttribute;
    astListener->m_onExitAnnotationAttribute = zen_BinaryEntityGenerator_onExitAnnotationAttribute;

    astListener->m_onEnterComponentDeclaration = zen_BinaryEntityGenerator_onEnterComponentDeclaration;
    astListener->m_onExitComponentDeclaration = zen_BinaryEntityGenerator_onExitComponentDeclaration;

    astListener->m_onEnterFunctionDeclaration = zen_BinaryEntityGenerator_onEnterFunctionDeclaration;
    astListener->m_onExitFunctionDeclaration = zen_BinaryEntityGenerator_onExitFunctionDeclaration;

    astListener->m_onEnterFunctionParameters = zen_BinaryEntityGenerator_onEnterFunctionParameters;
    astListener->m_onExitFunctionParameters = zen_BinaryEntityGenerator_onExitFunctionParameters;

    astListener->m_onEnterFunctionBody = zen_BinaryEntityGenerator_onEnterFunctionBody;
    astListener->m_onExitFunctionBody = zen_BinaryEntityGenerator_onExitFunctionBody;

    astListener->m_onEnterStatementSuite = zen_BinaryEntityGenerator_onEnterStatementSuite;
    astListener->m_onExitStatementSuite = zen_BinaryEntityGenerator_onExitStatementSuite;

    astListener->m_onEnterSimpleStatement = zen_BinaryEntityGenerator_onEnterSimpleStatement;
    astListener->m_onExitSimpleStatement = zen_BinaryEntityGenerator_onExitSimpleStatement;

    astListener->m_onEnterStatement = zen_BinaryEntityGenerator_onEnterStatement;
    astListener->m_onExitStatement = zen_BinaryEntityGenerator_onExitStatement;

    astListener->m_onEnterEmptyStatement = zen_BinaryEntityGenerator_onEnterEmptyStatement;
    astListener->m_onExitEmptyStatement = zen_BinaryEntityGenerator_onExitEmptyStatement;

    astListener->m_onEnterVariableDeclaration = zen_BinaryEntityGenerator_onEnterVariableDeclaration;
    astListener->m_onExitVariableDeclaration = zen_BinaryEntityGenerator_onExitVariableDeclaration;

    astListener->m_onEnterVariableDeclarator = zen_BinaryEntityGenerator_onEnterVariableDeclarator;
    astListener->m_onExitVariableDeclarator = zen_BinaryEntityGenerator_onExitVariableDeclarator;

    astListener->m_onEnterConstantDeclaration = zen_BinaryEntityGenerator_onEnterConstantDeclaration;
    astListener->m_onExitConstantDeclaration = zen_BinaryEntityGenerator_onExitConstantDeclaration;

    astListener->m_onEnterConstantDeclarator = zen_BinaryEntityGenerator_onEnterConstantDeclarator;
    astListener->m_onExitConstantDeclarator = zen_BinaryEntityGenerator_onExitConstantDeclarator;

    astListener->m_onEnterAssertStatement = zen_BinaryEntityGenerator_onEnterAssertStatement;
    astListener->m_onExitAssertStatement = zen_BinaryEntityGenerator_onExitAssertStatement;

    astListener->m_onEnterBreakStatement = zen_BinaryEntityGenerator_onEnterBreakStatement;
    astListener->m_onExitBreakStatement = zen_BinaryEntityGenerator_onExitBreakStatement;

    astListener->m_onEnterContinueStatement = zen_BinaryEntityGenerator_onEnterContinueStatement;
    astListener->m_onExitContinueStatement = zen_BinaryEntityGenerator_onExitContinueStatement;

    astListener->m_onEnterReturnStatement = zen_BinaryEntityGenerator_onEnterReturnStatement;
    astListener->m_onExitReturnStatement = zen_BinaryEntityGenerator_onExitReturnStatement;

    astListener->m_onEnterThrowStatement = zen_BinaryEntityGenerator_onEnterThrowStatement;
    astListener->m_onExitThrowStatement = zen_BinaryEntityGenerator_onExitThrowStatement;

    astListener->m_onEnterCompoundStatement = zen_BinaryEntityGenerator_onEnterCompoundStatement;
    astListener->m_onExitCompoundStatement = zen_BinaryEntityGenerator_onExitCompoundStatement;

    astListener->m_onEnterIfStatement = zen_BinaryEntityGenerator_onEnterIfStatement;
    astListener->m_onExitIfStatement = zen_BinaryEntityGenerator_onExitIfStatement;

    astListener->m_onEnterIfClause = zen_BinaryEntityGenerator_onEnterIfClause;
    astListener->m_onExitIfClause = zen_BinaryEntityGenerator_onExitIfClause;

    astListener->m_onEnterElseIfClause = zen_BinaryEntityGenerator_onEnterElseIfClause;
    astListener->m_onExitElseIfClause = zen_BinaryEntityGenerator_onExitElseIfClause;

    astListener->m_onEnterElseClause = zen_BinaryEntityGenerator_onEnterElseClause;
    astListener->m_onExitElseClause = zen_BinaryEntityGenerator_onExitElseClause;

    astListener->m_onEnterIterativeStatement = zen_BinaryEntityGenerator_onEnterIterativeStatement;
    astListener->m_onExitIterativeStatement = zen_BinaryEntityGenerator_onExitIterativeStatement;

    astListener->m_onEnterLabelClause = zen_BinaryEntityGenerator_onEnterLabel;
    astListener->m_onExitLabelClause = zen_BinaryEntityGenerator_onExitLabel;

    astListener->m_onEnterWhileStatement = zen_BinaryEntityGenerator_onEnterWhileStatement;
    astListener->m_onExitWhileStatement = zen_BinaryEntityGenerator_onExitWhileStatement;

    astListener->m_onEnterForStatement = zen_BinaryEntityGenerator_onEnterForStatement;
    astListener->m_onExitForStatement = zen_BinaryEntityGenerator_onExitForStatement;

    astListener->m_onEnterForParameter = zen_BinaryEntityGenerator_onEnterForParameters;
    astListener->m_onExitForParameter = zen_BinaryEntityGenerator_onExitForParameters;

    astListener->m_onEnterTryStatement = zen_BinaryEntityGenerator_onEnterTryStatement;
    astListener->m_onExitTryStatement = zen_BinaryEntityGenerator_onExitTryStatement;

    astListener->m_onEnterTryClause = zen_BinaryEntityGenerator_onEnterTryClause;
    astListener->m_onExitTryClause = zen_BinaryEntityGenerator_onExitTryClause;

    astListener->m_onEnterCatchClause = zen_BinaryEntityGenerator_onEnterCatchClause;
    astListener->m_onExitCatchClause = zen_BinaryEntityGenerator_onExitCatchClause;

    astListener->m_onEnterCatchFilter = zen_BinaryEntityGenerator_onEnterCatchFilter;
    astListener->m_onExitCatchFilter = zen_BinaryEntityGenerator_onExitCatchFilter;

    astListener->m_onEnterFinallyClause = zen_BinaryEntityGenerator_onEnterFinallyClause;
    astListener->m_onExitFinallyClause = zen_BinaryEntityGenerator_onExitFinallyClause;

    astListener->m_onEnterSynchronizeStatement = zen_BinaryEntityGenerator_onEnterSynchronizeStatement;
    astListener->m_onExitSynchronizeStatement = zen_BinaryEntityGenerator_onExitSynchronizeStatement;

    astListener->m_onEnterWithStatement = zen_BinaryEntityGenerator_onEnterWithStatement;
    astListener->m_onExitWithStatement = zen_BinaryEntityGenerator_onExitWithStatement;

    astListener->m_onEnterClassDeclaration = zen_BinaryEntityGenerator_onEnterClassDeclaration;
    astListener->m_onExitClassDeclaration = zen_BinaryEntityGenerator_onExitClassDeclaration;

    astListener->m_onEnterClassExtendsClause = zen_BinaryEntityGenerator_onEnterClassExtendsClause;
    astListener->m_onExitClassExtendsClause = zen_BinaryEntityGenerator_onExitClassExtendsClause;

    astListener->m_onEnterClassSuite = zen_BinaryEntityGenerator_onEnterClassSuite;
    astListener->m_onExitClassSuite = zen_BinaryEntityGenerator_onExitClassSuite;

    astListener->m_onEnterClassMember = zen_BinaryEntityGenerator_onEnterClassMember;
    astListener->m_onExitClassMember = zen_BinaryEntityGenerator_onExitClassMember;

    // astListener->m_onEnterConstructorDeclaration = zen_BinaryEntityGenerator_onEnterConstructorDeclaration;
    // astListener->m_onExitConstructorDeclaration = zen_BinaryEntityGenerator_onExitConstructorDeclaration;

    astListener->m_onEnterEnumerationDeclaration = zen_BinaryEntityGenerator_onEnterEnumerationDeclaration;
    astListener->m_onExitEnumerationDeclaration = zen_BinaryEntityGenerator_onExitEnumerationDeclaration;

    astListener->m_onEnterEnumerationBaseClause = zen_BinaryEntityGenerator_onEnterEnumrationBaseClass;
    astListener->m_onExitEnumerationBaseClause = zen_BinaryEntityGenerator_onExitEnumerationBaseClause;

    astListener->m_onEnterEnumerationSuite = zen_BinaryEntityGenerator_onEnterEnumerationSuite;
    astListener->m_onExitEnumerationSuite = zen_BinaryEntityGenerator_onExitEnumerationSuite;

    astListener->m_onEnterEnumerate = zen_BinaryEntityGenerator_onEnterEnumerate;
    astListener->m_onExitEnumerate = zen_BinaryEntityGenerator_onExitEnumerate;

    astListener->m_onEnterExpressions = zen_BinaryEntityGenerator_onEnterExpressions;
    astListener->m_onExitExpressions = zen_BinaryEntityGenerator_onExitExpressions;

    astListener->m_onEnterExpression = zen_BinaryEntityGenerator_onEnterExpression;
    astListener->m_onExitExpression = zen_BinaryEntityGenerator_onExitExpression;

    astListener->m_onEnterAssignmentExpression = zen_BinaryEntityGenerator_onEnterAssignmentExpression;
    astListener->m_onExitAssignmentExpression = zen_BinaryEntityGenerator_onExitAssignmentExpression;

    astListener->m_onEnterConditionalExpression = zen_BinaryEntityGenerator_onEnterConditionalExpression;
    astListener->m_onExitConditionalExpression = zen_BinaryEntityGenerator_onExitConditionalExpression;

    astListener->m_onEnterLogicalOrExpression = zen_BinaryEntityGenerator_onEnterLogicalOrExpression;
    astListener->m_onExitLogicalOrExpression = zen_BinaryEntityGenerator_onExitLogicalOrExpression;

    astListener->m_onEnterLogicalAndExpression = zen_BinaryEntityGenerator_onEnterLogicalAndExpression;
    astListener->m_onExitLogicalAndExpression = zen_BinaryEntityGenerator_onExitLogicalAndExpression;

    astListener->m_onEnterInclusiveOrExpression = zen_BinaryEntityGenerator_onEnterInclusiveOrExpression;
    astListener->m_onExitInclusiveOrExpression = zen_BinaryEntityGenerator_onExitInclusiveOrExpression;

    astListener->m_onEnterExclusiveOrExpression = zen_BinaryEntityGenerator_onEnterExclusiveOrExpression;
    astListener->m_onExitExclusiveOrExpression = zen_BinaryEntityGenerator_onExitExclusiveOrExpression;

    astListener->m_onEnterAndExpression = zen_BinaryEntityGenerator_onEnterAndExpression;
    astListener->m_onExitAndExpression = zen_BinaryEntityGenerator_onExitAndExpression;

    astListener->m_onEnterEqualityExpression = zen_BinaryEntityGenerator_onEnterEqualityExpression;
    astListener->m_onExitEqualityExpression = zen_BinaryEntityGenerator_onExitEqualityExpression;

    astListener->m_onEnterRelationalExpression = zen_BinaryEntityGenerator_onEnterRelationalExpression;
    astListener->m_onExitRelationalExpression = zen_BinaryEntityGenerator_onExitRelationalExpression;

    astListener->m_onEnterShiftExpression = zen_BinaryEntityGenerator_onEnterShiftExpression;
    astListener->m_onExitShiftExpression = zen_BinaryEntityGenerator_onExitShiftExpression;

    astListener->m_onEnterAdditiveExpression = zen_BinaryEntityGenerator_onEnterAdditiveExpression;
    astListener->m_onExitAdditiveExpression = zen_BinaryEntityGenerator_onExitAdditiveExpression;

    astListener->m_onEnterMultiplicativeExpression = zen_BinaryEntityGenerator_onEnterMultiplicativeExpression;
    astListener->m_onExitMultiplicativeExpression = zen_BinaryEntityGenerator_onExitMultiplicativeExpression;

    astListener->m_onEnterUnaryExpression = zen_BinaryEntityGenerator_onEnterUnaryExpression;
    astListener->m_onExitUnaryExpression = zen_BinaryEntityGenerator_onExitUnaryExpression;

    astListener->m_onEnterPostfixExpression = zen_BinaryEntityGenerator_onEnterPostfixExpression;
    astListener->m_onExitPostfixExpression = zen_BinaryEntityGenerator_onExitPostfixExpression;

    astListener->m_onEnterSubscript = zen_BinaryEntityGenerator_onEnterSubscript;
    astListener->m_onExitSubscript = zen_BinaryEntityGenerator_onExitSubscript;

    astListener->m_onEnterFunctionArguments = zen_BinaryEntityGenerator_onEnterFunctionArguments;
    astListener->m_onExitFunctionArguments = zen_BinaryEntityGenerator_onExitFunctionArguments;

    astListener->m_onEnterMemberAccess = zen_BinaryEntityGenerator_onEnterMemberAccess;
    astListener->m_onExitMemberAccess = zen_BinaryEntityGenerator_onExitMemberAccess;

    astListener->m_onEnterPostfixOperator = zen_BinaryEntityGenerator_onEnterPostfixOperator;
    astListener->m_onExitPostfixOperator = zen_BinaryEntityGenerator_onExitPostfixOperator;

    astListener->m_onEnterPrimaryExpression = zen_BinaryEntityGenerator_onEnterPrimaryExpression;
    astListener->m_onExitPrimaryExpression = zen_BinaryEntityGenerator_onExitPrimaryExpression;

    astListener->m_onEnterMapExpression = zen_BinaryEntityGenerator_onEnterMapExpression;
    astListener->m_onExitMapExpression = zen_BinaryEntityGenerator_onExitMapExpression;

    astListener->m_onEnterMapEntries = zen_BinaryEntityGenerator_onEnterMapEntries;
    astListener->m_onExitMapEntries = zen_BinaryEntityGenerator_onExitMapEntries;

    astListener->m_onEnterMapEntry = zen_BinaryEntityGenerator_onEnterMapEntry;
    astListener->m_onExitMapEntry = zen_BinaryEntityGenerator_onExitMapEntry;

    astListener->m_onEnterListExpression = zen_BinaryEntityGenerator_onEnterListExpression;
    astListener->m_onExitListExpression = zen_BinaryEntityGenerator_onExitListExpression;

    astListener->m_onEnterNewExpression = zen_BinaryEntityGenerator_onEnterNewExpression;
    astListener->m_onExitNewExpression = zen_BinaryEntityGenerator_onExitNewExpression;

    return generator;
}

// Constructor

void zen_BinaryEntityGenerator_delete(zen_BinaryEntityGenerator_t* generator) {
    jtk_Assert_assertObject(generator, "The specified generator is null.");

    /* This code here looks so convoluted. Oops! :V */

#warning "TODO: The exception handler sites must be deleted when the associated function entity and instruction attribute are destroyed."
    int32_t exceptionHandlerSiteCount = jtk_ArrayList_getSize(generator->m_exceptionHandlerSites);
    int32_t exceptionHandlerSiteIndex;
    for (exceptionHandlerSiteIndex = 0;
        exceptionHandlerSiteIndex < exceptionHandlerSiteCount;
        exceptionHandlerSiteIndex++) {
        zen_ExceptionHandlerSite_t* exceptionHandlerSite =
            (zen_ExceptionHandlerSite_t*)jtk_ArrayList_getValue(
                generator->m_exceptionHandlerSites,
                exceptionHandlerSiteIndex);
        jtk_Memory_deallocate(exceptionHandlerSite);
    }
    jtk_ArrayList_delete(generator->m_exceptionHandlerSites);

    zen_BinaryEntityBuilder_delete(generator->m_builder);

    int32_t fieldCount = jtk_ArrayList_getSize(generator->m_fields);
    int32_t fieldIndex;
    for (fieldIndex = 0; fieldIndex < fieldCount; fieldIndex++) {
        zen_FieldEntity_t* fieldEntity = (zen_FieldEntity_t*)jtk_ArrayList_getValue(
            generator->m_fields, fieldIndex);
        zen_FieldEntity_delete(fieldEntity);
    }
    jtk_ArrayList_delete(generator->m_fields);

    int32_t functionCount = jtk_ArrayList_getSize(generator->m_functions);
    int32_t functionIndex;
    for (functionIndex = 0; functionIndex < functionCount; functionIndex++) {
        zen_FunctionEntity_t* functionEntity = (zen_FunctionEntity_t*)jtk_ArrayList_getValue(
            generator->m_functions, functionIndex);
        zen_FunctionEntity_delete(functionEntity);
    }
    jtk_ArrayList_delete(generator->m_functions);

    /* Destroy the constant pool builder. It takes care of destroying the
     * constant pool entries.
     */
    zen_ConstantPoolBuilder_delete(generator->m_constantPoolBuilder);
    zen_Memory_deallocate(generator->m_entityFile);

    zen_ASTListener_delete(generator->m_astListener);
    jtk_Memory_deallocate(generator);
}

// Generate

void zen_BinaryEntityGenerator_generate(zen_BinaryEntityGenerator_t* generator) {
    jtk_Assert_assertObject(generator, "The specified generator is null.");

    zen_ASTWalker_walk(generator->m_astListener, generator->m_compilationUnit);
    // zen_BinaryEntityGenerator_writeBytes(generator, generator->m_outputStream);
    zen_BinaryEntityGenerator_writeEntity(generator);
}

// Reset

void zen_BinaryEntityGenerator_reset(zen_BinaryEntityGenerator_t* generator,
    zen_SymbolTable_t* symbolTable, zen_ASTAnnotations_t* scopes,
    zen_ASTNode_t* compilationUnit, jtk_OutputStream_t* outputStream) {
    jtk_Assert_assertObject(generator, "The specified generator is null.");

    // zen_BinaryEntityBuilder_clear(generator->m_builder);
    generator->m_symbolTable = symbolTable;
    generator->m_scopes = scopes;
    generator->m_compilationUnit = compilationUnit;
    generator->m_outputStream = outputStream;
}

// Event Handlers

void zen_BinaryEntityGenerator_onVisitErrorNode(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    fprintf(stderr, "[warning] Cannot resolve symbols on erroneous AST.\n");
}

void zen_BinaryEntityGenerator_onVisitTerminal(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onEnterEveryRule(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitEveryRule(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

/* compilationUnit */

void zen_BinaryEntityGenerator_onEnterCompilationUnit(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    /* Retrieve the generator associated with the AST listener. */
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    /* Retrieve the scope associated with the current AST node. */
    zen_Scope_t* scope = zen_ASTAnnotations_get(generator->m_scopes, node);
    /* Activate the scope associated with the current AST node. */
    zen_SymbolTable_setCurrentScope(generator->m_symbolTable, scope);

    /* Push a data channel, where the bytes generated will be written. */
    int32_t primaryChannelIndex = zen_BinaryEntityBuilder_addChannel(generator->m_builder);
    zen_BinaryEntityBuilder_setActiveChannelIndex(generator->m_builder, primaryChannelIndex);
}

void zen_BinaryEntityGenerator_onExitCompilationUnit(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    /* Retrieve the generator associated with the AST listener. */
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;

    /* Set the major version of the target binary entity format. */
    generator->m_entityFile->m_version.m_majorVersion = 0x0000;
    /* Set the minor version of the target binary entity format. */
    generator->m_entityFile->m_version.m_minorVersion = 0x0001;

    /* Invalidate the current scope in the symbol table. */
    zen_SymbolTable_invalidateCurrentScope(generator->m_symbolTable);

    /* Push a data channel, where the bytes generated will be written. */
    // int32_t primaryChannelIndex = zen_BinaryEntityBuilder_addChannel(generator->m_builder);
    // zen_BinaryEntityBuilder_setActiveChannelIndex(generator->m_builder, primaryChannelIndex);
}

#include <stdio.h>

void zen_BinaryEntityGenerator_writeEntity(zen_BinaryEntityGenerator_t* generator) {
    /* Write magic number, major version, and minor version on the main channel. */
    zen_BinaryEntityBuilder_writeMagicNumber(generator->m_builder);
    /* Log the magic number written. */
    printf("[log] Encoding stream with magic number 0xFEB72000.\n");

    /* Write the major version of the binary entity file format the stream is encoded in. */
    zen_BinaryEntityBuilder_writeMajorVersion(generator->m_builder,
        generator->m_entityFile->m_version.m_majorVersion);

    /* Write the minor version of the binary entity file format the stream is encoded in. */
    zen_BinaryEntityBuilder_writeMinorVersion(generator->m_builder,
        generator->m_entityFile->m_version.m_minorVersion);

    /* Log the version of the target binary entity file format. */
    printf("[log] The target binary entity format version is v%d.%d\n",
        generator->m_entityFile->m_version.m_majorVersion,
        generator->m_entityFile->m_version.m_minorVersion);

    /* Write additional flags on how the binary entity file should be loaded. */
    zen_BinaryEntityBuilder_writeStreamFlags(generator->m_builder,
        generator->m_entityFile->m_flags);

    /* Log the flags that determine how the binary entity should be treated. */
    printf("[log] Applying flags 0x%X\n", generator->m_entityFile->m_flags);

    /* At this point, all the constant pool entries required by the binary entity
     * file should be available to the constant pool builder. The constant pool
     * can now be built.
     */
    int32_t entryCount = zen_ConstantPoolBuilder_countEntries(generator->m_constantPoolBuilder);
    /* The constant pool builder counts the null entry at the beginning, too.
     * However, the virtual machine's binary entity parser implicitly recognizes
     * first entry. Therefore, subtract the constant pool entry count by one.
     */
    zen_BinaryEntityBuilder_writeConstantPoolHeader(generator->m_builder, entryCount - 1);
    int32_t i;
    for (i = 1; i < entryCount; i++) {
        zen_ConstantPoolEntry_t* entry = zen_ConstantPoolBuilder_getEntry(generator->m_constantPoolBuilder, i);
        zen_BinaryEntityBuilder_writeConstantPoolEntry(generator->m_builder, entry);

        switch (entry->m_tag) {
            case ZEN_CONSTANT_POOL_TAG_INTEGER: {
                zen_ConstantPoolInteger_t* constantPoolInteger =
                    (zen_ConstantPoolInteger_t*)entry;

                printf("[debug] #%d Integer (value* = %d)\n", i,
                    constantPoolInteger->m_bytes);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_LONG: {
                zen_ConstantPoolLong_t* constantPoolLong =
                    (zen_ConstantPoolLong_t*)entry;

                printf("[debug] #%d Long (value* = %d)\n", i,
                    (constantPoolLong->m_highBytes << 32) | constantPoolLong->m_lowBytes);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FLOAT: {
                zen_ConstantPoolFloat_t* constantPoolFloat =
                    (zen_ConstantPoolFloat_t*)entry;

                printf("[debug] #%d Float (value* = %f)\n", i,
                    jtk_Float_pack(constantPoolFloat->m_bytes));

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_DOUBLE: {
                zen_ConstantPoolDouble_t* constantPoolDouble =
                    (zen_ConstantPoolDouble_t*)entry;

                printf("[debug] #%d Double (value* = %f)\n", i,
                    jtk_Double_pack((constantPoolDouble->m_highBytes << 32) |
                        constantPoolDouble->m_lowBytes));

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_UTF8: {
                zen_ConstantPoolUtf8_t* constantPoolUtf8 =
                    (zen_ConstantPoolUtf8_t*)entry;

                printf("[debug] #%d UTF-8 (length = %d, bytes = \"%.*s\")\n", i,
                    constantPoolUtf8->m_length, constantPoolUtf8->m_length,
                    constantPoolUtf8->m_bytes);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_STRING: {
                zen_ConstantPoolString_t* constantPoolString =
                    (zen_ConstantPoolString_t*)entry;

                printf("[debug] #%d String (string = %d)\n", i, constantPoolString->m_stringIndex);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FUNCTION: {
                zen_ConstantPoolFunction_t* constantPoolFunction =
                    (zen_ConstantPoolFunction_t*)entry;

                printf("[debug] #%d Function (class = %d, descriptor = %d, name = %d)\n",
                    i, constantPoolFunction->m_classIndex, constantPoolFunction->m_descriptorIndex,
                    constantPoolFunction->m_nameIndex);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FIELD: {
                zen_ConstantPoolField_t* constantPoolField =
                    (zen_ConstantPoolField_t*)entry;

                printf("[debug] #%d Field (class = %d, descriptor = %d, name = %d)\n",
                    i, constantPoolField->m_classIndex, constantPoolField->m_descriptorIndex,
                    constantPoolField->m_nameIndex);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_CLASS: {
                zen_ConstantPoolClass_t* constantPoolClass =
                    (zen_ConstantPoolClass_t*)entry;

                printf("[debug] #%d Class (name = %d)\n", i, constantPoolClass->m_nameIndex);

                break;
            }

            default: {
                printf("[error] Unknown constant pool tag %d\n", entry->m_tag);
            }
        }
    }

    /* Log the constant pool details, including the number of entries and the trends
     * seen among the entries.
     */
    printf("[log] Constant pool size is %d.\n", entryCount);

    /* Retrieve the entity to write. */
    zen_Entity_t* entity = &generator->m_entityFile->m_entity;

    /* Write the entity header. */
    zen_BinaryEntityBuilder_writeEntityHeader(generator->m_builder, entity->m_type,
        entity->m_flags, entity->m_reference);
    /* Log the entity header information written, including type, flags, and reference. */
    printf("[debug] Entity header includes type = %d, flags = 0x%X, and reference = %d.\n",
        entity->m_type, entity->m_flags, entity->m_reference);

    /* Write the superclasses. */
    zen_BinaryEntityBuilder_writeSuperclasses(generator->m_builder, entity->m_superclassCount,
        entity->m_superclasses);
    /* Log the superclass indexes that the entity inherits. */
    printf("[debug] Entity inherits %d superclasses.\n", entity->m_superclassCount);

    /* Write the attribute count. */
    zen_BinaryEntityBuilder_writeAttributeCount(generator->m_builder, entity->m_attributeTable.m_size);
    /* Log the attribute count. */
    printf("[debug] Entity has %d attributes.\n", entity->m_attributeTable.m_size);

    // TODO: Write the attribute

    /* Retrieve the field count. */
    int32_t fieldCount = jtk_ArrayList_getSize(generator->m_fields);
    /* Write the field count. */
    zen_BinaryEntityBuilder_writeFieldCount(generator->m_builder, fieldCount);
    /* Log the field count. */
    printf("[debug] Entity has %d fields.\n", fieldCount);

    int32_t fieldIndex;
    for (fieldIndex = 0; fieldIndex < fieldCount; fieldIndex++) {
        /* Retrieve the next field to write. */
        zen_FieldEntity_t* fieldEntity = (zen_FieldEntity_t*)jtk_ArrayList_getValue(
            generator->m_fields, fieldIndex);

        /* Write the field to the data channel. */
        zen_BinaryEntityBuilder_writeField(generator->m_builder, fieldEntity->m_flags,
            fieldEntity->m_nameIndex, fieldEntity->m_descriptorIndex);
            
        /* Write the attribute count. */
        zen_BinaryEntityBuilder_writeAttributeCount(generator->m_builder, fieldEntity->m_attributeTable.m_size);

        // TODO: Write the attribute!

        /* Log the details of the field. */
        printf("[debug] A field was written with the features (flags = 0x%X, nameIndex = %d, descriptorIndex = %d).\n",
            fieldEntity->m_flags, fieldEntity->m_nameIndex, fieldEntity->m_descriptorIndex);
        
        /* Log the attribute count. */
        printf("[debug] Field entity has %d attributes.\n", fieldEntity->m_attributeTable.m_size);
    }

    /* Retrieve the function count. */
    int32_t functionCount = jtk_ArrayList_getSize(generator->m_functions);
    /* Write the function count. */
    zen_BinaryEntityBuilder_writeFunctionCount(generator->m_builder, functionCount);
    /* Log the function count. */
    printf("[debug] Entity has %d functions.\n", functionCount);

    int32_t functionIndex;
    for (functionIndex = 0; functionIndex < functionCount; functionIndex++) {
        /* Retrieve the next function to write. */
        zen_FunctionEntity_t* functionEntity = (zen_FunctionEntity_t*)jtk_ArrayList_getValue(
            generator->m_functions, functionIndex);

        /* Write the function to the data channel. */
        zen_BinaryEntityBuilder_writeFunction(generator->m_builder,
            functionEntity->m_nameIndex, functionEntity->m_descriptorIndex,
            functionEntity->m_flags);

        /* Log the details of the function. */
        printf("[debug] A function was written with the features (flags = 0x%X, nameIndex = %d, descriptorIndex = %d).\n",
            functionEntity->m_flags, functionEntity->m_nameIndex, functionEntity->m_descriptorIndex);

        /* Retrieve the attribute table for the current function entity. */
        zen_AttributeTable_t* attributeTable = &functionEntity->m_attributeTable;

        /* Write the total number of attributes. */
        zen_BinaryEntityBuilder_writeAttributeCount(generator->m_builder, attributeTable->m_size);

        /* Log the total number of attributes declared in the attribute table. */
        printf("[debug] The function has %d attributes.\n", attributeTable->m_size);

        int32_t attributeIndex;
        for (attributeIndex = 0; attributeIndex < attributeTable->m_size; attributeIndex++) {
            zen_Attribute_t* attribute = attributeTable->m_attributes[attributeIndex];

            /* Retrieve the name of the current attribute. */
            zen_ConstantPoolUtf8_t* name = zen_ConstantPoolBuilder_getUtf8Entry(
                generator->m_constantPoolBuilder, attribute->m_nameIndex);

            /* If the current attribute is an instruction attribute, extract
             * the instruction attribute and write to the data channel.
             */
            if (jtk_CString_equals(name->m_bytes, name->m_length,
                ZEN_PREDEFINED_ATTRIBUTE_INSTRUCTION, ZEN_PREDEFINED_ATTRIBUTE_INSTRUCTION_SIZE)) {
                /* Cast to zen_InstructionAttribute_t to extract further information. */
                zen_InstructionAttribute_t* instructionAttribute =
                    (zen_InstructionAttribute_t*)attribute;

                // TODO: CHANGE THIS!!!!!!!!!!!!!!!!!!!!!!!!!!
                instructionAttribute->m_maxStackSize = 100;

                /* Write the instruction attribute for the current function. */
                zen_BinaryEntityBuilder_writeInstructionAttribute(
                    generator->m_builder,
                    instructionAttribute->m_nameIndex,
                    instructionAttribute->m_length,
                    instructionAttribute->m_maxStackSize,
                    instructionAttribute->m_localVariableCount,
                    instructionAttribute->m_instructionLength,
                    instructionAttribute->m_instructions,
                    &instructionAttribute->m_exceptionTable);

                /* Log the details of the instruction attribute. */
                printf("[debug] The function has an instruction attribute with the features "
                       "(nameIndex = %d, length = %d, maxStackSize = %d, localVariableCount = %d, "
                       "instructionLength = %d)\n",
                        instructionAttribute->m_nameIndex,
                        instructionAttribute->m_length,
                        instructionAttribute->m_maxStackSize,
                        instructionAttribute->m_localVariableCount,
                        instructionAttribute->m_instructionLength);

                // TODO: Write the exception table.
            }
        }
    }

    FILE* fp = fopen("/mnt/g/project/pulsar/build/Test.feb", "w+");
    if (fp != NULL) {
        zen_DataChannel_t* channel = jtk_ArrayList_getValue(generator->m_builder->m_channels, 0);
        fwrite(channel->m_bytes, channel->m_index, 1, fp);
        fclose(fp);
    }
}

// importDeclaration

void zen_BinaryEntityGenerator_onEnterImportDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitImportDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// annotatedComponentDeclaration

void zen_BinaryEntityGenerator_onEnterAnnotatedComponentDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitAnnotatedComponentDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// annotations

void zen_BinaryEntityGenerator_onEnterAnnotations(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitAnnotations(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// annotation

void zen_BinaryEntityGenerator_onEnterAnnotation(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitAnnotation(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// annotationType

void zen_BinaryEntityGenerator_onEnterAnnotationType(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitAnnotationType(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// annotationAttribute

void zen_BinaryEntityGenerator_onEnterAnnotationAttribute(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitAnnotationAttribute(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// componentDeclaration

void zen_BinaryEntityGenerator_onEnterComponentDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitComponentDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// functionDeclaration

void zen_BinaryEntityGenerator_onEnterFunctionDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_FunctionDeclarationContext_t* context = (zen_FunctionDeclarationContext_t*)node->m_context;

    zen_Scope_t* scope = zen_ASTAnnotations_get(generator->m_scopes, node);
    zen_SymbolTable_setCurrentScope(generator->m_symbolTable, scope);

    int32_t instructionChannelIndex = zen_BinaryEntityBuilder_addChannel(generator->m_builder);
    zen_BinaryEntityBuilder_setActiveChannelIndex(generator->m_builder, instructionChannelIndex);

    // TODO: Remove the following statement. Make sure that the instruction
    // length is never zero.
    zen_BinaryEntityBuilder_emitNop(generator->m_builder);
}

/* The format of a function descriptor is shown below:
 * descriptor
 * :    returnType ':' parameters
 * ;
 *
 * returnType
 * :    type
 * ;
 *
 * parameters
 * :    type+
 * ;
 *
 * type
 * :    'v'
 * |    valueType
 * ;
 *
 * valueType
 * :    'z'
 * |    'b'
 * |    'c'
 * |    's'
 * |    'i'
 * |    'l'
 * |    'f'
 * |    'd'
 * |    className
 * |    arrayType
 * ;
 *
 * arrayType
 * :    '@'+ valueType
 * ;
 *
 * className
 * :    '(' IDENTIFIER ('/' IDENTIFIER)* ')'
 * ;
 *
 * Here is an example function.
 * function printf(format, ...arguments)
 *
 * The function descriptor for this function is shown below.
 * (zen/core/Object):(zen/core/Object)@(zen/core/Object)
 */
jtk_String_t* zen_BinaryEntityGenerator_getDescriptorEx(zen_BinaryEntityGenerator_t* generator,
    zen_ASTNode_t* functionParameters, bool constructor) {
    zen_FunctionParametersContext_t* functionParametersContext =
        (zen_FunctionParametersContext_t*)functionParameters->m_context;

    jtk_StringBuilder_t* builder = jtk_StringBuilder_new();
    jtk_StringBuilder_append_z(builder, constructor? "v:" : "(zen/core/Object):");

    int32_t fixedParameterCount = jtk_ArrayList_getSize(functionParametersContext->m_fixedParameters);
    int32_t i;
    for (i = 0; i < fixedParameterCount; i++) {
        jtk_StringBuilder_append_z(builder, "(zen/core/Object)");
    }

    if (functionParametersContext->m_variableParameter != NULL) {
        jtk_StringBuilder_append_z(builder, "@(zen/core/Object)");
    }

    if ((fixedParameterCount == 0) &&
        (functionParametersContext->m_variableParameter == NULL)) {
        jtk_StringBuilder_append_c(builder, 'v');
    }

    jtk_String_t* result = jtk_StringBuilder_toString(builder);

    /* Destroy the string builder. */
    jtk_StringBuilder_delete(builder);

    return result;
}

jtk_String_t* zen_BinaryEntityGenerator_getDescriptor(zen_BinaryEntityGenerator_t* generator,
    zen_ASTNode_t* functionParameters) {
    return zen_BinaryEntityGenerator_getDescriptorEx(generator, functionParameters, false);
}

zen_InstructionAttribute_t* zen_BinaryEntityGenerator_makeInstructionAttribute(
    zen_BinaryEntityGenerator_t* generator) {

    /* Retrieve the data channel on which the instructions of the
     * function/initializer were written.
     */
    zen_DataChannel_t* channel = zen_BinaryEntityBuilder_getActiveChannel(generator->m_builder);
    /* Retrieve the bytes that were written on the data channel. */
    uint8_t* instructionBytes = zen_DataChannel_getBytes(channel);

    /* Retrieve a valid index into the constant pool where an UTF-8 entry
     * represents "vm/primary/Instruction".
     */
    uint16_t attributeNameIndex = zen_ConstantPoolBuilder_getUtf8EntryIndexEx(
        generator->m_constantPoolBuilder, ZEN_PREDEFINED_ATTRIBUTE_INSTRUCTION,
        ZEN_PREDEFINED_ATTRIBUTE_INSTRUCTION_SIZE);
    /* Load the maximum stack size. */
    uint16_t maxStackSize = generator->m_maxStackSize;
    /* Load the number of local variables. */
    uint16_t localVariableCount = generator->m_localVariableCount;
    /* Retrieve the length of the instructions. */
    uint32_t instructionLength = zen_DataChannel_getSize(channel);
    /* The instructions of the function. */
    uint8_t* instructions = jtk_Arrays_clone_b(instructionBytes, instructionLength);
    /* The total number of exception handler sistes within the function. */
    uint16_t exceptionHandlerSiteCount = 0;
    /* Calculate the length of the attribute immediately after the m_length
     * field.
     */
    uint32_t attributeLength =
        2 + // maxStackSize occupies two bytes.
        2 + // localVariableCount occupies two bytes.
        4 + // instructionLength occupies four bytes.
        instructionLength + // The total number of bytes the instructions occupy.
        2 + // m_exceptionTable.m_size occupies two bytes.
        exceptionHandlerSiteCount * ( // Each exception handler site entry occupies the following width.
            2 + // startIndex occupies two bytes.
            2 + // stopIndex occupies two bytes.
            2 + // handlerIndex occupies two bytes.
            2); // exceptionClassIndex occupies two bytes.

    zen_InstructionAttribute_t* instructionAttribute = zen_InstructionAttribute_new(
        attributeNameIndex,
        attributeLength,
        maxStackSize,
        localVariableCount,
        instructionLength,
        instructions);

    zen_ExceptionTable_t* exceptionTable = &instructionAttribute->m_exceptionTable;
    exceptionTable->m_size = jtk_ArrayList_getSize(generator->m_exceptionHandlerSites);
    exceptionTable->m_exceptionHandlerSites = jtk_Memory_allocate(zen_ExceptionHandlerSite_t*,
        exceptionTable->m_size);
#warning "TODO: Destroy the memory allocated here."

    int32_t exceptionHandlerSiteIndex;
    for (exceptionHandlerSiteIndex = 0;
        exceptionHandlerSiteIndex < exceptionTable->m_size;
        exceptionHandlerSiteIndex++) {
        zen_ExceptionHandlerSite_t* exceptionHandlerSite =
            (zen_ExceptionHandlerSite_t*)jtk_ArrayList_getValue(
                generator->m_exceptionHandlerSites, exceptionHandlerSiteIndex);
        exceptionTable->m_exceptionHandlerSites[exceptionHandlerSiteIndex] =
            exceptionHandlerSite;
    }

    jtk_StringBuilder_t* builder = jtk_StringBuilder_new();
    zen_BinaryEntityDisassember_disassembleInstructions(instructionBytes, instructionLength, builder);
    printf("%.*s", builder->m_size, builder->m_value);
    jtk_StringBuilder_delete(builder);

    return instructionAttribute;
}

void zen_BinaryEntityDisassember_disassembleInstructions(
    uint8_t* bytes, int32_t size, jtk_StringBuilder_t* builder) {

    int32_t i;
    for (i = 0; i < size; i++) {
        uint8_t byteCode = bytes[i];
        jtk_StringBuilder_appendEx_z(builder, "[debug] #", 9);
        jtk_StringBuilder_append_i(builder, i);
        jtk_StringBuilder_appendEx_z(builder, " ", 1);

        switch (byteCode) {
            /* No Operation */
            case ZEN_BYTE_CODE_NOP: {
                jtk_StringBuilder_appendEx_z(builder, "nop", 3);

                break;
            }

            /* Add */

            case ZEN_BYTE_CODE_ADD_I: {
                jtk_StringBuilder_appendEx_z(builder, "add_i", 5);

                break;
            }

            case ZEN_BYTE_CODE_ADD_L: {
                jtk_StringBuilder_appendEx_z(builder, "add_l", 5);

                break;
            }

            case ZEN_BYTE_CODE_ADD_F: {
                jtk_StringBuilder_appendEx_z(builder, "add_f", 5);

                break;
            }

            case ZEN_BYTE_CODE_ADD_D: {
                jtk_StringBuilder_appendEx_z(builder, "add_d", 5);

                break;
            }

            /* Bitwise AND */

            case ZEN_BYTE_CODE_AND_I: {
                jtk_StringBuilder_appendEx_z(builder, "and_i", 5);

                break;
            }

            case ZEN_BYTE_CODE_AND_L: {
                jtk_StringBuilder_appendEx_z(builder, "and_l", 5);

                break;
            }

            /* Bitwise OR */

            case ZEN_BYTE_CODE_OR_I: {
                jtk_StringBuilder_appendEx_z(builder, "or_i", 4);

                break;
            }

            case ZEN_BYTE_CODE_OR_L: {
                jtk_StringBuilder_appendEx_z(builder, "or_l", 4);

                break;
            }

            /* Bitwise Shift */

            case ZEN_BYTE_CODE_SHIFT_LEFT_I: {
                jtk_StringBuilder_appendEx_z(builder, "shift_left_i", 12);

                break;
            }

            case ZEN_BYTE_CODE_SHIFT_LEFT_L: {
                jtk_StringBuilder_appendEx_z(builder, "shift_left_l", 12);

                break;
            }

            case ZEN_BYTE_CODE_SHIFT_RIGHT_I: {
                jtk_StringBuilder_appendEx_z(builder, "shift_right_i", 13);

                break;
            }

            case ZEN_BYTE_CODE_SHIFT_RIGHT_L: {
                jtk_StringBuilder_appendEx_z(builder, "shift_right_l", 13);

                break;
            }

            case ZEN_BYTE_CODE_SHIFT_RIGHT_UI: {
                jtk_StringBuilder_appendEx_z(builder, "shift_right_ui", 14);

                break;
            }

            case ZEN_BYTE_CODE_SHIFT_RIGHT_UL: {
                jtk_StringBuilder_appendEx_z(builder, "shift_right_ul", 14);

                break;
            }

            /* Bitwise XOR */

            case ZEN_BYTE_CODE_XOR_I: {
                jtk_StringBuilder_appendEx_z(builder, "xor_i", 5);

                break;
            }

            case ZEN_BYTE_CODE_XOR_L: {
                jtk_StringBuilder_appendEx_z(builder, "xor_l", 5);

                break;
            }

            /* Cast */

            case ZEN_BYTE_CODE_CAST_ITL: {
                jtk_StringBuilder_appendEx_z(builder, "cast_itl", 8);

                break;
            }

            case ZEN_BYTE_CODE_CAST_ITF: {
                jtk_StringBuilder_appendEx_z(builder, "cast_itf", 8);

                break;
            }

            case ZEN_BYTE_CODE_CAST_ITD: {
                jtk_StringBuilder_appendEx_z(builder, "cast_itd", 8);

                break;
            }

            case ZEN_BYTE_CODE_CAST_LTI: {
                jtk_StringBuilder_appendEx_z(builder, "cast_lti", 8);

                break;
            }

            case ZEN_BYTE_CODE_CAST_LTF: {
                jtk_StringBuilder_appendEx_z(builder, "cast_itf", 8);

                break;
            }

            case ZEN_BYTE_CODE_CAST_LTD: {
                jtk_StringBuilder_appendEx_z(builder, "cast_itd", 8);

                break;
            }

            case ZEN_BYTE_CODE_CAST_FTI: {
                jtk_StringBuilder_appendEx_z(builder, "cast_fti", 8);

                break;
            }

            case ZEN_BYTE_CODE_CAST_FTL: {
                jtk_StringBuilder_appendEx_z(builder, "cast_ftl", 8);

                break;
            }

            case ZEN_BYTE_CODE_CAST_FTD: {
                jtk_StringBuilder_appendEx_z(builder, "cast_ftd", 8);

                break;
            }

            case ZEN_BYTE_CODE_CAST_DTI: {
                jtk_StringBuilder_appendEx_z(builder, "cast_dti", 8);

                break;
            }

            case ZEN_BYTE_CODE_CAST_DTL: {
                jtk_StringBuilder_appendEx_z(builder, "cast_dtl", 8);

                break;
            }

            case ZEN_BYTE_CODE_CAST_DTF: {
                jtk_StringBuilder_appendEx_z(builder, "cast_dtf", 8);

                break;
            }

            case ZEN_BYTE_CODE_CAST_ITC: {
                jtk_StringBuilder_appendEx_z(builder, "cast_itc", 8);

                break;
            }

            /* Check Cast */

            case ZEN_BYTE_CODE_CHECK_CAST: {
                jtk_StringBuilder_appendEx_z(builder, "check_cast", 10);

                break;
            }

            /* Compare */

            case ZEN_BYTE_CODE_COMPARE_L: {
                jtk_StringBuilder_appendEx_z(builder, "compare_l", 9);

                break;
            }

            case ZEN_BYTE_CODE_COMPARE_LT_F: {
                jtk_StringBuilder_appendEx_z(builder, "compare_lt_f", 12);

                break;
            }

            case ZEN_BYTE_CODE_COMPARE_GT_F: {
                jtk_StringBuilder_appendEx_z(builder, "compare_gt_f", 12);

                break;
            }

            case ZEN_BYTE_CODE_COMPARE_LT_D: {
                jtk_StringBuilder_appendEx_z(builder, "compare_lt_d", 12);

                break;
            }

            case ZEN_BYTE_CODE_COMPARE_GT_D: {
                jtk_StringBuilder_appendEx_z(builder, "compare_gt_d", 12);

                break;
            }

            /* Divide */

            case ZEN_BYTE_CODE_DIVIDE_I: {
                jtk_StringBuilder_appendEx_z(builder, "divide_i", 8);

                break;
            }

            case ZEN_BYTE_CODE_DIVIDE_L: {
                jtk_StringBuilder_appendEx_z(builder, "divide_l", 8);

                break;
            }

            case ZEN_BYTE_CODE_DIVIDE_F: {
                jtk_StringBuilder_appendEx_z(builder, "divide_f", 8);

                break;
            }

            case ZEN_BYTE_CODE_DIVIDE_D: {
                jtk_StringBuilder_appendEx_z(builder, "divide_d", 8);

                break;
            }

            /* Duplicate */

            case ZEN_BYTE_CODE_DUPLICATE: {
                jtk_StringBuilder_appendEx_z(builder, "duplicate", 9);

                break;
            }

            case ZEN_BYTE_CODE_DUPLICATE_X1: {
                jtk_StringBuilder_appendEx_z(builder, "duplicate_x1", 12);

                break;
            }

            case ZEN_BYTE_CODE_DUPLICATE_X2: {
                jtk_StringBuilder_appendEx_z(builder, "duplicate_x2", 12);

                break;
            }

            case ZEN_BYTE_CODE_DUPLICATE2: {
                jtk_StringBuilder_appendEx_z(builder, "duplicate2", 10);

                break;
            }

            case ZEN_BYTE_CODE_DUPLICATE2_X1: {
                jtk_StringBuilder_appendEx_z(builder, "duplicate2_x1", 13);

                break;
            }

            case ZEN_BYTE_CODE_DUPLICATE2_X2: {
                jtk_StringBuilder_appendEx_z(builder, "duplicate2_x2", 13);

                break;
            }

            /* Jump */

            case ZEN_BYTE_CODE_JUMP_EQ0_I: {
                jtk_StringBuilder_appendEx_z(builder, "jump_eq0_i ", 11);

                uint16_t offset = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, offset);

                break;
            }

            case ZEN_BYTE_CODE_JUMP_NE0_I: {
                jtk_StringBuilder_appendEx_z(builder, "jump_ne0_i ", 11);

                uint16_t offset = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, offset);

                break;
            }

            case ZEN_BYTE_CODE_JUMP_LT0_I: {
                jtk_StringBuilder_appendEx_z(builder, "jump_lt0_i ", 11);

                uint16_t offset = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, offset);

                break;
            }

            case ZEN_BYTE_CODE_JUMP_GT0_I: {
                jtk_StringBuilder_appendEx_z(builder, "jump_gt0_i ", 11);

                uint16_t offset = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, offset);

                break;
            }

            case ZEN_BYTE_CODE_JUMP_GE0_I: {
                jtk_StringBuilder_appendEx_z(builder, "jump_ge0_i ", 11);

                uint16_t offset = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, offset);

                break;
            }

            case ZEN_BYTE_CODE_JUMP_LE0_I: {
                jtk_StringBuilder_appendEx_z(builder, "jump_le0_i ", 11);

                uint16_t offset = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, offset);

                break;
            }

            case ZEN_BYTE_CODE_JUMP_EQ_I: {
                jtk_StringBuilder_appendEx_z(builder, "jump_eq_i ", 10);

                uint16_t offset = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, offset);

                break;
            }

            case ZEN_BYTE_CODE_JUMP_NE_I: {
                jtk_StringBuilder_appendEx_z(builder, "jump_ne_i ", 10);

                uint16_t offset = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, offset);

                break;
            }

            case ZEN_BYTE_CODE_JUMP_LT_I: {
                jtk_StringBuilder_appendEx_z(builder, "jump_lt_i ", 10);

                uint16_t offset = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, offset);

                break;
            }

            case ZEN_BYTE_CODE_JUMP_GT_I: {
                jtk_StringBuilder_appendEx_z(builder, "jump_gt_i ", 10);

                uint16_t offset = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, offset);

                break;
            }

            case ZEN_BYTE_CODE_JUMP_GE_I: {
                jtk_StringBuilder_appendEx_z(builder, "jump_ge_i ", 10);

                uint16_t offset = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, offset);

                break;
            }

            case ZEN_BYTE_CODE_JUMP_LE_I: {
                jtk_StringBuilder_appendEx_z(builder, "jump_le_i ", 10);

                uint16_t offset = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, offset);

                break;
            }

            case ZEN_BYTE_CODE_JUMP_EQ_A: {
                jtk_StringBuilder_appendEx_z(builder, "jump_eq_a ", 10);

                uint16_t offset = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, offset);

                break;
            }

            case ZEN_BYTE_CODE_JUMP_NE_A: {
                jtk_StringBuilder_appendEx_z(builder, "jump_ne_a ", 10);

                uint16_t offset = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, offset);

                break;
            }

            case ZEN_BYTE_CODE_JUMP_EQN_A: {
                jtk_StringBuilder_appendEx_z(builder, "jump_eqn_a ", 11);

                uint16_t offset = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, offset);

                break;
            }

            case ZEN_BYTE_CODE_JUMP_NEN_A: {
                jtk_StringBuilder_appendEx_z(builder, "jump_nen_a ", 11);

                uint16_t offset = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, offset);

                break;
            }


            /* Increment */

            case ZEN_BYTE_CODE_INCREMENT_I: {
                jtk_StringBuilder_appendEx_z(builder, "increment_i ", 12);
                jtk_StringBuilder_append_i(builder, bytes[++i]);
                jtk_StringBuilder_appendEx_z(builder, " ", 1);
                jtk_StringBuilder_append_i(builder, bytes[++i]);

                break;
            }

            /* Invoke */

            case ZEN_BYTE_CODE_INVOKE_SPECIAL: {
                jtk_StringBuilder_appendEx_z(builder, "invoke_special ", 15);

                uint16_t index = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, index);

                break;
            }

            case ZEN_BYTE_CODE_INVOKE_VIRTUAL: {
                jtk_StringBuilder_appendEx_z(builder, "invoke_virtual ", 15);

                uint16_t index = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, index);

                break;
            }

            case ZEN_BYTE_CODE_INVOKE_DYNAMIC: {
                jtk_StringBuilder_appendEx_z(builder, "invoke_dynamic ", 15);

                uint16_t index = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, index);

                break;
            }

            case ZEN_BYTE_CODE_INVOKE_STATIC: {
                jtk_StringBuilder_appendEx_z(builder, "invoke_static ", 14);

                uint16_t index = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, index);

                break;
            }

            /* Jump */

            case ZEN_BYTE_CODE_JUMP: {
                jtk_StringBuilder_appendEx_z(builder, "jump ", 5);

                uint16_t offset = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, offset);

                break;
            }

            /* Load */

            case ZEN_BYTE_CODE_LOAD_I: {
                jtk_StringBuilder_appendEx_z(builder, "load_i ", 7);
                jtk_StringBuilder_append_i(builder, bytes[++i]);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_L: {
                jtk_StringBuilder_appendEx_z(builder, "load_l ", 7);
                jtk_StringBuilder_append_i(builder, bytes[++i]);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_F: {
                jtk_StringBuilder_appendEx_z(builder, "load_f ", 7);
                jtk_StringBuilder_append_i(builder, bytes[++i]);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_D: {
                jtk_StringBuilder_appendEx_z(builder, "load_d ", 7);
                jtk_StringBuilder_append_i(builder, bytes[++i]);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_A: {
                jtk_StringBuilder_appendEx_z(builder, "load_a ", 7);
                jtk_StringBuilder_append_i(builder, bytes[++i]);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_I0: {
                jtk_StringBuilder_appendEx_z(builder, "load_i0", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_I1: {
                jtk_StringBuilder_appendEx_z(builder, "load_i1", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_I2: {
                jtk_StringBuilder_appendEx_z(builder, "load_i2", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_I3: {
                jtk_StringBuilder_appendEx_z(builder, "load_i3", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_L0: {
                jtk_StringBuilder_appendEx_z(builder, "load_l0", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_L1: {
                jtk_StringBuilder_appendEx_z(builder, "load_l1", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_L2: {
                jtk_StringBuilder_appendEx_z(builder, "load_l2", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_L3: {
                jtk_StringBuilder_appendEx_z(builder, "load_l3", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_F0: {
                jtk_StringBuilder_appendEx_z(builder, "load_f0", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_F1: {
                jtk_StringBuilder_appendEx_z(builder, "load_f1", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_F2: {
                jtk_StringBuilder_appendEx_z(builder, "load_f2", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_F3: {
                jtk_StringBuilder_appendEx_z(builder, "load_f3", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_D0: {
                jtk_StringBuilder_appendEx_z(builder, "load_d0", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_D1: {
                jtk_StringBuilder_appendEx_z(builder, "load_d1", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_D2: {
                jtk_StringBuilder_appendEx_z(builder, "load_d2", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_D3: {
                jtk_StringBuilder_appendEx_z(builder, "load_d3", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_A0: {
                jtk_StringBuilder_appendEx_z(builder, "load_a0", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_A1: {
                jtk_StringBuilder_appendEx_z(builder, "load_a1", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_A2: {
                jtk_StringBuilder_appendEx_z(builder, "load_a2", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_A3: {
                jtk_StringBuilder_appendEx_z(builder, "load_a3", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_AB: {
                jtk_StringBuilder_appendEx_z(builder, "load_ab", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_AC: {
                jtk_StringBuilder_appendEx_z(builder, "load_ac", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_AS: {
                jtk_StringBuilder_appendEx_z(builder, "load_as", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_AI: {
                jtk_StringBuilder_appendEx_z(builder, "load_ai", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_AL: {
                jtk_StringBuilder_appendEx_z(builder, "load_al", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_AF: {
                jtk_StringBuilder_appendEx_z(builder, "load_af", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_AD: {
                jtk_StringBuilder_appendEx_z(builder, "load_ad", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_AA: {
                jtk_StringBuilder_appendEx_z(builder, "load_aa", 7);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_INSTANCE_FIELD: {
                jtk_StringBuilder_appendEx_z(builder, "load_instance_field ", 20);

                uint16_t index = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, index);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_STATIC_FIELD: {
                jtk_StringBuilder_appendEx_z(builder, "load_static_field ", 18);

                uint16_t index = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, index);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_CPR: {
                jtk_StringBuilder_appendEx_z(builder, "load_cpr ", 9);

                jtk_StringBuilder_append_i(builder, bytes[++i]);

                break;
            }

            case ZEN_BYTE_CODE_LOAD_ARRAY_SIZE: {
                jtk_StringBuilder_appendEx_z(builder, "load_array_size", 15);

                break;
            }

            /* Modulo */

            case ZEN_BYTE_CODE_MODULO_I: {
                jtk_StringBuilder_appendEx_z(builder, "modulo_i", 8);

                break;
            }

            case ZEN_BYTE_CODE_MODULO_L: {
                jtk_StringBuilder_appendEx_z(builder, "modulo_l", 8);

                break;
            }

            case ZEN_BYTE_CODE_MODULO_F: {
                jtk_StringBuilder_appendEx_z(builder, "modulo_f", 8);

                break;
            }

            case ZEN_BYTE_CODE_MODULO_D: {
                jtk_StringBuilder_appendEx_z(builder, "modulo_d", 8);

                break;
            }


            /* Multiply */

            case ZEN_BYTE_CODE_MULTIPLY_I: {
                jtk_StringBuilder_appendEx_z(builder, "multiply_i", 10);

                break;
            }

            case ZEN_BYTE_CODE_MULTIPLY_L: {
                jtk_StringBuilder_appendEx_z(builder, "multiply_l", 10);

                break;
            }

            case ZEN_BYTE_CODE_MULTIPLY_F: {
                jtk_StringBuilder_appendEx_z(builder, "multiply_f", 10);

                break;
            }

            case ZEN_BYTE_CODE_MULTIPLY_D: {
                jtk_StringBuilder_appendEx_z(builder, "multiply_d", 10);

                break;
            }


            /* Negate */

            case ZEN_BYTE_CODE_NEGATE_I: {
                jtk_StringBuilder_appendEx_z(builder, "negate_i", 8);

                break;
            }

            case ZEN_BYTE_CODE_NEGATE_L: {
                jtk_StringBuilder_appendEx_z(builder, "negate_l", 8);

                break;
            }

            case ZEN_BYTE_CODE_NEGATE_F: {
                jtk_StringBuilder_appendEx_z(builder, "negate_f", 8);

                break;
            }

            case ZEN_BYTE_CODE_NEGATE_D: {
                jtk_StringBuilder_appendEx_z(builder, "negate_d", 8);

                break;
            }

            /* New */

            case ZEN_BYTE_CODE_NEW: {
                jtk_StringBuilder_appendEx_z(builder, "new ", 4);

                uint16_t index = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, index);

                break;
            }

            case ZEN_BYTE_CODE_NEW_ARRAY: {
                jtk_StringBuilder_appendEx_z(builder, "new_array ", 10);
                jtk_StringBuilder_append_i(builder, bytes[++i]);


                break;
            }

            case ZEN_BYTE_CODE_NEW_ARRAY_A: {
                jtk_StringBuilder_appendEx_z(builder, "new_array_a ", 12);

                uint16_t index = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, index);

                break;
            }

            case ZEN_BYTE_CODE_NEW_ARRAY_AN: {
                jtk_StringBuilder_appendEx_z(builder, "new_array_an ", 13);

                uint16_t index = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, index);

                jtk_StringBuilder_appendEx_z(builder, " ", 1);

                jtk_StringBuilder_append_i(builder, bytes[++i]);

                break;
            }


            /* Pop */

            case ZEN_BYTE_CODE_POP: {
                jtk_StringBuilder_appendEx_z(builder, "pop", 3);

                break;
            }

            case ZEN_BYTE_CODE_POP2: {
                jtk_StringBuilder_appendEx_z(builder, "pop2", 4);

                break;
            }

            /* Push */

            case ZEN_BYTE_CODE_PUSH_NULL: {
                jtk_StringBuilder_appendEx_z(builder, "push_null", 9);

                break;
            }

            case ZEN_BYTE_CODE_PUSH_IN1: {
                jtk_StringBuilder_appendEx_z(builder, "push_in1", 8);

                break;
            }

            case ZEN_BYTE_CODE_PUSH_I0: {
                jtk_StringBuilder_appendEx_z(builder, "push_i0", 7);

                break;
            }

            case ZEN_BYTE_CODE_PUSH_I1: {
                jtk_StringBuilder_appendEx_z(builder, "push_i1", 7);

                break;
            }

            case ZEN_BYTE_CODE_PUSH_I2: {
                jtk_StringBuilder_appendEx_z(builder, "push_i2", 7);

                break;
            }

            case ZEN_BYTE_CODE_PUSH_I3: {
                jtk_StringBuilder_appendEx_z(builder, "push_i3", 7);

                break;
            }

            case ZEN_BYTE_CODE_PUSH_I4: {
                jtk_StringBuilder_appendEx_z(builder, "push_i4", 7);

                break;
            }

            case ZEN_BYTE_CODE_PUSH_I5: {
                jtk_StringBuilder_appendEx_z(builder, "push_i5", 7);

                break;
            }

            case ZEN_BYTE_CODE_PUSH_L0: {
                jtk_StringBuilder_appendEx_z(builder, "push_l0", 7);

                break;
            }

            case ZEN_BYTE_CODE_PUSH_L1: {
                jtk_StringBuilder_appendEx_z(builder, "push_l1", 7);

                break;
            }

            case ZEN_BYTE_CODE_PUSH_F0: {
                jtk_StringBuilder_appendEx_z(builder, "push_f0", 7);

                break;
            }

            case ZEN_BYTE_CODE_PUSH_F1: {
                jtk_StringBuilder_appendEx_z(builder, "push_f1", 7);

                break;
            }

            case ZEN_BYTE_CODE_PUSH_F2: {
                jtk_StringBuilder_appendEx_z(builder, "push_f2", 7);

                break;
            }

            case ZEN_BYTE_CODE_PUSH_D0: {
                jtk_StringBuilder_appendEx_z(builder, "push_d0", 7);

                break;
            }

            case ZEN_BYTE_CODE_PUSH_D1: {
                jtk_StringBuilder_appendEx_z(builder, "push_d1", 7);

                break;
            }

            case ZEN_BYTE_CODE_PUSH_B: {
                jtk_StringBuilder_appendEx_z(builder, "push_b ", 7);

                jtk_StringBuilder_append_i(builder, bytes[++i]);

                break;
            }

            case ZEN_BYTE_CODE_PUSH_S: {
                jtk_StringBuilder_appendEx_z(builder, "push_s ", 7);

                uint16_t value = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, value);

                break;
            }

            /* Return */

            case ZEN_BYTE_CODE_RETURN: {
                jtk_StringBuilder_appendEx_z(builder, "return", 6);

                break;
            }

            case ZEN_BYTE_CODE_RETURN_I: {
                jtk_StringBuilder_appendEx_z(builder, "return_i", 8);

                break;
            }

            case ZEN_BYTE_CODE_RETURN_L: {
                jtk_StringBuilder_appendEx_z(builder, "return_l", 8);

                break;
            }

            case ZEN_BYTE_CODE_RETURN_F: {
                jtk_StringBuilder_appendEx_z(builder, "return_f", 8);

                break;
            }

            case ZEN_BYTE_CODE_RETURN_D: {
                jtk_StringBuilder_appendEx_z(builder, "return_d", 8);

                break;
            }

            case ZEN_BYTE_CODE_RETURN_A: {
                jtk_StringBuilder_appendEx_z(builder, "return_a", 8);

                break;
            }

            /* RTTI */

            case ZEN_BYTE_CODE_RTTI: {
                jtk_StringBuilder_appendEx_z(builder, "rtti", 4);

                break;
            }

            /* Store */

            case ZEN_BYTE_CODE_STORE_I: {
                jtk_StringBuilder_appendEx_z(builder, "store_i ", 8);

                jtk_StringBuilder_append_i(builder, bytes[++i]);

                break;
            }

            case ZEN_BYTE_CODE_STORE_I0: {
                jtk_StringBuilder_appendEx_z(builder, "store_i0", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_I1: {
                jtk_StringBuilder_appendEx_z(builder, "store_i1", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_I2: {
                jtk_StringBuilder_appendEx_z(builder, "stor_i2", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_I3: {
                jtk_StringBuilder_appendEx_z(builder, "store_i3", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_L: {
                jtk_StringBuilder_appendEx_z(builder, "store_l ", 8);

                jtk_StringBuilder_append_i(builder, bytes[++i]);

                break;
            }

            case ZEN_BYTE_CODE_STORE_L0: {
                jtk_StringBuilder_appendEx_z(builder, "store_l0", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_L1: {
                jtk_StringBuilder_appendEx_z(builder, "store_l1", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_L2: {
                jtk_StringBuilder_appendEx_z(builder, "store_l2", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_L3: {
                jtk_StringBuilder_appendEx_z(builder, "store_l3", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_F: {
                jtk_StringBuilder_appendEx_z(builder, "store_f ", 8);

                jtk_StringBuilder_append_i(builder, bytes[++i]);

                break;
            }

            case ZEN_BYTE_CODE_STORE_F0: {
                jtk_StringBuilder_appendEx_z(builder, "store_f0", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_F1: {
                jtk_StringBuilder_appendEx_z(builder, "store_f1", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_F2: {
                jtk_StringBuilder_appendEx_z(builder, "store_f2", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_F3: {
                jtk_StringBuilder_appendEx_z(builder, "store_f3", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_D: {
                jtk_StringBuilder_appendEx_z(builder, "store_d ", 8);

                jtk_StringBuilder_append_i(builder, bytes[++i]);

                break;
            }

            case ZEN_BYTE_CODE_STORE_D0: {
                jtk_StringBuilder_appendEx_z(builder, "store_d0", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_D1: {
                jtk_StringBuilder_appendEx_z(builder, "store_d1", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_D2: {
                jtk_StringBuilder_appendEx_z(builder, "store_d2", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_D3: {
                jtk_StringBuilder_appendEx_z(builder, "store_d3", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_A: {
                jtk_StringBuilder_appendEx_z(builder, "store_a ", 8);

                jtk_StringBuilder_append_i(builder, bytes[++i]);

                break;
            }

            case ZEN_BYTE_CODE_STORE_A0: {
                jtk_StringBuilder_appendEx_z(builder, "store_a0", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_A1: {
                jtk_StringBuilder_appendEx_z(builder, "store_a1", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_A2: {
                jtk_StringBuilder_appendEx_z(builder, "store_a2", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_A3: {
                jtk_StringBuilder_appendEx_z(builder, "store_a3", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_AB: {
                jtk_StringBuilder_appendEx_z(builder, "store_ab", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_AC: {
                jtk_StringBuilder_appendEx_z(builder, "store_ac", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_AS: {
                jtk_StringBuilder_appendEx_z(builder, "store_as", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_AI: {
                jtk_StringBuilder_appendEx_z(builder, "store_ai", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_AL: {
                jtk_StringBuilder_appendEx_z(builder, "store_al", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_AF: {
                jtk_StringBuilder_appendEx_z(builder, "store_af", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_AD: {
                jtk_StringBuilder_appendEx_z(builder, "store_ad", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_AA: {
                jtk_StringBuilder_appendEx_z(builder, "store_aa", 8);

                break;
            }

            case ZEN_BYTE_CODE_STORE_STATIC_FIELD: {
                jtk_StringBuilder_appendEx_z(builder, "store_static_field ", 19);

                uint16_t index = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, index);

                break;
            }

            case ZEN_BYTE_CODE_STORE_INSTANCE_FIELD: {
                jtk_StringBuilder_appendEx_z(builder, "store_instance_field ", 21);

                uint16_t index = (bytes[++i] << 8) | bytes[++i];
                jtk_StringBuilder_append_i(builder, index);

                break;
            }

            /* Swap */

            case ZEN_BYTE_CODE_SWAP: {
                jtk_StringBuilder_appendEx_z(builder, "swap", 4);

                break;
            }

            /* Switch */

            case ZEN_BYTE_CODE_SWITCH_TABLE: {
                jtk_StringBuilder_appendEx_z(builder, "switch_table", 12);

                break;
            }

            case ZEN_BYTE_CODE_SWITCH_SEARCH: {
                jtk_StringBuilder_appendEx_z(builder, "switch_search", 13);

                break;
            }

            /* Throw */

            case ZEN_BYTE_CODE_THROW: {
                jtk_StringBuilder_appendEx_z(builder, "throw", 5);

                break;
            }

            /* Wide */

            case ZEN_BYTE_CODE_WIDE: {
                jtk_StringBuilder_appendEx_z(builder, "wide", 4);

                break;
            }
        }

        jtk_StringBuilder_appendEx_z(builder, "\n", 1);
    }
}

// TODO: Somebody has to destroy the instruction attribute that was allocated here.
void zen_BinaryEntityGenerator_onExitFunctionDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    /* Retrieve the generator associated with the AST listener. */
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;

    /* Retrieve the context associated with the AST node. */
    zen_FunctionDeclarationContext_t* context = (zen_FunctionDeclarationContext_t*)node->m_context;

    zen_ASTNode_t* identifier = context->m_identifier;
    zen_Token_t* identifierToken = (zen_Token_t*)identifier->m_context;

    bool constructor = zen_Token_getType(identifierToken) == ZEN_TOKEN_KEYWORD_NEW;
    jtk_String_t* descriptor = zen_BinaryEntityGenerator_getDescriptorEx(generator,
        context->m_functionParameters, constructor);

    uint8_t* temporary = jtk_CString_newWithSize(identifierToken->m_text, identifierToken->m_length);
    zen_Symbol_t* symbol = zen_SymbolTable_resolve(generator->m_symbolTable, temporary);
    jtk_Memory_deallocate(temporary);

    uint16_t flags = symbol->m_modifiers;
    uint16_t nameIndex = constructor?
        zen_ConstantPoolBuilder_getUtf8EntryIndexEx(
            generator->m_constantPoolBuilder, "<initialize>", 12) :
        zen_ConstantPoolBuilder_getUtf8EntryIndexEx(
            generator->m_constantPoolBuilder, identifierToken->m_text,
            identifierToken->m_length);

    uint16_t descriptorIndex = zen_ConstantPoolBuilder_getUtf8EntryIndex(
        generator->m_constantPoolBuilder, descriptor);

    /* Destroy the descriptor. A constant pool reference to the descriptor was
     * acquired.
     */
    jtk_String_delete(descriptor);

    zen_FunctionEntity_t* functionEntity = zen_FunctionEntity_new(flags,
        nameIndex, descriptorIndex);
    zen_AttributeTable_t* attributeTable = &functionEntity->m_attributeTable;

    zen_InstructionAttribute_t* instructionAttribute =
        zen_BinaryEntityGenerator_makeInstructionAttribute(generator);

    attributeTable->m_size = 1;
    attributeTable->m_attributes = zen_Memory_allocate(zen_Attribute_t*, 1);
    attributeTable->m_attributes[0] = (zen_Attribute_t*)instructionAttribute;

    /* Add the function entity to the list of functions. */
    jtk_ArrayList_add(generator->m_functions, functionEntity);

    zen_SymbolTable_invalidateCurrentScope(generator->m_symbolTable);

    int32_t exceptionHandlerSiteCount = jtk_ArrayList_getSize(
        generator->m_exceptionHandlerSites);
    int32_t exceptionHandlerSiteIndex;
    printf("[debug] Exception Handler Sites\n");
    for (exceptionHandlerSiteIndex = 0;
        exceptionHandlerSiteIndex < exceptionHandlerSiteCount;
        exceptionHandlerSiteIndex++) {
        zen_ExceptionHandlerSite_t* exceptionHandlerSite =
            (zen_ExceptionHandlerSite_t*)jtk_ArrayList_getValue(
                generator->m_exceptionHandlerSites, exceptionHandlerSiteIndex);
        printf("[debug] (startIndex = %d, stopIndex = %d, handlerIndex = %d, exceptionClassIndex = %d)\n",
            exceptionHandlerSite->m_startIndex, exceptionHandlerSite->m_stopIndex,
            exceptionHandlerSite->m_handlerIndex, exceptionHandlerSite->m_exceptionClassIndex);
    }

    /* Here, we assume that the channels are added and removed as if they were
     * stored in a stack. The primary channel is assumed to be at zeroth index.
     */
    int32_t instructionChannelIndex = zen_BinaryEntityBuilder_getActiveChannelIndex(generator->m_builder);
    zen_BinaryEntityBuilder_removeChannel(generator->m_builder, instructionChannelIndex);

#warning "TODO: Implement a stack like behaviour to alter active channel."
    zen_BinaryEntityBuilder_setActiveChannelIndex(generator->m_builder, 0);

    /* Reset the counters used for tracking certain properties of the function
     * being declared.
     */
    generator->m_maxStackSize = 0;
    generator->m_localVariableCount = 0;
    /* The exception handler sites are destroyed when the entity file is destroyed. */
    jtk_ArrayList_clear(generator->m_exceptionHandlerSites);
}

// functionParameters

void zen_BinaryEntityGenerator_onEnterFunctionParameters(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitFunctionParameters(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// functionBody

void zen_BinaryEntityGenerator_onEnterFunctionBody(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitFunctionBody(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// statementSuite

void zen_BinaryEntityGenerator_onEnterStatementSuite(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;

    zen_Scope_t* scope = zen_ASTAnnotations_get(generator->m_scopes, node);
    zen_SymbolTable_setCurrentScope(generator->m_symbolTable, scope);
}

void zen_BinaryEntityGenerator_onExitStatementSuite(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;

    zen_SymbolTable_invalidateCurrentScope(generator->m_symbolTable);
}

// simpleStatement

void zen_BinaryEntityGenerator_onEnterSimpleStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitSimpleStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// statement

void zen_BinaryEntityGenerator_onEnterStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// emptyStatement

void zen_BinaryEntityGenerator_onEnterEmptyStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator =
        (zen_BinaryEntityGenerator_t*)astListener->m_context;

    /* Emit the nop instruction. */
    zen_BinaryEntityBuilder_emitNop(generator->m_builder);

    /* Log the emission of the nop instruction. */
    printf("[debug] Emitted nop\n");
}

void zen_BinaryEntityGenerator_onExitEmptyStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// variableDeclaration

void zen_BinaryEntityGenerator_onEnterVariableDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitVariableDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    /* Retrieve the generator associated with the AST listener. */
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;

    /* Retrieve the context associated with the AST node. */
    zen_VariableDeclarationContext_t* context = (zen_VariableDeclarationContext_t*)node->m_context;

    /* Retrieve the current scope from the symbol table. */
    zen_Scope_t* currentScope = zen_SymbolTable_getCurrentScope(generator->m_symbolTable);

    int32_t variableCount = jtk_ArrayList_getSize(context->m_variableDeclarators);
    int32_t i;
    for (i = 0; i < variableCount; i++) {
        zen_ASTNode_t* variableDeclarator = (zen_ASTNode_t*)jtk_ArrayList_getValue(
            context->m_variableDeclarators, i);
        zen_VariableDeclaratorContext_t* variableDeclaratorContext =
            (zen_VariableDeclaratorContext_t*)variableDeclarator->m_context;

        /* Retrieve the identifier AST node. */
        zen_ASTNode_t* identifier = variableDeclaratorContext->m_identifier;
        /* Retrieve the identifier token. */
        zen_Token_t* identifierToken = (zen_Token_t*)identifier->m_context;

        /* If a class scope encloses the variable being declared, we are processing
         * a class member declaration.
         */
        if (zen_Scope_isClassScope(currentScope)) {
            uint16_t flags = 0;

            /* Retrieve the constant pool index for the variable name. */
            uint16_t nameIndex = zen_ConstantPoolBuilder_getUtf8EntryIndexEx(
                generator->m_constantPoolBuilder,
                identifierToken->m_text, identifierToken->m_length);

            uint16_t descriptorIndex = zen_ConstantPoolBuilder_getUtf8EntryIndexEx(
                generator->m_constantPoolBuilder,
                "com/todo/Class", 14);

            /* Create an instance of the field entity that represents the variable
             * declared.
             */
            zen_FieldEntity_t* fieldEntity = zen_FieldEntity_new(flags, nameIndex, descriptorIndex);

            /* Add the field entity to the list of fields. */
            jtk_ArrayList_add(generator->m_fields, fieldEntity);
        }
        else if (zen_Scope_isLocalScope(currentScope)) {
            /* Retrieve the string equivalent to the identifier node. */
            int32_t identifierSize;
            uint8_t* identifierText = zen_ASTNode_toCString(identifier, &identifierSize);

            /* TODO: If the local scope belongs to an instance function, then a local
             * variable for the "this" reference should be created.
             */

            zen_Symbol_t* symbol = zen_Scope_resolve(currentScope, identifierText);
            if (zen_Symbol_isVariable(symbol)) {
                zen_VariableSymbol_t* variableSymbol = (zen_VariableSymbol_t*)symbol->m_context;
                /* Generate and assign the index of the local variable only if it
                 * was not previously assigned an index.
                 */
                if (variableSymbol->m_index < 0) {
                    variableSymbol->m_index = generator->m_localVariableCount++;
                }
            }
            else {
                printf("[internal error] Identifier recognized by variable declarator registered as a non-variable entity in the symbol table.\n");
            }
        }
    }
}

// variableDeclarator

void zen_BinaryEntityGenerator_onEnterVariableDeclarator(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitVariableDeclarator(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// constantDeclaration

void zen_BinaryEntityGenerator_onEnterConstantDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitConstantDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    /* Retrieve the generator associated with the AST listener. */
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;

    /* Retrieve the context associated with the AST node. */
    zen_ConstantDeclarationContext_t* context = (zen_ConstantDeclarationContext_t*)node->m_context;

    /* Retrieve the current scope from the symbol table. */
    zen_Scope_t* currentScope = zen_SymbolTable_getCurrentScope(generator->m_symbolTable);

    int32_t constantCount = jtk_ArrayList_getSize(context->m_constantDeclarators);
    int32_t i;
    for (i = 0; i < constantCount; i++) {
        zen_ASTNode_t* constantDeclarator = (zen_ASTNode_t*)jtk_ArrayList_getValue(
            context->m_constantDeclarators, i);
        zen_ConstantDeclaratorContext_t* constantDeclaratorContext =
            (zen_ConstantDeclaratorContext_t*)constantDeclarator->m_context;

        /* Retrieve the identifier AST node. */
        zen_ASTNode_t* identifier = constantDeclaratorContext->m_identifier;
        /* Retrieve the identifier token. */
        zen_Token_t* identifierToken = (zen_Token_t*)identifier->m_context;

        /* If a class scope encloses the constant being declared, we are processing
         * a class member declaration.
         */
        if (zen_Scope_isClassScope(currentScope)) {
            uint16_t flags = 0;

            /* Retrieve the constant pool index for the constant name. */
            uint16_t nameIndex = zen_ConstantPoolBuilder_getUtf8EntryIndexEx(
                generator->m_constantPoolBuilder,
                identifierToken->m_text, identifierToken->m_length);

            uint16_t descriptorIndex = zen_ConstantPoolBuilder_getUtf8EntryIndexEx(
                generator->m_constantPoolBuilder,
                "com/todo/Class", 14);

            /* Create an instance of the field entity that represents the constant
             * declared.
             */
            zen_FieldEntity_t* fieldEntity = zen_FieldEntity_new(flags, nameIndex, descriptorIndex);

            /* Add the field entity to the list of fields. */
            jtk_ArrayList_add(generator->m_fields, fieldEntity);
        }
        else if (zen_Scope_isLocalScope(currentScope)) {
            /* Retrieve the string equivalent to the identifier node. */
            int32_t identifierSize;
            uint8_t* identifierText = zen_ASTNode_toCString(identifier, &identifierSize);

            /* Although a constant is being declared, we increment the local variable
             * count here. This is because the virtual machine recognizes only local
             * variables. Local constants are syntatic sugar over local variables.
             */
            zen_Symbol_t* symbol = zen_Scope_resolve(currentScope, identifierText);
            if (zen_Symbol_isConstant(symbol)) {
                zen_ConstantSymbol_t* constantSymbol = (zen_ConstantSymbol_t*)symbol->m_context;
                /* Generate and assign the index of the local variable only if it
                 * was not previously assigned an index.
                 */
                if (constantSymbol->m_index < 0) {
                    constantSymbol->m_index = generator->m_localVariableCount++;
                }
            }
            else {
                printf("[internal error] Identifier recognized by constant declarator registered as a non-constant entity in the symbol table.\n");
            }
        }
    }
}

// constantDeclarator

void zen_BinaryEntityGenerator_onEnterConstantDeclarator(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitConstantDeclarator(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// assertStatement

/*
 * ALGORITHM TO GENERATE INSTRUCTIONS CORRESPONDING TO ASSERT STATEMENT
 *
 * When an assert statement is used within the bounds of a class, a static
 * field is created by the compiler. This field has the identifier '$assertionEnabled'.
 * It is initialized in the static initializer block, which is generated
 * by the compiler.
 *
 * 1. Load the static field '$assertionEnabled'.
 * 2. If assertions are disabled, skip the instruction assertion statement.
 * 3. Otherwise, execute the instructions corresponding to the condition expression
 *    specified to the assert statement.
 * 4. Convert the resulting object into a Boolean primitive.
 * 5. If the result is true, then skip the assert statement.
 * 6. Otherwise, execute the instructions corresponding to the message expression
 *    specified to the assert statement.
 * 7. Generate the instructions required to create and throw an exception.
 *    The exception is an instance of the zen.core.AssertionError class.
 */
void zen_BinaryEntityGenerator_onEnterAssertStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    /* Retrieve the generator associated with the AST listener. */
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    /* Retrieve the context of the AST node. */
    zen_AssertStatementContext_t* context = (zen_AssertStatementContext_t*)node->m_context;

    /* The normal behaviour of the AST walker causes the generator to emit instructions
     * in an undesirable fashion. Therefore, we partially switch from the listener
     * to visitor design pattern. The AST walker can be guided to switch to this
     * mode via zen_ASTListener_skipChildren() function which causes the AST walker
     * to skip iterating over the children nodes.
     */
    zen_ASTListener_skipChildren(astListener);
}

void zen_BinaryEntityGenerator_onExitAssertStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
/* Retrieve the generator associated with the AST listener. */
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    /* Retrieve the context of the AST node. */
    zen_AssertStatementContext_t* context = (zen_AssertStatementContext_t*)node->m_context;

    int32_t parentChannelIndex = zen_BinaryEntityBuilder_getActiveChannelIndex(
         generator->m_builder);
    zen_DataChannel_t* parentChannel = zen_BinaryEntityBuilder_getChannel(
         generator->m_builder, parentChannelIndex);

    uint8_t* className = "Unknown"; // TODO: Get the name of the current class!
    int32_t classNameSize = 7;
    uint8_t* descriptor = "z";
    int32_t descriptorSize = 1;
    uint8_t* name = "$assertionEnabled";
    int32_t nameSize = 17;
    int32_t assertionEnabledIndex = zen_ConstantPoolBuilder_getFieldEntryIndexEx(
        generator->m_constantPoolBuilder, className, classNameSize,
        descriptor, descriptorSize, name, nameSize);

    const uint8_t* booleanClassName = "zen/core/Boolean";
    int32_t booleanClassNameSize = 16;
    const uint8_t* getValueDescriptor = "z:v";
    int32_t getValueDescriptorSize = 3;
    const uint8_t* getValueName = "getValue";
    int32_t getValueNameSize = 8;
    uint16_t getValueIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, booleanClassName, booleanClassNameSize,
        getValueDescriptor, getValueDescriptorSize, getValueName,
        getValueNameSize);

    const uint8_t* assertionErrorClassName = "zen/core/AssertionError";
    int32_t assertionErrorClassNameSize = 26;
    uint16_t assertionErrorClassIndex = zen_ConstantPoolBuilder_getClassEntryIndexEx(
        generator->m_constantPoolBuilder, assertionErrorClassName,
        assertionErrorClassNameSize);

    // TODO: Change <constructor> to <initialize>
    const uint8_t* constructor1Descriptor = "v:@(zen/core/String)";
    int32_t constructor1DescriptorSize = 20;
    const uint8_t* constructorName = "<constructor>";
    int32_t constructorNameSize = 13;
    uint16_t constructor1Index = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, assertionErrorClassName,
        assertionErrorClassNameSize, constructor1Descriptor, constructor1DescriptorSize,
        constructorName, constructorNameSize);

    const uint8_t* constructor2Descriptor = "v:v";
    int32_t constructor2DescriptorSize = 3;
    uint16_t constructor2Index = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, assertionErrorClassName,
        assertionErrorClassNameSize, constructor2Descriptor, constructor2DescriptorSize,
        constructorName, constructorNameSize);

    /* Emit the load_static_field instruction. */
    zen_BinaryEntityBuilder_emitLoadStaticField(generator->m_builder,
        assertionEnabledIndex);
    /* Log the emission of the load_static_field instruction. */
    printf("[debug] Emitted load_static_field %d\n", assertionEnabledIndex);

    /* Emit the jump_eq0_i instruction. */
    zen_BinaryEntityBuilder_emitJumpEqual0Integer(generator->m_builder, 0);
    /* Log the emission of the jump_eq0_i instruction. */
    printf("[debug] Emitted jump_eq0_i 0 (dummy index)\n");

    /* Save the index of the byte where the dummy data was written. */
    int32_t updateIndex1 = zen_DataChannel_getSize(parentChannel) - 2;

    /* Generate the instructions corresponding to the condition expression
     * specified to the assert statement.
     */
    zen_ASTWalker_walk(astListener, context->m_conditionExpression);

    /* Invoke the Boolean#getValue() function to retrieve the primitive equivalent
     * of the resulting object.
     */
    zen_BinaryEntityBuilder_emitInvokeVirtual(generator->m_builder,
        getValueIndex);
    /* Log the emission of the invoke_virtual instruction. */
    printf("[debug] Emitted invoke_virtual %d\n", getValueIndex);

    /* Emit the jump_ne0_i instruction. */
    zen_BinaryEntityBuilder_emitJumpNotEqual0Integer(generator->m_builder, 0);
    /* Log the emission of the jump_ne0_i instruction. */
    printf("[debug] Emitted jump_ne0_i 0 (dummy index)\n");

    /* Save the index of the byte where the dummy data was written. */
    int32_t updateIndex2 = zen_DataChannel_getSize(parentChannel) - 2;

    int32_t constructorIndex = constructor2Index;
    if (context->m_messageExpression != NULL) {
        /* Generate the instructions corresponding to the message expression
         * specified to the assert statement.
         */
        zen_ASTWalker_walk(astListener, context->m_messageExpression);

        /* Use the constructor that accepts a detail message parameter. */
        constructorIndex = constructor1Index;
    }

    /* Create an instance of the AssertionError class. */
    zen_BinaryEntityBuilder_emitNew(generator->m_builder, assertionErrorClassIndex);
    /* Log the emission of the new instruction. */
    printf("[debug] Emitted new %d\n", assertionErrorClassIndex);

    /* Duplicate the reference of the newly created exception. */
    zen_BinaryEntityBuilder_emitDuplicate(generator->m_builder);
    /* Log the emission of the duplicate instruction. */
    printf("[debug] Emitted duplicate\n");

    /* Invoke the constructor to initialize the new exception instance. */
    zen_BinaryEntityBuilder_emitInvokeSpecial(generator->m_builder,
        constructorIndex);
    /* Log the emission of the invoke_special instruction. */
    printf("[debug] Emitted invoke_special %d\n", constructorIndex);

    /* Throw the newly created exception. */
    zen_BinaryEntityBuilder_emitThrow(generator->m_builder);
    /* Log the emission of the throw instruction. */
    printf("[debug] Emitted throw\n");

    int32_t newParentChannelSize = zen_DataChannel_getSize(parentChannel);
    /* Update the jump offset of the first jump instruction. */
    parentChannel->m_bytes[updateIndex1] = (newParentChannelSize & 0x0000FF00) >> 8;
    parentChannel->m_bytes[updateIndex1 + 1] = newParentChannelSize & 0x000000FF;
    /* Update the jump offset of the second jump instruction. */
    parentChannel->m_bytes[updateIndex2] = (newParentChannelSize & 0x0000FF00) >> 8;
    parentChannel->m_bytes[updateIndex2 + 1] = newParentChannelSize & 0x000000FF;
}

void zen_BinaryEntityGenerator_onEnterBreakStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// breakStatement

void zen_BinaryEntityGenerator_onExitBreakStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// continueStatement

void zen_BinaryEntityGenerator_onEnterContinueStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitContinueStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// returnStatement

void zen_BinaryEntityGenerator_onEnterReturnStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitReturnStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    /* Retrieve the generator associated with the AST listener. */
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;

    /* Retrieve the context of the AST node. */
    zen_ReturnStatementContext_t* context =
        (zen_ReturnStatementContext_t*)node->m_context;

    /* Emit the return_a instruction. */
    zen_BinaryEntityBuilder_emitReturnReference(generator->m_builder);

    /* Log the emission of the instruction. */
    printf("[debug] Emitted return_a\n");
}

// throwStatement

void zen_BinaryEntityGenerator_onEnterThrowStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitThrowStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    /* Retrieve the generator associated with the AST listener. */
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;

    /* Retrieve the context of the AST node. */
    zen_ThrowStatementContext_t* context =
        (zen_ThrowStatementContext_t*)node->m_context;

    /* Emit the throw instruction. */
    zen_BinaryEntityBuilder_emitThrow(generator->m_builder);

    /* Log the emission of the instruction. */
    printf("[debug] Emitted throw\n");
}

// compoundStatement

void zen_BinaryEntityGenerator_onEnterCompoundStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitCompoundStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// ifStatement

void zen_BinaryEntityGenerator_onEnterIfStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_IfStatementContext_t* context = (zen_IfStatementContext_t*)node->m_context;

    /* The normal behaviour of the AST walker causes the generator to emit instructions
     * in an undesirable fashion. Therefore, we partially switch from the listener
     * to visitor design pattern. The AST walker can be guided to switch to this
     * mode via zen_ASTListener_skipChildren() function which causes the AST walker
     * to skip iterating over the children nodes.
     */
    zen_ASTListener_skipChildren(astListener);
}

void zen_BinaryEntityGenerator_onExitIfStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_IfStatementContext_t* context = (zen_IfStatementContext_t*)node->m_context;

    const uint8_t* booleanClassName = "zen/core/Boolean";
    int32_t booleanClassNameSize = 16;
    const uint8_t* getValueDescriptor = "z:v";
    int32_t getValueDescriptorSize = 3;
    const uint8_t* getValueName = "getValue";
    int32_t getValueNameSize = 8;
    uint16_t getValueIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, booleanClassName, booleanClassNameSize,
        getValueDescriptor, getValueDescriptorSize, getValueName,
        getValueNameSize);

    int32_t parentChannelIndex = zen_BinaryEntityBuilder_getActiveChannelIndex(
         generator->m_builder);
    zen_DataChannel_t* parentChannel = zen_BinaryEntityBuilder_getChannel(
         generator->m_builder, parentChannelIndex);

    zen_ASTNode_t* ifClause = context->m_ifClause;
    zen_IfClauseContext_t* ifClauseContext = (zen_IfClauseContext_t*)ifClause->m_context;
    zen_ASTNode_t* expression = ifClauseContext->m_expression;
    zen_ASTNode_t* statementSuite = ifClauseContext->m_statementSuite;

    int32_t size = jtk_ArrayList_getSize(context->m_elseIfClauses);
    int32_t* skipIndexes = jtk_Memory_allocate(int32_t, size + 1);

    int32_t index = -1;
    do {
        /* Generate the instructions corresponding to the conditional expression
         * specified to the if clause.
         */
        zen_ASTWalker_walk(astListener, expression);

        /* Invoke the Boolean#getValue() function to retrieve the primitive equivalent
         * of the resulting object.
         */
        zen_BinaryEntityBuilder_emitInvokeVirtual(generator->m_builder,
            getValueIndex);

        /* Log the emission of the invoke_special instruction. */
        printf("[debug] Emitted invoke_virtual %d\n", getValueIndex);

        /* Emit the jump_eq0_i instruction. */
        zen_BinaryEntityBuilder_emitJumpEqual0Integer(generator->m_builder, 0);

        /* Log the emission of the jump_eq0_i instruction. */
        printf("[debug] Emitted jump_eq0_i 0 (dummy index)\n");

        /* Save the index of the byte where the dummy data was written. */
        int32_t updateIndex = zen_DataChannel_getSize(parentChannel) - 2;

        /* Generate the instructions corresponding to the statement suite specified
         * to the if clause.
         */
        zen_ASTWalker_walk(astListener, statementSuite);

        /* Update the loop counter and prepare for the next iteration, if any. */
        index++;

        /* A jump instrution should not be generated only if the current
         * clause is the last if/else if clause.
         */
        if (index != size) {
            /* If the if clause was selected and executed all the other clauses should
             * be skipped. Generate a jump instruction to skip the ladder. Given the
             * whole ladder has not been generated yet, the jump offset cannot be
             * evaluated right now. Therefore, emit the jump instruction with a dummy
             * offset.
             */
            zen_BinaryEntityBuilder_emitJump(generator->m_builder, 0);

            /* Log the emission of the jump instruction. */
            printf("[debug] Emitted jump 0 (dummy index)\n");

            /* Save the index of the bytes where the dummy data was written. */
            skipIndexes[index] = zen_DataChannel_getSize(parentChannel) - 2;

            zen_ASTNode_t* elseIfClause = jtk_ArrayList_getValue(context->m_elseIfClauses, index);
            zen_ElseIfClauseContext_t* elseIfClauseContext =
                (zen_ElseIfClauseContext_t*)elseIfClause->m_context;
            expression = elseIfClauseContext->m_expression;
            statementSuite = elseIfClauseContext->m_statementSuite;
        }

        /* The new parent channel size is equal to the offset where the jump
         * instruction should branch when the condition is false.
         *
         * Note: the instructions are indexed beginning from zero. Otherwise,
         * an extra 1 should be added to the offset.
         */
        int32_t newParentChannelSize = zen_DataChannel_getSize(parentChannel);

        parentChannel->m_bytes[updateIndex] = (newParentChannelSize & 0x0000FF00) >> 8;
        parentChannel->m_bytes[updateIndex + 1] = newParentChannelSize & 0x000000FF;
    }
    while (index < size);

    /* When there is no else clause, the number of skip indexes to fill is given
     * by the number of else if clauses. However, when an else clause is present,
     * the number of skip indexes to fill is given by the number of else if clauses
     * plus 1, which is also equal to the maximum number of skip indexes.
     */
    int32_t numberOfSkips = size;
    /* A jump instruction is generated immediately after the instructions
     * corresponding to the body of an if/else if clause when an else clause
     * is present.
     */
    if (context->m_elseClause != NULL) {
        /* If the previous if/else if clause was selected and executed then
         * the else clause should be skipped. Generate a jump instruction to skip
         * the else clause. Given the else clause has not been generated yet,
         * the jump offset cannot be evaluated right now. Therefore, emit the
         * jump instruction with a dummy offset.
         */
        zen_BinaryEntityBuilder_emitJump(generator->m_builder, 0);

        /* Log the emission of the jump instruction. */
        printf("[debug] Emitted jump 0 (dummy index)\n");

        /* Save the index of the bytes where the dummy data was written. */
        skipIndexes[index] = zen_DataChannel_getSize(parentChannel) - 2;

        /* Increase the number of skips to indicate the presence of an else clause. */
        numberOfSkips++;

        /* Retrieve the AST node for the else clause. */
        zen_ASTNode_t* elseClause = context->m_elseClause;
        /* Retrieve the context associated with the AST node of the else clause. */
        zen_ElseClauseContext_t* elseClauseContext =
            (zen_ElseClauseContext_t*)elseClause->m_context;
        /* Generate the instructions for the statements within the else clause. */
        zen_ASTWalker_walk(astListener, elseClauseContext->m_statementSuite);
    }

    int32_t newParentChannelSize = zen_DataChannel_getSize(parentChannel);
    int32_t i;
    for (i = 0; i < numberOfSkips; i++) {
        uint16_t skipIndex = skipIndexes[i];
        parentChannel->m_bytes[skipIndex] = (newParentChannelSize & 0x0000FF00) >> 8;
        parentChannel->m_bytes[skipIndex + 1] = newParentChannelSize & 0x000000FF;
    }

    jtk_Memory_deallocate(skipIndexes);
}

// ifClause

void zen_BinaryEntityGenerator_onEnterIfClause(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitIfClause(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// elseIfClause

void zen_BinaryEntityGenerator_onEnterElseIfClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitElseIfClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// elseClause

void zen_BinaryEntityGenerator_onEnterElseClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitElseClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// iterativeStatement

void zen_BinaryEntityGenerator_onEnterIterativeStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitIterativeStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// label

void zen_BinaryEntityGenerator_onEnterLabel(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitLabel(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// whileStatement

void zen_BinaryEntityGenerator_onEnterWhileStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_WhileStatementContext_t* context = (zen_WhileStatementContext_t*)node->m_context;

    /* The normal behaviour of the AST walker causes the generator to emit instructions
     * in an undesirable fashion. Therefore, we partially switch from the listener
     * to visitor design pattern. The AST walker can be guided to switch to this
     * mode via zen_ASTListener_skipChildren() function which causes the AST walker
     * to skip iterating over the children nodes.
     */
    zen_ASTListener_skipChildren(astListener);
}

void zen_BinaryEntityGenerator_onExitWhileStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_WhileStatementContext_t* context = (zen_WhileStatementContext_t*)node->m_context;

    const uint8_t* booleanClassName = "zen/core/Boolean";
    int32_t booleanClassNameSize = 16;
    const uint8_t* getValueDescriptor = "z:v";
    int32_t getValueDescriptorSize = 3;
    const uint8_t* getValueName = "getValue";
    int32_t getValueNameSize = 8;
    uint16_t getValueIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, booleanClassName, booleanClassNameSize,
        getValueDescriptor, getValueDescriptorSize, getValueName,
        getValueNameSize);

    int32_t parentChannelIndex = zen_BinaryEntityBuilder_getActiveChannelIndex(
        generator->m_builder);
    zen_DataChannel_t* parentChannel = zen_BinaryEntityBuilder_getChannel(
        generator->m_builder, parentChannelIndex);
    uint16_t loopIndex = zen_DataChannel_getSize(parentChannel);

    /* Generate the instructions corresponding to the conditional expression
     * specified to the while statement.
     */
    zen_ASTWalker_walk(astListener, context->m_expression);

    /* Invoke the Boolean#getValue() function to retrieve the primitive equivalent
     * of the resulting object.
     */
    zen_BinaryEntityBuilder_emitInvokeVirtual(generator->m_builder,
        getValueIndex);

    /* Log the emission of the invoke_virtual instruction. */
    printf("[debug] Emitted invoke_virtual %d\n", getValueIndex);

    /* Emit the jump_eq0_i instruction. */
    zen_BinaryEntityBuilder_emitJumpEqual0Integer(generator->m_builder, 0);

    /* Log the emission of the jump_eq0_i instruction. */
    printf("[debug] Emitted jump_eq0_i 0 (dummy index)\n");

    /* Save the index of the byte where the dummy data was written. */
    int32_t updateIndex = zen_DataChannel_getSize(parentChannel) - 2;

    /* Generate the instructions corresponding to the statement suite specified
     * to the while statement.
     */
    zen_ASTWalker_walk(astListener, context->m_statementSuite);

    /* Generate a jump instruction to loop back to the conditional expression. */
    zen_BinaryEntityBuilder_emitJump(generator->m_builder, loopIndex);

    /* Log the emission of the jump instruction. */
    printf("[debug] Emitted jump %d\n", loopIndex);

    uint16_t newParentChannelSize = zen_DataChannel_getSize(parentChannel);
    parentChannel->m_bytes[updateIndex] = (newParentChannelSize & 0x0000FF00) >> 8;
    parentChannel->m_bytes[updateIndex + 1] = newParentChannelSize & 0x000000FF;
}

// forStatement

void zen_BinaryEntityGenerator_onEnterForStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_ForStatementContext_t* context = (zen_ForStatementContext_t*)node->m_context;

    /* The normal behaviour of the AST walker causes the generator to emit instructions
     * in an undesirable fashion. Therefore, we partially switch from the listener
     * to visitor design pattern. The AST walker can be guided to switch to this
     * mode via zen_ASTListener_skipChildren() function which causes the AST walker
     * to skip iterating over the children nodes.
     */
    zen_ASTListener_skipChildren(astListener);
}

void zen_BinaryEntityGenerator_onExitForStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_ForStatementContext_t* context = (zen_ForStatementContext_t*)node->m_context;

    const uint8_t* iterableClassName = "zen/core/Iterable";
    int32_t iterableClassNameSize = 17;
    const uint8_t* getIteratorDescriptor = "(zen/core/Object):v";
    int32_t getIteratorDescriptorSize = 19;
    const uint8_t* getIteratorName = "getIterator";
    int32_t getIteratorNameSize = 11;
    uint16_t getIteratorIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, iterableClassName, iterableClassNameSize,
        getIteratorDescriptor, getIteratorDescriptorSize, getIteratorName,
        getIteratorNameSize);

    int32_t parentChannelIndex = zen_BinaryEntityBuilder_getActiveChannelIndex(
        generator->m_builder);
    zen_DataChannel_t* parentChannel = zen_BinaryEntityBuilder_getChannel(
        generator->m_builder, parentChannelIndex);

    const uint8_t* iteratorClassName = "zen/core/Iterator";
    int32_t iteratorClassNameSize = 17;
    const uint8_t* hasNextDescriptor = "z:v";
    int32_t hasNextDescriptorSize = 3;
    const uint8_t* hasNextName = "hasNext";
    int32_t hasNextNameSize = 7;
    uint16_t hasNextIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, iteratorClassName, iteratorClassNameSize,
        hasNextDescriptor, hasNextDescriptorSize, hasNextName,
        hasNextNameSize);

    const uint8_t* getNextDescriptor = "(zen/core/Object):v";
    int32_t getNextDescriptorSize = 19;
    const uint8_t* getNextName = "getNext";
    int32_t getNextNameSize = 7;
    uint16_t getNextIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, iteratorClassName, iteratorClassNameSize,
        getNextDescriptor, getNextDescriptorSize, getNextName,
        getNextNameSize);

    zen_ASTNode_t* forParameter = context->m_forParameter;
    zen_ForParameterContext_t* forParameterContext =
        (zen_ForParameterContext_t*)forParameter->m_context;

    /* Generate the instructions corresponding to the expression specified to
     * the for statement.
     */
    zen_ASTWalker_walk(astListener, context->m_expression);

    /* Invoke the Iterable#getIterator() function to retrieve the iterator.
     */
    zen_BinaryEntityBuilder_emitInvokeVirtual(generator->m_builder,
        getIteratorIndex);

    /* Log the emission of the invoke_virtual instruction. */
    printf("[debug] Emitted invoke_virtual %d\n", getIteratorIndex);

    /* Store the obtained iterator in a local variable for future reference.
     * The actual emission of the instruction is delegated to the
     * zen_BinaryEntityGenerator_storeLocalReference() function which takes
     * care of optimizing the emission.
     *
     * TODO: Implement the zen_BinaryEntityGenerator_storeLocalReference() function.
     */
    zen_BinaryEntityBuilder_emitStoreReference(generator->m_builder, 0);

    /* Log the emission of the store_a instruction. */
    printf("[debug] Emitted store_a %d (dummy index)\n", 0);

    uint16_t loopIndex = zen_DataChannel_getSize(parentChannel);

    /* Load the iterator from the local variable.
     * The actual emission of the instruction is delegated to the
     * zen_BinaryEntityGenerator_loadLocalReference() function which takes
     * care of optimizing the emission.
     *
     * TODO: Implement the zen_BinaryEntityGenerator_loadLocalReference() function.
     */
    zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder, 0);

    /* Log the emission of the load_a instruction. */
    printf("[debug] Emitted load_a %d (dummy index)\n", 0);

    /* Invoke the Iterator#hasNext() function to determine whether the iterator
     * has more values to return.
     */
    zen_BinaryEntityBuilder_emitInvokeVirtual(generator->m_builder, hasNextIndex);

    /* Log the emission of the invoke_virtual instruction. */
    printf("[debug] Emitted invoke_virtual %d\n", hasNextIndex);

    /* Emit the jump_eq0_i instruction. */
    zen_BinaryEntityBuilder_emitJumpEqual0Integer(generator->m_builder, 0);

    /* Log the emission of the jump_eq0_i instruction. */
    printf("[debug] Emitted jump_eq0_i 0 (dummy index)\n");

    /* Save the index of the byte where the dummy data was written. */
    int32_t updateIndex = zen_DataChannel_getSize(parentChannel) - 2;

    /* Invoke the Iterator#getNext() function to retrieve the next value. */
    zen_BinaryEntityBuilder_emitInvokeVirtual(generator->m_builder, getNextIndex);

    /* Log the emission of the invoke_virtual instruction. */
    printf("[debug] Emitted invoke_virtual %d\n", getNextIndex);

    /* Store the retrieved value in a local variable. */
    zen_BinaryEntityBuilder_emitStoreReference(generator->m_builder, 0);

    /* Log the emission of the store_a instruction. */
    printf("[debug] Emitted store_a %d (dummy index)\n", 0);

    /* Generate the instructions corresponding to the statement suite specified
     * to the while statement.
     */
    zen_ASTWalker_walk(astListener, context->m_statementSuite);

    /* Generate a jump instruction to loop back to the conditional expression. */
    zen_BinaryEntityBuilder_emitJump(generator->m_builder, loopIndex);

    /* Log the emission of the jump instruction. */
    printf("[debug] Emitted jump %d\n", loopIndex);

    uint16_t newParentChannelSize = zen_DataChannel_getSize(parentChannel);
    parentChannel->m_bytes[updateIndex] = (newParentChannelSize & 0x0000FF00) >> 8;
    parentChannel->m_bytes[updateIndex + 1] = newParentChannelSize & 0x000000FF;
}

// forParameters

void zen_BinaryEntityGenerator_onEnterForParameters(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitForParameters(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// tryStatement

void zen_BinaryEntityGenerator_onEnterTryStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    /* Retrieve the generator associated with the AST listener. */
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    /* Retrieve the context of the AST node. */
    zen_TryStatementContext_t* context = (zen_TryStatementContext_t*)node->m_context;

    /* The normal behaviour of the AST walker causes the generator to emit instructions
     * in an undesirable fashion. Therefore, we partially switch from the listener
     * to visitor design pattern. The AST walker can be guided to switch to this
     * mode via zen_ASTListener_skipChildren() function which causes the AST walker
     * to skip iterating over the children nodes.
     */
    zen_ASTListener_skipChildren(astListener);
}

/*
 * The algorithm for generating instructions corresponding to a try statement is
 * given below.
 *
 * ### Algorithm 1
 *
 * - Try Clause
 *   1. Generate instructions corresponding to the statement suite specified
 *      to the try clause.
 *   2. Record the start and stop indexes of these instructions on the data
 *      channel. They will be later used to generate the exception table.
 *   3. Generate instructions corresponding to the statement suite specified to
 *      the finally clause.
 *   4. Jump to the instruction immediately following the try statement.
 *
 * - Catch Clause
 *   1. The virtual machine pushes the exception that was caught to the operand stack.
 *      Store this reference in a local variable.
 *   2. Generate instructions corresponding to the statement suite specified
 *      to the catch clause.
 *   3. Record the start and stop indexes of these instructions on the data
 *      channel. They will be later used to generate the exception table.
 *   4. Generate instructions corresponding to the statement suite specified to
 *      the finally clause.
 *   5. Jump to the instruction immediately following the try statement.
 *
 * - Finally Clause
 *   This section of instructions handle exceptions that were triggered either
 *   by the statement suite specified to the try clause or the catch clause.
 *   In both the cases, the exceptions are thrown again.
 *
 *   This section is basically an implicit catch clause that handles instances
 *   of the Throwable class. A record of this implicit clause is added to the
 *   exception table.
 *
 *   1. The virtual machine pushes the exception that was caught to the operand
 *      stack. Store this reference in a local variable.
 *   2. Generate instructions corresponding to the statement suite specified
 *      to the finally clause.
 *   3. Load the caught exception from the local variable.
 *   4. Throw the caught exception again.
 *
 * The primary disadvantage with the algorithm described above is the duplicate
 * copies of the finally clause. Consider a try statement with `n + 1` clauses,
 * where n is the number catch clauses and 1 represents the try clause. For such
 * a statement this algorithm generates `n + 2` copies of the finally clause.
 * The additional 1 represents the finally clause generated to handle unhandled
 * exceptions thrown either by the try clause or the catch clause.
 *
 * The following algorithm describes another approach where there are only
 * 2 copies of the finally clause.
 *
 * ### Algorithm 2
 *
 * - Try Clause
 *   1. Generate instructions corresponding to the statement suite specified
 *      to the try clause.
 *   2. Record the start and stop indexes of these instructions on the data
 *      channel. They will be later used to generate the exception table.
 *   3. Jump to the section (FC1) where the instructions of the finally clause
 *      was generated.
 *
 * - Catch Clause
 *   1. The virtual machine pushes the exception that was caught to the operand
 *      stack. Store this reference in a local variable.
 *   2. Generate instructions corresponding to the statement suite specified
 *      to the catch clause.
 *   3. Record the start and stop indexes of these instructions on the data
 *      channel. They will be later used to generate the exception table.
 *   4. Jump to the section (FC1) where the instructions of the finally clause
 *      was generated.
 *
 * - Finally Clause 1 (FC1)
 *   1. Generate instructions corresponding to the statement suite specified
 *      to the finally clause.
 *   2. Jump to the instruction immediately following the try statement.
 *
 * - Finally Clause 2 (FC2)
 *   This section of instructions handle exceptions that were triggered either
 *   by the statement suite specified to the try clause or the catch clause.
 *   In both the cases, the exceptions are thrown again.
 *
 *   This section is basically an implicit catch clause that handles instances
 *   of the Throwable class. A record of this implicit clause is added to the
 *   exception table.
 *
 *   1. The virtual machine pushes the exception that was caught to the operand
 *      stack. Store this reference in a local variable.
 *   2. Generate instructions corresponding to the statement suite specified
 *      to the finally clause.
 *   3. Load the caught exception from the local variable.
 *   4. Throw the caught exception again.
 *
 * For both the algorithms, the following records are stored in the exception
 * table.
 *  - The exceptions triggered within the try clause are handled by the catch
 *    clauses. These exceptions are not implicitly thrown again
 *  - The exceptions triggered within the try clause but are not handled by the
 *    catch clauses. These exceptions are thrown again implicity by the finally
 *    clause.
 *  - The exceptions triggered within the catch clause. These exceptions are thrown
 *    again implicity by the finally clause.
 *
 * The last two types of exception records are added to the exception table
 * only if a finally clause is present in the try statement.
 *
 * The latter algorithm is used in this implementation.
 */
void zen_BinaryEntityGenerator_onExitTryStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    /* Retrieve the generator associated with the AST listener. */
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    /* Retrieve the context of the AST node. */
    zen_TryStatementContext_t* context = (zen_TryStatementContext_t*)node->m_context;

    int32_t parentChannelIndex = zen_BinaryEntityBuilder_getActiveChannelIndex(
        generator->m_builder);
    zen_DataChannel_t* parentChannel = zen_BinaryEntityBuilder_getChannel(
        generator->m_builder, parentChannelIndex);

    zen_ASTNode_t* tryClause = context->m_tryClause;
    zen_TryClauseContext_t* tryClauseContext = (zen_TryClauseContext_t*)tryClause->m_context;

    zen_ASTNode_t* statementSuite = tryClauseContext->m_statementSuite;

    int32_t numberOfCatchClauses = jtk_ArrayList_getSize(context->m_catchClauses);
    /* This includes indexes for the try clause and n - 1 catch clauses,
     * where n represents the number of catch clauses. A jump instruction
     * is not generated for the last catch clause. Because it can fall through
     * to the FC1 section without an explicit jump.
     */
    int32_t* updateIndexes = jtk_Memory_allocate(int32_t, numberOfCatchClauses);
    int32_t* catchClauseIndexes = jtk_Memory_allocate(int32_t, numberOfCatchClauses * 2);

    int32_t tryClauseStartIndex = -1;
    int32_t tryClauseStopIndex = -1;
    uint8_t* exceptionClassName = "Exception";
    int32_t exceptionClassSize = 9;

    zen_ASTNode_t* catchClause = NULL;
    zen_CatchClauseContext_t* catchClauseContext = NULL;

    int32_t index = -1;
    do {
        /* Save the index where the instruction section for the current clause
         * begins (inclusive).
         */
        int32_t startIndex = zen_DataChannel_getSize(parentChannel);

        /* Generate the instructions corresponding to the statement suite specified
         * to the current clause.
         */
        zen_ASTWalker_walk(astListener, statementSuite);

        /* Save the index where the instruction section for the current clause
         * ends (exclusive).
         */
        int32_t stopIndex = zen_DataChannel_getSize(parentChannel);

        if (index == -1) {
            tryClauseStartIndex = startIndex;
            tryClauseStopIndex = stopIndex;
        }
        else {
            /* At this point, the load_a instruction that loads the caught
             * exception onto the operand stack has been written to the parent
             * data channel. Therefore, adjust the start index by subtracting
             * 2, the length of the load_a instruction.
             */
            catchClauseIndexes[(index * 2) + 0] = startIndex - 2;
            catchClauseIndexes[(index * 2) + 1] = stopIndex;

            uint16_t exceptionClassIndex = zen_ConstantPoolBuilder_getClassEntryIndexEx(
                generator->m_constantPoolBuilder, exceptionClassName, exceptionClassSize);

            zen_ASTNode_t* catchFilter = catchClauseContext->m_catchFilter;
            zen_CatchFilterContext_t* catchFilterContext =
                (zen_CatchFilterContext_t*)catchFilter->m_context;
            int32_t filterCount = jtk_ArrayList_getSize(catchFilterContext->m_typeNames);
            int32_t filterIndex;
            for (filterIndex = 0; filterIndex < filterCount; filterIndex++) {
                zen_ASTNode_t* typeName = (zen_ASTNode_t*)jtk_ArrayList_getValue(
                    catchFilterContext->m_typeNames, filterIndex);

                // TODO: Evaluate the class name

                /* In this exception handler site, the exceptions triggered within the
                 * try clause are handled by the catch clauses. These exceptions are
                 * not implicitly thrown again.
                 */
                zen_ExceptionHandlerSite_t* type1Handler =
                    jtk_Memory_allocate(zen_ExceptionHandlerSite_t, 1);
                type1Handler->m_startIndex = tryClauseStartIndex;
                type1Handler->m_stopIndex = tryClauseStopIndex;
                type1Handler->m_handlerIndex = startIndex - 2;
                type1Handler->m_exceptionClassIndex = exceptionClassIndex;

                jtk_ArrayList_add(generator->m_exceptionHandlerSites, type1Handler);
            }
        }

        index++;

        /* A jump instrution should not be generated only if there are no
         * catch clauses or if the current catch clause is the last catch
         * clause.
         */
        if (index != numberOfCatchClauses) {
            /* If the current clause is a try clause and it executes completely,
             * without triggering any exception, the catch clauses and the finally
             * clause should be skipped.
             *
             * Similarly, if the current clause is catch clause and it executes
             * completely, without triggering any exception, subsequence catch
             * clauses and the finally clause should be skipped.
             *
             * Given the statement has not been generated yet, the jump offset
             * cannot be evaluated right now. Therefore, emit the jump instruction
             * with a dummy offset.
             *
             * Emit the jump instruction to skip the other clauses.
             */
            zen_BinaryEntityBuilder_emitJump(generator->m_builder, 0);

            /* Log the emission of the jump instruction. */
            printf("[debug] Emitted jump 0 (dummy index)\n");

            /* Save the index of the bytes where the dummy data was written. */
            updateIndexes[index] = zen_DataChannel_getSize(parentChannel) - 2;

            catchClause = (zen_ASTNode_t*)jtk_ArrayList_getValue(context->m_catchClauses,
                index);
            catchClauseContext =(zen_CatchClauseContext_t*)catchClause->m_context;
            statementSuite = catchClauseContext->m_statementSuite;
            // Update the exception class name here.

            /* The virtual machine pushes the exception that was caught to the
             * operand stack. Store this reference in a local variable.
             */
            zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder, 0);

            /* Log the emission of the load_a instruction. */
            printf("[debug] Emitted load_a 0 (dummy index)\n");
        }
    }
    while (index < numberOfCatchClauses);

    int32_t fc1StartIndex = zen_DataChannel_getSize(parentChannel);
    int32_t i;
    for (i = 0; i < numberOfCatchClauses; i++) {
        uint16_t updateIndex = updateIndexes[i];
        parentChannel->m_bytes[updateIndex] = (0x0000FF00 & fc1StartIndex) >> 8;
        parentChannel->m_bytes[updateIndex + 1] = (0x000000FF & fc1StartIndex);
    }

    if (context->m_finallyClause != NULL) {
        /* Retrieve the AST node for the finally clause. */
        zen_ASTNode_t* finallyClause = context->m_finallyClause;
        /* Retrieve the context associated with the AST node of the finally clause. */
        zen_FinallyClauseContext_t* finallyClauseContext =
            (zen_FinallyClauseContext_t*)finallyClause->m_context;

        /* Generate the instructions for the statement suite specified to the
         * finally clause. This constitutes the bulk of the FC1 section.
         */
        zen_ASTWalker_walk(astListener, finallyClauseContext->m_statementSuite);

        /* Once the FC1 section is completed, the program should skip the FC2
         * section.
         *
         * Given the FC2 section has not been generated yet, the jump offset
         * cannot be evaluated right now. Therefore, emit the jump instruction
         * with a dummy offset.
         *
         * Emit the jump instruction to skip the FC2 section.
         */
        zen_BinaryEntityBuilder_emitJump(generator->m_builder, 0);

        /* Log the emission of the jump instruction. */
        printf("[debug] Emitted jump 0 (dummy index)\n");

        /* Save the index of the bytes where the dummy data was written. */
        int32_t skipIndex = zen_DataChannel_getSize(parentChannel) - 2;

        /* Generate the FC2 section. */

        int32_t fc2StartIndex = zen_DataChannel_getSize(parentChannel);

        /* In this exception handler site, the exceptions triggered within the try
         * clause but are not handled by the catch clauses are taken care of. These
         * exceptions are thrown again implicity by the finally clause.
         */
        zen_ExceptionHandlerSite_t* type2Handler =
            jtk_Memory_allocate(zen_ExceptionHandlerSite_t, 1);
        type2Handler->m_startIndex = tryClauseStartIndex;
        type2Handler->m_stopIndex = tryClauseStopIndex;
        type2Handler->m_handlerIndex = fc2StartIndex;
        type2Handler->m_exceptionClassIndex = 0;

        jtk_ArrayList_add(generator->m_exceptionHandlerSites, type2Handler);

        /* The virtual machine pushes the exception that was caught to the operand
         * stack. Store this reference in a local variable.
         */
        zen_BinaryEntityBuilder_emitStoreReference(generator->m_builder, 0);

        /* Log the emission of the store_a instruction. */
        printf("[debug] Emitted store_a 0 (dummy index)\n");

        /* Generate the instructions for the statement suite specified to the
         * finally clause. This constitutes the bulk of the FC2 section.
         */
        zen_ASTWalker_walk(astListener, finallyClauseContext->m_statementSuite);

        /* Load the caught exception from the local variable. */
        zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder, 0);

        /* Log the emission of the store_a instruction. */
        printf("[debug] Emitted load_a 0 (dummy index)\n");

        /* Throw the caught exception again. */
        zen_BinaryEntityBuilder_emitThrow(generator->m_builder);

        /* Log the emission of the throw instruction. */
        printf("[debug] Emitted throw\n");

        int32_t fc2StopIndex = zen_DataChannel_getSize(parentChannel);

        /* Update the jump instruction in the FC1 section that skips the FC2
         * section.
         */
        parentChannel->m_bytes[skipIndex] = (0x0000FF00 & fc2StopIndex) >> 8;
        parentChannel->m_bytes[skipIndex + 1] = (0x000000FF & fc2StopIndex);

        /* In these exception handler sites, the exceptions triggered within
         * the catch clauses are taken care of. These exceptions are thrown
         * again implicity by the finally clause.
         */
        int32_t j;
        for (j = 0; j < numberOfCatchClauses; j++) {
            zen_ExceptionHandlerSite_t* type3Handler =
                jtk_Memory_allocate(zen_ExceptionHandlerSite_t, 1);
            type3Handler->m_startIndex = catchClauseIndexes[(j * 2) + 0];
            type3Handler->m_stopIndex = catchClauseIndexes[(j * 2) + 1];
            type3Handler->m_handlerIndex = fc2StartIndex;
            type3Handler->m_exceptionClassIndex = 0;

            jtk_ArrayList_add(generator->m_exceptionHandlerSites, type3Handler);
        }
    }

    jtk_Memory_deallocate(catchClauseIndexes);
    jtk_Memory_deallocate(updateIndexes);
}

// tryClause

void zen_BinaryEntityGenerator_onEnterTryClause(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitTryClause(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// catchClause

void zen_BinaryEntityGenerator_onEnterCatchClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitCatchClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// catchFilter

void zen_BinaryEntityGenerator_onEnterCatchFilter(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitCatchFilter(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// finallyClause

void zen_BinaryEntityGenerator_onEnterFinallyClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitFinallyClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// synchronizeStatement

void zen_BinaryEntityGenerator_onEnterSynchronizeStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    /* Retrieve the generator associated with the AST listener. */
    zen_BinaryEntityGenerator_t* generator =
        (zen_BinaryEntityGenerator_t*)astListener->m_context;
    /* Retrieve the context of the AST node. */
    zen_SynchronizeStatementContext_t* context =
        (zen_SynchronizeStatementContext_t*)node->m_context;

    /* The normal behaviour of the AST walker causes the generator to emit instructions
     * in an undesirable fashion. Therefore, we partially switch from the listener
     * to visitor design pattern. The AST walker can be guided to switch to this
     * mode via zen_ASTListener_skipChildren() function which causes the AST walker
     * to skip iterating over the children nodes.
     */
    zen_ASTListener_skipChildren(astListener);
}

/*
 * ALGORITHM FOR GENERATING INSTRUCTIONS CORRESPONDING TO SYNCHRONIZE
 * STATEMENTS
 *
 * 1. Generate the instructions corresponding to the expression specified
 *    to the synchronize statement.
 * 2. Make a duplicate of the resulting lock object.
 * 3. Store the duplicate reference in a local variable that only the
 *    compiler has access to.
 * 4. Invoke the Lock#acquire() function.
 * 5. Generate the instructions corresponding to the statement suite specified
 *    to the synchronize statement.
 * 6. Load the reference to the lock object from the local variable.
 * 7. Invoke the Lock#release() function.
 * 8. The synchronize statement generates an implicit finally clause that releases
 *    the lock when an exception is triggered within the statement suite specified
 *    to the synchronize statement. Therefore, generate a jump instruction to
 *    skip the finally clause section.
 * 9. An exception is pushed onto the operand stack for the finally clause to
 *    throw again. Therefore, store the reference to the exception in a local
 *    variable.
 * 10. Load the reference to the lock object from the local variable.
 * 11. Invoke the Lock#release() function.
 * 12. Load the reference to the exception that was thrown.
 * 13. Rethrow the exception that caused the finally clause to execute.
 *
 * The following records will be stored in the exception table.
 * 1. Any exception that is triggered within the statement suite is handled
 *    by the implicit finally clause.
 * 2. Any exception that is thrown by the Lock#release() function is handled
 *    by the finally clause.
 *
 * The Lock#release() may throw an exception within the bounds of the finally
 * clause. From the above records we know that the control is passed to the
 * implicit finally clause when an exception is thrown by the Lock#release()
 * function. This results in the implicit finally clause forming a loop.
 */
void zen_BinaryEntityGenerator_onExitSynchronizeStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    /* Retrieve the generator associated with the AST listener. */
    zen_BinaryEntityGenerator_t* generator =
        (zen_BinaryEntityGenerator_t*)astListener->m_context;
    /* Retrieve the context of the AST node. */
    zen_SynchronizeStatementContext_t* context =
        (zen_SynchronizeStatementContext_t*)node->m_context;

    int32_t parentChannelIndex = zen_BinaryEntityBuilder_getActiveChannelIndex(
        generator->m_builder);
    zen_DataChannel_t* parentChannel = zen_BinaryEntityBuilder_getChannel(
        generator->m_builder, parentChannelIndex);

    const uint8_t* lockClassName = "zen/concurrency/lock/Lock";
    int32_t lockClassNameSize = 25;

    const uint8_t* acquireDescriptor = "v:v";
    int32_t acquireDescriptorSize = 3;
    const uint8_t* acquireName = "acquire";
    int32_t acquireNameSize = 7;
    uint16_t acquireIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, lockClassName, lockClassNameSize,
        acquireDescriptor, acquireDescriptorSize, acquireName, acquireNameSize);

    const uint8_t* releaseDescriptor = "v:v";
    int32_t releaseDescriptorSize = 3;
    const uint8_t* releaseName = "release";
    int32_t releaseNameSize = 7;
    uint16_t releaseIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, lockClassName, lockClassNameSize,
        releaseDescriptor, releaseDescriptorSize, releaseName, releaseNameSize);

    /* Generate the instructions for the expression specified to the
     * synchronize statement.
     */
    zen_ASTWalker_walk(astListener, context->m_expression);

    /* Make a duplicate of the resulting lock object. */
    zen_BinaryEntityBuilder_emitDuplicate(generator->m_builder);

    /* Log the emission of the duplicate instruction. */
    printf("[debug] Emitted duplicate\n");

    /* Store the duplicate reference in a local variable that only the
     * compiler has access to.
     */
    zen_BinaryEntityBuilder_emitStoreReference(generator->m_builder, 0);

    /* Log the emission of the store_a instruction. */
    printf("[debug] Emitted store_a 0 (dummy index)\n");

    /* Invoke the Lock#acquire() function. */
    zen_BinaryEntityBuilder_emitInvokeVirtual(generator->m_builder, acquireIndex);

    /* Log the emission of the invoke_virtual instruction. */
    printf("[debug] Emitted invoke_virtual %d\n", acquireIndex);

    /* Save the index where the instruction section for the statement suite
     * specified to the synchronize statement begins (inclusive).
     */
    int32_t startIndex1 = zen_DataChannel_getSize(parentChannel);

    /* Generate the instructions corresponding to the statement suite specified
     * to the synchronize statement.
     */
    zen_ASTWalker_walk(astListener, context->m_statementSuite);

    /* Load the reference to the lock object from the local variable. */
    zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder, 0);

    /* Log the emission of the load_a instruction. */
    printf("[debug] Emitted load_a 0 (dummy index)\n");

    /* Invoke the Lock#release() function. */
    zen_BinaryEntityBuilder_emitInvokeVirtual(generator->m_builder, releaseIndex);

    /* Log the emission of the invoke_virtual instruction. */
    printf("[debug] Emitted invoke_virtual %d\n", releaseIndex);

    /* Save the index where the instruction section for the statement suite
     * specified to the synchronize statement ends (exclusive).
     */
    int32_t stopIndex1 = zen_DataChannel_getSize(parentChannel);

    /* The synchronize statement generates an implicit finally clause that releases
     * the lock when an exception is triggered within the statement suite specified
     * to the synchronize statement. Therefore, generate a jump instruction to
     * skip the finally clause section.
     *
     * Given the implicity finally clause section has not been generated yet,
     * the jump offset cannot be evaluated right now. Therefore, emit the jump
     * instruction with a dummy offset.
     */
    zen_BinaryEntityBuilder_emitJump(generator->m_builder, 0);

    /* Log the emission of the jump instruction. */
    printf("[debug] Emitted jump 0 (dummy index)\n");

    /* Save the index of the bytes where the dummy data was written. */
    int32_t skipIndex = zen_DataChannel_getSize(parentChannel) - 2;

    /* Save the index where the instruction section for the implicit finally
     * clause begins (inclusive).
     */
    int32_t startIndex2 = zen_DataChannel_getSize(parentChannel);

    /* The virtual machine pushes the exception that was thrown to the operand
     * stack before the control is passed to the implicit finally clause, thanks
     * to the exception table. Store this reference in a local variable.
     */
    zen_BinaryEntityBuilder_emitStoreReference(generator->m_builder, 0);

    /* Log the emission of the store_a instruction. */
    printf("[debug] Emitted store_a 0 (dummy index)\n");

    /* Load the reference to the lock object from the local variable. */
    zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder, 0);

    /* Log the emission of the load_a instruction. */
    printf("[debug] Emitted load_a 0 (dummy index)\n");

    /* Invoke the Lock#release() function. */
    zen_BinaryEntityBuilder_emitInvokeVirtual(generator->m_builder, releaseIndex);

    /* Log the emission of the invoke_virtual instruction. */
    printf("[debug] Emitted invoke_virtual %d\n", releaseIndex);

    /* Save the index where the instruction section for the implicit finally
     * clause ends (exclusive).
     */
    int32_t stopIndex2 = zen_DataChannel_getSize(parentChannel);

    /* Load the thrown exception from the local variable. */
    zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder, 0);

    /* Log the emission of the store_a instruction. */
    printf("[debug] Emitted load_a 0 (dummy index)\n");

    /* Throw the exception again. */
    zen_BinaryEntityBuilder_emitThrow(generator->m_builder);

    /* Log the emission of the throw instruction. */
    printf("[debug] Emitted throw\n");

    uint16_t newParentChannelSize = zen_DataChannel_getSize(parentChannel);
    parentChannel->m_bytes[skipIndex] = (newParentChannelSize & 0x0000FF00) >> 8;
    parentChannel->m_bytes[skipIndex + 1] = newParentChannelSize & 0x000000FF;

    zen_ExceptionHandlerSite_t* type1Handler = jtk_Memory_allocate(zen_ExceptionHandlerSite_t, 1);
    type1Handler->m_startIndex = startIndex1;
    type1Handler->m_stopIndex = stopIndex1;
    type1Handler->m_handlerIndex = startIndex2;
    type1Handler->m_exceptionClassIndex = 0;

    zen_ExceptionHandlerSite_t* type2Handler = jtk_Memory_allocate(zen_ExceptionHandlerSite_t, 1);
    type2Handler->m_startIndex = startIndex2;
    type2Handler->m_stopIndex = stopIndex2;
    type2Handler->m_handlerIndex = startIndex2;
    type2Handler->m_exceptionClassIndex = 0;

    jtk_ArrayList_add(generator->m_exceptionHandlerSites, type1Handler);
    jtk_ArrayList_add(generator->m_exceptionHandlerSites, type2Handler);
}

// withStatement

/*
 * ALGORITHM FOR GENERATING INSTRUCTIONS CORRESPONDING TO WITH STATEMENT
 *
 * The following steps should be repeated for managing N objects.
 * Note that the closing of the resources is in the order opposite to that
 * specified to the with statement.
 *
 * The structure of the instructions generated for 3 expressions is shown below.
 * [expression-1]
 *     [expression-2]
 *         [expression-3]
 *             [statement suite]
 *         [FC1-3]
 *         [FC2-3]
 *     [FC1-2]
 *     [FC2-2]
 * [FC1-1]
 * [FC1-2]
 *
 * In the following paragraphs, I refer to anything in between expression-k
 * and FC1-k as the core section for the kth iteration.
 *
 * Repeat the following sections N times, where N is the number of expressions
 * specified to the with statement.
 *
 * [Primary]
 * 1. Generate the instructions corresponding to the kth expression specified to
 *    the with statement.
 * 2. Store the resulting objects in local variable, say Lk, which can be
 *    accessed only by the compiler.
 * 3. If k is equal to N, generate the instructions corresponding to the
 *    statement suite specified to the with statement.
 * 4. Load the object from the local variable Lk.
 * 5. Invoke the Closeable#close() function to close the resource.
 * 6. Jump to skip FC1-k and FC2-k sections.
 *
 * [Finally Clause 1 (FC1)]
 * This section handles the exceptions thrown by the core section.
 *
 * 1. The virtual machine pushes the thrown exception to the operand stack.
 *    Store this reference in a local variable, say L[k + x] (where x is the
 *    base local variable index for private FC1 local variables), which can
 *    be accessed only by the compiler.
 *    UPDATE: Unlike the previous algorithm, the new algorithm allocates only
 *    one local variable. It can be reused, given only one FC1 section is active
 *    at any given time and the exception objects are propogated by the virtual
 *    machine. This helps improve the economy of the local variables.
 * 2. Load the resource object from the local variable Lk.
 * 3. Invoke the Closeable#close() function to close the resource.
 * 4. Load the exception object that was thrown by the core section.
 * 5. Throw the exception again.
 *
 * [Finally Clause 2 (FC2)]
 * This section handles the exceptions thrown by the FC2 section.
 *
 * 1. The virtual machine pushes the thrown exception to the operand stack.
 *    Store this reference in a local variable, say L[k + y] (where y is the base
 *    local variable index for private FC2 local variables), which can be accessed
 *    only by the compiler.
 *    UPDATE: Unlike the previous algorithm, the new algorithm allocates only
 *    one local variable. It can be reused, given only one FC2 section is active
 *    at any given time and the exception objects are propogated by the virtual
 *    machine. This helps improve the economy of the local variables.
 * 2. Load the exception object that was thrown by the core section. In other
 *    words, load the object referenced by L[k + x].
 * 3. Load the exception object that was thrown by the Closeable#close() function
 *    in the FC1 section. In other words, load the object referenced by L[k + y].
 * 4. Invoke the Throwable#suppress() function to add the exception thrown by
 *    the FC1 section to the exception thrown by the core section.
 *    In other words, `L[k + x].suppress(L[k + y])`.
 * 5. Load the exception object that was thrown by the core section. In other
 *    words, load the object referenced by L[k + x].
 * 6. Throw the exception again.
 *
 * The following cases of exceptions can be summarized along with the behavior
 * of the with statement.
 * 1. The core section does not trigger any exception.
 *    The Closeable#close() function does not trigger any exception.
 *
 * 2. The core section does not trigger any exception.
 *    The Closeable#close() function triggers any exception.
 *
 * 3. The core section triggers an exception.
 *    The Closeable#close() function does not trigger any exception.
 *
 *    In such cases, the control is transferred to the FC1-k section, which
 *    closes the resource. The exception thrown by the core is
 *    thrown again.
 *
 * 4. The core triggers an exception.
 *    The Closeable#close() function triggers an exception.
 *
 *    The core section trigger an exception. This results in the control being
 *    transferred to the FC1-k section, which makes an attempt to close the
 *    resource. This triggers another exception. Thus, the control is transferred
 *    to the FC2-k section. In the FC2-k section, the exception thrown by the
 *    Closeable#close() function is suppressed by the exception thrown by the
 *    core section using the Throwable#suppress() function. The oldest exception,
 *    that is, the exception thrown by the core section, is thrown again.
 *
 * The following records are added to the exception table.
 * 1. The exceptions thrown by the core section are handled by the FC1-k section.
 * 2. The exceptions thrown by the FC1-k section are handled by the FC2-k section.
 *
 * Notice that for N expressions, N * 2 records are added to the exception table.
 * Similarly, 3N + 1 sections of instructions are generated, that is,
 * N expressions, N FC1 sections, N FC2 sections, and 1 statement suite.
 */
void zen_BinaryEntityGenerator_onEnterWithStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    /* Retrieve the generator associated with the AST listener. */
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;

    /* Retrieve the context of the AST node. */
    zen_WithStatementContext_t* context = (zen_WithStatementContext_t*)node->m_context;

    /* The normal behaviour of the AST walker causes the generator to emit instructions
     * in an undesirable fashion. Therefore, we partially switch from the listener
     * to visitor design pattern. The AST walker can be guided to switch to this
     * mode via zen_ASTListener_skipChildren() function which causes the AST walker
     * to skip iterating over the children nodes.
     */
    zen_ASTListener_skipChildren(astListener);
}

#define ZEN_BINARY_ENTITY_GENERATOR_MAX_LOCAL_VARIABLES 255

int32_t zen_BinaryEntityGenerator_allocateLocalVariables(zen_BinaryEntityGenerator_t*
    generator, int32_t count) {
    jtk_Assert_assertObject(generator, "The specified binary entity generator is null.");
    jtk_Assert_assertTrue(count > 0, "The specified local variable count is invalid.");

    int32_t result = -1;
    if (generator->m_localVariableCount + count <= ZEN_BINARY_ENTITY_GENERATOR_MAX_LOCAL_VARIABLES) {
        result = generator->m_localVariableCount;
        generator->m_localVariableCount += count;
    }
    else {
        printf("[internal error] Too many local variables declared within the same function. Looks like the phases prior to the code generator have malfunctioned.\n");
    }
    return result;
}

/* As of version 1.0.0, the wide instruction is only part of the virtual machine
 * design. This goes to say that certain features of Zen are limited. One such
 * example is that local variables are limited to a maximum of 255 per function.
 * In other words, you cannot declare more than 255 local variables in a function.
 * Remember this includes all the implicit variables generated by the code
 * generated, such as "this" reference, implicit variables used by compound statements
 * like for/with statements, and so on.
 *
 * In particular, one should be cautious when using the with statement. Of all
 * the compound statements, it is the most complex. The old algorithm generated at
 * least 3N local variables, where N is the total number of resources specified
 * to the same. The new algorithm for generating instructions corresponding
 * to the with statement allocates a minimum of N + 2 local variables for internal
 * operations.
 *
 * [EXAMPLE]
 *
 * Consider the following program written in Zen.
 *
 * function main()
 *     with 1, 2
 *         ;
 *
 * Notice that the expressions specified to the with statement evaluate to
 * integers. In reality, one cannot specify integers to the same. However, for
 * the sake of demonstration I have used integers for expressions and an empty
 * statement as the statement suite. This allows us to focus on the instructions
 * that provide the functionality of the with statement.
 *
 * ; Expression 1
 * #1 push_i1
 * #2 store_a 0
 *
 * ; Expression 2
 * #4 push_i2
 * #5 store_a 1
 *
 * ; Statement suite
 * #7 nop
 *
 * ; The default instructions that make an attempt to close the resource obtained
 * ; from expression 1. If an exception is thrown here, it propogates without causing
 * ; transfer of control to FC1-1 or FC2-1 sections. In other words, any exception
 * ; thrown here will transfer control to FC1-2 section, without causing the FC1-1
 * ; and FC2-1 sections to execute.
 * #8 load_a 0
 * #10 invoke_virtual 6
 * #13 jump 38
 *
 * ; The FC1-1 section, which makes an attempt to close the resource obtained
 * ; from expression 1. It throws the exception that originated from the statement
 * ; suite. The control is transferred to FC1-2 section.
 * #16 store_a 2
 * #18 load_a 0
 * #20 invoke_virtual 6
 * #23 load_a 2
 * #25 throw
 *
 * ; The following instructions constitute the FC2-1 section. It supresses the
 * ; exception thrown by Closeable#close() function with the exception thrown by
 * ; the statement suite. It throws the exception that originated from the statement
 * ; suite. The control is transferred to FC1-2 section.
 * #26 store_a 3
 * #28 load_a 2
 * #30 load_a 3
 * #32 invoke_virtual 11
 * #35 load_a 2
 * #37 throw
 *
 * ; The default instructions that make an attempt to close the resource obtained
 * ; from expression 2. If an exception is thrown here, it propogates without causing
 * ; transfer of control to FC1-2 or FC2-2 sections. In other words, any exception
 * ; thrown here will transfer control to instructions beyond the with statement,
 * ; without causing the FC1-2 and FC2-2 sections to execute.
 * #38 load_a 1
 * #40 invoke_virtual 6
 * #43 jump 68
 *
 * ; The FC1-2 section, which makes an attempt to close the resource obtained
 * ; from expression 2. It throws the exception that originated from the FC2-1
 * ; section or expression 2. The control is transferred to instructions beyond
 * ; the with statement.
 * #46 store_a 2
 * #48 load_a 1
 * #50 invoke_virtual 6
 * #53 load_a 2
 * #55 throw
 *
 * ; The following instructions constitute the FC2-2 section. It supresses the
 * ; exception thrown by Closeable#close() function in FC1-2 section with the
 * ; exception thrown by the statement suite. It throws the exception that originated
 * ; from the FC2-1 section or expression 2. The control is transferred to
 * ; instructions beyond the with statement.
 * #56 store_a 3
 * #58 load_a 2
 * #60 load_a 3
 * #62 invoke_virtual 11
 * #65 load_a 2
 * #67 throw
 */
void zen_BinaryEntityGenerator_onExitWithStatement(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    /* Retrieve the generator associated with the AST listener. */
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;

    /* Retrieve the context of the AST node. */
    zen_WithStatementContext_t* context = (zen_WithStatementContext_t*)node->m_context;

    const uint8_t* closeableClassName = "zen/core/Closeable";
    int32_t closeableClassNameSize = 18;
    const uint8_t* closeDescriptor = "v:v";
    int32_t closeDescriptorSize = 3;
    const uint8_t* closeName = "close";
    int32_t closeNameSize = 5;
    uint16_t closeIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, closeableClassName, closeableClassNameSize,
        closeDescriptor, closeDescriptorSize, closeName, closeNameSize);

    const uint8_t* throwableClassName = "zen/core/Throwable";
    int32_t throwableClassNameSize = 18;
    const uint8_t* suppressDescriptor = "v:(zen/core/Throwable)";
    int32_t suppressDescriptorSize = 22;
    const uint8_t* suppressName = "suppress";
    int32_t suppressNameSize = 8;
    uint16_t suppressIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, throwableClassName, throwableClassNameSize,
        suppressDescriptor, suppressDescriptorSize, suppressName, suppressNameSize);

    int32_t parentChannelIndex = zen_BinaryEntityBuilder_getActiveChannelIndex(
         generator->m_builder);
    zen_DataChannel_t* parentChannel = zen_BinaryEntityBuilder_getChannel(
         generator->m_builder, parentChannelIndex);

    /* Retrieve the the AST node for with parameters rule. */
    zen_ASTNode_t* withParameters = context->m_withParameters;
    /* Retrieve the context of the with parameters AST node. */
    zen_WithParametersContext_t* withParametersContext =
        (zen_WithParametersContext_t*)withParameters->m_context;

    int32_t withParameterCount = jtk_ArrayList_getSize(withParametersContext->m_withParameters);

    // int32_t* skipIndexes = jtk_Memory_allocate(int32_t, withParameterCount + 1);

    /* Allocate local variables for storing the resource objects. These local
     * variables can only be accessed by the compiler.
     */
    int32_t resourceBaseIndex = zen_BinaryEntityGenerator_allocateLocalVariables(
        generator, withParameterCount);
    /* Allocate local variables for storing the exception objects thrown by
     * the core sections. These local variables can only be accessed by the
     * compiler.
     */
    // int32_t fc1BaseIndex = zen_BinaryEntityGenerator_allocateLocalVariables(
    //    generator, withParameterCount);
    /* Allocate the local variables for storing the exception objects thrown
     * by the FC1-k sections. These local variables can only be accessed by
     * the compiler.
     */
    // int32_t fc2BaseIndex = zen_BinaryEntityGenerator_allocateLocalVariables(
    //    generator, withParameterCount);

    /* Allocate a local variable for storing the exception objects thrown by
     * the core sections. Unlike the previous algorithm, the new algorithm
     * allocates only one local variable. It can be reused given only one FC1
     * section is active at any given time and the exception objects are
     * propogated by the virtual machine. This local variable can only be
     * accessed by the compiler.
     */
    int32_t fc1ExceptionIndex = zen_BinaryEntityGenerator_allocateLocalVariables(
        generator, 1);
    /* Allocate a local variable for storing the exception objects thrown by
     * the FC1 sections. Unlike the previous algorithm, the new algorithm
     * allocates only one local variable. It can be reused given only one FC2
     * section is active at any given time and the exception objects are
     * propogated by the virtual machine. This local variable can only be
     * accessed by the compiler.
     */
    int32_t fc2ExceptionIndex = zen_BinaryEntityGenerator_allocateLocalVariables(
        generator, 1);

    int32_t withParameterIndex;
    for (withParameterIndex = 0; withParameterIndex < withParameterCount;
        withParameterIndex++) {
        /* Retreive the current with parameter AST node. */
        zen_ASTNode_t* withParameter = (zen_ASTNode_t*)jtk_ArrayList_getValue(
            withParametersContext->m_withParameters, withParameterIndex);

        /* Retrieve the context of the current with parameter. */
        zen_WithParameterContext_t* withParameterContext =
            (zen_WithParameterContext_t*)withParameter->m_context;

        // TODO: Allocate a local variable if an identifier was specified.

        /* Generate the instructions corresponding to the expression specified to
         * the with statement.
         */
        zen_ASTWalker_walk(astListener, withParameterContext->m_expression);

        /* Allocate a local variable only the compiler has access to. */
        uint8_t resourceIndex = resourceBaseIndex + withParameterIndex;

        /* Store the resulting object in the local variable allocated in the previous
         * step.
         *
         * TODO: Allocate a local variable!
         */
        zen_BinaryEntityBuilder_emitStoreReference(generator->m_builder, resourceIndex);
        /* Log the emission of the store_a instruction. */
        printf("[debug] Emitted store_a %d\n", resourceIndex);

        int32_t coreStartIndex = zen_DataChannel_getSize(parentChannel);
        zen_ExceptionHandlerSite_t* type1Handler = jtk_Memory_allocate(
            zen_ExceptionHandlerSite_t, 1);
        type1Handler->m_startIndex = coreStartIndex;
        type1Handler->m_exceptionClassIndex = 0;
        jtk_ArrayList_add(generator->m_exceptionHandlerSites, type1Handler);
    }

    /* Save the index where the instruction for the statement suite clause
     * begins (inclusive).
     */
    int32_t startIndex = zen_DataChannel_getSize(parentChannel);

    /* Generate the instructions corresponding to the statement suite specified to
     * the with statement.
     */
    zen_ASTWalker_walk(astListener, context->m_statementSuite);

    for (withParameterIndex = withParameterIndex - 1; withParameterIndex >= 0;
        withParameterIndex--) {
        int32_t normalizedIndex = withParameterCount - withParameterIndex - 1;
        uint8_t resourceIndex = resourceBaseIndex + normalizedIndex;
        // uint8_t fc1ExceptionIndex = fc1BaseIndex + normalizedIndex;
        // uint8_t fc2ExceptionIndex = fc2BaseIndex + normalizedIndex;

        int32_t coreStopIndex = zen_DataChannel_getSize(parentChannel);
        zen_ExceptionHandlerSite_t* type1Handler = (zen_ExceptionHandlerSite_t*)
            jtk_ArrayList_getValue(generator->m_exceptionHandlerSites, withParameterIndex);
        type1Handler->m_stopIndex = coreStopIndex;

        /* Load the resulting object from the local variable we allocated in the
         * previous loop.
         */
        zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder, resourceIndex);
        /* Log the emission of the load_a instruction. */
        printf("[debug] Emitted load_a %d\n", resourceIndex);

        /* Invoke the Closeable#close() function to close the resource. */
        zen_BinaryEntityBuilder_emitInvokeVirtual(generator->m_builder, closeIndex);
        /* Log the emission of the invoke_virtual instruction. */
        printf("[debug] Emitted invoke_virtual %d\n", closeIndex);

        /* Jump to skip FC1-k and FC2-k sections. */
        zen_BinaryEntityBuilder_emitJump(generator->m_builder, 0);
        /* Log the emission of the jump instruction. */
        printf("[debug] Emitted jump 0 (dummy index)\n");

        /* Save the index of the bytes where the dummy data was written. */
        int32_t skipIndex = zen_DataChannel_getSize(parentChannel) - 2;

        /* -- Finally Clause 1 -- */

        int32_t fc1StartIndex = zen_DataChannel_getSize(parentChannel);
        type1Handler->m_handlerIndex = fc1StartIndex;

        /* The virtual machine pushes the thrown exception to the operand stack.
         * Store this reference in a local variable, say L[k + x] (where x is the
         * base local variable index for private FC1 local variables), which can
         * be accessed only by the compiler.
         */
        zen_BinaryEntityBuilder_emitStoreReference(generator->m_builder,
            fc1ExceptionIndex);
        /* Log the emission of the store_a instruction. */
        printf("[debug] store_a %d\n", fc1ExceptionIndex);

        /* Load the resource object from the local variable Lk. */
        zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder,
            resourceIndex);
        /* Log the emission of the load_a instruction. */
        printf("[debug] load_a %d\n", resourceIndex);

        /* Invoke the Closeable#close() function to close the resource. */
        zen_BinaryEntityBuilder_emitInvokeVirtual(generator->m_builder, closeIndex);
        /* Log the emission of the invoke_virtual instruction. */
        printf("[debug] Emitted invoke_virtual %d\n", closeIndex);

        int32_t fc1StopIndex = zen_DataChannel_getSize(parentChannel);

        /* Load the exception object that was thrown by the core section. */
        zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder,
            fc1ExceptionIndex);
        /* Log the emission of the load_a instruction. */
        printf("[debug] Emitted load_a %d\n", fc1ExceptionIndex);

        /* Throw the exception again. */
        zen_BinaryEntityBuilder_emitThrow(generator->m_builder);
        /* Log the emission of the throw instruction. */
        printf("[debug] Emitted throw\n");

        /* -- Finally Clause 2 -- */

        int32_t fc2StartIndex = zen_DataChannel_getSize(parentChannel);

        /* The virtual machine pushes the thrown exception to the operand stack.
         * Store this reference in a local variable, say L[k + y] (where y is the base
         * local variable index for private FC2 local variables), which can be
         * accessed only by the compiler.
         */
        zen_BinaryEntityBuilder_emitStoreReference(generator->m_builder,
            fc2ExceptionIndex);
        /* Log the emission of the store_a instruction. */
        printf("[debug] store_a %d\n", fc2ExceptionIndex);

        /* Load the exception object that was thrown by the core section. In other
         * words, load the object referenced by L[k + x].
         */
        zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder,
            fc1ExceptionIndex);
        /* Log the emission of the load_a instruction. */
        printf("[debug] Emitted load_a %d\n", fc1ExceptionIndex);

        /* Load the exception object that was thrown by the Closeable#close()
         * function in the FC1 section. In other words, load the object referenced
         * by L[k + y].
         */
        zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder,
            fc2ExceptionIndex);
        /* Log the emission of the load_a instruction. */
        printf("[debug] Emitted load_a %d\n", fc2ExceptionIndex);

        /* Invoke the Throwable#suppress() function to add the exception thrown
         * by the FC1 section to the exception thrown by the core section.
         * In other words, `L[k + x].suppress(L[k + y])`.
         */
        zen_BinaryEntityBuilder_emitInvokeVirtual(generator->m_builder, suppressIndex);
        /* Log the emission of the invoke_virtual instruction. */
        printf("[debug] Emitted invoke_virtual %d\n", suppressIndex);

        /* Load the exception object that was thrown by the core section. In
         * other words, load the object referenced by L[k + x].
         */
        zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder,
            fc1ExceptionIndex);
        /* Log the emission of the load_a instruction. */
        printf("[debug] Emitted load_a %d\n", fc1ExceptionIndex);

        /* Throw the exception again. */
        zen_BinaryEntityBuilder_emitThrow(generator->m_builder);
        /* Log the emission of the throw instruction. */
        printf("[debug] Emitted throw\n");

        int32_t newParentChannelSize = zen_DataChannel_getSize(parentChannel);
        parentChannel->m_bytes[skipIndex] = (newParentChannelSize & 0x0000FF00) >> 8;
        parentChannel->m_bytes[skipIndex + 1] = newParentChannelSize & 0x000000FF;

        zen_ExceptionHandlerSite_t* type2Handler = jtk_Memory_allocate(zen_ExceptionHandlerSite_t, 1);
        type2Handler->m_startIndex = fc1StartIndex;
        type2Handler->m_stopIndex = fc1StopIndex;
        type2Handler->m_handlerIndex = fc2StartIndex;
        type2Handler->m_exceptionClassIndex = 0;
        jtk_ArrayList_add(generator->m_exceptionHandlerSites, type2Handler);

        /* I just had an epiphany! All the "Emitted *" log messages could have been
         * placed in the binary entity builder. This way I would have to repeat the
         * code less often! I am almost done with the code generate and I do not
         * have the energy to refactor everything. :(
         */
    }
}

// classDeclaration

// BUG: Looks like we are not setting the current scope in this function.
void zen_BinaryEntityGenerator_onEnterClassDeclaration(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    /* Retrieve the generator associated with the AST listener. */
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;

    /* Retrieve the current scope from the symbol table. At this point, it is
     * the enclosing scope.
     */
    zen_Scope_t* parentScope = zen_SymbolTable_getCurrentScope(generator->m_symbolTable);

    /* Retrieve the context of the AST node. */
    zen_ClassDeclarationContext_t* context =
        (zen_ClassDeclarationContext_t*)node->m_context;

    /* Retrieve the scope associated with the class being declared. */
    zen_Scope_t* scope = zen_ASTAnnotations_get(generator->m_scopes, node);

    /* Update the current scope in the symbol table. */
    zen_SymbolTable_setCurrentScope(generator->m_symbolTable, scope);

    zen_ASTNode_t* identifier = context->m_identifier;
    zen_Token_t* identifierToken = (zen_Token_t*)identifier->m_context;
    jtk_String_t* reference = NULL;
    if (generator->m_package != NULL) {
        reference = jtk_String_newFromJoinEx(generator->m_package->m_value,
            generator->m_package->m_size, identifierToken->m_text, identifierToken->m_length);
    }
    else {
        reference = jtk_String_newEx(identifierToken->m_text, identifierToken->m_length);
    }

    uint16_t flags = 0;
    uint16_t referenceIndex =
        zen_ConstantPoolBuilder_getUtf8EntryIndex(generator->m_constantPoolBuilder, reference);
    uint16_t* superclassIndexes = NULL;
    uint16_t superclassCount = 0;

    /* At this point, the reference is not required anymore. We have the index into
     * the constant pool which represents it. Therefore, destroy it.
     */
    jtk_String_delete(reference);

    if (context->m_classExtendsClause != NULL) {
        /* Retrieve the extends clause context to extract information about the
         * superclasses.
         */
        zen_ClassExtendsClauseContext_t* extendsClauseContext =
            (zen_ClassExtendsClauseContext_t*)context->m_classExtendsClause->m_context;

        /* Calculate the total number of superclasses. */
        superclassCount = jtk_ArrayList_getSize(extendsClauseContext->m_typeNames);
        /* Allocate an array to store the constant pool indexes to the superclass
         * references.
         */
        superclassIndexes = jtk_Memory_allocate(uint16_t, superclassCount);

        /* The entity generator does not ensure the validity of the identifiers.
         * It is the responsibility of the resolution phase.
         */
        int32_t index;
        for (index = 0; index < superclassCount; index++) {
            zen_ASTNode_t* typeNameNode = (zen_ASTNode_t*)jtk_ArrayList_getValue(extendsClauseContext->m_typeNames, index);
            zen_TypeNameContext_t* typeNameContext = (zen_TypeNameContext_t*)typeNameNode->m_context;

            // TODO: Prepare a qualified name from the type name context.
            jtk_String_t* qualifiedName = NULL;

            /* Retrieve the symbol for the current superclass. Do not begin the resolution
             * from the current scope, which is this class. In the future, if Zen allows
             * inner classes, this would allow inner classes to be inherited by their enclosing
             * classes!
             */
            zen_Symbol_t* symbol = zen_Scope_resolveQualifiedSymbol(
                parentScope, qualifiedName);
            if (zen_Symbol_isClass(symbol)) {
                zen_ClassSymbol_t* classSymbol = (zen_ClassSymbol_t*)symbol->m_context;
                jtk_String_t* fullyQualifiedName = zen_ClassSymbol_getQualifiedName(classSymbol);

                uint16_t superclassIndex = zen_ConstantPoolBuilder_getUtf8EntryIndex(
                    generator->m_constantPoolBuilder, fullyQualifiedName);
                superclassIndexes[index] = superclassIndex;
            }
            else {
                printf("[error] Looks like the semantic anlysis failed.\n");
            }
        }
    }
    else {
        /* The extends clause has not been explicitly written. Therefore,
         * the compiler generates the default extends clause which inherits
         * the zen.core.Object class.
         */
        jtk_String_t* objectClassName = jtk_String_newEx("zen.core.Object", 15);

        superclassCount = 1;
        superclassIndexes = jtk_Memory_allocate(uint16_t, 1);
        superclassIndexes[0] = zen_ConstantPoolBuilder_getUtf8EntryIndex(
            generator->m_constantPoolBuilder, objectClassName);

        jtk_String_delete(objectClassName);
    }

    zen_Entity_t* entity = &generator->m_entityFile->m_entity;
    entity->m_type = ZEN_ENTITY_TYPE_CLASS;
    entity->m_flags = 0;
    entity->m_reference = referenceIndex;
    entity->m_superclassCount = superclassCount;
    entity->m_superclasses = superclassIndexes;
    entity->m_attributeTable.m_size = 0;
    entity->m_fieldCount = 0;
    entity->m_fields = NULL;
    entity->m_functionCount = 0;
    entity->m_functions = NULL;
}

void zen_BinaryEntityGenerator_onExitClassDeclaration(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    /* Retrieve the generator associated with the AST listener. */
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;

    /* Retrieve the context of the AST node. */
    zen_ClassDeclarationContext_t* context =
        (zen_ClassDeclarationContext_t*)node->m_context;

    /* Invalidate the current scope in the symbol table. */
    zen_SymbolTable_invalidateCurrentScope(generator->m_symbolTable);

    /* Write the number of fields in this class. *
    zen_BinaryEntityBuilder_writeFieldCount(generator->m_builder, generator->m_fieldCount);

    /* Merge the class and field channels. Further, the data in the field channel
     * is not required anymore. Therefore, clear it.
     *
     * It should be noted that the commit function both merges and clears the
     * source channel.
     *
    zen_ChannelManager_commit(generator->m_channels, generator->m_fieldChannel,
        generator->m_classChannel);

    /* At this point, the class channel should contain the class entity header
     * and the fields. Further, the function channel should contain all the
     * function entities. Therefore, we need to merge the class and function
     * channels together.
     *
     * The channels are separated by function count. Therefore, activate the
     * class channel and write the function count before merging.
     *
    zen_BinaryEntityBuilder_writeFunctionCount(generator->m_builder, generator->m_functionCount);

    /* Merge the class and function channels. Further, the data in the function
     * channel is not required anymore. Therefore, clear it.
     *
     * It should be noted that the commit function both merges and clears the
     * source channel.
     *
    zen_ChannelManager_commit(generator->m_channels, generator->m_functionChannel,
        generator->m_classChannel);

    zen_SymbolTable_invalidateCurrentScope(generator->m_symbolTable);
    */
}

// classExtends

void zen_BinaryEntityGenerator_onEnterClassExtendsClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitClassExtendsClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// classSuite

void zen_BinaryEntityGenerator_onEnterClassSuite(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitClassSuite(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// classMember

void zen_BinaryEntityGenerator_onEnterClassMember(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitClassMember(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// void zen_BinaryEntityGenerator_onEnterConstructorDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
// }

// void zen_BinaryEntityGenerator_onExitConstructorDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
// }


// enumerationDeclaration

void zen_BinaryEntityGenerator_onEnterEnumerationDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitEnumerationDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// enumerationBaseClass

void zen_BinaryEntityGenerator_onEnterEnumrationBaseClass(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitEnumerationBaseClause(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// enumerationSuite

void zen_BinaryEntityGenerator_onEnterEnumerationSuite(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitEnumerationSuite(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// enumerate

void zen_BinaryEntityGenerator_onEnterEnumerate(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitEnumerate(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// expressions

void zen_BinaryEntityGenerator_onEnterExpressions(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitExpressions(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// expression

void zen_BinaryEntityGenerator_onEnterExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    /*zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_ExpressionContext_t* context = (zen_AssignmentExpressionContext_t*)node->m_context;

    zen_ASTAnnotation_t* annotation = zen_ASTAnnotations_get(generator->m_annotations, node);
    if (annotation != NULL) {
        if (zen_ASTAnnotation_getType(annotation) == ZEN_AST_ANNOTATION_TYPE_ASYMETRICAL_CHANNEL_MANAGEMENT) {
            zen_BinaryEntityBuilder_addChannel(generator->m_builder);
        }
    }*/
}

void zen_BinaryEntityGenerator_onExitExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// assignmentExpression

bool lhs = false;

// How to differentiate between function calls

/* The left hand side of an assignment may take the following forms:
 * - local variable
 * - local class member variable
 * - local class member using 'this' reference
 * - super class member
 * - super class member using 'super' reference
 * - instance member variable (where, the instance is an expression)
 * - static member variable
 *
 *    ...
 *    invoke_virtual #function_descriptor_index
 *    store_a #x
 *
 * NOTE: The binary entity generator assumes that the left values were verified
 * to be valid in the previous phases of the compiler.
 */
void zen_BinaryEntityGenerator_onEnterAssignmentExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_AssignmentExpressionContext_t* context = (zen_AssignmentExpressionContext_t*)node->m_context;

    /* I have no idea, but the following statement just fixed a bug magically.
     * However, it has something to do with which type of code is generated,
     * the LHS or RHS, for postfix expressions.
     */
    lhs = false;
    zen_ASTNode_t* assignmentOperator = context->m_assignmentOperator;
    if (assignmentOperator != NULL) {
        lhs = false;
        zen_ASTWalker_walk(astListener, context->m_assignmentExpression);
        lhs = true;
        zen_ASTWalker_walk(astListener, context->m_conditionalExpression);

        zen_ASTListener_skipChildren(astListener);
    }

    // zen_Symbol_t* symbol = zen_SymbolTable_resolve(generator->m_symbolTable, identifierText);
    // if (zen_Symbol_isVariable(symbol)) {
        // int32_t targetIndex;
        // bool localVariable = false;

        // zen_Scope_t* enclosingScope = zen_Symbol_getEnclosingScope(symbol);
        // if (zen_Scope_isClassScope(enclosingScope)) {
           // targetIndex = ;// Retreive the field descriptor index.
//         }
   //      else {
            // targetIndex = ; // Retrieve the index into the local variable array.
      //   }

        // zen_ASTNode_t* assignmentOperator = context->m_assignmentOperator;
        // zen_Token_t* operatorToken = (zen_Token_t*)assignmentOperator->m_context;
        // if (operatorToken->m_type != ZEN_TOKEN_EQUAL) {
           //  zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder, targetIndex);

            /* The values of symbol and symbolSize are the only arbitrary variables
             * when invoking the zen_BinaryEntityGenerator_invokeEvaluate() function.
             * Therefore, instead of rewriting the invocation expression multiple
             * times, I have factored it out.
             */
            // uint8_t* symbol = NULL;
            // int32_t symbolSize = 1;
            // switch (operatorToken->m_type) {
               //  case ZEN_AMPERSAND_EQUAL: {
                    /* The kernel should find a function annotated with the Operator
                     * annotation that handles the '&' symbol.
                     */
                  //   symbol = "&";
                    // break;
                // }

                // case ZEN_ASTERISK_EQUAL: {
                    /* The kernel should find a function annotated with the Operator
                     * annotation that handles the '*' symbol.
                     */
                   //  symbol = "*"
                    // break;
                // }

                // case ZEN_TOKEN_PLUS_EQUAL: {
                    /* The kernel should find a function annotated with the Operator
                     * annotation that handles the '+' symbol.
                     */
                   //  symbol = "+";
                    // break;
                // }

                // case ZEN_TOKEN_DASH_EQUAL: {
                    /* The kernel should find a function annotated with the Operator
                     * annotation that handles the '-' symbol.
                     */
                   //  symbol = "-";
                    // break;
                // }

                // case ZEN_FORWARD_SLASH_EQUAL: {
                    /* The kernel should find a function annotated with the Operator
                     * annotation that handles the '/' symbol.
                     */
                   //  symbol = "/";
                    // break;
                // }

                // case ZEN_LEFT_ANGLE_BRACKET_2_EQUAL: {
                    /* The kernel should find a function annotated with the Operator
                     * annotation that handles the '<<' symbol.
                     */
                   //  symbol = "<<";
                    // break;
                // }

                // case ZEN_RIGHT_ANGLE_BRACKET_3_EQUAL: {
                    /* The kernel should find a function annotated with the Operator
                     * annotation that handles the '>>>' symbol.
                     */
                    // symbol = ">>>";
                    // break;
                // }

                // case ZEN_RIGHT_ANGLE_BRACKET_2_EQUAL: {
                    /* The kernel should find a function annotated with the Operator
                     * annotation that handles the '>>' symbol.
                     */
                   //  symbol = ">>";
                    // break;
                // }

                // case VERTICAL_BAR_EQUAL: {
                    /* The kernel should find a function annotated with the Operator
                     * annotation that handles the '|' symbol.
                     */
                    // symbol = "|";
                    // break;
                // }

                // case CARET_EQUAL : {
                    /* The kernel should find a function annotated with the Operator
                     * annotation that handles the '^' symbol.
                     */
                    // symbol = "^";
                // }

                // default: {
                    /* The generator should not reach this code! */
                    // printf("[internal error] Control should not reach here.\n");
                // }
            // }

            /* Generate the instructions corresponding to invoking the
             * ZenKernel.evaluate() function. Since, Zen is dynamically typed
             * the compiler cannot determine the type of the operands. Therefore,
             * the addition and subtraction operations are delegated to
             * functions annotated with the Operator annotation.
             */
            // zen_BinaryEntityGenerator_invokeEvaluate(generator, symbol, symbolSize);
        // }

        // zen_BinaryEntityGenerator_emitStoreReference(generator->m_builder, targetIndex);
    // }
    // else if (zen_Symbol_isConstant(symbol)) {
        // error: Constant cannot be assigned
    // }
    // else {
        // error: LHS is not variable
    // }

    // jtk_ArrayList_delete(children);
}

void zen_BinaryEntityGenerator_onExitAssignmentExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {/*
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_AssignmentExpressionContext_t* context = (zen_AssignmentExpressionContext_t*)node->m_context;

    if (context->m_assignmentOperator != NULL) {
        zen_ASTNode_t* assignmentOperator = context->m_assignmentOperator;
        zen_Token_t* operatorToken = (zen_Token_t*)assignmentOperator->m_context;
        switch (operatorToken->m_type) {
            /* The equal operator only stores a reference. Therefore, it requires no
             * special implementation.
             *
            /*
            case ZEN_TOKEN_EQUAL: {
                break;
            }
            *

            case ZEN_TOKEN_ASTERISK_EQUAL: {
                break;
            }

            case ZEN_TOKEN_FORWARD_SLASH_EQUAL: {
                break;
            }

            case ZEN_TOKEN_MODULUS_EQUAL: {
                break;
            }

            case ZEN_TOKEN_PLUS_EQUAL: {
                break;
            }

            case ZEN_TOKEN_DASH_EQUAL: {
                break;
            }

            case ZEN_TOKEN_LEFT_ANGLE_BRACKET_2_EQUAL: {
                break;
            }

            case ZEN_TOKEN_RIGHT_ANGLE_BRACKET_2_EQUAL: {
                break;
            }

            case ZEN_TOKEN_RIGHT_ANGLE_BRACKET_3_EQUAL: {
                break;
            }

            case ZEN_TOKEN_AMPERSAND_EQUAL: {
                break;
            }

            case ZEN_TOKEN_CARET_EQUAL: {
                break;
            }

            case ZEN_TOKEN_VERTICAL_BAR_EQUAL: {
                break;
            }

            default: {
                // [internal error]
            }
        }
        // Depending on the first push reference
        zen_BinaryEntityGenerator_emitStoreReference(generator, 10);
    }*/
}

// conditionalExpression

void zen_BinaryEntityGenerator_onEnterConditionalExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_ConditionalExpressionContext_t* context = (zen_ConditionalExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityGenerator_onExitConditionalExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_ConditionalExpressionContext_t* context = (zen_ConditionalExpressionContext_t*)node->m_context;

    /* TODO */
}

// logicalOrExpression

void zen_BinaryEntityGenerator_onEnterLogicalOrExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_LogicalOrExpressionContext_t* context = (zen_LogicalOrExpressionContext_t*)node->m_context;

    /* Generates the instructions corresponding to the very first child of
     * the node.
     */
    zen_ASTListener_visitFirstChild(astListener);
}

void zen_BinaryEntityGenerator_onExitLogicalOrExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_LogicalOrExpressionContext_t* context = (zen_LogicalOrExpressionContext_t*)node->m_context;

    int32_t size = jtk_ArrayList_getSize(context->m_logicalAndExpressions);
    int32_t i;
    for (i = 0; i < size; i++) {
        zen_ASTNode_t* logicalAndExpressions = jtk_ArrayList_getValue(context->m_logicalAndExpressions, i);

        /* At this point, the instructions corresponding to the left operand
         * should be generated. The generation of the instructions for
         * logical OR expressions follow the order: operand1 operand2 operator.
         * In other words, the compiler generates instructions for logical OR
         * expressions in postfix order. Therefore, generate the instructions for
         * the right operand and invoking the ZenKernel.evaluate(...) function,
         * which takes care of *aggregating* the result.
         */
        zen_ASTWalker_walk(astListener, logicalAndExpressions);

        /* Generate the instructions corresponding to invoking the
         * ZenKernel.evaluate() function. Since, Zen is dynamically typed
         * the compiler cannot determine the type of the operands. Therefore,
         * logical OR operation is delegated to functions annotated with the
         * Operator annotation.
         */
        zen_BinaryEntityGenerator_invokeEvaluate(generator, "||", 2);
    }
}

// logicalAndExpression

void zen_BinaryEntityGenerator_onEnterLogicalAndExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_LogicalAndExpressionContext_t* context = (zen_LogicalAndExpressionContext_t*)node->m_context;

    /* Generates the instructions corresponding to the very first child of
     * the node.
     */
    zen_ASTListener_visitFirstChild(astListener);
}

void zen_BinaryEntityGenerator_onExitLogicalAndExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_LogicalAndExpressionContext_t* context = (zen_LogicalAndExpressionContext_t*)node->m_context;

    int32_t size = jtk_ArrayList_getSize(context->m_inclusiveOrExpressions);
    int32_t i;
    for (i = 0; i < size; i++) {
        zen_ASTNode_t* inclusiveOrExpression = jtk_ArrayList_getValue(context->m_inclusiveOrExpressions, i);

        /* At this point, the instructions corresponding to the left operand
         * should be generated. The generation of the instructions for
         * logical AND expressions follow the order: operand1 operand2 operator.
         * In other words, the compiler generates instructions for logical AND
         * expressions in postfix order. Therefore, generate the instructions for
         * the right operand and invoking the ZenKernel.evaluate(...) function,
         * which takes care of *aggregating* the result.
         */
        zen_ASTWalker_walk(astListener, inclusiveOrExpression);

        /* Generate the instructions corresponding to invoking the
         * ZenKernel.evaluate() function. Since, Zen is dynamically typed
         * the compiler cannot determine the type of the operands. Therefore,
         * logical AND operation is delegated to functions annotated with the
         * Operator annotation.
         */
        zen_BinaryEntityGenerator_invokeEvaluate(generator, "&&", 2);
    }
}

// inclusiveOrExpression

void zen_BinaryEntityGenerator_onEnterInclusiveOrExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_InclusiveOrExpressionContext_t* context = (zen_InclusiveOrExpressionContext_t*)node->m_context;

    /* Generates the instructions corresponding to the very first child of
     * the node.
     */
    zen_ASTListener_visitFirstChild(astListener);
}

void zen_BinaryEntityGenerator_onExitInclusiveOrExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_InclusiveOrExpressionContext_t* context = (zen_InclusiveOrExpressionContext_t*)node->m_context;

    int32_t size = jtk_ArrayList_getSize(context->m_exclusiveOrExpressions);
    int32_t i;
    for (i = 0; i < size; i++) {
        zen_ASTNode_t* exclusiveOrExpression = jtk_ArrayList_getValue(context->m_exclusiveOrExpressions, i);

        /* At this point, the instructions corresponding to the left operand
         * should be generated. The generation of the instructions for
         * inclusive or expressions follow the order: operand1 operand2 operator.
         * In other words, the compiler generates instructions for inclusive or
         * expressions in postfix order. Therefore, generate the instructions for
         * the right operand and invoking the ZenKernel.evaluate(...) function,
         * which takes care of *aggregating* the result.
         */
        zen_ASTWalker_walk(astListener, exclusiveOrExpression);

        /* Generate the instructions corresponding to invoking the
         * ZenKernel.evaluate() function. Since, Zen is dynamically typed
         * the compiler cannot determine the type of the operands. Therefore,
         * bitwise and operation is delegated to functions annotated with the
         * Operator annotation.
         */
        zen_BinaryEntityGenerator_invokeEvaluate(generator, "|", 1);
    }
}

// exclusiveOrExpression

void zen_BinaryEntityGenerator_onEnterExclusiveOrExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_ExclusiveOrExpressionContext_t* context = (zen_ExclusiveOrExpressionContext_t*)node->m_context;

    /* Generates the instructions corresponding to the very first child of
     * the node.
     */
    zen_ASTListener_visitFirstChild(astListener);
}

void zen_BinaryEntityGenerator_onExitExclusiveOrExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_ExclusiveOrExpressionContext_t* context = (zen_ExclusiveOrExpressionContext_t*)node->m_context;

    int32_t size = jtk_ArrayList_getSize(context->m_andExpressions);
    int32_t i;
    for (i = 0; i < size; i++) {
        zen_ASTNode_t* andExpression = jtk_ArrayList_getValue(context->m_andExpressions, i);

        /* At this point, the instructions corresponding to the left operand
         * should be generated. The generation of the instructions for
         * and expressions follow the order: operand1 operand2 operator.
         * In other words, the compiler generates instructions for and
         * expressions in postfix order. Therefore, generate the instructions for
         * the right operand and invoking the ZenKernel.evaluate(...) function,
         * which takes care of *aggregating* the result.
         */
        zen_ASTWalker_walk(astListener, andExpression);

        /* Generate the instructions corresponding to invoking the
         * ZenKernel.evaluate() function. Since, Zen is dynamically typed
         * the compiler cannot determine the type of the operands. Therefore,
         * bitwise and operation is delegated to functions annotated with the
         * Operator annotation.
         */
        zen_BinaryEntityGenerator_invokeEvaluate(generator, "^", 1);
    }
}

// andExpression

void zen_BinaryEntityGenerator_onEnterAndExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_AndExpressionContext_t* context = (zen_AndExpressionContext_t*)node->m_context;

    /* Generates the instructions corresponding to the very first child of
     * the node.
     */
    zen_ASTListener_visitFirstChild(astListener);
}

void zen_BinaryEntityGenerator_onExitAndExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_AndExpressionContext_t* context = (zen_AndExpressionContext_t*)node->m_context;

    int32_t size = jtk_ArrayList_getSize(context->m_equalityExpressions);
    int32_t i;
    for (i = 0; i < size; i++) {
        zen_ASTNode_t* equalityExpression = jtk_ArrayList_getValue(context->m_equalityExpressions, i);

        /* At this point, the instructions corresponding to the left operand
         * should be generated. The generation of the instructions for
         * shift expressions follow the order: operand1 operand2 operator.
         * In other words, the compiler generates instructions for shift
         * expressions in postfix order. Therefore, generate the instructions for
         * the right operand and invoking the ZenKernel.evaluate(...) function,
         * which takes care of *aggregating* the result.
         */
        zen_ASTWalker_walk(astListener, equalityExpression);

        /* Generate the instructions corresponding to invoking the
         * ZenKernel.evaluate() function. Since, Zen is dynamically typed
         * the compiler cannot determine the type of the operands. Therefore,
         * bitwise and operation is delegated to functions annotated with the
         * Operator annotation.
         */
        zen_BinaryEntityGenerator_invokeEvaluate(generator, "&", 1);
    }
}

// equalityExpression

void zen_BinaryEntityGenerator_onEnterEqualityExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_EqualityExpressionContext_t* context = (zen_EqualityExpressionContext_t*)node->m_context;

    /* Generates the instructions corresponding to the very first child of
     * the node.
     */
    zen_ASTListener_visitFirstChild(astListener);
}

void zen_BinaryEntityGenerator_onExitEqualityExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_EqualityExpressionContext_t* context = (zen_EqualityExpressionContext_t*)node->m_context;

    int32_t size = jtk_ArrayList_getSize(context->m_relationalExpressions);
    int32_t i;
    for (i = 0; i < size; i++) {
        jtk_Pair_t* pair = (jtk_Pair_t*)jtk_ArrayList_getValue(context->m_relationalExpressions, i);

        /* Retrieve the equality operator. */
        zen_ASTNode_t* equalityOperator = pair->m_left;
        /* At this point, the instructions corresponding to the left operand
         * should be generated. The generation of the instructions for
         * equality expressions follow the order: operand1 operand2 operator.
         * In other words, the compiler generates instructions for equality
         * expressions in postfix order. Therefore, generate the instructions for
         * the right operand and invoking the ZenKernel.evaluate(...) function,
         * which takes care of *aggregating* the result.
         */
        zen_ASTWalker_walk(astListener, (zen_ASTNode_t*)pair->m_right);

        /* Retrieve the corresponding equality operator token from the AST
         * node.
         */
        zen_Token_t* equalityOperatorToken = (zen_Token_t*)equalityOperator->m_context;
        /* Retrieve the type of the equality operator. */
        zen_TokenType_t equalityOperatorTokenType = zen_Token_getType(equalityOperatorToken);

        /* The values of symbol and symbolSize are the only arbitrary variables
         * when invoking the zen_BinaryEntityGenerator_invokeEvaluate() function.
         * Therefore, instead of rewriting the invocation expression multiple
         * times, I have factored it out.
         */
        uint8_t* symbol = NULL;
        int32_t symbolSize = 2;

        switch (equalityOperatorTokenType) {
            case ZEN_TOKEN_EQUAL_2: {
                /* The kernel should find a function annotated with the Operator
                 * annotation that handles the '==' symbol.
                 */
                symbol = "==";

                break;
            }

            case ZEN_TOKEN_EXCLAMATION_MARK_EQUAL: {
                /* The kernel should find a function annotated with the Operator
                 * annotation that handles the '!=' symbol.
                 */
                symbol = "!=";

                break;
            }

            default: {
                /* The generator should not reach this code! */
                printf("[error] Control should not reach here.\n");
            }
        }

        /* Generate the instructions corresponding to invoking the
         * ZenKernel.evaluate() function. Since, Zen is dynamically typed
         * the compiler cannot determine the type of the operands. Therefore,
         * the addition and subtraction operations are delegated to
         * functions annotated with the Operator annotation.
         */
        zen_BinaryEntityGenerator_invokeEvaluate(generator, symbol, symbolSize);
    }
}

// relationalExpression

void zen_BinaryEntityGenerator_onEnterRelationalExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_RelationalExpressionContext_t* context = (zen_RelationalExpressionContext_t*)node->m_context;

    /* Generates the instructions corresponding to the very first child of
     * the node.
     */
    zen_ASTListener_visitFirstChild(astListener);
}

void zen_BinaryEntityGenerator_onExitRelationalExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_RelationalExpressionContext_t* context = (zen_RelationalExpressionContext_t*)node->m_context;

    /* NOTE: Relational operators have no associativity. In order to implement
     * this behavior, the parser first recognizes relational operators as if
     * they have left/right associativity. After which, it manually checks
     * for the number of operands on the right hand side of the very first
     * subexpression.
     *
     * At this point, we assume size of the shift expressions list is 1 or 0.
     */
    int32_t size = jtk_ArrayList_getSize(context->m_shiftExpressions);
    int32_t i;
    for (i = 0; i < size; i++) {
        jtk_Pair_t* pair = (jtk_Pair_t*)jtk_ArrayList_getValue(context->m_shiftExpressions, i);

        /* Retrieve the relational operator. */
        zen_ASTNode_t* relationalOperator = pair->m_left;
        /* At this point, the instructions corresponding to the left operand
         * should be generated. The generation of the instructions for
         * relational expressions follow the order: operand1 operand2 operator.
         * In other words, the compiler generates instructions for relational
         * expressions in postfix order. Therefore, generate the instructions for
         * the right operand and invoking the ZenKernel.evaluate(...) function,
         * which takes care of *aggregating* the result.
         */
        zen_ASTWalker_walk(astListener, (zen_ASTNode_t*)pair->m_right);

        /* Retrieve the corresponding relational operator token from the AST
         * node.
         */
        zen_Token_t* relationalOperatorToken = (zen_Token_t*)relationalOperator->m_context;
        /* Retrieve the type of the relational operator. */
        zen_TokenType_t relationalOperatorTokenType = zen_Token_getType(relationalOperatorToken);

        if (relationalOperatorTokenType == ZEN_TOKEN_KEYWORD_IS) {

        }
        else {
            /* The values of symbol and symbolSize are the only arbitrary variables
             * when invoking the zen_BinaryEntityGenerator_invokeEvaluate() function.
             * Therefore, instead of rewriting the invocation expression multiple
             * times, I have factored it out.
             */
            uint8_t* symbol = NULL;
            int32_t symbolSize = 1;

            switch (relationalOperatorTokenType) {
                case ZEN_TOKEN_LEFT_ANGLE_BRACKET: {
                    /* The kernel should find a function annotated with the Operator
                     * annotation that handles the '+' symbol.
                     */
                    symbol = "<";

                    break;
                }

                case ZEN_TOKEN_RIGHT_ANGLE_BRACKET: {
                    /* The kernel should find a function annotated with the Operator
                     * annotation that handles the '>' symbol.
                     */
                    symbol = ">";

                    break;
                }

                case ZEN_TOKEN_LEFT_ANGLE_BRACKET_EQUAL: {
                    /* The kernel should find a function annotated with the Operator
                     * annotation that handles the '<=' symbol.
                     */
                    symbol = "<=";
                    /* All the other symbols are of length 1, except for the '<='
                     * and '>=' symbols. Therefore, update the symbol size.
                     */
                    symbolSize = 2;

                    break;
                }

                case ZEN_TOKEN_RIGHT_ANGLE_BRACKET_EQUAL: {
                    /* The kernel should find a function annotated with the Operator
                     * annotation that handles the '>=' symbol.
                     */
                    symbol = ">=";
                    /* All the other symbols are of length 1, except for the '<='
                     * and '>=' symbols. Therefore, update the symbol size.
                     */
                    symbolSize = 2;

                    break;
                }

                default: {
                    /* The generator should not reach this code! */
                    printf("[error] Control should not reach here.\n");
                }
            }

            /* Generate the instructions corresponding to invoking the
             * ZenKernel.evaluate() function. Since, Zen is dynamically typed
             * the compiler cannot determine the type of the operands. Therefore,
             * the left shift, right shift, and extended right shift operations are
             * delegated to functions annotated with the Operator annotation.
             */
            zen_BinaryEntityGenerator_invokeEvaluate(generator, symbol, symbolSize);
        }
    }
}

// shiftExpression

void zen_BinaryEntityGenerator_onEnterShiftExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_ShiftExpressionContext_t* context = (zen_ShiftExpressionContext_t*)node->m_context;

    /* Generates the instructions corresponding to the very first child of
     * the node.
     */
    zen_ASTListener_visitFirstChild(astListener);
}

void zen_BinaryEntityGenerator_onExitShiftExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_ShiftExpressionContext_t* context = (zen_ShiftExpressionContext_t*)node->m_context;

    int32_t size = jtk_ArrayList_getSize(context->m_additiveExpressions);
    int32_t i;
    for (i = 0; i < size; i++) {
        jtk_Pair_t* pair = (jtk_Pair_t*)jtk_ArrayList_getValue(context->m_additiveExpressions, i);

        /* Retrieve the shift operator. */
        zen_ASTNode_t* shiftOperator = pair->m_left;
        /* At this point, the instructions corresponding to the left operand
         * should be generated. The generation of the instructions for
         * shift expressions follow the order: operand1 operand2 operator.
         * In other words, the compiler generates instructions for shift
         * expressions in postfix order. Therefore, generate the instructions for
         * the right operand and invoking the ZenKernel.evaluate(...) function,
         * which takes care of *aggregating* the result.
         */
        zen_ASTWalker_walk(astListener, (zen_ASTNode_t*)pair->m_right);

        /* Retrieve the corresponding shift operator token from the AST
         * node.
         */
        zen_Token_t* shiftOperatorToken = (zen_Token_t*)shiftOperator->m_context;
        /* Retrieve the type of the shift operator. */
        zen_TokenType_t shiftOperatorTokenType = zen_Token_getType(shiftOperatorToken);

        /* The values of symbol and symbolSize are the only arbitrary variables
         * when invoking the zen_BinaryEntityGenerator_invokeEvaluate() function.
         * Therefore, instead of rewriting the invocation expression multiple
         * times, I have factored it out.
         */
        uint8_t* symbol = NULL;
        int32_t symbolSize = 2;

        switch (shiftOperatorTokenType) {
            case ZEN_TOKEN_LEFT_ANGLE_BRACKET_2: {
                /* The kernel should find a function annotated with the Operator
                 * annotation that handles the '+' symbol.
                 */
                symbol = ">>";

                break;
            }

            case ZEN_TOKEN_RIGHT_ANGLE_BRACKET_2: {
                /* The kernel should find a function annotated with the Operator
                 * annotation that handles the '-' symbol.
                 */
                symbol = "<<";

                break;
            }

            case ZEN_TOKEN_RIGHT_ANGLE_BRACKET_3: {
                /* The kernel should find a function annotated with the Operator
                 * annotation that handles the '-' symbol.
                 */
                symbol = "<<<";
                /* All the other symbols are of length 2, except for the '<<<' symbol.
                 * Therefore, update the symbol size.
                 */
                symbolSize = 3;

                break;
            }

            default: {
                /* The generator should not reach this code! */
                printf("[error] Control should not reach here.\n");
            }
        }

        /* Generate the instructions corresponding to invoking the
         * ZenKernel.evaluate() function. Since, Zen is dynamically typed
         * the compiler cannot determine the type of the operands. Therefore,
         * the left shift, right shift, and extended right shift operations are
         * delegated to functions annotated with the Operator annotation.
         */
        zen_BinaryEntityGenerator_invokeEvaluate(generator, symbol, symbolSize);
    }
}

// additiveExpression

void zen_BinaryEntityGenerator_onEnterAdditiveExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_AdditiveExpressionContext_t* context = (zen_AdditiveExpressionContext_t*)node->m_context;

    /* Generates the instructions corresponding to the very first child of
     * the node.
     */
    zen_ASTListener_visitFirstChild(astListener);
}

void zen_BinaryEntityGenerator_onExitAdditiveExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_AdditiveExpressionContext_t* context = (zen_AdditiveExpressionContext_t*)node->m_context;

    int32_t size = jtk_ArrayList_getSize(context->m_multiplicativeExpressions);
    int32_t i;
    for (i = 0; i < size; i++) {
        jtk_Pair_t* pair = (jtk_Pair_t*)jtk_ArrayList_getValue(context->m_multiplicativeExpressions, i);

        /* Retrieve the additive operator. */
        zen_ASTNode_t* additiveOperator = pair->m_left;
        /* At this point, the instructions corresponding to the left operand
         * should be generated. The generation of the instructions for
         * additive expressions follow the order: operand1 operand2 operator.
         * In other words, the compiler generates instructions for additive
         * expressions in postfix order. Therefore, generate the instructions for
         * the right operand and invoking the ZenKernel.evaluate(...) function,
         * which takes care of *aggregating* the result.
         */
        zen_ASTWalker_walk(astListener, (zen_ASTNode_t*)pair->m_right);

        /* Retrieve the corresponding additive operator token from the AST
         * node.
         */
        zen_Token_t* additiveOperatorToken = (zen_Token_t*)additiveOperator->m_context;
        /* Retrieve the type of the additive operator. */
        zen_TokenType_t additiveOperatorTokenType = zen_Token_getType(additiveOperatorToken);

        /* The values of symbol and symbolSize are the only arbitrary variables
         * when invoking the zen_BinaryEntityGenerator_invokeEvaluate() function.
         * Therefore, instead of rewriting the invocation expression multiple
         * times, I have factored it out.
         */
        uint8_t* symbol = NULL;
        int32_t symbolSize = 1;

        switch (additiveOperatorTokenType) {
            case ZEN_TOKEN_PLUS: {
                /* The kernel should find a function annotated with the Operator
                 * annotation that handles the '+' symbol.
                 */
                symbol = "+";

                break;
            }

            case ZEN_TOKEN_DASH: {
                /* The kernel should find a function annotated with the Operator
                 * annotation that handles the '-' symbol.
                 */
                symbol = "-";

                break;
            }

            default: {
                /* The generator should not reach this code! */
                printf("[internal error] Control should not reach here.\n");
            }
        }

        /* Generate the instructions corresponding to invoking the
         * ZenKernel.evaluate() function. Since, Zen is dynamically typed
         * the compiler cannot determine the type of the operands. Therefore,
         * the addition and subtraction operations are delegated to
         * functions annotated with the Operator annotation.
         */
        zen_BinaryEntityGenerator_invokeEvaluate(generator, symbol, symbolSize);
    }
}

// multiplicativeExpression

void zen_BinaryEntityGenerator_onEnterMultiplicativeExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_MultiplicativeExpressionContext_t* context = (zen_MultiplicativeExpressionContext_t*)node->m_context;

    /* Generates the instructions corresponding to the very first child of
     * the node.
     */
    zen_ASTListener_visitFirstChild(astListener);
}

void zen_BinaryEntityGenerator_onExitMultiplicativeExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_MultiplicativeExpressionContext_t* context = (zen_MultiplicativeExpressionContext_t*)node->m_context;

    int32_t size = jtk_ArrayList_getSize(context->m_unaryExpressions);
    int32_t i;
    for (i = 0; i < size; i++) {
        jtk_Pair_t* pair = (jtk_Pair_t*)jtk_ArrayList_getValue(context->m_unaryExpressions, i);

        /* Retrieve the multiplicative operator. */
        zen_ASTNode_t* multiplicativeOperator = pair->m_left;
        /* At this point, the instructions corresponding to the left operand
         * should be generated. The generation of the instructions for
         * multiplicative expressions follow the order: operand1 operand2 operator.
         * In other words, the compiler generates instructions for multiplicative
         * expressions in postfix order. Therefore, generate the instructions for
         * the right operand and invoking the ZenKernel.evaluate(...) function,
         * which takes care of *aggregating* the result.
         */
        zen_ASTWalker_walk(astListener, (zen_ASTNode_t*)pair->m_right);

        /* Retrieve the corresponding multiplicative operator token from the AST
         * node.
         */
        zen_Token_t* multiplicativeOperatorToken = (zen_Token_t*)multiplicativeOperator->m_context;
        /* Retrieve the type of the multiplicative operator. */
        zen_TokenType_t multiplicativeOperatorTokenType = zen_Token_getType(multiplicativeOperatorToken);

        /* The values of symbol and symbolSize are the only arbitrary variables
         * when invoking the zen_BinaryEntityGenerator_invokeEvaluate() function.
         * Therefore, instead of rewriting the invocation expression multiple
         * times, I have factored it out.
         */
        uint8_t* symbol = NULL;
        int32_t symbolSize = 1;

        switch (multiplicativeOperatorTokenType) {
            case ZEN_TOKEN_ASTERISK: {
                /* The kernel should find a function annotated with the Operator
                 * annotation that handles the '*' symbol.
                 */
                symbol = "*";

                break;
            }

            case ZEN_TOKEN_FORWARD_SLASH: {
                /* The kernel should find a function annotated with the Operator
                 * annotation that handles the '/' symbol.
                 */
                symbol = "/";

                break;
            }

            case ZEN_TOKEN_MODULUS: {
                /* The kernel should find a function annotated with the Operator
                 * annotation that handles the '%' symbol.
                 */
                symbol = "%";

                break;
            }

            default: {
                /* The generator should not reach this code! */
                printf("[error] Control should not reach here.\n");
            }
        }

        /* Generate the instructions corresponding to invoking the
         * ZenKernel.evaluate() function. Since, Zen is dynamically typed
         * the compiler cannot determine the type of the operands. Therefore,
         * the multiplication/division/modulus operations are delegated to
         * functions annotated with the Operator annotation.
         */
        zen_BinaryEntityGenerator_invokeEvaluate(generator, symbol, symbolSize);
    }
}

// unaryExpression

void zen_BinaryEntityGenerator_onEnterUnaryExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_UnaryExpressionContext_t* context = (zen_UnaryExpressionContext_t*)node->m_context;

    zen_ASTNode_t* unaryOperator = context->m_unaryOperator;
    if (unaryOperator != NULL) {
        /* Generate the instructions corresponding to the unary expression. */
        zen_ASTWalker_walk(astListener, context->m_unaryExpression);

        /* Retrieve the corresponding unary operator token from the AST
         * node.
         */
        zen_Token_t* unaryOperatorToken = (zen_Token_t*)unaryOperator->m_context;
        /* Retrieve the type of the unary operator. */
        zen_TokenType_t unaryOperatorType = zen_Token_getType(unaryOperatorToken);

        /* The values of symbol and symbolSize are the only arbitrary variables
         * when invoking the zen_BinaryEntityGenerator_invokeEvaluate() function.
         * Therefore, instead of rewriting the invocation expression multiple
         * times, I have factored it out.
         */
        uint8_t* symbol = NULL;
        int32_t symbolSize = 1;

        switch (unaryOperatorType) {
            case ZEN_TOKEN_PLUS: {
                /* The kernel should find a function annotated with the Operator
                 * annotation that handles the '1+' symbol.
                 */
                symbol = "1+";
                symbolSize = 2;

                break;
            }

            case ZEN_TOKEN_DASH: {
                /* The kernel should find a function annotated with the Operator
                 * annotation that handles the '1-' symbol.
                 */
                symbol = "1-";
                symbolSize = 2;

                break;
            }

            case ZEN_TOKEN_TILDE: {
                /* The kernel should find a function annotated with the Operator
                 * annotation that handles the '~' symbol.
                 */
                symbol = "~";

                break;
            }

            case ZEN_TOKEN_EXCLAMATION_MARK: {
                /* The kernel should find a function annotated with the Operator
                 * annotation that handles the '!' symbol.
                 */
                symbol = "!";

                break;
            }

            /*
            case ZEN_TOKEN_PLUS_2:
            case ZEN_TOKEN_DASH_2: {
                if (unaryOperatorType == ZEN_TOKEN_PLUS_2) {
                    /* The onPreIncrement() function is invoked against the object whose internal
                     * state has to be "incremented by 1". It returns an object with its internal
                     * state "incremented by 1".
                     *
                    // zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                }
                else {
                    /* The onPreDecrement() function is invoked against the object whose internal
                     * state has to be "incremented by 1". It returns an object with its internal
                     * state "incremented by 1".
                     *
                    // zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                }
                /* A copy of the "incremented" object is required on the operand stack for
                 * assignment.
                 */
                // zen_BinaryEntityGenerator_emitDuplicate(generator);
                /* Assign the variable the object which represents the new state.
                 *
                 * TODO: Change store_a to store_a1 (and friends) and
                 *       store_field when necessary.
                 *
                // zen_BinaryEntityGenerator_emitStoreReference(generator, 0);

                break;
            }
            */
        }

        /* Generate the instructions corresponding to invoking the
         * ZenKernel.evaluate() function. Since, Zen is dynamically typed
         * the compiler cannot determine the type of the operands. Therefore,
         * the multiplication/division/modulus operations are delegated to
         * functions annotated with the Operator annotation.
         */
        zen_BinaryEntityGenerator_invokeEvaluate(generator, symbol, symbolSize);

        /* The instructions corresponding to the children nodes have been generated.
         * Therefore, do not visit them again.
         */
        zen_ASTListener_skipChildren(astListener);
    }
}

void zen_BinaryEntityGenerator_onExitUnaryExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// postfixExpression

void zen_BinaryEntityGenerator_invokeEvaluate(zen_BinaryEntityGenerator_t* generator,
    uint8_t* symbol, int32_t symbolSize) {

    // ZenKernel.evaluate(Object operand, String symbol)
    // ZenKernel.evaluate(Object operand1, Object operand2, String symbol)

    uint16_t symbolIndex = zen_ConstantPoolBuilder_getStringEntryIndexEx(
        generator->m_constantPoolBuilder, symbol, symbolSize);
    zen_BinaryEntityBuilder_emitLoadCPR(generator->m_builder, symbolIndex);
    printf("[debug] Emitted load_cpr %d\n", symbolIndex);

    const uint8_t* kernelClass = "zen/core/ZenKernel";
    int32_t kernelClassSize = 18;
    const uint8_t* evaluateDescriptor = "(zen/core/Object):(zen/core/Object)(zen/core/Object)(zen/core/Object)";
    int32_t evaluateDescriptorSize = 69;
    const uint8_t* evaluateName = "evaluate";
    int32_t evaluateNameSize = 8;
    uint16_t evaluateIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, kernelClass, kernelClassSize,
        evaluateDescriptor, evaluateDescriptorSize, evaluateName,
        evaluateNameSize);

    /* Invoke the static function to evaluate the expression. */
    zen_BinaryEntityBuilder_emitInvokeStatic(generator->m_builder,
        evaluateIndex);

    /* Log the emission of the invoke_static instruction. */
    printf("[debug] Emitted invoke_static %d\n", evaluateIndex);
}

int64_t zen_Long_convert(const uint8_t* text, int32_t length, int32_t radix) {
    jtk_Assert_assertObject(text, "The specified text is null.");

    bool error = false;
    int64_t result = -1;
    if ((length != 0) && (radix >= JTK_INTEGER_MIN_RADIX) &&
        (radix <= JTK_INTEGER_MAX_RADIX)) {
        bool negative = false;
        int32_t i = 0;
        int64_t limit = -JTK_LONG_MAX_VALUE;
        uint8_t first = text[0];
        result = 0;
        if (!((first >= '0') && (first <= '9'))) {
            if (first == '+') {
                negative = false;
            }
            else if (first == '-') {
                negative = true;
                limit = JTK_LONG_MIN_VALUE;
            }
            else {
                error = true;
            }

            if (length == 1) {
                error = true;
            }

            i++;
        }

        if (result != -1) {
            int64_t m = limit / radix;
            while (i < length) {
                uint8_t value = text[i++];
                if (value != '_') {
                    int32_t digit = jtk_Integer_digit(value, radix);
                    if ((digit < 0) || (result < m)) {
                        error = true;
                        break;
                    }
                    result *= radix;
                    if (result < (limit + digit)) {
                        error = true;
                        break;
                    }
                    result -= digit;
                }
            }
            if (error) {
                result = -1;
            }
            else {
                result = negative? result : -result;
            }
        }
    }
    return result;
}

void zen_BinaryEntityGenerator_loadLong(zen_BinaryEntityGenerator_t* generator,
    int64_t value) {
    switch (value) {
        case 0: {
            /* Emit the push_l0 instruction. */
            zen_BinaryEntityBuilder_emitPushLong0(generator->m_builder);

            /* Log the emission of the instruction. */
            printf("[debug] Emitted push_l0\n");

            break;
        }

        case 1: {
            /* Emit the push_l1 instruction. */
            zen_BinaryEntityBuilder_emitPushLong1(generator->m_builder);

            /* Log the emission of the instruction. */
            printf("[debug] Emitted push_l1\n");

            break;
        }

        case 2: {
            /* Emit the push_l2 instruction. */
            zen_BinaryEntityBuilder_emitPushLong2(generator->m_builder);

            /* Log the emission of the instruction. */
            printf("[debug] Emitted push_l2\n");

            break;
        }

        default: {
            /* If the integer literal is less than or equal to 255, then it can be
             * pushed onto the operand stack with the push_b instruction. Another
             * instruction can pad the remaining 32 bits on the operand stack with
             * zeroes.
             *
             * The range of a byte is [-128, 127].
             *
             * As of now, the compiler generates negative values with two instructions.
             * One instruction pushes the value onto the operand stack. The other
             * instruction takes care of multiplying a value of -1 to the previously
             * pushed integer. This will be fixed in the future.
             */
            if ((value >= 6) && (value <= 127)) {
                /* Emit the push_i0 instruction in order to provide padding on the operand
                 * stack.
                 */
                zen_BinaryEntityBuilder_emitPushInteger0(generator->m_builder);

                /* Log the emission of the instruction. */
                printf("[debug] Emitted push_i0\n");

                /* Emit the push_b instruction. */
                zen_BinaryEntityBuilder_emitPushByte(generator->m_builder,
                    value);

                /* Log the emission of the instruction. */
                printf("[debug] Emitted push_b %d\n", value);
            }
            else if (value <= 32767) {
                /* Emit the push_i0 instruction in order to provide padding on the operand
                 * stack.
                 */
                zen_BinaryEntityBuilder_emitPushInteger0(generator->m_builder);

                /* Log the emission of the instruction. */
                printf("[debug] Emitted push_i0\n");

                /* If the integer literal is less than or equal to 32767, then it
                 * can be pushed onto the operand stack with the push_s instruction.
                 *
                 * The range of a short is [-32768, 32767].
                 *
                 * As of now, the compiler generates negative values with two instructions.
                 * One instruction pushes the value onto the operand stack. The other
                 * instruction takes care of multiplying a value of -1 to the previously
                 * pushed integer. This will be fixed in the future.
                 */
                /* Emit the push_s instruction. */
                zen_BinaryEntityBuilder_emitPushShort(generator->m_builder,
                    value);

                /* Log the emission of the instruction. */
                printf("[debug] Emitted push_s %d\n", value);
            }
            else {
                // TODO: Filter emission of values larger than the integer threshold.

                /* If the integer literal is larger than 32767, then it should be pushed
                 * the operand stack with the load_cpr instruction.
                 *
                 * As of now, the compiler generates negative values with two instructions.
                 * One instruction pushes the value onto the operand stack. The other
                 * instruction takes care of multiplying a value of -1 to the previously
                 * pushed integer. This will be fixed in the future.
                 */

                uint8_t longIndex = zen_ConstantPoolBuilder_getLongEntryIndex(
                    generator->m_constantPoolBuilder, value);

                /* Emit the load_cpr instruction. */
                zen_BinaryEntityBuilder_emitLoadCPR(generator->m_builder,
                    longIndex);

                /* Log the emission of the instruction. */
                printf("[debug] Emitted load_cpr %d\n", longIndex);
            }
        }
    }
}

void zen_BinaryEntityGenerator_loadInteger(zen_BinaryEntityGenerator_t* generator,
    int32_t value) {
    switch (value) {
        case 0: {
            /* Emit the push_i0 instruction. */
            zen_BinaryEntityBuilder_emitPushInteger0(generator->m_builder);

            /* Log the emission of the instruction. */
            printf("[debug] Emitted push_i0\n");

            break;
        }

        case 1: {
            /* Emit the push_i1 instruction. */
            zen_BinaryEntityBuilder_emitPushInteger1(generator->m_builder);

            /* Log the emission of the instruction. */
            printf("[debug] Emitted push_i1\n");

            break;
        }

        case 2: {
            /* Emit the push_i2 instruction. */
            zen_BinaryEntityBuilder_emitPushInteger2(generator->m_builder);

            /* Log the emission of the instruction. */
            printf("[debug] Emitted push_i2\n");

            break;
        }

        case 3: {
            /* Emit the push_i3 instruction. */
            zen_BinaryEntityBuilder_emitPushInteger3(generator->m_builder);

            /* Log the emission of the instruction. */
            printf("[debug] Emitted push_i3\n");

            break;
        }

        case 4: {
            /* Emit the push_i4 instruction. */
            zen_BinaryEntityBuilder_emitPushInteger4(generator->m_builder);

            /* Log the emission of the instruction. */
            printf("[debug] Emitted push_i4\n");

            break;
        }

        case 5: {
            /* Emit the push_i5 instruction. */
            zen_BinaryEntityBuilder_emitPushInteger5(generator->m_builder);

            /* Log the emission of the instruction. */
            printf("[debug] Emitted push_i5\n");

            break;
        }

        default: {
            /* If the integer literal is less than or equal to 255, then it can be
             * pushed onto the operand stack with the push_b instruction.
             *
             * The range of a byte is [-128, 127].
             *
             * As of now, the compiler generates negative values with two instructions.
             * One instruction pushes the value onto the operand stack. The other
             * instruction takes care of multiplying a value of -1 to the previously
             * pushed integer. This will be fixed in the future.
             */
            if ((value >= 6) && (value <= 127)) {
                /* Emit the push_b instruction. */
                zen_BinaryEntityBuilder_emitPushByte(generator->m_builder,
                    value);

                /* Log the emission of the instruction. */
                printf("[debug] Emitted push_b %d\n", value);
            }
            else if (value <= 32767) {
                /* If the integer literal is less than or equal to 32767, then it
                 * can be pushed onto the operand stack with the push_s instruction.
                 *
                 * The range of a short is [-32768, 32767].
                 *
                 * As of now, the compiler generates negative values with two instructions.
                 * One instruction pushes the value onto the operand stack. The other
                 * instruction takes care of multiplying a value of -1 to the previously
                 * pushed integer. This will be fixed in the future.
                 */
                /* Emit the push_s instruction. */
                zen_BinaryEntityBuilder_emitPushShort(generator->m_builder,
                    value);

                /* Log the emission of the instruction. */
                printf("[debug] Emitted push_s %d\n", value);
            }
            else {
                // TODO: Filter emission of values larger than the integer threshold.

                /* If the integer literal is larger than 32767, then it should be pushed
                 * the operand stack with the load_cpr instruction.
                 *
                 * As of now, the compiler generates negative values with two instructions.
                 * One instruction pushes the value onto the operand stack. The other
                 * instruction takes care of multiplying a value of -1 to the previously
                 * pushed integer. This will be fixed in the future.
                 */

                uint8_t integerIndex = zen_ConstantPoolBuilder_getIntegerEntryIndex(
                    generator->m_constantPoolBuilder, value);

                /* Emit the load_cpr instruction. */
                zen_BinaryEntityBuilder_emitLoadCPR(generator->m_builder,
                    integerIndex);

                /* Log the emission of the instruction. */
                printf("[debug] Emitted load_cpr %d\n", integerIndex);
            }

            break;
        }
    }
}

/*
 * Zen allows operator overriding through a combination of functions and
 * annotations.
 *
 * All the operators in Zen are dispatched to a function call.
 * The ZenKernel.evaluate(...) function finds a suitable handler for the
 * operator defined within the operand object and dispatches it. In other words,
 * the compiler translates expressions with operators to equivalent
 * ZenKernel.evaluate(...) calls.
 *
 * For example, in the HashMap class the following annotation
 * overrides the subscript operator.
 *
 * @Operator symbol='[]'
 * function getValue(key)
 *     ...
 *
 * With that information, consider the following snippet of code.
 *
 * var emailAddresses = {
 *     'Samuel Rowe' : 'samuelrowe1999@gmail.com',
 *     'Joel E. Rego' : 'joelerego@gmail.com'
 * }
 * var myEmailAddress = emailAddresses['Samuel Rowe']
 *
 * The above code snippet is equivalent to the following expression statement.
 *
 * var emailAddresses = {
 *     'Samuel Rowe' : 'samuelrowe1999@gmail.com',
 *     'Joel E. Rego' : 'joelerego@gmail.com'
 * }
 * var myEmailAddress = ZenKernel.evaluate(emailAddresses, 'Samuel Rowe', '[]')
 *
 * In fact, when you compile the former code snippet the compiler generates
 * instructions as if the code was written in the latter form.
 */

void zen_BinaryEntityGenerator_handleRhsPostfixExpression(
    zen_ASTListener_t* astListener, zen_BinaryEntityGenerator_t* generator,
    zen_PostfixExpressionContext_t* context, zen_PrimaryExpressionContext_t* primaryExpressionContext) {
    zen_ASTNode_t* expression = primaryExpressionContext->m_expression;

    zen_Symbol_t* primarySymbol = NULL;
    int32_t postfixPartCount = jtk_ArrayList_getSize(context->m_postfixParts);

    /* Emit a push instruction if the primary expression is a literal or an
     * identifier.
     */
    if (zen_ASTNode_isTerminal(expression)) {
        /* Retrieve the token that the primary expression represents. */
        zen_Token_t* token = (zen_Token_t*)expression->m_context;

        switch (zen_Token_getType(token)) {
            case ZEN_TOKEN_IDENTIFIER: {
                /*zen_ASTNode_t* assignmentExpression = zen_ASTHelper_getAncestor(primaryExpressionContext->m_expression, ZEN_AST_NODE_ASSIGNMENT_EXPRESSION);
                zen_AssignmentExpressionContext_t* assignmentExpressionContext = (zen_AssignmentExpressionContext_t*)assignmentExpression->m_context;
                if (assignmentExpression != NULL) {
                    zen_ASTNode_t* assignmentOperator = assignmentExpressionContext->m_assignmentOperator;
                    if (assignmentOperator != NULL) {
                        zen_Token_t* operatorToken = (zen_Token_t*)(assignmentOperator->m_context);
                        if (zen_Token_getType(operatorToken) == ZEN_TOKEN_EQUAL) {
                            if (token->m_startIndex < operatorToken->m_startIndex) {
                                return;
                            }
                        }
                    }
                }
                */

                /* Retrieve the string equivalent to the identifier node. */
                int32_t identifierSize;
                uint8_t* identifierText = zen_ASTNode_toCString(expression, &identifierSize);

                /* Resolve the symbol in the symbol table. */
                zen_Symbol_t* symbol = zen_SymbolTable_resolve(generator->m_symbolTable, identifierText);

                zen_Scope_t* enclosingScope = zen_Symbol_getEnclosingScope(symbol);
                if (zen_Symbol_isVariable(symbol) || zen_Symbol_isConstant(symbol)) {
                    if (zen_Scope_isClassScope(enclosingScope)) {
                        if (zen_Symbol_isVariable(symbol)) {
                            zen_VariableSymbol_t* variableSymbol = (zen_VariableSymbol_t*)symbol->m_context;
                            if (true /* !zen_VariableSymbol_isStatic(variableSymbol)) */) {
                                /* The this reference is always stored at the zeroth position
                                 * in the local variable array. Further, we assume that the
                                 * class member and the expression being processed appear in
                                 * in the same class. Therefore, emit a load reference to the
                                 * this reference.
                                 */
                                zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder, 0);

                                /* Log the emission of the load_a instruction. */
                                printf("[debug] Emitted load_a 0\n");

                                /* Load the instance field. */
                                zen_BinaryEntityBuilder_emitLoadInstanceField(generator->m_builder, 0);

                                /* Log the emission of the load_instance_field instruction. */
                                printf("[debug] Emitted load_instance_field 0 (dummy index)\n");
                            }
                            else {
                                /* Load the static field. */
                                zen_BinaryEntityBuilder_emitLoadStaticField(generator->m_builder, 0);

                                /* Log the emission of the load_static_field instruction. */
                                printf("[debug] Emitted load_static_field 0 (dummy index)\n");
                            }
                        }
                        else {
                            zen_ConstantSymbol_t* constantSymbol = (zen_ConstantSymbol_t*)symbol->m_context;
                            if (true /* !zen_ConstantSymbol_isStatic(variableSymbol)) */) {
                                /* The this reference is always stored at the zeroth position
                                 * in the local variable array. Further, we assume that the
                                 * class member and the expression being processed appear in
                                 * in the same class. Therefore, emit a load reference to the
                                 * this reference.
                                 */
                                zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder, 0);

                                /* Log the emission of the load_a instruction. */
                                printf("[debug] Emitted load_a 0\n");

                                /* Load the instance field. */
                                zen_BinaryEntityBuilder_emitLoadInstanceField(generator->m_builder, 0);

                                /* Log the emission of the load_instance_field instruction. */
                                printf("[debug] Emitted load_instance_field 0 (dummy index)\n");
                            }
                            else {
                                /* Load the static field. */
                                zen_BinaryEntityGenerator_emitLoadStaticField(generator, 0);

                                /* Log the emission of the load_static_field instruction. */
                                printf("[debug] Emitted load_static_field 0 (dummy index)\n");
                            }
                        }
                    }
                    else if (zen_Scope_isLocalScope(enclosingScope)) {
                        int32_t index = -1;
                        if (zen_Symbol_isVariable(symbol)) {
                            zen_VariableSymbol_t* variableSymbol = (zen_VariableSymbol_t*)symbol->m_context;
                            index = variableSymbol->m_index;
                        }
                        else {
                            zen_ConstantSymbol_t* constantSymbol = (zen_ConstantSymbol_t*)symbol->m_context;
                            index = constantSymbol->m_index;
                        }

                        /* Emit the load_a instruction. */
                        zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder, index);

                        /* Log the emission of the load_a instruction. */
                        printf("[debug] Emitted load_a %d\n", index);
                    }
                }
                else {
                    /* Pass the reference to the symbol to the next phase. */
                    primarySymbol = symbol;
                }
                break;
            }

            case ZEN_TOKEN_INTEGER_LITERAL: {
                const uint8_t* integerClassName = "zen/core/Integer";
                int32_t integerClassNameSize = 16;
                uint16_t integerClassIndex = zen_ConstantPoolBuilder_getClassEntryIndexEx(
                    generator->m_constantPoolBuilder, integerClassName,
                    integerClassNameSize);

                /* Emit the new instruction. */
                zen_BinaryEntityBuilder_emitNew(generator->m_builder, integerClassIndex);
                
                /* Log the emission of the new instruction. */
                printf("[debug] Emitted new %d\n", integerClassIndex);
                
                /* Emit the duplicate instruction. */
                zen_BinaryEntityBuilder_emitDuplicate(generator->m_builder);
                
                /* Log the emission of the duplicate instruction. */
                printf("[debug] Emitted duplicate\n");
                
                uint8_t* integerText = zen_Token_getText(token);
                int32_t actualIntegerLength = zen_Token_getLength(token);
                int32_t integerLength = actualIntegerLength;

                int32_t radix = 10;
                if (integerLength > 2) {
                    if ((integerText[0] == '0') && ((integerText[0] == 'x') || (integerText[0] == 'X'))) {
                        radix = 16;
                        integerText += 2;
                        integerLength -= 2;
                    }
                    else if ((integerText[0] == '0') && ((integerText[0] == 'b') || (integerText[0] == 'B'))) {
                        radix = 2;
                        integerText += 2;
                        integerLength -= 2;
                    }
                    else if ((integerText[0] == '0') && ((integerText[0] == 'c') || (integerText[0] == 'C'))) {
                        /* TODO: Octal integer literals begin with 0c or 0C according
                         * to the logic written here. Therefore, please modify the
                         * lexer accordingly.
                         */

                        radix = 8;
                        integerText += 2;
                        integerLength -= 2;
                    }
                }

                bool longLiteral = (integerText[actualIntegerLength - 1] == 'L') ||
                    (integerText[actualIntegerLength - 1] == 'l');

                if (longLiteral) {
                    integerLength--;
                }

                int64_t value = zen_Long_convert(integerText, integerLength, radix);

                if (longLiteral) {
                    zen_BinaryEntityGenerator_loadLong(generator, value);
                }
                else {
                    zen_BinaryEntityGenerator_loadInteger(generator, value);
                }
                
                // TODO: Implement integer interning.

                const uint8_t* constructorName = "<initialize>";
                int32_t constructorNameSize = 12;
                // const uint8_t* constructorDescriptor = "v:i";
                const uint8_t* constructorDescriptor = "v:(zen/core/Object)";
                int32_t constructorDescriptorSize = 19;
                uint16_t constructorIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
                    generator->m_constantPoolBuilder, integerClassName,
                    integerClassNameSize, constructorDescriptor, constructorDescriptorSize,
                    constructorName, constructorNameSize);

                /* Invoke the constructor to initialize the new integer instance. */
                zen_BinaryEntityBuilder_emitInvokeSpecial(generator->m_builder,
                    constructorIndex);
                /* Log the emission of the invoke_special instruction. */
                printf("[debug] Emitted invoke_special %d\n", constructorIndex);

                break;
            }

            case ZEN_TOKEN_KEYWORD_TRUE: {
                /* Emit push_i1. In the operand stack, 1 represents true. */
                zen_BinaryEntityBuilder_emitPushInteger1(generator->m_builder);

                /* Log the emission of the instruction. */
                printf("[debug] Emitted push_i1\n");

                break;
            }

            case ZEN_TOKEN_KEYWORD_FALSE: {
                /* Emit push_i0 instruction. In the operand stack, 0 represents false. */
                zen_BinaryEntityBuilder_emitPushInteger0(generator->m_builder);

                /* Log the emission of the instruction. */
                printf("[debug] Emitted push_i0\n");

                break;
            }

            case ZEN_TOKEN_STRING_LITERAL: {
                /* Retrieve a valid index into the constant pool. The entry at
                 * this index is a constant pool string. The token text encloses
                 * the content within double quotes. Therefore, the first quote
                 * is skipped using pointer arithmetic and the last quote
                 * is skipped by subtracting 1 from the length of the text.
                 * Another 1 is subtracted from the text length because the first
                 * quote was skipped.
                 */
                uint8_t stringIndex = zen_ConstantPoolBuilder_getStringEntryIndexEx(
                    generator->m_constantPoolBuilder, token->m_text + 1, token->m_length - 2);

                /* Emit load_cpr instruction. */
                zen_BinaryEntityBuilder_emitLoadCPR(generator->m_builder,
                    stringIndex);

                /* Log the emission of the instruction. */
                printf("[debug] Emitted load_cpr %d\n", stringIndex);

                break;
            }

            case ZEN_TOKEN_KEYWORD_NULL: {
                /* Emit the push_null instruction. */
                zen_BinaryEntityBuilder_emitPushNull(generator->m_builder);

                /* Log the emission of the push_null instruction. */
                printf("[debug] Emitted push_null\n");

                break;
            }

            case ZEN_TOKEN_KEYWORD_THIS: {
                /* Emit the load_a instruction. */
                zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder, 0);

                /* Log the emission of the load_a instruction. */
                printf("[debug] Emitted load_a 0\n");

                break;
            }

            /*
            default: {
                /* [internal error] *
            }*/
        }
    }
    else if ((expression->m_type == ZEN_AST_NODE_TYPE_MAP_EXPRESSION) ||
        (expression->m_type == ZEN_AST_NODE_TYPE_LIST_EXPRESSION) ||
        (expression->m_type == ZEN_AST_NODE_TYPE_EXPRESSION) ||
        (expression->m_type == ZEN_AST_NODE_TYPE_NEW_EXPRESSION)) {
        zen_ASTWalker_walk(astListener, expression);
    }
    else {
        // Error: What node do we have here?
    }

    const uint8_t* kernelClassName = "zen/core/ZenKernel";
    int32_t kernelClassNameSize = 18;
    const uint8_t* invokeDescriptor = "(zen/core/Object):(zen/core/Object)(zen/core/String)";
    int32_t invokeDescriptorSize = 52;
    const uint8_t* invokeName = "invoke";
    int32_t invokeNameSize = 6;
    uint16_t invokeIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, kernelClassName, kernelClassNameSize,
        invokeDescriptor, invokeDescriptorSize, invokeName,
        invokeNameSize);

    const uint8_t* invoke2Descriptor = "(zen/core/Object):(zen/core/Object)(zen/core/String)@(zen/core/Object)";
    int32_t invoke2DescriptorSize = 70;
    uint16_t invoke2Index = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, kernelClassName, kernelClassNameSize,
        invoke2Descriptor, invoke2DescriptorSize, invokeName,
        invokeNameSize);

    // const uint8_t* invokeStaticDescriptor = "(zen/core/Object):(zen/core/Class)(zen/core/String)";
    const uint8_t* invokeStaticDescriptor = "(zen/core/Object):(zen/core/Object)(zen/core/Object)";
    int32_t invokeStaticDescriptorSize = 52;
    const uint8_t* invokeStaticName = "invokeStatic";
    int32_t invokeStaticNameSize = 12;
    uint16_t invokeStaticIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, kernelClassName, kernelClassNameSize,
        invokeStaticDescriptor, invokeStaticDescriptorSize, invokeStaticName,
        invokeStaticNameSize);

    // const uint8_t* invokeStatic2Descriptor = "(zen/core/Object):(zen/core/Class)(zen/core/String)@(zen/core/Object)";
    const uint8_t* invokeStatic2Descriptor = "(zen/core/Object):(zen/core/Object)(zen/core/Object)@(zen/core/Object)";
    int32_t invokeStatic2DescriptorSize = 70;
    uint16_t invokeStatic2Index = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, kernelClassName, kernelClassNameSize,
        invokeStatic2Descriptor, invokeStatic2DescriptorSize, invokeStaticName,
        invokeStaticNameSize);

    const uint8_t* loadFieldDescriptor = "(zen/core/Object):(zen/core/Object)(zen/core/String)";
    int32_t loadFieldDescriptorSize = 52;
    const uint8_t* loadFieldName = "loadField";
    int32_t loadFieldNameSize = 9;
    uint16_t loadFieldIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, kernelClassName, kernelClassNameSize,
        loadFieldDescriptor, loadFieldDescriptorSize, loadFieldName,
        loadFieldNameSize);

    const uint8_t* objectClassName = "zen.core.Object";
    int32_t objectClassNameSize = 15;
    uint16_t objectClassIndex = zen_ConstantPoolBuilder_getClassEntryIndexEx(
        generator->m_constantPoolBuilder, objectClassName, objectClassNameSize);

    int32_t i;
    for (i = 0; i < postfixPartCount; i++) {
        zen_ASTNode_t* postfixPart = (zen_ASTNode_t*)jtk_ArrayList_getValue(
            context->m_postfixParts, i);
        zen_ASTNodeType_t type = zen_ASTNode_getType(postfixPart);

        /* When code written in a statically typed ZVM language is in integrated
         * with code written in dynamically typed ZVM language such as Zen,
         * special care should be taken. The operand stack is vulnerable to
         * pollution. For example, assume the following function written in a
         * hypothetical ZVM language that is statically typed.
         *
         * int getIndex()
         *     ...
         *
         * Now, consider the following code written in Zen.
         *
         * function main(...arguments)
         *     var index = getIndex()
         *     index++
         *
         * When getIndex() function is invoked from Zen, the primitive value
         * is passed around the code. The postfix increment operator causes
         * the invocation of ZenEnvironment.invokeOperator(...) against a primitive
         * value, given the operand stack does not store the type of its entries.
         * Therefore, the compiler of the dynamically typed language should take
         * care of wrapping and unwrapping primitive values to their corresponding
         * wrapper class objects.
         */
        switch (type) {
            case ZEN_AST_NODE_TYPE_SUBSCRIPT: {
                zen_SubscriptContext_t* subscriptContext = (zen_SubscriptContext_t*)postfixPart->m_context;

                /* Visit the index expression node and generate the relevant
                 * instructions.
                 */
                zen_ASTWalker_walk(astListener, subscriptContext->m_expression);

                /* Generate the instructions corresponding to invoking the
                 * ZenKernel.evaluate() function. Since, Zen is dynamically typed
                 * the compiler cannot determine the type of the operands. Therefore,
                 * the subscript operation is delegated to the function annotated
                 * with the Operator annotation.
                 */
                zen_BinaryEntityGenerator_invokeEvaluate(generator, "[]", 2);

                /* The normal behaviour of the AST walker causes the generator to
                 * emit instructions in an undesirable fashion. Therefore, we partially
                 * switch from the listener to visitor design pattern. The AST walker
                 * can be guided to switch to this mode via zen_ASTListener_skipChildren()
                 * function which causes the AST walker to skip iterating over the children
                 * nodes.
                 */
                zen_ASTListener_skipChildren(astListener);

                break;
            }

            case ZEN_AST_NODE_TYPE_FUNCTION_ARGUMENTS: {
                zen_FunctionArgumentsContext_t* functionArgumentsContext =
                    (zen_FunctionArgumentsContext_t*)postfixPart->m_context;

                if (i == 0) {
                    if (primarySymbol == NULL) {
                        printf("[internal error] Variable treated as function. Looks like the semantic analysis phase malfunctioned.\n");
                    }
                    else if (zen_Symbol_isFunction(primarySymbol)) {
                        zen_FunctionSymbol_t* functionSymbol = (zen_FunctionSymbol_t*)primarySymbol->m_context;
                        int32_t index = invokeStaticIndex;
                        if (zen_Modifier_hasStatic(primarySymbol->m_modifiers)) {
                            // NOTE: I am assuming that only class members can be declared as static.
                            zen_Scope_t* enclosingScope = primarySymbol->m_enclosingScope;
                            zen_ClassScope_t* classScope = (zen_ClassScope_t*)enclosingScope->m_context;
                            zen_Symbol_t* classSymbol = classScope->m_classSymbol;
                            zen_ASTNode_t* identifier = classSymbol->m_identifier;
                            zen_Token_t* identifierToken = (zen_Token_t*)identifier->m_context;

                            uint16_t identifierIndex = zen_ConstantPoolBuilder_getStringEntryIndexEx(
                                generator->m_constantPoolBuilder, identifierToken->m_text,
                                identifierToken->m_length);

                            // TODO: Should retrieve a constant pool index to a class entry.
                            zen_BinaryEntityBuilder_emitLoadCPR(generator->m_builder, identifierIndex);

                            /* Log the emission of the load_cpr instruction. */
                            printf("[debug] Emitted load_cpr %d\n", identifierIndex);

                        }
                        else {
                            /* The "this" reference is always stored at the zeroth position
                             * in the local variable array. Further, we assume that the
                             * class member and the expression being processed appear in
                             * in the same class. Therefore, emit a load reference to the
                             * this reference.
                             */
                            zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder, 0);

                            /* Log the emission of the load_a instruction. */
                            printf("[debug] Emitted load_a 0\n");

                            index = invokeIndex;
                        }

                        zen_ASTNode_t* primarySymbolIdentifier = primarySymbol->m_identifier;
                        zen_Token_t* primarySymbolToken = (zen_Token_t*)primarySymbolIdentifier->m_context;

                        /* The name of the function to invoke. */
                        int32_t targetNameIndex = zen_ConstantPoolBuilder_getStringEntryIndexEx(
                            generator->m_constantPoolBuilder, primarySymbolToken->m_text,
                            primarySymbolToken->m_length);

                        /* Push the name of the target function on the operand stack. */
                        zen_BinaryEntityBuilder_emitLoadCPR(generator->m_builder,
                            targetNameIndex);

                        zen_ASTNode_t* expressions = functionArgumentsContext->m_expressions;
                        if (expressions != NULL) {
                            zen_ExpressionsContext_t* expressionsContext = (zen_ExpressionsContext_t*)expressions->m_context;
                            int32_t argumentCount = jtk_ArrayList_getSize(expressionsContext->m_expressions);
                            if (argumentCount > 0) {
                                /* Push the size of the list onto the operand stack. */
                                zen_BinaryEntityGenerator_loadInteger(generator, argumentCount);

                                /* In Zen, function invocations are simulated using the ZenKerenel.dispatch()
                                 * function. It requires the arguments of the function invocation in an array.
                                 * Therefore, create an array and fill it with the arguments.
                                 *
                                 * Emit the new_array_a instruction to create the array.
                                 */
                                zen_BinaryEntityBuilder_emitNewReferenceArray(generator->m_builder,
                                    objectClassIndex);

                                /* Log the emission of the new_array_a instruction. */
                                printf("[debug] Emitted new_array_a %d\n", objectClassIndex);

                                int32_t argumentIndex;
                                for (argumentIndex = 0; argumentIndex < argumentCount; argumentIndex++) {
                                    /* Retrieve the expression for the current argument. */
                                    zen_ASTNode_t* argument = (zen_ASTNode_t*)jtk_ArrayList_getValue(
                                        expressionsContext->m_expressions, argumentIndex);

                                    /* Duplicate the reference to the argument array. */
                                    zen_BinaryEntityBuilder_emitDuplicate(generator->m_builder);

                                    /* Log the emission of the duplicate instruction. */
                                    printf("[debug] Emitted duplicate\n");

                                    /* Push the index at which the result of the expression will be stored. */
                                    zen_BinaryEntityGenerator_loadInteger(generator, i);

                                    /* Visit the expression node and generate the relevant instructions. */
                                    zen_ASTWalker_walk(astListener, argument);

                                    /* Store the result in the argument array. */
                                    zen_BinaryEntityBuilder_emitStoreArrayReference(generator->m_builder);

                                    /* Log the emission of the store_aa instruction. */
                                    printf("[debug] Emitted store_aa\n");

                                }

                                index = !zen_Modifier_hasStatic(primarySymbol->m_modifiers)?
                                    invoke2Index : invokeStatic2Index;
                            }
                        }

                        /* Invoke the ZenKernel.dispatch() function to simulate a function
                         * call.
                         */
                        zen_BinaryEntityBuilder_emitInvokeStatic(generator->m_builder, index);

                        /* Log the emission of the invoke_static instruction. */
                        printf("[debug] Emitted invoke_static %d\n", index);
                    }
                    else {
                        printf("[internal error] Trying to invoke a non-function entity. The phases prior to the code generation must have malfunctioned.\n");
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

                /* The name of the function/field to invoke/load. */
                int32_t targetNameIndex = zen_ConstantPoolBuilder_getStringEntryIndexEx(
                    generator->m_constantPoolBuilder, identifierToken->m_text,
                    identifierToken->m_length);

                zen_ASTNode_t* functionArguments = NULL;

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
                    zen_FunctionArgumentsContext_t* functionArgumentsContext =
                        (zen_FunctionArgumentsContext_t*)functionArguments->m_context;

                    /* A static function invocation can occur only if the function arguments
                     * postfix part occurs at the zeroth position. This behavior is a direct
                     * result of Zen not supporting nested classes.
                     */

                    /* Push the name of the target function on the operand stack. */
                    zen_BinaryEntityBuilder_emitLoadCPR(generator->m_builder,
                        targetNameIndex);

                    /* Log the emission of the load_cpr instruction. */
                    printf("[debug] Emitted load_cpr %d\n", targetNameIndex);

                    int32_t index = invoke2Index;

                    zen_ASTNode_t* expressions = functionArgumentsContext->m_expressions;
                    if (expressions != NULL) {
                        zen_ExpressionsContext_t* expressionsContext = (zen_ExpressionsContext_t*)expressions->m_context;
                        int32_t argumentCount = jtk_ArrayList_getSize(expressionsContext->m_expressions);
                        if (argumentCount > 0) {
                            /* Push the size of the list onto the operand stack. */
                            zen_BinaryEntityGenerator_loadInteger(generator, argumentCount);

                            /* In Zen, function invocations are simulated using the ZenKerenel.dispatch()
                             * function. It requires the arguments of the function invocation in an array.
                             * Therefore, create an array and fill it with the arguments.
                             *
                             * Emit the new_array_a instruction to create the array.
                             */
                            zen_BinaryEntityBuilder_emitNewReferenceArray(generator->m_builder,
                                objectClassIndex);

                            /* Log the emission of the new_array_a instruction. */
                            printf("[debug] Emitted new_array_a %d\n", objectClassIndex);

                            int32_t argumentIndex;
                            for (argumentIndex = 0; argumentIndex < argumentCount; argumentIndex++) {
                                /* Retrieve the expression for the current argument. */
                                zen_ASTNode_t* argument = (zen_ASTNode_t*)jtk_ArrayList_getValue(
                                    expressionsContext->m_expressions, argumentIndex);

                                /* Duplicate the reference to the argument array. */
                                zen_BinaryEntityBuilder_emitDuplicate(generator->m_builder);

                                /* Log the emission of the duplicate instruction. */
                                printf("[debug] Emitted duplicate\n");

                                /* Push the index at which the result of the expression will be stored. */
                                zen_BinaryEntityGenerator_loadInteger(generator, i);

                                /* Visit the expression node and generate the relevant instructions. */
                                zen_ASTWalker_walk(astListener, argument);

                                /* Store the result in the argument array. */
                                zen_BinaryEntityBuilder_emitStoreArrayReference(generator->m_builder);

                                /* Log the emission of the store_aa instruction. */
                                printf("[debug] Emitted store_aa\n");
                            }

                            index = invokeIndex;
                        }
                    }

                    /* Invoke the ZenKernel.dispatch() function to simulate a function
                     * call.
                     */
                    zen_BinaryEntityBuilder_emitInvokeStatic(generator->m_builder, index);

                    /* Log the emission of the invoke_static instruction. */
                    printf("[debug] Emitted invoke_static %d\n", index);
                }
                else {
                    /* Push the name of the field to load on the operand stack. */
                    zen_BinaryEntityBuilder_emitLoadCPR(generator->m_builder,
                        targetNameIndex);

                    /* Log the emission of the load_cpr instruction. */
                    printf("[debug] Emitted load_cpr %d\n", targetNameIndex);

                    /* Invoke the ZenKernel.loadField() function to load the value
                     * stored in a field.
                     */
                    zen_BinaryEntityBuilder_emitInvokeStatic(generator->m_builder, loadFieldIndex);

                    /* Log the emission of the invoke_static instruction. */
                    printf("[debug] Emitted invoke_static %d\n", loadFieldIndex);
                }

                break;
            }

            case ZEN_AST_NODE_TYPE_POSTFIX_OPERATOR: {

                break;
            }

            default: {
                printf("[error] Invalid AST node type %d encountered.\n", type);
            }
        }
    }
}

void zen_BinaryEntityGenerator_handleLhsPostfixExpression(
    zen_ASTListener_t* astListener, zen_BinaryEntityGenerator_t* generator,
    zen_PostfixExpressionContext_t* context,
    zen_PrimaryExpressionContext_t* primaryExpressionContext) {
    zen_ASTNode_t* expression = primaryExpressionContext->m_expression;

    zen_Symbol_t* primarySymbol = NULL;
    int32_t postfixPartCount = jtk_ArrayList_getSize(context->m_postfixParts);

    /* Load a variable or a constant. */
    if (zen_ASTNode_isTerminal(expression)) {
        /* Retrieve the token that the primary expression represents. */
        zen_Token_t* token = (zen_Token_t*)expression->m_context;

        switch (zen_Token_getType(token)) {
            case ZEN_TOKEN_IDENTIFIER: {
                /* Retrieve the string equivalent to the identifier node. */
                int32_t identifierSize;
                uint8_t* identifierText = zen_ASTNode_toCString(expression, &identifierSize);

                /* Resolve the symbol in the symbol table. */
                zen_Symbol_t* symbol = zen_SymbolTable_resolve(generator->m_symbolTable, identifierText);
                /* Pass the reference to the symbol to the next phase. */
                primarySymbol = symbol;

                break;
            }

            case ZEN_TOKEN_INTEGER_LITERAL: {
                uint8_t* integerText = zen_Token_getText(token);
                int32_t actualIntegerLength = zen_Token_getLength(token);
                int32_t integerLength = actualIntegerLength;

                int32_t radix = 10;
                if (integerLength > 2) {
                    if ((integerText[0] == '0') && ((integerText[0] == 'x') || (integerText[0] == 'X'))) {
                        radix = 16;
                        integerText += 2;
                        integerLength -= 2;
                    }
                    else if ((integerText[0] == '0') && ((integerText[0] == 'b') || (integerText[0] == 'B'))) {
                        radix = 2;
                        integerText += 2;
                        integerLength -= 2;
                    }
                    else if ((integerText[0] == '0') && ((integerText[0] == 'c') || (integerText[0] == 'C'))) {
                        /* TODO: Octal integer literals begin with 0c or 0C according
                         * to the logic written here. Therefore, please modify the
                         * lexer accordingly.
                         */

                        radix = 8;
                        integerText += 2;
                        integerLength -= 2;
                    }
                }

                bool longLiteral = (integerText[actualIntegerLength - 1] == 'L') ||
                    (integerText[actualIntegerLength - 1] == 'l');

                if (longLiteral) {
                    integerLength--;
                }

                int64_t value = zen_Long_convert(integerText, integerLength, radix);

                if (longLiteral) {
                    zen_BinaryEntityGenerator_loadLong(generator, value);
                }
                else {
                    zen_BinaryEntityGenerator_loadInteger(generator, value);
                }

                break;
            }

            case ZEN_TOKEN_KEYWORD_TRUE: {
                /* Emit push_i1. In the operand stack, 1 represents true. */
                zen_BinaryEntityBuilder_emitPushInteger1(generator->m_builder);

                /* Log the emission of the instruction. */
                printf("[debug] Emitted push_i1\n");

                break;
            }

            case ZEN_TOKEN_KEYWORD_FALSE: {
                /* Emit push_i0 instruction. In the operand stack, 0 represents false. */
                zen_BinaryEntityBuilder_emitPushInteger0(generator->m_builder);

                /* Log the emission of the instruction. */
                printf("[debug] Emitted push_i0\n");

                break;
            }

            case ZEN_TOKEN_STRING_LITERAL: {
                /* Retrieve a valid index into the constant pool. The entry at
                 * this index is a constant pool string. The token text encloses
                 * the content within double quotes. Therefore, the first quote
                 * is skipped using pointer arithmetic and the last quote
                 * is skipped by subtracting 1 from the length of the text.
                 * Another 1 is subtracted from the text length because the first
                 * quote was skipped.
                 */
                uint8_t stringIndex = zen_ConstantPoolBuilder_getStringEntryIndexEx(
                    generator->m_constantPoolBuilder, token->m_text + 1, token->m_length - 2);

                /* Emit load_cpr instruction. */
                zen_BinaryEntityBuilder_emitLoadCPR(generator->m_builder,
                    stringIndex);

                /* Log the emission of the instruction. */
                printf("[debug] Emitted load_cpr %d\n", stringIndex);

                break;
            }

            case ZEN_TOKEN_KEYWORD_NULL: {
                /* Emit the push_null instruction. */
                zen_BinaryEntityBuilder_emitPushNull(generator->m_builder);

                /* Log the emission of the push_null instruction. */
                printf("[debug] Emitted push_null\n");

                break;
            }

            case ZEN_TOKEN_KEYWORD_THIS: {
                /* Emit the load_a instruction. */
                zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder, 0);

                /* Log the emission of the load_a instruction. */
                printf("[debug] Emitted load_a 0\n");

                break;
            }
        }
    }
    else if ((expression->m_type == ZEN_AST_NODE_TYPE_MAP_EXPRESSION) ||
        (expression->m_type == ZEN_AST_NODE_TYPE_LIST_EXPRESSION) ||
        (expression->m_type == ZEN_AST_NODE_TYPE_EXPRESSION) ||
        (expression->m_type == ZEN_AST_NODE_TYPE_NEW_EXPRESSION)) {
        zen_ASTWalker_walk(astListener, expression);
    }
    else {
        // Error: What node do we have here?
    }

    const uint8_t* objectClassName = "zen.core.Object";
    int32_t objectClassNameSize = 15;
    uint16_t objectClassIndex = zen_ConstantPoolBuilder_getClassEntryIndexEx(
        generator->m_constantPoolBuilder, objectClassName, objectClassNameSize);

    const uint8_t* kernelClassName = "zen/core/ZenKernel";
    int32_t kernelClassNameSize = 18;
    const uint8_t* storeFieldDescriptor = "(zen/core/Object):(zen/core/Object)(zen/core/String)(zen/core/Object)";
    int32_t storeFieldDescriptorSize = 69;
    const uint8_t* storeFieldName = "storeField";
    int32_t storeFieldNameSize = 10;
    uint16_t storeFieldIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, kernelClassName, kernelClassNameSize,
        storeFieldDescriptor, storeFieldDescriptorSize, storeFieldName,
        storeFieldNameSize);

    const uint8_t* storeClassFieldDescriptor = "(zen/core/Object):(zen/core/Object)(zen/core/String)(zen/core/Object)";
    int32_t storeClassFieldDescriptorSize = 69;
    const uint8_t* storeClassFieldName = "storeClassField";
    int32_t storeClassFieldNameSize = 15;
    uint16_t storeClassFieldIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, kernelClassName, kernelClassNameSize,
        storeClassFieldDescriptor, storeClassFieldDescriptorSize, storeClassFieldName,
        storeClassFieldNameSize);

    const uint8_t* invokeDescriptor = "(zen/core/Object):(zen/core/Object)(zen/core/String)";
    int32_t invokeDescriptorSize = 52;
    const uint8_t* invokeName = "invoke";
    int32_t invokeNameSize = 6;
    uint16_t invokeIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, kernelClassName, kernelClassNameSize,
        invokeDescriptor, invokeDescriptorSize, invokeName,
        invokeNameSize);

    const uint8_t* invoke2Descriptor = "(zen/core/Object):(zen/core/Object)(zen/core/String)@(zen/core/Object)";
    int32_t invoke2DescriptorSize = 70;
    uint16_t invoke2Index = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, kernelClassName, kernelClassNameSize,
        invoke2Descriptor, invoke2DescriptorSize, invokeName,
        invokeNameSize);

    // const uint8_t* invokeStaticDescriptor = "(zen/core/Object):(zen/core/Class)(zen/core/String)";
    const uint8_t* invokeStaticDescriptor = "(zen/core/Object):(zen/core/Object)(zen/core/Object)";
    int32_t invokeStaticDescriptorSize = 52;
    const uint8_t* invokeStaticName = "invokeStatic";
    int32_t invokeStaticNameSize = 12;
    uint16_t invokeStaticIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, kernelClassName, kernelClassNameSize,
        invokeStaticDescriptor, invokeStaticDescriptorSize, invokeStaticName,
        invokeStaticNameSize);

    // const uint8_t* invokeStatic2Descriptor = "(zen/core/Object):(zen/core/Class)(zen/core/String)@(zen/core/Object)";
    const uint8_t* invokeStatic2Descriptor = "(zen/core/Object):(zen/core/Object)(zen/core/Object)@(zen/core/Object)";
    int32_t invokeStatic2DescriptorSize = 70;
    uint16_t invokeStatic2Index = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, kernelClassName, kernelClassNameSize,
        invokeStatic2Descriptor, invokeStatic2DescriptorSize, invokeStaticName,
        invokeStaticNameSize);

    const uint8_t* loadFieldDescriptor = "(zen/core/Object):(zen/core/Object)(zen/core/String)";
    int32_t loadFieldDescriptorSize = 52;
    const uint8_t* loadFieldName = "loadField";
    int32_t loadFieldNameSize = 9;
    uint16_t loadFieldIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, kernelClassName, kernelClassNameSize,
        loadFieldDescriptor, loadFieldDescriptorSize, loadFieldName,
        loadFieldNameSize);

    if (postfixPartCount == 0) {
        if (zen_Symbol_isVariable(primarySymbol) || zen_Symbol_isConstant(primarySymbol)) {
            zen_Scope_t* enclosingScope = zen_Symbol_getEnclosingScope(primarySymbol);
            if (zen_Scope_isClassScope(enclosingScope)) {
                bool instance = false;
                if (zen_Symbol_isVariable(primarySymbol)) {
                    zen_VariableSymbol_t* variableSymbol = (zen_VariableSymbol_t*)primarySymbol->m_context;
                    instance = true; /* !zen_VariableSymbol_isStatic(variableSymbol)) */
                }
                else {
                    zen_ConstantSymbol_t* constantSymbol = (zen_ConstantSymbol_t*)primarySymbol->m_context;
                    instance = true; /* !zen_ConstantSymbol_isStatic(constantSymbol)) */
                }

                zen_ASTNode_t* identifier = primarySymbol->m_identifier;
                zen_Token_t* identifierToken = (zen_Token_t*)identifier->m_context;
                uint16_t identifierIndex = zen_ConstantPoolBuilder_getStringEntryIndexEx(
                    generator->m_constantPoolBuilder, identifierToken->m_text,
                    identifierToken->m_length);

                if (instance) {
                    /* The this reference is always stored at the zeroth position
                     * in the local variable array. Further, we assume that the
                     * class member and the expression being processed appear in
                     * in the same class. Therefore, emit a load reference to the
                     * this reference.
                     */
                    zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder, 0);

                    /* Log the emission of the load_a instruction. */
                    printf("[debug] Emitted load_a 0\n");

                    /* Load the name of the field. */
                    zen_BinaryEntityBuilder_emitLoadCPR(generator->m_builder, identifierIndex);

                    /* Log the emission of the load_cpr instruction. */
                    printf("[debug] Emitted load_cpr %d\n", identifierIndex);

                    /* Invoke the ZenKernel.storeField() function to update
                     * the field.
                     */
                    zen_BinaryEntityBuilder_emitInvokeStatic(generator->m_builder, storeFieldIndex);

                    /* Log the emission of the invoke_static instruction. */
                    printf("[debug] Emitted invoke_static %d\n", storeFieldIndex);
                }
                else {
                    /* Emit the `load_cpr` instruction to load the reference of the
                     * class to which the static field belongs.
                     */
                    zen_BinaryEntityBuilder_emitLoadCPR(generator->m_builder, 0);

                    /* Log the emission of the load_cpr instruction. */
                    printf("[debug] Emitted load_cpr 0 (dummy)\n");

                    /* Load the name of the field. */
                    zen_BinaryEntityBuilder_emitLoadCPR(generator->m_builder, identifierIndex);

                    /* Log the emission of the load_cpr instruction. */
                    printf("[debug] Emitted load_cpr %d\n", identifierIndex);

                    /* Invoke the ZenKernel.storeField() function to update
                     * the field.
                     */
                    zen_BinaryEntityBuilder_emitInvokeStatic(generator->m_builder, storeFieldIndex);

                    /* Log the emission of the invoke_static instruction. */
                    printf("[debug] Emitted invoke_static %d\n", storeFieldIndex);
                }
            }
            else if (zen_Scope_isLocalScope(enclosingScope)) {
                int32_t index = -1;
                if (zen_Symbol_isVariable(primarySymbol)) {
                    zen_VariableSymbol_t* variableSymbol = (zen_VariableSymbol_t*)primarySymbol->m_context;
                    index = variableSymbol->m_index;
                }
                else {
                    zen_ConstantSymbol_t* constantSymbol = (zen_ConstantSymbol_t*)primarySymbol->m_context;
                    index = constantSymbol->m_index;
                }

                /* Emit the store_a instruction. */
                zen_BinaryEntityBuilder_emitStoreReference(generator->m_builder, index);

                /* Log the emission of the store_a instruction. */
                printf("[debug] Emitted store_a %d\n", index);
            }
        }
    }
    else {
        int32_t i;
        for (i = 0; i < postfixPartCount; i++) {
            zen_ASTNode_t* postfixPart = (zen_ASTNode_t*)jtk_ArrayList_getValue(
                context->m_postfixParts, i);
            zen_ASTNodeType_t type = zen_ASTNode_getType(postfixPart);

            /* When code written in a statically typed ZVM language is integrated
             * with code written in a dynamically typed ZVM language, such as Zen,
             * special care should be taken. The operand stack is vulnerable to
             * pollution. For example, assume the following function written in a
             * hypothetical ZVM language that is statically typed.
             *
             * int getIndex()
             *     ...
             *
             * Now, consider the following code written in Zen.
             *
             * function main(...arguments)
             *     var index = getIndex()
             *     index += 1
             *
             * When getIndex() function is invoked from Zen, the primitive value
             * is passed around the code. The compound assignment operator causes
             * the invocation of ZenEnvironment.invokeOperator(...) against a primitive
             * value, given the operand stack does not store the type of its entries.
             * Therefore, the compiler of the dynamically typed language should take
             * care of wrapping and unwrapping primitive values to their corresponding
             * wrapper class objects.
             */
            switch (type) {
                case ZEN_AST_NODE_TYPE_SUBSCRIPT: {
                    zen_SubscriptContext_t* subscriptContext = (zen_SubscriptContext_t*)postfixPart->m_context;

                    /* Visit the index expression node and generate the relevant
                     * instructions.
                     */
                    zen_ASTWalker_walk(astListener, subscriptContext->m_expression);

                    /* Generate the instructions corresponding to invoking the
                     * ZenKernel.evaluate() function.
                     */
                    if (i + 1 < postfixPartCount) {
                        zen_BinaryEntityGenerator_invokeEvaluate(generator, "[]", 2);
                    }
                    else {
                        zen_BinaryEntityGenerator_invokeEvaluate(generator, "[]=", 3);
                    }

                    /* The normal behaviour of the AST walker causes the generator to
                     * emit instructions in an undesirable fashion. Therefore, we partially
                     * switch from the listener to visitor design pattern. The AST walker
                     * can be guided to switch to this mode via zen_ASTListener_skipChildren()
                     * function which causes the AST walker to skip iterating over the children
                     * nodes.
                     */
                    zen_ASTListener_skipChildren(astListener);

                    break;
                }

                case ZEN_AST_NODE_TYPE_FUNCTION_ARGUMENTS: {
                    zen_FunctionArgumentsContext_t* functionArgumentsContext =
                        (zen_FunctionArgumentsContext_t*)postfixPart->m_context;

                    if (i == 0) {
                        if (zen_Symbol_isFunction(primarySymbol)) {
                            zen_FunctionSymbol_t* functionSymbol = (zen_FunctionSymbol_t*)primarySymbol->m_context;
                            int32_t index = invokeStaticIndex;
                            if (zen_Modifier_hasStatic(primarySymbol->m_modifiers)) {
                                // NOTE: I am assuming that only class members can be declared as static.
                                zen_Scope_t* enclosingScope = primarySymbol->m_enclosingScope;
                                zen_ClassScope_t* classScope = (zen_ClassScope_t*)enclosingScope->m_context;
                                zen_Symbol_t* classSymbol = classScope->m_classSymbol;
                                zen_ASTNode_t* identifier = classSymbol->m_identifier;
                                zen_Token_t* identifierToken = (zen_Token_t*)identifier->m_context;

                                uint16_t identifierIndex = zen_ConstantPoolBuilder_getStringEntryIndexEx(
                                    generator->m_constantPoolBuilder, identifierToken->m_text,
                                    identifierToken->m_length);

                                // TODO: Should retrieve a constant pool index to a class entry.
                                zen_BinaryEntityBuilder_emitLoadCPR(generator->m_builder, identifierIndex);

                                /* Log the emission of the load_cpr instruction. */
                                printf("[debug] Emitted load_cpr %d\n", identifierIndex);

                            }
                            else {
                                /* The "this" reference is always stored at the zeroth position
                                 * in the local variable array. Further, we assume that the
                                 * class member and the expression being processed appear in
                                 * in the same class. Therefore, emit a load reference to the
                                 * this reference.
                                 */
                                zen_BinaryEntityBuilder_emitLoadReference(generator->m_builder, 0);

                                /* Log the emission of the load_a instruction. */
                                printf("[debug] Emitted load_a 0\n");

                                index = invokeIndex;
                            }

                            zen_ASTNode_t* primarySymbolIdentifier = primarySymbol->m_identifier;
                            zen_Token_t* primarySymbolToken = (zen_Token_t*)primarySymbolIdentifier->m_context;

                            /* The name of the function to invoke. */
                            int32_t targetNameIndex = zen_ConstantPoolBuilder_getStringEntryIndexEx(
                                generator->m_constantPoolBuilder, primarySymbolToken->m_text,
                                primarySymbolToken->m_length);

                            /* Push the name of the target function on the operand stack. */
                            zen_BinaryEntityBuilder_emitLoadCPR(generator->m_builder,
                                targetNameIndex);

                            zen_ASTNode_t* expressions = functionArgumentsContext->m_expressions;
                            if (expressions != NULL) {
                                zen_ExpressionsContext_t* expressionsContext = (zen_ExpressionsContext_t*)expressions->m_context;
                                int32_t argumentCount = jtk_ArrayList_getSize(expressionsContext->m_expressions);
                                if (argumentCount > 0) {
                                    /* Push the size of the list onto the operand stack. */
                                    zen_BinaryEntityGenerator_loadInteger(generator, argumentCount);

                                    /* In Zen, function invocations are simulated using the ZenKerenel.dispatch()
                                     * function. It requires the arguments of the function invocation in an array.
                                     * Therefore, create an array and fill it with the arguments.
                                     *
                                     * Emit the new_array_a instruction to create the array.
                                     */
                                    zen_BinaryEntityBuilder_emitNewReferenceArray(generator->m_builder,
                                        objectClassIndex);

                                    /* Log the emission of the new_array_a instruction. */
                                    printf("[debug] Emitted new_array_a %d\n", objectClassIndex);

                                    int32_t argumentIndex;
                                    for (argumentIndex = 0; argumentIndex < argumentCount; argumentIndex++) {
                                        /* Retrieve the expression for the current argument. */
                                        zen_ASTNode_t* argument = (zen_ASTNode_t*)jtk_ArrayList_getValue(
                                            expressionsContext->m_expressions, argumentIndex);

                                        /* Duplicate the reference to the argument array. */
                                        zen_BinaryEntityBuilder_emitDuplicate(generator->m_builder);

                                        /* Log the emission of the duplicate instruction. */
                                        printf("[debug] Emitted duplicate\n");

                                        /* Push the index at which the result of the expression will be stored. */
                                        zen_BinaryEntityGenerator_loadInteger(generator, i);

                                        /* Visit the expression node and generate the relevant instructions. */
                                        zen_ASTWalker_walk(astListener, argument);

                                        /* Store the result in the argument array. */
                                        zen_BinaryEntityBuilder_emitStoreArrayReference(generator->m_builder);

                                        /* Log the emission of the store_aa instruction. */
                                        printf("[debug] Emitted store_aa\n");

                                    }

                                    index = !zen_Modifier_hasStatic(primarySymbol->m_modifiers)?
                                        invoke2Index : invokeStatic2Index;
                                }
                            }

                            /* Invoke the ZenKernel.dispatch() function to simulate a function
                             * call.
                             */
                            zen_BinaryEntityBuilder_emitInvokeStatic(generator->m_builder, index);

                            /* Log the emission of the invoke_static instruction. */
                            printf("[debug] Emitted invoke_static %d\n", index);
                        }
                        else {
                            printf("[internal error] Trying to invoke a non-function entity. The phases prior to the code generation must have malfunctioned.\n");
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

                    /* The name of the function/field to invoke/load. */
                    int32_t targetNameIndex = zen_ConstantPoolBuilder_getStringEntryIndexEx(
                    generator->m_constantPoolBuilder, identifierToken->m_text,
                    identifierToken->m_length);

                    if ((i + 1) < postfixPartCount) {
                        zen_ASTNode_t* nextPostfixPart = (zen_ASTNode_t*)jtk_ArrayList_getValue(
                            context->m_postfixParts, i + 1);
                        zen_ASTNodeType_t nextPostfixPartType = zen_ASTNode_getType(nextPostfixPart);

                        if (nextPostfixPartType == ZEN_AST_NODE_TYPE_FUNCTION_ARGUMENTS) {
                            zen_ASTNode_t* functionArguments = nextPostfixPart;
                            i++;

                            zen_FunctionArgumentsContext_t* functionArgumentsContext =
                                (zen_FunctionArgumentsContext_t*)functionArguments->m_context;

                            /* A static function invocation can occur only if the function arguments
                             * postfix part occurs at the zeroth position. This behavior is a direct
                             * result of Zen not supporting nested classes.
                             */

                            /* Push the name of the target function on the operand stack. */
                            zen_BinaryEntityBuilder_emitLoadCPR(generator->m_builder,
                                targetNameIndex);

                            /* Log the emission of the load_cpr instruction. */
                            printf("[debug] Emitted load_cpr %d\n", targetNameIndex);

                            int32_t index = invoke2Index;

                            zen_ASTNode_t* expressions = functionArgumentsContext->m_expressions;
                            if (expressions != NULL) {
                                zen_ExpressionsContext_t* expressionsContext = (zen_ExpressionsContext_t*)expressions->m_context;
                                int32_t argumentCount = jtk_ArrayList_getSize(expressionsContext->m_expressions);
                                if (argumentCount > 0) {
                                    /* Push the size of the list onto the operand stack. */
                                    zen_BinaryEntityGenerator_loadInteger(generator, argumentCount);

                                    /* In Zen, function invocations are simulated using the ZenKerenel.dispatch()
                                     * function. It requires the arguments of the function invocation in an array.
                                     * Therefore, create an array and fill it with the arguments.
                                     *
                                     * Emit the new_array_a instruction to create the array.
                                     */
                                    zen_BinaryEntityBuilder_emitNewReferenceArray(generator->m_builder,
                                        objectClassIndex);

                                    /* Log the emission of the new_array_a instruction. */
                                    printf("[debug] Emitted new_array_a %d\n", objectClassIndex);

                                    int32_t argumentIndex;
                                    for (argumentIndex = 0; argumentIndex < argumentCount; argumentIndex++) {
                                        /* Retrieve the expression for the current argument. */
                                        zen_ASTNode_t* argument = (zen_ASTNode_t*)jtk_ArrayList_getValue(
                                            expressionsContext->m_expressions, argumentIndex);

                                        /* Duplicate the reference to the argument array. */
                                        zen_BinaryEntityBuilder_emitDuplicate(generator->m_builder);

                                        /* Log the emission of the duplicate instruction. */
                                        printf("[debug] Emitted duplicate\n");

                                        /* Push the index at which the result of the expression will be stored. */
                                        zen_BinaryEntityGenerator_loadInteger(generator, i);

                                        /* Visit the expression node and generate the relevant instructions. */
                                        zen_ASTWalker_walk(astListener, argument);

                                        /* Store the result in the argument array. */
                                        zen_BinaryEntityBuilder_emitStoreArrayReference(generator->m_builder);

                                        /* Log the emission of the store_aa instruction. */
                                        printf("[debug] Emitted store_aa\n");
                                    }

                                    index = invokeIndex;
                                }
                            }

                            /* Invoke the ZenKernel.dispatch() function to simulate a function
                             * call.
                             */
                            zen_BinaryEntityBuilder_emitInvokeStatic(generator->m_builder, index);

                            /* Log the emission of the invoke_static instruction. */
                            printf("[debug] Emitted invoke_static %d\n", index);
                        }
                        else {
                            // TODO: Handle static field loads

                            /* Push the name of the field to load on the operand stack. */
                            zen_BinaryEntityBuilder_emitLoadCPR(generator->m_builder,
                                targetNameIndex);

                            /* Log the emission of the load_cpr instruction. */
                            printf("[debug] Emitted load_cpr %d\n", targetNameIndex);

                            /* Invoke the ZenKernel.loadField() function to load the value
                             * stored in a field.
                             */
                            zen_BinaryEntityBuilder_emitInvokeStatic(generator->m_builder, loadFieldIndex);

                            /* Log the emission of the invoke_static instruction. */
                            printf("[debug] Emitted invoke_static %d\n", loadFieldIndex);
                        }
                    }
                    else {
                        // TODO: Handle static field loads

                        /* Push the name of the field to load on the operand stack. */
                        zen_BinaryEntityBuilder_emitLoadCPR(generator->m_builder,
                            targetNameIndex);

                        /* Log the emission of the load_cpr instruction. */
                        printf("[debug] Emitted load_cpr %d\n", targetNameIndex);

                        /* Invoke the ZenKernel.storeField() function to update the value
                         * of the field.
                         */
                        zen_BinaryEntityBuilder_emitInvokeStatic(generator->m_builder, storeFieldIndex);

                        /* Log the emission of the invoke_static instruction. */
                        printf("[debug] Emitted invoke_static %d\n", storeFieldIndex);
                    }

                    break;
                }

                default: {
                    printf("[error] Invalid AST node type %d encountered.\n", type);
                }
            }
        }
    }
}

/*
 * The generation is divided into two categories:
 *  i. Only the primary expression is present.
 *  ii. The primary expression along with the postfix parts are present.
 *
 * ALGORITHM FOR GENERATION WHEN ONLY THE PRIMARY EXPRESSION IS PRESENT
 *
 * 1. For an identifier
 *    a. Resolve the symbol for with the given identifier.
 *    b. If there are not postfix parts and the symbol is local and variable/constant,
 *       then generate the load_a instruction.
 *    c. If the symbol is a class member and variable or constant, then
 *       generate load_instance_field or load_static_field instruction depending
 *       on the type of the member.
 *    d. If the symbol is a class, enumeration, or annotation and there are no
 *       postfix parts then generate the load_cpr instruction.
 *       Otherwise, pass the reference of the symbol to the next phase, whose
 *       algorithm is described below.
 *    e. If the symbol is a function and there are no postfix parts then generate
 *       the load_cpr instruction. Otherwise, pass the reference of the symbol
 *       to the next phase, whose algorithm is described below.
 *    f. For all other symbols, print an error message that the previous phases
 *       have malfunctioned.
 *
 * 2. For literals
 *    a. For an integer value, generate one of the following instructions depending on
 *       value: load_cpr, push_b, push_s, push_i*, and push_l*.
 *    b. For a floating-point value, generate one of the following instructions
 *       depending on the value: load_cpr, push_f*, and push_d*.
 *    c. For a string value, generate the load_cpr instruction.
 *    d. For true and false literals generate push_i1 and push_i0, respectively.
 *    e. For null literal, generate the push_null instruction.
 *
 * 3. For expressions enclosed in parenthesis, simply walk through the expressions
 *    tree.
 *
 * 4. For map and list expressions, please refer to the algorithms documented with
 *    their respective generators.
 *
 * 5. For this keyword, load the reference from the zeroth position in the local
 *    variable array.
 *
 * ALGORITHM FOR GENERATION WHEN BOTH THE PRIMARY EXPRESSION AND THE POSTFIX PARTS
 * ARE PRESENT
 *
 * 1. The subscript operator requires an object that implements the operator
 *    on the operand stack, which becomes the first operand. Therefore,
 *    generate the instructions corresponding to the primary expression and the
 *    postfix parts prior to the current postfix part. The second operand should
 *    be evaluated by walking over the tree of the expression specified inside
 *    the square brackets. After which, the ZenKernel.evaluate(operand1, operand2, '[]')
 *    call should be made.
 * 2. For function arguments
 *    a. If function arguments is the very first postfix part, then use the
 *       function symbol resolved in the previous phase. Generate instructions
 *       to invoke the ZenKernel.dispatch() function to simulate the function
 *       call.
 *    b. When function arguments postfix part occurs at a position other than the
 *       beginning of the list of postfix parts, it is processed along with the
 *       member access postfix part.
 *    In any case, the generator walks through each expression that constitutes
 *    a function argument generating instructions. The arguments are processed
 *    from left to right.
 * 3. For member access postfix parts, the subsequent postfix part is checked to
 *    see if it is a function arguments postfix part. If true, the algorithm
 *    generates instructions to invoke the ZenKernel.dispatch() function to
 *    simulate the function call.
 *    However, if the subsequent postfix part is not a function arguments postfix
 *    part, then either the load_static_field or  the load_instance_field instruction
 *    is generated.
 *    It should be noted that the identifiers in the member access are not resolved
 *    for their declaration in the symbol table. They are intended to be verified
 *    at runtime due to the dynamic nature of Zen.
 */
void zen_BinaryEntityGenerator_onExitPostfixExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_PostfixExpressionContext_t* context = (zen_PostfixExpressionContext_t*)node->m_context;
    zen_ASTNode_t* primaryExpression = context->m_primaryExpression;
    zen_PrimaryExpressionContext_t* primaryExpressionContext = (zen_PrimaryExpressionContext_t*)primaryExpression->m_context;

    if (lhs) {
        zen_BinaryEntityGenerator_handleLhsPostfixExpression(astListener,
            generator, context, primaryExpressionContext);
    }
    else {
        zen_BinaryEntityGenerator_handleRhsPostfixExpression(astListener,
            generator, context, primaryExpressionContext);
    }
}

void zen_BinaryEntityGenerator_onEnterPostfixExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_PostfixExpressionContext_t* context = (zen_PostfixExpressionContext_t*)node->m_context;

    /* The normal behaviour of the AST walker causes the generator to
     * emit instructions in an undesirable fashion. Therefore, we partially
     * switch from the listener to visitor design pattern. The AST walker
     * can be guided to switch to this mode via zen_ASTListener_skipChildren()
     * function which causes the AST walker to skip iterating over the children
     * nodes.
     */
    zen_ASTListener_skipChildren(astListener);
}

// subscript

void zen_BinaryEntityGenerator_onEnterSubscript(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitSubscript(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// functionArguments

void zen_BinaryEntityGenerator_onEnterFunctionArguments(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitFunctionArguments(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// memberAccess

void zen_BinaryEntityGenerator_onEnterMemberAccess(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitMemberAccess(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// postfixOperator

void zen_BinaryEntityGenerator_onEnterPostfixOperator(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitPostfixOperator(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// primaryExpression

void zen_BinaryEntityGenerator_onEnterPrimaryExpression(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
}

void zen_BinaryEntityGenerator_onExitPrimaryExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// mapExpression

/**
 * load_cpr size ; Push the size of the key array onto the operand stack.
 * new_array_a ; It is more efficient to create a temporary array before creating
 *             ; the hash map. Otherwise, the HashMap#putValue() should be invoked
 *             ; n number of times, where n is the number of the entries.
 *             ; The temporary array created by this instruction will store keys.
 * duplicate ; Duplicate the reference to the temporary key array.
 * load_cpr keyIndex ; Push the index at which the result of the key expression will be stored.
 * (expression) ; Evaluate the result of the key expression.
 * store_aa ; Store the result of the key expression in the temporary key array.
 *
 * load_cpr size ; Push the size of the value array onto the operand stack.
 * new_array_a ; It is more efficient to create a temporary array before creating
 *             ; the hash map. Otherwise, the HashMap#putValue() should be invoked
 *             ; n number of times, where n is the number of the entries.
 *             ; The temporary array created by this instruction will store values.
 * duplicate ; Duplicate the reference to the temporary value array.
 * load_cpr valueIndex ; Push the index at which the result of the value expression will be stored.
 * (expression) ; Evaluate the result of the value expression.
 * store_aa ; Store the result of the value expression in the temporary value array.
 *
 * new classIndex ; Create an instance of the HashMap class.
 * duplicate ; Duplicate the reference of the newly created map.
 * invoke_special functionIndex ; Invoke the constructor to initialize the new map instance.
 */
void zen_BinaryEntityGenerator_onEnterMapExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_MapExpressionContext_t* context = (zen_MapExpressionContext_t*)node->m_context;
    zen_MapEntriesContext_t* entriesContext = (zen_MapEntriesContext_t*)context->m_mapEntries->m_context;

    /* Retrieve the size of the map. */
    int32_t size = jtk_ArrayList_getSize(entriesContext->m_mapEntries);

    /* Push the size of the map onto the operand stack. The map size here indicates
     * the size of the temporary key array.
     */
    zen_BinaryEntityGenerator_loadInteger(generator, size);

    const uint8_t* objectClassName = "zen.core.Object";
    int32_t objectClassNameSize = 15;
    uint16_t objectClassIndex = zen_ConstantPoolBuilder_getClassEntryIndexEx(
        generator->m_constantPoolBuilder, objectClassName, objectClassNameSize);

    /* It is more efficient to create a temporary array before creating
     * the hash map. Otherwise, the HashMap#putValue() function
     * should be invoked n number of times, where n is the size of the array.
     *
     * Emit the new_array_a instruction to create the temporary key array.
     */
    zen_BinaryEntityBuilder_emitNewReferenceArray(generator->m_builder,
        objectClassIndex);

    /* Log the emission of the new_array_a instruction. */
    printf("[debug] Emitted new_array_a %d\n", objectClassIndex);

    int32_t i;
    for (i = 0; i < size; i++) {
        /* Retrieve the map entry for the current iteration. */
        zen_ASTNode_t* mapEntry = (zen_ASTNode_t*)jtk_ArrayList_getValue(entriesContext->m_mapEntries, i);

        /* Retrieve the context for the current map entry. */
        zen_MapEntryContext_t* mapEntryContext = (zen_MapEntryContext_t*)mapEntry->m_context;

        /* Duplicate the reference to the temporary key array. */
        zen_BinaryEntityBuilder_emitDuplicate(generator->m_builder);

        /* Log the emission of the duplicate instruction. */
        printf("[debug] Emitted duplicate\n");

        /* Push the index at which the result of the key expression will be stored. */
        zen_BinaryEntityGenerator_loadInteger(generator, i);


        /* Visit the key expression node and generate the relevant instructions. */
        zen_ASTWalker_walk(astListener, mapEntryContext->m_keyExpression);

        /* Store the result in the temporary key array. */
        zen_BinaryEntityBuilder_emitStoreArrayReference(generator->m_builder);

        /* Log the emission of the store_aa instruction. */
        printf("[debug] Emitted store_aa\n");
    }

    /* Push the size of the map onto the operand stack. The map size here indicates
     * the size of the temporary value array.
     */
    zen_BinaryEntityGenerator_loadInteger(generator, size);

    /* It is more efficient to create a temporary array before creating
     * the hash map. Otherwise, the HashMap#putValue() function
     * should be invoked n number of times, where n is the size of the value
     * array.
     *
     * TODO: Can we optimize map expressions by invoking HashMap#putValue()
     * when the number of entries is less than a certain threshold?
     *
     * Emit the new_array_a instruction to create the temporary array.
     */
    zen_BinaryEntityBuilder_emitNewReferenceArray(generator->m_builder,
        objectClassIndex);

    /* Log the emission of the new_array_a instruction. */
    printf("[debug] Emitted new_array_a %d\n", objectClassIndex);

    int32_t j;
    for (j = 0; j < size; j++) {
        /* Retrieve the map entry for the current iteration. */
        zen_ASTNode_t* mapEntry = (zen_ASTNode_t*)jtk_ArrayList_getValue(entriesContext->m_mapEntries, j);

        /* Retrieve the context for the current map entry. */
        zen_MapEntryContext_t* mapEntryContext = (zen_MapEntryContext_t*)mapEntry->m_context;

        /* Duplicate the reference to the temporary value array. */
        zen_BinaryEntityBuilder_emitDuplicate(generator->m_builder);

        /* Log the emission of the duplicate instruction. */
        printf("[debug] Emitted duplicate\n");

        /* Push the index at which the result of the value expression will be stored. */
        zen_BinaryEntityGenerator_loadInteger(generator, j);

        /* Visit the value expression node and generate the relevant instructions. */
        zen_ASTWalker_walk(astListener, mapEntryContext->m_valueExpression);

        /* Store the result in the temporary value array. */
        zen_BinaryEntityBuilder_emitStoreArrayReference(generator->m_builder);

        /* Log the emission of the store_aa instruction. */
        printf("[debug] Emitted store_aa\n");
    }

    const uint8_t* hashMapClassName = "zen/collection/map/HashMap";
    int32_t hashMapClassNameSize = 26;
    uint16_t hashMapClassIndex = zen_ConstantPoolBuilder_getClassEntryIndexEx(
        generator->m_constantPoolBuilder, hashMapClassName, hashMapClassNameSize);

    /* Create an instance of the HashMap class. */
    zen_BinaryEntityBuilder_emitNew(generator->m_builder, hashMapClassIndex);

    /* Log the emission of the new instruction. */
    printf("[debug] Emitted new %d\n", hashMapClassIndex);

    /* Duplicate the reference of the newly created map. */
    zen_BinaryEntityBuilder_emitDuplicate(generator->m_builder);

    /* Log the emission of the duplicate instruction. */
    printf("[debug] Emitted duplicate\n");

    const uint8_t* constructorDescriptor = "v:@(zen/core/Object)@(zen/core/Object)";
    int32_t constructorDescriptorSize = 38;
    const uint8_t* constructorName = "<constructor>";
    int32_t constructorNameSize = 13;
    uint16_t hashMapConstructorIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, hashMapClassName, hashMapClassNameSize,
        constructorDescriptor, constructorDescriptorSize, constructorName,
        constructorNameSize);

    /* Invoke the constructor to initialize the new map instance. */
    zen_BinaryEntityBuilder_emitInvokeSpecial(generator->m_builder,
        hashMapConstructorIndex);

    /* Log the emission of the invoke_special instruction. */
    printf("[debug] Emitted invoke_special %d\n", hashMapConstructorIndex);

    /* The normal behaviour of the AST walker causes the generator to emit instructions
     * in an undesirable fashion. Therefore, we partially switch from the listener
     * to visitor design pattern. The AST walker can be guided to switch to this
     * mode via zen_ASTListener_skipChildren() function which causes the AST walker
     * to skip iterating over the children nodes.
     */
    zen_ASTListener_skipChildren(astListener);
}

void zen_BinaryEntityGenerator_onExitMapExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// mapEntries

void zen_BinaryEntityGenerator_onEnterMapEntries(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_MapEntriesContext_t* context = (zen_MapEntriesContext_t*)node->m_context;
}

void zen_BinaryEntityGenerator_onExitMapEntries(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// mapEntry

void zen_BinaryEntityGenerator_onEnterMapEntry(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitMapEntry(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
}

// listExpression

/*
 * load_cpr size ; Push the size of the list onto the operand stack.
 * new_array_a ; It is more efficient to create a temporary array before creating
 *             ; the array list. Otherwise, the ArrayList#setValue() or ArrayList#add() functions
 *             ; should be invoked n number of times, where n is the size of the array.
 *
 * duplicate ; Duplicate the reference to the temporary array.
 * load_cpr index ; Push the index at which the result of the expression will be stored.
 * (expression) ; Evaluate the result of the expression.
 * store_aa ; Store the result in the temporary array.
 * ...
 *
 * new classIndex ; Create an instance of the ArrayList class.
 * duplicate ; Duplicate the reference of the newly created list.
 * invoke_special functionIndex ; Invoke the constructor to initialize the new list instance.
 */
void zen_BinaryEntityGenerator_onEnterListExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_ListExpressionContext_t* context = (zen_ListExpressionContext_t*)node->m_context;
    zen_ExpressionsContext_t* expressionsContext = (zen_ExpressionsContext_t*)context->m_expressions->m_context;

    /* Retrieve the size of the list. */
    int32_t size = jtk_ArrayList_getSize(expressionsContext->m_expressions);

    /* Push the size of the list onto the operand stack. */
    zen_BinaryEntityGenerator_loadInteger(generator, size);

    const uint8_t* objectClassName = "zen.core.Object";
    int32_t objectClassNameSize = 15;
    uint16_t objectClassIndex = zen_ConstantPoolBuilder_getClassEntryIndexEx(
        generator->m_constantPoolBuilder, objectClassName, objectClassNameSize);

    /* It is more efficient to create a temporary array before creating
     * the array list. Otherwise, the ArrayList#setValue() or ArrayList#add() functions
     * should be invoked n number of times, where n is the size of the array.
     *
     * Emit the new_array_a instruction to create the temporary array.
     */
    zen_BinaryEntityBuilder_emitNewReferenceArray(generator->m_builder,
        objectClassIndex);

    /* Log the emission of the new_array_a instruction. */
    printf("[debug] Emitted new_array_a %d\n", objectClassIndex);

    int32_t i;
    for (i = 0; i < size; i++) {
        /* Retrieve the expression for the current element. */
        zen_ASTNode_t* expression = (zen_ASTNode_t*)jtk_ArrayList_getValue(expressionsContext->m_expressions, i);

        /* Duplicate the reference to the temporary array. */
        zen_BinaryEntityBuilder_emitDuplicate(generator->m_builder);

        /* Log the emission of the duplicate instruction. */
        printf("[debug] Emitted duplicate\n");

        /* Push the index at which the result of the expression will be stored. */
        zen_BinaryEntityGenerator_loadInteger(generator, i);

        /* Visit the expression node and generate the relevant instructions. */
        zen_ASTWalker_walk(astListener, expression);

        /* Store the result in the temporary array. */
        zen_BinaryEntityBuilder_emitStoreArrayReference(generator->m_builder);

        /* Log the emission of the store_aa instruction. */
        printf("[debug] Emitted store_aa\n");
    }

    const uint8_t* arrayListClassName = "zen.collection.list.ArrayList";
    int32_t arrayListClassNameSize = 29;
    uint16_t arrayListClassIndex = zen_ConstantPoolBuilder_getClassEntryIndexEx(
        generator->m_constantPoolBuilder, arrayListClassName, arrayListClassNameSize);

    /* Create an instance of the ArrayList class. */
    zen_BinaryEntityBuilder_emitNew(generator->m_builder, arrayListClassIndex);

    /* Log the emission of the new instruction. */
    printf("[debug] Emitted new %d\n", arrayListClassIndex);

    /* Duplicate the reference of the newly created list. */
    zen_BinaryEntityBuilder_emitDuplicate(generator->m_builder);

    /* Log the emission of the duplicate instruction. */
    printf("[debug] Emitted duplicate\n");

    const uint8_t* constructorDescriptor = "v:@(zen/core/Object)";
    int32_t constructorDescriptorSize = 20;
    const uint8_t* constructorName = "<constructor>";
    int32_t constructorNameSize = 13;
    uint16_t arrayListConstructorIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, arrayListClassName, arrayListClassNameSize,
        constructorDescriptor, constructorDescriptorSize, constructorName,
        constructorNameSize);

    /* Invoke the constructor to initialize the new list instance. */
    zen_BinaryEntityBuilder_emitInvokeSpecial(generator->m_builder,
        arrayListConstructorIndex);

    /* Log the emission of the invoke_special instruction. */
    printf("[debug] Emitted invoke_special %d\n", arrayListConstructorIndex);

    /* The normal behaviour of the AST walker causes the generator to emit instructions
     * in an undesirable fashion. Therefore, we partially switch from the listener
     * to visitor design pattern. The AST walker can be guided to switch to this
     * mode via zen_ASTListener_skipChildren() function which causes the AST walker
     * to skip iterating over the children nodes.
     */
    zen_ASTListener_skipChildren(astListener);
}

void zen_BinaryEntityGenerator_onExitListExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// New Expression

/*
 * new classIndex ; Create an instance of the specified class.
 * duplicate ; Duplicate the reference to the newly created instance.
 * invoke_special functionIndex ; Invoke the constructor to initialize the instance.
 */
void zen_BinaryEntityGenerator_onEnterNewExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    // TODO: Debug this function when superclasses are implemented!

    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_NewExpressionContext_t* context = (zen_NewExpressionContext_t*)node->m_context;

    /* Retrieve the scope within which the new expression appears. */
    // zen_Scope_t* scope = zen_SymbolTable_getCurrentScope(generator->m_symbolTable);

    /* Retrieve the string equivalent of the type name node. */
    int32_t typeNameSize;
    uint8_t* typeNameText = zen_ASTNode_toCString(context->m_typeName, &typeNameSize);

    /* Resolve the class symbol for the type name. */
    zen_Symbol_t* symbol = zen_SymbolTable_resolve(generator->m_symbolTable, typeNameText);

    if (symbol == NULL) {
        printf("[error] Undeclared class %s\n", typeNameText);
        printf("[warning] Looks like a resolution phase failure was detected.\n");
    }

    if (!zen_Symbol_isClass(symbol)) {
        printf("[error] %s is a non-class symbol\n", typeNameText);
        printf("[warning] Looks like the syntatical phase or the resolution phase failed.\n");
    }

    zen_ClassSymbol_t* classSymbol = (zen_ClassSymbol_t*)symbol->m_context;
    /* Retrieve the scope corresponding to the class symbol. */
    zen_Scope_t* scope = zen_ClassSymbol_getClassScope(classSymbol);
    if (!zen_Scope_isClassScope(scope)) {
        printf("[error] %s is a non-class scope\n", typeNameText);
        printf("[warning] Looks like the syntatical phase or the resolution phase failed.\n");
    }

    zen_ClassScope_t* classScope = (zen_ClassScope_t*)scope->m_context;
    /* Retrieve the constructor declared in this class. */
    zen_Symbol_t* constructorSymbol = zen_ClassScope_resolve(classScope, "new");

    if (zen_Symbol_getEnclosingScope(constructorSymbol) != scope) {
        printf("[error] No constructor defined in class %s, neither explicitly nor implicity.\n", typeNameText);
        printf("[warning] Looks like a resolution phase failure was detected.\n");
    }

    if (!zen_Symbol_isFunction(constructorSymbol)) {
        printf("[error] 'new' declared as non-constructor symbol in class %s.\n", typeNameText);
        printf("[warning] Looks like the syntatical phase or the resolution phase failed.\n");
    }

    /* The binary entity format requires the identifiers of a class to be separated
     * using the forward slash character.
     */
    int32_t i;
    for (i = 0; i < typeNameSize; i++) {
        if (typeNameText[i] == '.') {
            typeNameText[i] = '/';
        }
    }
    /* Retrieve the class entry index for the type name. */
    uint16_t typeNameIndex = zen_ConstantPoolBuilder_getClassEntryIndexEx(
        generator->m_constantPoolBuilder, typeNameText, typeNameSize);

    /* Create an instance of the specified class. */
    zen_BinaryEntityBuilder_emitNew(generator->m_builder, typeNameIndex);

    /* Log the emission of the new instruction. */
    printf("[debug] Emitted new %d\n", typeNameIndex);

    /* Duplicate the reference of the newly created instance. */
    zen_BinaryEntityBuilder_emitDuplicate(generator->m_builder);

    /* Log the emission of the duplicate instruction. */
    printf("[debug] Emitted duplicate\n");

    zen_FunctionSymbol_t* functionSymbol = (zen_FunctionSymbol_t*)constructorSymbol->m_context;

    uint8_t* constructorName = "<initialize>";
    int32_t constructorNameSize = 12;
    jtk_String_t* constructorDescriptor0 = NULL;
    const uint8_t* constructorDescriptor = "v:v";
    int32_t constructorDescriptorSize = 3;

    zen_ASTNode_t* functionArguments = context->m_functionArguments;
    if (functionArguments != NULL) {
        zen_FunctionArgumentsContext_t* functionArgumentsContext
            = (zen_FunctionArgumentsContext_t*)functionArguments->m_context;

        if (functionArgumentsContext->m_expressions != NULL) {
            zen_ASTNode_t* expressions = functionArgumentsContext->m_expressions;
            zen_ExpressionsContext_t* expressionsContext = (zen_ExpressionsContext_t*)expressions->m_context;
            int32_t numberOfArguments = jtk_ArrayList_getSize(expressionsContext->m_expressions);
            int32_t parameterThreshold = zen_FunctionSymbol_getParameterThreshold(functionSymbol);

            /* NOTE: This function assumes that the previous phases were successful.
             * Therefore, it blindly generates the descriptor of the constructor.
             */
            jtk_StringBuilder_t* builder = jtk_StringBuilder_new();
            jtk_StringBuilder_appendEx_z(builder, "v:", 2);

            int32_t j;
            int32_t numberOfFixedArguments = numberOfArguments;
            int32_t numberOfVariableArguments = 0;

            if (parameterThreshold != -1) {
                numberOfFixedArguments = jtk_Integer_min(numberOfArguments, parameterThreshold);
                numberOfVariableArguments = jtk_Integer_max(0, numberOfArguments - parameterThreshold);
            }

            for (j = 0; j < numberOfFixedArguments; j++) {
                jtk_StringBuilder_appendEx_z(builder, "(zen.core.Object)", 17);

                zen_ASTNode_t* argument = (zen_ASTNode_t*)jtk_ArrayList_getValue(
                    expressionsContext->m_expressions, j);
                zen_ASTWalker_walk(astListener, argument);
            }

            /* When one of the versions of a function has a variable parameter,
             * then the function has a parameter threshold.
             */
            if ((parameterThreshold != -1) && (numberOfArguments >= parameterThreshold)) {
                // Generate the array for the variable argument.
                jtk_StringBuilder_appendEx_z(builder, "@(zen.core.Object)", 18);

                /* Evaluate the number of the variable arguments. */
                int32_t size = numberOfArguments - parameterThreshold;

                /* Push the size of the list onto the operand stack. */
                zen_BinaryEntityGenerator_loadInteger(generator, size);

                const uint8_t* objectClassName = "zen.core.Object";
                int32_t objectClassNameSize = 15;
                uint16_t objectClassIndex = zen_ConstantPoolBuilder_getClassEntryIndexEx(
                    generator->m_constantPoolBuilder, objectClassName, objectClassNameSize);

                /* Emit the new_array_a instruction to create an array to
                 * represent the variable arguments.
                 */
                zen_BinaryEntityBuilder_emitNewReferenceArray(generator->m_builder,
                    objectClassIndex);

                /* Log the emission of the new_array_a instruction. */
                printf("[debug] Emitted new_array_a %d\n", objectClassIndex);

                while (j < numberOfArguments) {
                    /* Retrieve the expression for the current argument. */
                    zen_ASTNode_t* argument = (zen_ASTNode_t*)jtk_ArrayList_getValue(
                        expressionsContext->m_expressions, j);

                    /* Duplicate the reference to the variable argument array. */
                    zen_BinaryEntityBuilder_emitDuplicate(generator->m_builder);

                    /* Log the emission of the duplicate instruction. */
                    printf("[debug] Emitted duplicate\n");

                    /* Push the index at which the result of the expression will be stored. */
                    zen_BinaryEntityGenerator_loadInteger(generator, j - parameterThreshold);

                    /* Visit the argument expression node and generate the relevant
                     * instructions.
                     */
                    zen_ASTWalker_walk(astListener, argument);

                    /* Store the result in the variable argument array. */
                    zen_BinaryEntityBuilder_emitStoreArrayReference(generator->m_builder);

                    /* Log the emission of the store_aa instruction. */
                    printf("[debug] Emitted store_aa\n");

                    j++;
                }
            }

            constructorDescriptor0 = jtk_StringBuilder_toString(builder);
            constructorDescriptor = constructorDescriptor0->m_value;
            constructorDescriptorSize = jtk_String_getSize(constructorDescriptor0);
            jtk_StringBuilder_delete(builder);
        }
    }

    uint16_t constructorIndex = zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
        generator->m_constantPoolBuilder, typeNameText, typeNameSize,
        constructorDescriptor, constructorDescriptorSize, constructorName,
        constructorNameSize);

    /* Invoke the constructor to initialize the new instance. */
    zen_BinaryEntityBuilder_emitInvokeSpecial(generator->m_builder,
        constructorIndex);

    /* Log the emission of the invoke_special instruction. */
    printf("[debug] Emitted invoke_special %d\n", constructorIndex);

    if (constructorDescriptor0 != NULL) {
        jtk_String_delete(constructorDescriptor0);
    }

    // TODO: Generate arrays when variable parameters are encountered!
    /* Delete the type name text. The typeNameText is used to generate the constructor
     * entry in the constant pool. Therefore, we delay its destruction.
     */
    jtk_CString_delete(typeNameText);
    
    /* The normal behaviour of the AST walker causes the generator to emit instructions
     * in an undesirable fashion. Therefore, we partially switch from the listener
     * to visitor design pattern. The AST walker can be guided to switch to this
     * mode via zen_ASTListener_skipChildren() function which causes the AST walker
     * to skip iterating over the children nodes.
     */
    zen_ASTListener_skipChildren(astListener);
}

void zen_BinaryEntityGenerator_onExitNewExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// var j = [ 1, 2, 3, 4 ].freeze().clone().add(5).add(5).removeIndex(2)
// var j = [ 1, 2, 3, 4 ].freeze().size