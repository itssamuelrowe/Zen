// Saturday, March 31, 2018

#ifndef ZEN_BCG_BINARY_ENTITY_BUILDER_H
#define ZEN_BCG_BINARY_ENTITY_BUILDER_H

#include <zen/ASTListener.h>
#include <zen/ASTNode.h>
#include <zen/ast/AstAnnotations.h>
#include <zen/bcg/ConstantPoolBuilder.h>
#include <zen/bcg/ChannelManager.h>
#include <zen/collection/ArrayList.h>
#include <zen/feb/ByteCode.h>
#include <zen/st/SymbolTable.h>

/*******************************************************************************
 * BinaryEntityBuilder                                                         *
 *******************************************************************************/

struct zen_BinaryEntityBuilder_t {
    zen_ASTListener_t* m_astListener;
    zen_ConstantPoolBuilder_t* m_constantPoolBuilder;
    zen_AstAnnotations_t* m_annotations;
    zen_ChannelManager_t* m_channelManager;
    zen_SymbolTable_t* m_symbolTable;
    zen_AstAnnotations_t* m_scopes;
};

typedef struct zen_BinaryEntityBuilder_t zen_BinaryEntityBuilder_t;

zen_BinaryEntityBuilder_t* zen_BinaryEntityBuilder_new(zen_SymbolTable_t* symbolTable, zen_AstAnnotations_t* scopes);
void zen_BinaryEntityBuilder_delete(zen_BinaryEntityBuilder_t* builder);
void zen_BinaryEntityBuilder_build(zen_BinaryEntityBuilder_t* builder, zen_ASTNode_t* compilationUnit);

void zen_BinaryEntityBuilder_emit(zen_BinaryEntityBuilder_t* builder, zen_ByteCode_t byteCode, ...);
void zen_BinaryEntityBuilder_emitPushField(zen_BinaryEntityBuilder_t* builder, int32_t index);
void zen_BinaryEntityBuilder_emitLoadReference(zen_BinaryEntityBuilder_t* builder, int32_t index);
void zen_BinaryEntityBuilder_emitStoreReference(zen_BinaryEntityBuilder_t* builder, int32_t index);
void zen_BinaryEntityBuilder_emitStoreInstanceField(zen_BinaryEntityBuilder_t* builder, int32_t index);
void zen_BinaryEntityBuilder_emitStoreStaticField(zen_BinaryEntityBuilder_t* builder, int32_t index);
void zen_BinaryEntityBuilder_emitLoadInstanceField(zen_BinaryEntityBuilder_t* builder, int32_t index);
void zen_BinaryEntityBuilder_emitLoadStaticField(zen_BinaryEntityBuilder_t* builder, int32_t index);
void zen_BinaryEntityBuilder_emitInvokeVirtual(zen_BinaryEntityBuilder_t* builder, int32_t index);
void zen_BinaryEntityBuilder_emitInvokeStatic(zen_BinaryEntityBuilder_t* builder, int32_t index);
void zen_BinaryEntityBuilder_emitJump(zen_BinaryEntityBuilder_t* builder, int32_t index);
void zen_BinaryEntityBuilder_emitLoadCpr(zen_BinaryEntityBuilder_t* builder, int32_t index);
void zen_BinaryEntityBuilder_emitPushIntegerEx(zen_BinaryEntityBuilder_t* builder, const uint8_t* text);
void zen_BinaryEntityBuilder_emitPushTrue(zen_BinaryEntityBuilder_t* builder);
void zen_BinaryEntityBuilder_emitPushFalse(zen_BinaryEntityBuilder_t* builder);
void zen_BinaryEntityBuilder_emitPushNull(zen_BinaryEntityBuilder_t* builder);
void zen_BinaryEntityBuilder_emitPushUndefined(zen_BinaryEntityBuilder_t* builder);
void zen_BinaryEntityBuilder_emitNewReferenceArray(zen_BinaryEntityBuilder_t* builder, int32_t index);
void zen_BinaryEntityBuilder_emitNewReferenceArrayEx(zen_BinaryEntityBuilder_t* builder, int32_t index, int32_t size);
void zen_BinaryEntityBuilder_emitDuplicate(zen_BinaryEntityBuilder_t* builder);
void zen_BinaryEntityBuilder_emitStoreRerenceArray(zen_BinaryEntityBuilder_t* builder);
void zen_BinaryEntityBuilder_emitPushByte(zen_BinaryEntityBuilder_t* builder, int8_t byte);

int32_t zen_BinaryEntityBuilder_findFieldDescriptorIndex(zen_BinaryEntityBuilder_t* builder, zen_ArrayList_t* identifiers);
int32_t zen_BinaryEntityBuilder_findLocalVariableIndex(zen_BinaryEntityBuilder_t* builder, const char* identifier);

void zen_BinaryEntityBuilder_onEnterExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterAssignmentExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitAssignmentExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterCompilationUnit(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitCompilationUnit(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterFunctionDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitFunctionDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterStatementSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitStatementSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterClassDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitClassDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterConditionalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitConditionalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterLogicalOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitLogicalOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterLogicalAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitLogicalAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterInclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitInclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterExclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitExclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterEqualityExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitEqualityExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterRelationalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitRelationalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterShiftExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitShiftExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterAdditiveExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitAdditiveExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterMultiplicativeExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitMultiplicativeExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterUnaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitUnaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterPostfixExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitPostfixExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterPrimaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterListExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitListExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterMapExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitMapExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterMapEntries(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitMapEntries(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

void zen_BinaryEntityBuilder_onEnterMapEntry(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_BinaryEntityBuilder_onExitMapEntry(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

#endif /* ZEN_BCG_BINARY_ENTITY_BUILDER_H */