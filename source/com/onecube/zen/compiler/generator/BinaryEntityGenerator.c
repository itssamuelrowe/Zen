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
#include <jtk/core/VariableArguments.h>

#include <com/onecube/zen/compiler/ast/ASTWalker.h>
#include <com/onecube/zen/compiler/ast/context/Context.h>
#include <com/onecube/zen/compiler/ast/ASTAnnotation.h>
#include <com/onecube/zen/compiler/ast/ASTHelper.h>
#include <com/onecube/zen/compiler/lexer/Token.h>
#include <com/onecube/zen/compiler/lexer/TokenType.h>
#include <com/onecube/zen/compiler/generator/BinaryEntityBuilder.h>
#include <com/onecube/zen/compiler/symbol-table/Symbol.h>
#include <com/onecube/zen/compiler/generator/BinaryEntityGenerator.h>

// Constructor

zen_BinaryEntityGenerator_t* zen_BinaryEntityGenerator_new(
    zen_SymbolTable_t* symbolTable, zen_ASTAnnotations_t* scopes) {
    zen_BinaryEntityGenerator_t* generator = zen_Memory_allocate(zen_BinaryEntityGenerator_t, 1);
    generator->m_astListener = zen_ASTListener_newWithContext(generator);
    generator->m_builder = zen_BinaryEntityBuilder_new();
    generator->m_symbolTable = symbolTable;
    generator->m_scopes = scopes;

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

    astListener->m_onEnterForParameters = zen_BinaryEntityGenerator_onEnterForParameters;
    astListener->m_onExitForParameters = zen_BinaryEntityGenerator_onExitForParameters;

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

    jtk_Iterator_t* keyIterator = jtk_DualHashMap_getKeyIterator(generator->m_constantPool);
    while (jtk_Iterator_hasNext(keyIterator)) {
        zen_ConstantPoolEntry_t* entry = (zen_ConstantPoolEntry_t*)jtk_Iterator_getNext(keyIterator);

        if (entry->m_tag == ZEN_CONSTANT_POOL_TAG_UTF8) {
            zen_ConstantPoolUtf8_t* utf8Entry = (zen_ConstantPoolUtf8_t*)entry;
            jtk_Memory_deallocate(utf8Entry->m_bytes);
        }

        jtk_Memory_deallocate(entry);
    }
    jtk_Iterator_delete(keyIterator);

    jtk_DualHashMap_delete(generator->m_constantPool);
    zen_BinaryEntityBuilder_delete(generator->m_builder);
    zen_ASTListener_delete(generator->m_astListener);
    zen_Memory_deallocate(generator);
}

// Generate

void zen_BinaryEntityGenerator_generate(zen_BinaryEntityGenerator_t* generator,
    zen_ASTNode_t* compilationUnit, jtk_OutputStream_t* outputStream) {
    jtk_Assert_assertObject(generator, "The specified generator is null.");

    zen_ASTWalker_walk(generator->m_astListener, compilationUnit);
    zen_BinaryEntityGenerator_writeBytes(generator, outputStream);
}

// Reset

void zen_BinaryEntityGenerator_reset(zen_BinaryEntityGenerator_t* generator,
    zen_SymbolTable_t* symbolTable, zen_ASTAnnotations_t* scopes) {
    jtk_Assert_assertObject(generator, "The specified generator is null.");

    zen_BinaryEntityBuilder_clear(generator->m_builder);
    generator->m_symbolTable = symbolTable;
    generator->m_scopes = scopes;
}

// Event Handlers

void zen_BinaryEntityGenerator_onVisitErrorNode(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    fprintf(stderr, "[warning] Cannot generate binary entity on erroneous AST.");
}

void zen_BinaryEntityGenerator_onVisitTerminal(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onEnterEveryRule(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitEveryRule(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

/* compilationUnit */

void zen_BinaryEntityGenerator_onEnterCompilationUnit(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_Scope_t* scope = zen_ASTAnnotations_get(generator->m_scopes, node);
    zen_SymbolTable_setCurrentScope(generator->m_symbolTable, scope);

    /* Push the primary channel onto the channel stack. */
    generator->m_primaryChannel = zen_BinaryEntityBuilder_pushChannel(generator->m_builder);

    /* Write magic number, major version, and minor version on the main channel. */
    zen_BinaryEntityBuilder_writeMagicNumber(generator->m_builder);
    zen_BinaryEntityBuilder_writeMajorVersion(generator->m_builder);
    zen_BinaryEntityBuilder_writeMinorVersion(generator->m_builder);

    /* The stream size cannot be evaluated immediately. Therefore, push another
     * channel where the rest of the data will be written.
     */
    generator->m_secondaryChannel = zen_BinaryEntityBuilder_pushChannel(generator->m_builder);
}


// TODO: Delete constant pool entries in destructor!
void zen_BinaryEntityGenerator_onExitCompilationUnit(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_SymbolTable_invalidateCurrentScope(generator->m_symbolTable);

    /* At this point, the data should be written on the second channel. */
    zen_DataChannel_t* channel = zen_BinaryEntityBuilder_peekChannel(generator->m_builder);
    int32_t streamSize = zen_DataChannel_getSize(channel);

    /* Activate the primary channel. */
    zen_ChannelManager_activate(generator->m_builder, generator->m_primaryChannel);

    /* Write the remaining entity file headers. */
    zen_BinaryEntityBuilder_writeStreamSize(generator->m_builder, streamSize);
    zen_BinaryEntityBuilder_writeStreamFlags(generator->m_builder, 0);

    /* Prepare the constants. */

    uint16_t constantPoolCount = jtk_DualHashMap_getSize(generator->m_constantPool);

    /* The constant pool should be written according the indexes of the entries.
     * Because the dual hash map does not store the order, an array is used to
     * sort the constant pool entries. Given, each entry is associated with an
     * index, we iterate over the entries and store them at their corresponding
     * indexes to derive the sorted array.
     */
    jtk_Array_t* entries = jtk_Array_new(entryCount);

    jtk_Iterator_t* iterator = jtk_DualHashMap_getEntryIterator(generator->m_constantPool);
    while (jtk_Iterator_hasNext(iterator)) {
        jtk_HashMapEntry_t* entry = (jtk_HashMapEntry_t*)jtk_Iterator_getNext(iterator);
        zen_ConstantPoolEntry_t* constantPoolEntry = (zen_ConstantPoolEntry_t*)jtk_HashMapEntry_getKey(entry);
        uint16_t index = (uint16_t)jtk_HashMapEntry_getValue(entry);

        jtk_Array_setValue(entries, index, constantPoolEntry);
    }
    jtk_Iterator_delete(iterator);

    /* Generate the constant pool and its entries. */
    zen_BinaryEntityBuilder_activateChannel(generator->m_builder, generator->m_constantPoolIdentifier);
    zen_BinaryEntityBuilder_writeConstantPoolHeader(generator->m_builder, constantPoolCount);

    int32_t i;
    for (i = 0; i < constantPoolCount; i++) {
        zen_ConstantPoolEntry_t* entry = (zen_ConstantPoolEntry_t*)jtk_Array_getValue(entries, i);
        switch (entry->m_tag) {
            case ZEN_CONSTANT_POOL_TAG_INTEGER: {
                zen_ConstantPoolInteger_t* integerEntry = (zen_ConstantPoolInteger_t*)entry;
                zen_BinaryEntityBuilder_writeConstantPoolInteger(generator->m_builder,
                    integerEntry->m_bytes);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_LONG: {
                zen_ConstantPoolLong_t* longEntry = (zen_ConstantPoolLong_t*)entry;
                zen_BinaryEntityBuilder_writeConstantPoolLong(generator->m_builder,
                    longEntry->m_highBytes, longEntry->m_lowBytes);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FLOAT: {
                zen_ConstantPoolFloat_t* floatEntry = (zen_ConstantPoolFloat_t*)entry;
                zen_BinaryEntityBuilder_writeConstantPoolFloat(generator->m_builder,
                    floatEntry->m_bytes);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_DOUBLE: {
                zen_ConstantPoolDouble_t* doubleEntry = (zen_ConstantPoolDouble_t*)entry;
                zen_BinaryEntityBuilder_writeConstantPoolDouble(generator->m_builder,
                    doubleEntry->m_bytes);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_UTF8: {
                zen_ConstantPoolUtf_t* utf8Entry = (zen_ConstantPoolUtf8_t*)entry;
                zen_BinaryEntityBuilder_writeConstantPoolUtf(generator->m_builder,
                    utf8Entry->m_length, utf8Entry->m_bytes);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_STRING: {
                zen_ConstantPoolString_t* stringEntry = (zen_ConstantPoolString_t*)entry;
                zen_BinaryEntityBuilder_writeConstantPoolString(generator->m_builder,
                    stringEntry->m_stringIndex);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FUNCTION: {
                zen_ConstantPoolFunction_t* functionEntry = (zen_ConstantPoolFunction_t*)entry;
                zen_BinaryEntityBuilder_writeConstantPoolFunction(generator->m_builder,
                    functionEntry->m_classIndex, functionEntry->m_descriptorIndex,
                    functionEntry->m_nameIndex);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_FIELD: {
                zen_ConstantPoolField_t* fieldEntry = (zen_ConstantPoolField_t*)entry;
                zen_BinaryEntityBuilder_writeConstantPoolField(generator->m_builder,
                    fieldEntry->m_classIndex, fieldEntry->m_descriptorIndex,
                    fieldEntry->m_nameIndex);

                break;
            }

            case ZEN_CONSTANT_POOL_TAG_CLASS: {
                zen_ConstantPoolClass_t* classEntry = (zen_ConstantPoolClass_t*)entry;
                zen_BinaryEntityBuilder_writeConstantPoolClass(generator->m_builder,
                    fieldEntry->m_nameIndex);

                break;
            }
        }
    }

    /* The array of constant pool entries is not required anymore. Therefore,
     * destroy it.
     */
    jtk_Array_delete(entries);

    /* Merge the secondary channel with the primary channel. */
    zen_ChannelManager_merge(generator->m_channels, generator->m_primaryChannel,
        generator->m_secondaryChannel);
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

void zen_BinaryEntityGenerator_onEnterFunctionDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_FunctionDeclarationContext_t* context = (zen_FunctionDeclarationContext_t*)node->m_context;

    jtk_Token_t* identifier = (zen_Token_t*)(context->m_identifier);
    jtk_String_t* descriptor = zen_BinaryEntityGenerator_getDescriptor(generator, context->m_functionParameters);

    zen_Scope_t* scope = zen_ASTAnnotations_get(generator->m_scopes, node);
    zen_SymbolTable_setCurrentScope(generator->m_symbolTable, scope);

    uint16_t nameIndex = zen_BinaryEntityGenerator_getConstantPoolUtf8IndexEx(
        generator, identifier->m_text, identifier->m_length);
    uint16_t descriptorIndex = zen_BinaryEntityGenerator_getConstantPoolUtf8Index(
        generator, descriptor);
    uint16_t flags = 0;

    jtk_String_delete(descriptor);

    zen_ChannelManager_activate(generator->m_channels, generator->m_functionChannel);
    zen_BinaryEntityBuilder_writeFunction(generator->m_builder, nameIndex,
        descriptorIndex, flags);
}

void zen_BinaryEntityGenerator_onExitFunctionDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;

    zen_ChannelManager_commit(generator->m_channels, generator->m_codeChannel,
        generator->m_functionChannel);

    zen_SymbolTable_invalidateCurrentScope(generator->m_symbolTable);
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

void zen_BinaryEntityGenerator_onEnterVariableDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    /*for () {
        if (variableDeclarator->m_expression != NULL) {
            zen_BinaryEntityGenerator_storeReference(variableDeclarator);
        }
    }*/
}

void zen_BinaryEntityGenerator_onEnterExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_ExpressionContext_t* context = (zen_AssignmentExpressionContext_t*)node->m_context;

    zen_ASTAnnotation_t* annotation = zen_ASTAnnotations_get(generator->m_annotations, node);
    if (annotation != NULL) {
        if (zen_ASTAnnotation_getType(annotation) == ZEN_AST_ANNOTATION_TYPE_ASYMETRICAL_CHANNEL_MANAGEMENT) {
            zen_BinaryEntityBuilder_pushChannel(generator->m_builder);
        }
    }
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

void zen_ASTListener_onEnterBreakStatement(zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// breakStatement

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

// whileStatement

void zen_BinaryEntityGenerator_onEnterWhileStatement(zen_BinaryEntityGenerator_t* generator) {
    zen_BinaryEntityBuilder_pushChannel(generator->m_builder);
}

void zen_BinaryEntityGenerator_onExitWhileStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;

    zen_ByteCodeChannel_t* statementSuiteChannel = zen_ChannelManager_peek(generator->m_channelManager);
    uint32_t index = zen_ByteCodeChannel_getSize(statementSuiteChannel);
    zen_BinaryEntityGenerator_emitJump(generator, index);
    zen_ChannelManager_merge(generator->m_channelManager);
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

// classDeclaration

// BUG: Looks like we are not setting the current scope in this function.
void zen_BinaryEntityGenerator_onEnterClassDeclaration(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_ClassDeclarationContext_t* context =
        (zen_ClassDeclarationContext_t*)node->m_context;

    jtk_Token_t* identifier = (zen_Token_t*)(context->m_identifier);
    jtk_String_t* reference = jtk_String_newFromJoinEx(generator->m_package->m_value,
        generator->m_package->m_size, identifier->m_text, identifier->m_length);

    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    generator->m_classChannel = zen_BinaryEntityBuilder_pushChannel(generator->m_builder);

    uint16_t flags = 0;
    uint16_t referenceIndex =
        zen_BinaryEntityGenerator_getConstantPoolUtf8Index(generator, reference);
    uint16_t* superclassIndexes = NULL;
    uint16_t superclassCount = 0;

    jtk_String_delete(reference);

    if (node->m_extendsClause != NULL) {
        zen_ClassExtendsClauseContext_t* extendsClauseContext =
            (zen_ClassExtendsClauseContext_t*)node->m_extendsClause->m_context;

        superclassCount = jtk_ArrayList_getSize(extendsClauseContext->m_typeNames);
        superclassIndexes = jtk_Memory_allocate(uint16_t, superclassCount);

        /* The entity generator does not ensure that the validity of the
         * identifiers.
         */
        jtk_Iterator_t* iterator = jtk_ArrayList_getIterator();
        int32_t index;
        for (index = 0; index < superclassCount; index++) {
            zen_ASTNode_t* typeNameNode = (zen_ASTNode_t*)jtk_ArrayList_getValue(extendsClauseContext->m_typeNames, index);
            zen_TypeNameContext_t* typeNameContext = (zen_TypeNameContext_t*)typeNameNode->m_context;

            zen_Symbol_t* symbol = zen_Scope_resolveQualifiedSymbol(
                currentScope, typeNameContext->m_identifiers);
            if (zen_Symbol_isClass(symbol)) {
                zen_ClassSymbol_t* classSymbol = (zen_ClassSymbol_t*)symbol->m_context;
                jtk_String_t* qualifiedName = zen_ClassSymbol_getQualifiedName(classSymbol);

                uint16_t superclassIndex = zen_BinaryEntityBuilder_getConstantPoolClassIndex(
                    generator->m_builder, qualifiedName);
                superclassIndexes[index] = superclassIndex;
            }
            else {
                // Error: Looks like the semantic anlysis failed.
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
        superclassIndexes[0] = zen_BinaryEntityBuilder_getConstantPoolClassIndex(
            generator->m_builder, objectClassName);

        jtk_String_delete(objectClassName);
    }

    zen_BinaryEntityBuilder_writeClass(generator->m_builder, flags, referenceIndex,
        superclassCount, superclassIndexes);
    zen_BinaryEntityBuilder_writeAttributeCount(0);
}

// BUG
void zen_BinaryEntityGenerator_onExitClassDeclaration(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_ClassDeclarationContext_t* context =
        (zen_ClassDeclarationContext_t*)node->m_context;

    /* At this point, the class channel should contain the class entity header.
     * The field channel should contain all the field entities. Therefore, we
     * need to merge the class and field channels together.
     *
     * The channels are separated by field count. Therefore, activate the
     * class channel and write the field count before merging.
     */
    zen_ChannelManager_activate(generator->m_channels, generator->m_classChannel);

    /* Write the number of fields in this class. */
    zen_BinaryEntityBuilder_writeFieldCount(generator->m_builder, generator->m_fieldCount);

    /* Merge the class and field channels. Further, the data in the field channel
     * is not required anymore. Therefore, clear it.
     *
     * It should be noted that the commit function both merges and clears the
     * source channel.
     */
    zen_ChannelManager_commit(generator->m_channels, generator->m_fieldChannel,
        generator->m_classChannel);

    /* At this point, the class channel should contain the class entity header
     * and the fields. Further, the function channel should contain all the
     * function entities. Therefore, we need to merge the class and function
     * channels together.
     *
     * The channels are separated by function count. Therefore, activate the
     * class channel and write the function count before merging.
     */
    zen_BinaryEntityBuilder_writeFunctionCount(generator->m_builder, generator->m_functionCount);

    /* Merge the class and function channels. Further, the data in the function
     * channel is not required anymore. Therefore, clear it.
     *
     * It should be noted that the commit function both merges and clears the
     * source channel.
     */
    zen_ChannelManager_commit(generator->m_channels, generator->m_functionChannel,
        generator->m_classChannel);

    zen_SymbolTable_invalidateCurrentScope(generator->m_symbolTable);
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

// How to differentiate between function calls

/*
 *    ...
 *    invoke_virtual #function_descriptor_index
 *    store_a #x
 */
void zen_BinaryEntityGenerator_onEnterAssignmentExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_AssignmentExpressionContext_t* context = (zen_AssignmentExpressionContext_t*)node->m_context;

    /* The resolution phase ensures that the expression on the left hand
     * side of the assignment operator evaluates to an identifier or
     * field access.
     */
    // jtk_ArrayList_t* children = zen_ArrayList_new();
    // zen_ASTHelper_getFilteredTokens(context->m_conditionalExpression, children, ZEN_TOKEN_IDENTIFIER);

    /* The following line is valid when zen_ASTHelper_getFilteredTerminalNodes() function
     * is invoked instead of zen_ASTHelper_getFilteredTokens().
     */
    // zen_ASTNode_t* identifier = (zen_ASTNode_t*)zen_ArrayList_get(children, 0);
    // zen_Token_t* identifierToken = (zen_Token_t*)zen_ArrayList_get(children, 0);
    // const uint8_t* identifierText = zen_Token_getText(identifierToken);

    // zen_Symbol_t* symbol = zen_SymbolTable_resolve(generator->m_symbolTable, identifierText);
    // if (zen_Symbol_isVariable(symbol)) {
       //  int32_t targetIndex;
        // bool localVariable = false;

        // zen_Scope_t* enclosingScope = zen_Symbol_getEnclosingScope(symbol);
        // if (zen_Scope_isClassScope(enclosingScope)) {
           //  targetIndex = zen_BinaryEntityGenerator_findFieldDescriptorIndex(generator, children);
        // }
        // else {
           /* int32_t size = zen_ArrayList_getSize(children);
            if (size == 1) {
                targetIndex = zen_BinaryEntityGenerator_findLocalVariableIndex(generator, identifierText);
                localVariable = true;
            }
            else {
                targetIndex = zen_BinaryEntityGenerator_findFieldDescriptorIndex(generator, children);
            }
        }

        zen_BinaryEntityGenerator_emitLoadReference(generator, targetIndex);

        zen_ASTNode_t* assignmentOperator = context->m_assignmentOperator;
        zen_Token_t* operatorToken = (zen_Token_t*)assignmentOperator->m_context;
        switch (operatorToken->m_type) {
            case ZEN_TOKEN_EQUAL: {
                break;
            }

            case ZEN_TOKEN_PLUS_EQUAL: {
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, "Czen.base.Object;plusOperator(Czen.base.Object;)Czen.base.Object;");
                break;
            }

            case ZEN_TOKEN_DASH_EQUAL: {
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, "Czen.base.Object;minusOperator(Czen.base.Object;)Czen.base.Object;");
                break;
            }

            default: {
                // [internal error]
            }
        }

        void** state = zen_Memory_allocate(void*, ZEN_ASSIGNMENT_EXPRESSION_STATE_SIZE);
        state[ZEN_ASSIGNMENT_EXPRESSION_STATE_TARGET_INDEX] = (void*)targetIndex;
        state[ZEN_ASSIGNMENT_EXPRESSION_STATE_LOCAL_VARIABLE] = (void*)localVariable;
        zen_ASTAnnotations_put(generator->m_annotations, node, state);

    }
    else if (zen_Symbol_isConstant(symbol)) {
        // error: Constant cannot be assigned
    }
    else {
        // error: LHS is not variable
    }*/

    // zen_ArrayList_delete(children);
}

void zen_BinaryEntityGenerator_onExitAssignmentExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_AssignmentExpressionContext_t* context = (zen_AssignmentExpressionContext_t*)node->m_context;

    if (context->m_assignmentOperator != NULL) {
        zen_ASTNode_t* assignmentOperator = context->m_assignmentOperator;
        zen_Token_t* operatorToken = (zen_Token_t*)assignmentOperator->m_context;
        switch (operatorToken->m_type) {
            /* The equal operator only stores a reference. Therefore, it requires no
             * special implementation.
             */
            /*
            case ZEN_TOKEN_EQUAL: {
                break;
            }
            */

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
    }
}

// conditionalExpression

void zen_BinaryEntityGenerator_onEnterConditionalExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)listener->m_context;
    zen_ConditionalExpressionContext_t* context = (zen_ConditionalExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityGenerator_onExitConditionalExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)listener->m_context;
    zen_ConditionalExpressionContext_t* context = (zen_ConditionalExpressionContext_t*)node->m_context;

    /* TODO */
}

// logicalOrExpression

void zen_BinaryEntityGenerator_onEnterLogicalOrExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)listener->m_context;
    zen_LogicalOrExpressionContext_t* context = (zen_LogicalOrExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityGenerator_onExitLogicalOrExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)listener->m_context;
    zen_LogicalOrExpressionContext_t* context = (zen_LogicalOrExpressionContext_t*)node->m_context;

    /* TODO */
}

// logicalAndExpression

void zen_BinaryEntityGenerator_onEnterLogicalAndExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)listener->m_context;
    zen_LogicalAndExpressionContext_t* context = (zen_LogicalAndExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityGenerator_onExitLogicalAndExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)listener->m_context;
    zen_LogicalAndExpressionContext_t* context = (zen_LogicalAndExpressionContext_t*)node->m_context;

    /* TODO */
}

// inclusiveOrExpression

void zen_BinaryEntityGenerator_onEnterInclusiveOrExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)listener->m_context;
    zen_InclusiveOrExpressionContext_t* context = (zen_InclusiveOrExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityGenerator_onExitInclusiveOrExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)listener->m_context;
    zen_InclusiveOrExpressionContext_t* context = (zen_InclusiveOrExpressionContext_t*)node->m_context;

    if (context->m_inclusiveOrExpression != NULL) {
        zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
    }
}

// exclusiveOrExpression

void zen_BinaryEntityGenerator_onEnterExclusiveOrExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)listener->m_context;
    zen_ExclusiveOrExpressionContext_t* context = (zen_ExclusiveOrExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityGenerator_onExitExclusiveOrExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)listener->m_context;
    zen_ExclusiveOrExpressionContext_t* context = (zen_ExclusiveOrExpressionContext_t*)node->m_context;

    if (context->m_exclusiveOrExpression != NULL) {
        zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
    }
}

// andExpression

void zen_BinaryEntityGenerator_onEnterAndExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)listener->m_context;
    zen_AndExpressionContext_t* context = (zen_AndExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityGenerator_onExitAndExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)listener->m_context;
    zen_AndExpressionContext_t* context = (zen_AndExpressionContext_t*)node->m_context;

    if (context->m_andExpression != NULL) {
        zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
    }
}

// equalityExpression

void zen_BinaryEntityGenerator_onEnterEqualityExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)listener->m_context;
    zen_EqualityExpressionContext_t* context = (zen_EqualityExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityGenerator_onExitEqualityExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)listener->m_context;
    zen_EqualityExpressionContext_t* context = (zen_EqualityExpressionContext_t*)node->m_context;

    zen_ASTNode_t* equalityOperator = context->m_equalityOperator;
    if (equalityOperator != NULL) {
        zen_Token_t* equalityOperatorToken = (zen_Token_t*)(equalityOperator->m_context);
        zen_TokenType_t equalityOperatorTokenType = zen_Token_getType(equalityOperatorToken);

        switch (equalityOperatorTokenType) {
            case ZEN_TOKEN_EQUAL_2: {
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                break;
            }

            case ZEN_TOKEN_EXCLAMATION_MARK_EQUAL: {
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                break;
            }
        }
    }
}

// relationalExpression

void zen_BinaryEntityGenerator_onEnterRelationalExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)listener->m_context;
    zen_RelationalExpressionContext_t* context = (zen_RelationalExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityGenerator_onExitRelationalExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)listener->m_context;
    zen_RelationalExpressionContext_t* context = (zen_RelationalExpressionContext_t*)node->m_context;

    zen_ASTNode_t* relationalOperator = context->m_relationalOperator;
    if (relationalOperator != NULL) {
        zen_Token_t* relationalOperatorToken = (zen_Token_t*)(relationalOperator->m_context);
        zen_TokenType_t relationalOperatorTokenType = zen_Token_getType(relationalOperatorToken);
        switch (relationalOperatorTokenType) {
            case ZEN_TOKEN_LEFT_ANGLE_BRACKET: {
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                break;
            }

            case ZEN_TOKEN_RIGHT_ANGLE_BRACKET: {
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                break;
            }

            case ZEN_TOKEN_LEFT_ANGLE_BRACKET_EQUAL: {
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                break;
            }

            case ZEN_TOKEN_RIGHT_ANGLE_BRACKET_EQUAL: {
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                break;
            }

            case ZEN_TOKEN_KEYWORD_IS: {
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                break;
            }
        }
    }
}

// shiftExpression

void zen_BinaryEntityGenerator_onEnterShiftExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_ShiftExpressionContext_t* context = (zen_ShiftExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityGenerator_onExitShiftExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_ShiftExpressionContext_t* context = (zen_ShiftExpressionContext_t*)node->m_context;

    zen_ASTNode_t* shiftOperator = context->m_shiftOperator;
    if (shiftOperator != NULL) {
        zen_Token_t* shiftOperatorToken = (zen_Token_t*)(shiftOperator->m_context);
        zen_TokenType_t shiftOperatorTokenType = zen_Token_getType(shiftOperatorToken);
        switch (shiftOperatorTokenType) {
            case ZEN_TOKEN_LEFT_ANGLE_BRACKET_2: {
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                break;
            }

            case ZEN_TOKEN_RIGHT_ANGLE_BRACKET_2: {
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                break;
            }

            case ZEN_TOKEN_RIGHT_ANGLE_BRACKET_3: {
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                break;
            }
        }
    }
}

// additiveExpression

void zen_BinaryEntityGenerator_onEnterAdditiveExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_AdditiveExpressionContext_t* context = (zen_AdditiveExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityGenerator_onExitAdditiveExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_AdditiveExpressionContext_t* context = (zen_AdditiveExpressionContext_t*)node->m_context;

    zen_ASTNode_t* additiveOperator = context->m_additiveOperator;
    if (additiveOperator != NULL) {
        zen_Token_t* additiveOperatorToken = (zen_Token_t*)(additiveOperator->m_context);
        zen_TokenType_t additiveOperatorTokenType = zen_Token_getType(additiveOperatorToken);
        switch (additiveOperatorTokenType) {
            case ZEN_TOKEN_PLUS: {
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, /* "..." */ 0);
                break;
            }

            case ZEN_TOKEN_DASH: {
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                break;
            }

            default: {
                fprintf(stderr, "[internal error] Control should not reach here.\n");
                break;
            }
        }
    }
}

// multiplicativeExpression

void zen_BinaryEntityGenerator_onEnterMultiplicativeExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)listener->m_context;
    zen_MultiplicativeExpressionContext_t* context = (zen_MultiplicativeExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityGenerator_onExitMultiplicativeExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)listener->m_context;
    zen_MultiplicativeExpressionContext_t* context = (zen_MultiplicativeExpressionContext_t*)node->m_context;

    zen_ASTNode_t* multiplicativeOperator = context->m_multiplicativeOperator;
    if (multiplicativeOperator != NULL) {
        zen_Token_t* multiplicativeOperatorToken = (zen_Token_t*)(multiplicativeOperator->m_context);
        zen_TokenType_t multiplicativeOperatorTokenType = zen_Token_getType(multiplicativeOperatorToken);
        switch (multiplicativeOperatorTokenType) {
            case ZEN_TOKEN_ASTERISK: {
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                break;
            }

            case ZEN_TOKEN_FORWARD_SLASH: {
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                break;
            }

            case ZEN_TOKEN_MODULUS: {
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                break;
            }
        }
    }
}

// unaryExpression

void zen_BinaryEntityGenerator_onEnterUnaryExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)listener->m_context;
    zen_UnaryExpressionContext_t* context = (zen_UnaryExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityGenerator_onExitUnaryExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)listener->m_context;
    zen_UnaryExpressionContext_t* context = (zen_UnaryExpressionContext_t*)node->m_context;

    zen_ASTNode_t* unaryOperator = context->m_unaryOperator;
    if (unaryOperator != NULL) {
        zen_Token_t* unaryOperatorToken = (zen_Token_t*)unaryOperator->m_context;
        zen_TokenType_t unaryOperatorType = zen_Token_getType(unaryOperatorToken);
        switch (unaryOperatorType) {
            case ZEN_TOKEN_PLUS: {
                /* The UnaryPlusOperator.applyUnaryPlus() function is invoked against the object
                 * whose "positive" internal state is required. It returns an object with its
                 * internal state "positive".
                 */
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                break;
            }

            case ZEN_TOKEN_DASH: {
                /* The UnaryPlusOperator.applyUnaryPlus() function is invoked against the object
                 * whose "negative" internal state is required. It returns an object with its
                 * internal state "negative".
                 */
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                break;
            }

            case ZEN_TOKEN_TILDE: {
                /* The UnaryPlusOperator.applyUnaryPlus() function is invoked against the object
                 * whose "deeply toggled" internal state is required. It returns an object with its
                 * internal state "deeply toggled".
                 */
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                break;
            }

            case ZEN_TOKEN_EXCLAMATION_MARK: {
                /* The UnaryPlusOperator.applyUnaryPlus() function is invoked against the object
                 * whose "toggled" internal state is required. It returns an object with its
                 * internal state "toggled".
                 */
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                break;
            }

            case ZEN_TOKEN_PLUS_2:
            case ZEN_TOKEN_DASH_2: {
                if (unaryOperatorType == ZEN_TOKEN_PLUS_2) {
                    /* The onPreIncrement() function is invoked against the object whose internal
                     * state has to be "incremented by 1". It returns an object with its internal
                     * state "incremented by 1".
                     */
                    zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                }
                else {
                    /* The onPreDecrement() function is invoked against the object whose internal
                     * state has to be "incremented by 1". It returns an object with its internal
                     * state "incremented by 1".
                     */
                    zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                }
                /* A copy of the "incremented" object is required on the operand stack for
                 * assignment.
                 */
                zen_BinaryEntityGenerator_emitDuplicate(generator);
                /* Assign the variable the object which represents the new state.
                 *
                 * TODO: Change store_a to store_a1 (and friends) and
                 *       store_field when necessary.
                 */
                zen_BinaryEntityGenerator_emitStoreReference(generator, 0);

                break;
            }
        }
    }
}

// postfixExpression

void zen_BinaryEntityGenerator_onEnterPostfixExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitPostfixExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_PostfixExpressionContext_t* context = (zen_PostfixExpressionContext_t*)node->m_context;

    int32_t size = zen_ArrayList_getSize(context->m_postfixParts);
    int32_t i;
    for (i = 0; i < size; i++) {
        zen_ASTNode_t* postfixPart = (zen_ASTNode_t*)zen_ArrayList_get(context->m_postfixParts, i);
        switch (zen_ASTNode_getType(postfixPart)) {
            /*
            case ZEN_AST_NODE_SUBSCRIPT: {
                zen_SubscriptContext_t* subscriptContext = (zen_SubscriptContext_t*)postfixPart->m_context;
                // TODO: Determine whether this invocation is a load or assignment.
                zen_BinaryEntityGenerator_emitLoadArray(generator);
                break;
            }
            */

            case ZEN_AST_NODE_POSTFIX_OPERATOR: {
                zen_PostfixOperatorContext_t* postfixOperatorContext = (zen_PostfixOperatorContext_t*)postfixPart->m_context;
                zen_Token_t* postfixOperator = (zen_Token_t*)(postfixOperatorContext->m_postfixOperator->m_context);

                /* The virtual onPostIncrement() function is invoked against the object.
                 * Therefore, a duplicate of it is necessary on the operand stack.
                 */
                zen_BinaryEntityGenerator_emitDuplicate(generator);
                if (zen_Token_getType(postfixOperator) == ZEN_TOKEN_PLUS_2) {
                    /* The onPostIncrement() function is invoked against the object whose internal
                     * state has to be "incremented by 1". It returns an object with its internal
                     * state "incremented by 1".
                     */
                    // TODO: Emit an invocation to PostIncrementOperator.applyPostDecrement()
                    zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                }
                else {
                    /* The onPostDecrement() function is invoked against the object whose internal
                     * state has to be "incremented by 1". It returns an object with its internal
                     * state "incremented by 1".
                     */
                    // TODO: Emit an invocation to PostDecrementOperator.applyPostDecrement()
                    zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                }
                /* Assign the variable the object which represents the new state.
                 *
                 * TODO: Change store_a to store_a1 (and friends) and
                 *       store_field when necessary.
                 */
                zen_BinaryEntityGenerator_emitStoreReference(generator, 0);

                break;
            }
        }
    }
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

void zen_BinaryEntityGenerator_onEnterPrimaryExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_PrimaryExpressionContext_t* context = (zen_PrimaryExpressionContext_t*)node->m_context;

    if (zen_ASTNode_isTerminal(context->m_expression)) {
        zen_Token_t* token = (zen_Token_t*)context->m_expression->m_context;
        switch (zen_Token_getType(token)) {
            case ZEN_TOKEN_IDENTIFIER: {
                zen_ASTNode_t* assignmentExpression = zen_ASTHelper_getAncestor(context->m_expression, ZEN_AST_NODE_ASSIGNMENT_EXPRESSION);
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

                const uint8_t* identifierText = zen_Token_getText(token);
                zen_Symbol_t* symbol = zen_SymbolTable_resolve(generator->m_symbolTable, identifierText);
                zen_Scope_t* enclosingScope = zen_Symbol_getEnclosingScope(symbol);
                if (zen_Scope_isClassScope(enclosingScope)) {
                    zen_BinaryEntityGenerator_emitLoadReference(generator, 0);
                    zen_BinaryEntityGenerator_emitLoadInstanceField(generator, 10);
                }
                else {
                    zen_BinaryEntityGenerator_emitLoadReference(generator, 10);
                }
                break;
            }

            case ZEN_TOKEN_INTEGER_LITERAL: {
                const uint8_t* integerText = zen_Token_getText(token);
                zen_BinaryEntityGenerator_emitPushIntegerEx(generator, integerText);
                zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0);
                break;
            }

            case ZEN_TOKEN_KEYWORD_TRUE: {
                zen_BinaryEntityGenerator_emitPushTrue(generator);
                break;
            }

            case ZEN_TOKEN_KEYWORD_FALSE: {
                zen_BinaryEntityGenerator_emitPushFalse(generator);
                break;
            }

            case ZEN_TOKEN_STRING_LITERAL: {
                zen_BinaryEntityGenerator_emitLoadCpr(generator, 0);
                break;
            }

            case ZEN_TOKEN_KEYWORD_NULL: {
                zen_BinaryEntityGenerator_emitPushNull(generator);
                break;
            }

            case ZEN_TOKEN_KEYWORD_UNDEFINED: {
                zen_BinaryEntityGenerator_emitPushUndefined(generator);
                break;
            }

            default: {
                /* [internal error] */
            }
        }
    }
}

void zen_BinaryEntityGenerator_onExitPrimaryExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

// mapExpression

void zen_BinaryEntityGenerator_onEnterMapExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_MapExpressionContext_t* context = (zen_MapExpressionContext_t*)node->m_context;

    zen_BinaryEntityGenerator_emitPushByte(generator, 0); // TODO: With proper size
    zen_BinaryEntityGenerator_emitNewReferenceArray(generator, 0); // TODO: new_array_n
}

void zen_BinaryEntityGenerator_onExitMapExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_MapExpressionContext_t* context = (zen_MapExpressionContext_t*)node->m_context;

    zen_BinaryEntityGenerator_emitInvokeVirtual(generator, 0); // TODO
}

// mapEntries

void zen_BinaryEntityGenerator_onEnterMapEntries(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_MapEntriesContext_t* context = (zen_MapEntriesContext_t*)node->m_context;

    /*
    int32_t i;
    for (i = 0; i < size; i++) {
        zen_ASTNode_t* mapEntry = (zen_ASTNode_t*)zen_ArrayList_get(context->m_mapEntries, i);
        zen_ASTAnnotation_t* mapEntryAnnotation = zen_ASTAnnotation_new(ZEN_AST_ANNOTATION_TYPE_ASYMETRICAL_CHANNEL_MANAGEMENT, NULL);
        zen_ASTAnnotations_put(generator->m_annotations, mapEntryAnnotation);
    }
    */
}

void zen_BinaryEntityGenerator_onExitMapEntries(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_MapEntriesContext_t* context = (zen_MapEntriesContext_t*)node->m_context;

    /* TODO: Apparently, storing integers as void* is dangerous. Find an alternative. */

    int32_t size = zen_ArrayList_getSize(context->m_mapEntries);
    // TODO: Filter out map with 0 entries!
    /* Currently, a map entry channel is on top of the stack. Whatever,
     * byte codes are emitted will be placed on it. Therefore, we need to
     * swap the map entry channel with the local channel.
     */
    zen_ChannelManager_swap(generator->m_channelManager, 0, size);

    int32_t i;
    for (i = 0; i < size; i++) {
        /* The instructions which insert the second dimension array into the
         * first dimension array.
         *
         * The instructions are generated in the reverse order because the
         * channels are stored in a special stack, with an exception of the last
         * element. The last element was previously swapped with the local
         * channel which was previously on the bottom of the stack. Thus, the
         * last element is swapped from top to bottom of the stack. Moreover,
         * the execution order may remain the same for constant entries, but
         * changes for variable entries when the stack ordered in followed.
         */

        /* A textual representation of the channels for a map with four
         * entries.
         *
         * [0] local channel
         * [1] entry_channel_3
         * [2] entry_channel_2
         * [3] entry_channel_1
         * [4] entry_channel_4
         * ...
         */

        /* A new array was previously created for the map entries. Create a
         * duplicate of its reference on the operand stack for calling the
         * {@code store_aa} instruction.
         */

        /* Create a duplicate of the 2-d array. */
        zen_BinaryEntityGenerator_emitDuplicate(generator);
        zen_BinaryEntityGenerator_emitPushByte(generator, i);

        int32_t entryChannelIndex = ((i + 1) == size)? 1 : (size - i - 1);
        zen_ChannelManager_append(generator->m_channelManager, entryChannelIndex, 0);
        zen_ChannelManager_remove(generator->m_channelManager, entryChannelIndex);

        zen_BinaryEntityGenerator_emitStoreReferenceArray(generator);
    }
}

// mapEntry

void zen_BinaryEntityGenerator_onEnterMapEntry(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_MapEntryContext_t* context = (zen_MapEntryContext_t*)node->m_context;

    /* The byte codes generated for every map entry is redirected on a new
     * channel. Thus, an oppurtunity to insert some arbitrary instructions
     * between the map entries is provided.
     */
    zen_BinaryEntityBuilder_pushChannel(generator->m_builder);

    zen_BinaryEntityGenerator_emitPushByte(generator, 2);
    zen_BinaryEntityGenerator_emitNewReferenceArray(generator, 0);

    zen_ASTAnnotation_t* keyExpressionAnnotation = zen_ASTAnnotation_new(ZEN_AST_ANNOTATION_TYPE_ASYMETRICAL_CHANNEL_MANAGEMENT, NULL);
    zen_ASTAnnotations_put(generator->m_annotations, context->m_keyExpression, keyExpressionAnnotation);

    zen_ASTAnnotation_t* valueExpressionAnnotation = zen_ASTAnnotation_new(ZEN_AST_ANNOTATION_TYPE_ASYMETRICAL_CHANNEL_MANAGEMENT, NULL);
    zen_ASTAnnotations_put(generator->m_annotations, context->m_valueExpression, valueExpressionAnnotation);
}

void zen_BinaryEntityGenerator_onExitMapEntry(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_MapEntryContext_t* context = (zen_MapEntryContext_t*)node->m_context;

    /* Before swapping:
     *
     *   value_expression_channel
     *   key_expression_channel
     *   map_entry_channel
     *   ...
     */

    /* Currently, the value expression channel is on top of the stack. Whatever,
     * byte codes are emitted will be placed on it. Therefore, we need to swap
     * the value expression channel with the local channel.
     */
    zen_ChannelManager_swap(generator->m_channelManager, 0, 2);

    /* After swapping:
     *
     *   [0] map_entry_channel
     *   [1] key_expression_channel
     *   [2] value_expression_channel
     *   ...
     */

    /* The following section generates instructions which insert the key into
     * the array in second dimension.
     */
    /* A new array was previously created for this map entry. Create a duplicate
     * of its reference on the operand stack for calling the {@code store_aa}
     * instruction.
     */
    zen_BinaryEntityGenerator_emitDuplicate(generator);
    zen_BinaryEntityGenerator_emitPushByte(generator, 0); // TODO: push_i0
    zen_ChannelManager_append(generator->m_channelManager, 1, 0);
    zen_ChannelManager_remove(generator->m_channelManager, 1);
    zen_BinaryEntityGenerator_emitStoreReferenceArray(generator);

    /* The following section generates instructions which insert the value into
     * the array in second dimension.
     */
    /* A new array was previously created for this map entry. Create a duplicate
     * of its reference on the operand stack for calling the {@code store_aa}
     * instruction.
     */
    zen_BinaryEntityGenerator_emitDuplicate(generator);
    zen_BinaryEntityGenerator_emitPushByte(generator, 1);
    zen_ChannelManager_append(generator->m_channelManager, 1, 0);
    zen_ChannelManager_remove(generator->m_channelManager, 1);
    zen_BinaryEntityGenerator_emitStoreReferenceArray(generator);

    zen_ASTAnnotation_t* keyExpressionAnnotation = zen_ASTAnnotations_get(generator->m_annotations, context->m_keyExpression);
    zen_ASTAnnotation_t* valueExpressionAnnotation = zen_ASTAnnotations_get(generator->m_annotations, context->m_valueExpression);

    zen_ASTAnnotation_delete(keyExpressionAnnotation);
    zen_ASTAnnotation_delete(valueExpressionAnnotation);
}

/*
new_array_a #1
push_i 0
... -> key
... -> value
store_aa


new_array_a #1

duplicate
push_i 0
... -> key
store_aa

duplicate
push_i 1
... -> value
store_aa
*/

// listExpression

void zen_BinaryEntityGenerator_onEnterListExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_ListExpressionContext_t* context = (zen_ListExpressionContext_t*)node->m_context;
    zen_ExpressionsContext_t* expressionsContext = (zen_ExpressionsContext_t*)context->m_expressions->m_context;

    int32_t size = zen_ArrayList_getSize(expressionsContext->m_expressions);

    zen_BinaryEntityGenerator_emitPushByte(generator, size);
    zen_BinaryEntityGenerator_emitNewReferenceArray(generator, 0);

    int32_t i;
    for (i = 0; i < size; i++) {
        zen_ASTNode_t* expression = (zen_ASTNode_t*)zen_ArrayList_get(expressionsContext->m_expressions, i);
        zen_ASTAnnotation_t* expressionAnnotation = zen_ASTAnnotation_new(ZEN_AST_ANNOTATION_TYPE_ASYMETRICAL_CHANNEL_MANAGEMENT, NULL);
        zen_ASTAnnotations_put(generator->m_annotations, expression, expressionAnnotation);
    }
}

void zen_BinaryEntityGenerator_onExitListExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    zen_ListExpressionContext_t* context = (zen_ListExpressionContext_t*)node->m_context;
    zen_ExpressionsContext_t* expressionsContext = (zen_ExpressionsContext_t*)context->m_expressions->m_context;

    int32_t size = zen_ArrayList_getSize(expressionsContext->m_expressions);
    /* Currently, an expression channel is on top of the stack. Whatever,
     * byte codes are emitted will be placed on it. Therefore, we need to
     * swap the expression channel with the local channel.
     */
    zen_ChannelManager_swap(generator->m_channelManager, 0, size);

    int32_t i;
    for (i = 0; i < size; i++) {
        /* A textual representation of the channels for an array with four
         * elements.
         *
         * [0] local channel
         * [1] expression_channel_3
         * [2] expression_channel_2
         * [3] expression_channel_1
         * [4] expression_channel_4
         * ...
         */

        /* Create a duplicate of the array. */
        zen_BinaryEntityGenerator_emitDuplicate(generator);
        zen_BinaryEntityGenerator_emitPushByte(generator, i);

        int32_t expressionChannelIndex = ((i + 1) == size)? 1 : (size - i - 1);
        zen_ChannelManager_append(generator->m_channelManager, expressionChannelIndex, 0);
        zen_ChannelManager_remove(generator->m_channelManager, expressionChannelIndex);

        zen_BinaryEntityGenerator_emitStoreReferenceArray(generator);

        zen_ASTNode_t* expression = (zen_ASTNode_t*)zen_ArrayList_get(expressionsContext->m_expressions, i);
        zen_ASTAnnotation_t* expressionAnnotation = zen_ASTAnnotations_get(generator->m_annotations, expression);
        zen_ASTAnnotation_delete(expressionAnnotation);
    }
}