#include <stdarg.h>
#include <com/onecube/zen/compiler/astWalker.h>
#include <com/onecube/zen/Token.h>
#include <com/onecube/zen/TokenType.h>
#include <com/onecube/zen/Context.h>
#include <com/onecube/zen/compiler/ast/ASTHelper.h>
#include <com/onecube/zen/compiler/ast/AstAnnotation.h>
#include <com/onecube/zen/bcg/BinaryEntityBuilder.h>
#include <com/onecube/zen/core/Integer.h>
#include <com/onecube/zen/feb/Instruction.h>
#include <com/onecube/zen/st/Symbol.h>

zen_BinaryEntityBuilder_t* zen_BinaryEntityBuilder_new(zen_SymbolTable_t* symbolTable, zen_AstAnnotations_t* scopes) {
    zen_BinaryEntityBuilder_t* builder = zen_Memory_allocate(zen_BinaryEntityBuilder_t, 1);
    builder->m_astListener = zen_ASTListener_newWithContext(builder);
    builder->m_constantPoolBuilder = zen_ConstantPoolBuilder_new();
    builder->m_annotations = zen_AstAnnotations_new();
    builder->m_channelManager = zen_ChannelManager_new();
    builder->m_symbolTable = symbolTable;
    builder->m_scopes = scopes;

    zen_ASTListener_t* astListener = builder->m_astListener;

    astListener->m_onEnterCompilationUnit = zen_BinaryEntityBuilder_onEnterCompilationUnit;
    astListener->m_onExitCompilationUnit = zen_BinaryEntityBuilder_onExitCompilationUnit;

    astListener->m_onEnterFunctionDeclaration = zen_BinaryEntityBuilder_onEnterFunctionDeclaration;
    astListener->m_onExitFunctionDeclaration = zen_BinaryEntityBuilder_onExitFunctionDeclaration;

    astListener->m_onEnterStatementSuite = zen_BinaryEntityBuilder_onEnterStatementSuite;
    astListener->m_onExitStatementSuite = zen_BinaryEntityBuilder_onExitStatementSuite;

    astListener->m_onEnterClassDeclaration = zen_BinaryEntityBuilder_onEnterClassDeclaration;
    astListener->m_onExitClassDeclaration = zen_BinaryEntityBuilder_onExitClassDeclaration;

    astListener->m_onEnterExpression = zen_BinaryEntityBuilder_onEnterExpression;

    astListener->m_onEnterAssignmentExpression = zen_BinaryEntityBuilder_onEnterAssignmentExpression;
    astListener->m_onExitAssignmentExpression = zen_BinaryEntityBuilder_onExitAssignmentExpression;

    astListener->m_onEnterConditionalExpression = zen_BinaryEntityBuilder_onEnterConditionalExpression;
    astListener->m_onExitConditionalExpression = zen_BinaryEntityBuilder_onExitConditionalExpression;

    astListener->m_onEnterLogicalOrExpression = zen_BinaryEntityBuilder_onEnterLogicalOrExpression;
    astListener->m_onExitLogicalOrExpression = zen_BinaryEntityBuilder_onExitLogicalOrExpression;

    astListener->m_onEnterLogicalAndExpression = zen_BinaryEntityBuilder_onEnterLogicalAndExpression;
    astListener->m_onExitLogicalAndExpression = zen_BinaryEntityBuilder_onExitLogicalAndExpression;

    astListener->m_onEnterInclusiveOrExpression = zen_BinaryEntityBuilder_onEnterInclusiveOrExpression;
    astListener->m_onExitInclusiveOrExpression = zen_BinaryEntityBuilder_onExitInclusiveOrExpression;

    astListener->m_onEnterExclusiveOrExpression = zen_BinaryEntityBuilder_onEnterExclusiveOrExpression;
    astListener->m_onExitExclusiveOrExpression = zen_BinaryEntityBuilder_onExitExclusiveOrExpression;

    astListener->m_onEnterAndExpression = zen_BinaryEntityBuilder_onEnterAndExpression;
    astListener->m_onExitAndExpression = zen_BinaryEntityBuilder_onExitAndExpression;

    astListener->m_onEnterEqualityExpression = zen_BinaryEntityBuilder_onEnterEqualityExpression;
    astListener->m_onExitEqualityExpression = zen_BinaryEntityBuilder_onExitEqualityExpression;

    astListener->m_onEnterRelationalExpression = zen_BinaryEntityBuilder_onEnterRelationalExpression;
    astListener->m_onExitRelationalExpression = zen_BinaryEntityBuilder_onExitRelationalExpression;

    astListener->m_onEnterShiftExpression = zen_BinaryEntityBuilder_onEnterShiftExpression;
    astListener->m_onExitShiftExpression = zen_BinaryEntityBuilder_onExitShiftExpression;

    astListener->m_onEnterAdditiveExpression = zen_BinaryEntityBuilder_onEnterAdditiveExpression;
    astListener->m_onExitAdditiveExpression = zen_BinaryEntityBuilder_onExitAdditiveExpression;

    astListener->m_onEnterMultiplicativeExpression = zen_BinaryEntityBuilder_onEnterMultiplicativeExpression;
    astListener->m_onExitMultiplicativeExpression = zen_BinaryEntityBuilder_onExitMultiplicativeExpression;

    astListener->m_onEnterUnaryExpression = zen_BinaryEntityBuilder_onEnterUnaryExpression;
    astListener->m_onExitUnaryExpression = zen_BinaryEntityBuilder_onExitUnaryExpression;

    astListener->m_onEnterPostfixExpression = zen_BinaryEntityBuilder_onEnterPostfixExpression;
    astListener->m_onExitPostfixExpression = zen_BinaryEntityBuilder_onExitPostfixExpression;

    astListener->m_onEnterPrimaryExpression = zen_BinaryEntityBuilder_onEnterPrimaryExpression;

    astListener->m_onEnterListExpression = zen_BinaryEntityBuilder_onEnterListExpression;
    astListener->m_onExitListExpression = zen_BinaryEntityBuilder_onExitListExpression;

    astListener->m_onEnterMapExpression = zen_BinaryEntityBuilder_onEnterMapExpression;
    astListener->m_onExitMapExpression = zen_BinaryEntityBuilder_onExitMapExpression;

    astListener->m_onEnterMapEntries = zen_BinaryEntityBuilder_onEnterMapEntries;
    astListener->m_onExitMapEntries = zen_BinaryEntityBuilder_onExitMapEntries;

    astListener->m_onEnterMapEntry = zen_BinaryEntityBuilder_onEnterMapEntry;
    astListener->m_onExitMapEntry = zen_BinaryEntityBuilder_onExitMapEntry;

    return builder;
}

void zen_BinaryEntityBuilder_delete(zen_BinaryEntityBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");

    zen_ASTListener_delete(builder->m_astListener);
    zen_ConstantPoolBuilder_delete(builder->m_constantPoolBuilder);
    zen_AstAnnotations_delete(builder->m_annotations);
    zen_ChannelManager_delete(builder->m_channelManager);
    zen_Memory_deallocate(builder);
}

void zen_BinaryEntityBuilder_build(zen_BinaryEntityBuilder_t* builder, zen_ASTNode_t* compilationUnit) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");

    zen_ASTWalker_walk(builder->m_astListener, compilationUnit);
}

void zen_BinaryEntityBuilder_emit(zen_BinaryEntityBuilder_t* builder, zen_ByteCode_t byteCode, ...) {
    jtk_Assert_assertObject(builder, "The specified binary entity builder is null.");

    zen_ByteCodeChannel_t* channel = zen_ChannelManager_peek(builder->m_channelManager);
    zen_ByteCodeChannel_appendByte(channel, (uint8_t)byteCode);

    zen_Instruction_t* instruction = zen_Instruction_getInstance(byteCode);
    uint8_t argumentCount = zen_Instruction_getArgumentCount(instruction);
    if (argumentCount > 0) {
        /*
        int32_t i;
        for (i = 0; i < argumentCount; i++) {
            zen_ByteCodeChannel_appendByte(channel, 0);
        }
        */

        va_list variableParameters;
        va_start(variableParameters, byteCode);

        uint8_t* arguments = zen_Memory_allocate(uint8_t, argumentCount);
        int32_t i;
        for (i = 0; i < argumentCount; i++) {
            arguments[i] = (uint8_t)(va_arg(variableParameters, int32_t));
        }
        zen_ByteCodeChannel_appendBytes(channel, arguments, argumentCount);
        zen_Memory_deallocate(arguments);

        va_end(variableParameters);
    }
}

void zen_BinaryEntityBuilder_emitLoadReference(zen_BinaryEntityBuilder_t* builder, int32_t index) {
    zen_BinaryEntityBuilder_emit(builder, ZEN_BYTE_CODE_LOAD_A, index);
}

void zen_BinaryEntityBuilder_emitStoreReference(zen_BinaryEntityBuilder_t* builder, int32_t index) {
    zen_BinaryEntityBuilder_emit(builder, ZEN_BYTE_CODE_STORE_A, index);
}

void zen_BinaryEntityBuilder_emitStoreInstanceField(zen_BinaryEntityBuilder_t* builder, int32_t index) {
    zen_BinaryEntityBuilder_emit(builder, ZEN_BYTE_CODE_STORE_INSTANCE_FIELD, index);
}

void zen_BinaryEntityBuilder_emitStoreStaticField(zen_BinaryEntityBuilder_t* builder, int32_t index) {
    zen_BinaryEntityBuilder_emit(builder, ZEN_BYTE_CODE_STORE_INSTANCE_FIELD, index);
}

void zen_BinaryEntityBuilder_emitInvokeVirtual(zen_BinaryEntityBuilder_t* builder, int32_t index) {
    zen_BinaryEntityBuilder_emit(builder, ZEN_BYTE_CODE_INVOKE_VIRTUAL, index);
}

void zen_BinaryEntityBuilder_emitInvokeStatic(zen_BinaryEntityBuilder_t* builder, int32_t index) {
    zen_BinaryEntityBuilder_emit(builder, ZEN_BYTE_CODE_INVOKE_STATIC, index);
}

void zen_BinaryEntityBuilder_emitJump(zen_BinaryEntityBuilder_t* builder, int32_t index) {
    zen_BinaryEntityBuilder_emit(builder, ZEN_BYTE_CODE_JUMP, index);
}

void zen_BinaryEntityBuilder_emitLoadInstanceField(zen_BinaryEntityBuilder_t* builder, int32_t index) {
    zen_BinaryEntityBuilder_emit(builder, ZEN_BYTE_CODE_LOAD_INSTANCE_FIELD, index);
}

void zen_BinaryEntityBuilder_emitLoadCpr(zen_BinaryEntityBuilder_t* builder, int32_t index) {
    zen_BinaryEntityBuilder_emit(builder, ZEN_BYTE_CODE_LOAD_CPR, index);
}

void zen_BinaryEntityBuilder_emitPushIntegerEx(zen_BinaryEntityBuilder_t* builder, const uint8_t* text) {
    // TODO: Parse hexadecimal values too!
    int32_t integerValue = zen_Integer_parse(text, 10);
    int32_t index = 0; // Find the index here
    zen_BinaryEntityBuilder_emitLoadCpr(builder, index);
}

void zen_BinaryEntityBuilder_emitPushTrue(zen_BinaryEntityBuilder_t* builder) {
    zen_BinaryEntityBuilder_emitPushByte(builder, 1);
    zen_BinaryEntityBuilder_emitInvokeStatic(builder, 0);
}

void zen_BinaryEntityBuilder_emitPushFalse(zen_BinaryEntityBuilder_t* builder) {
    zen_BinaryEntityBuilder_emitPushByte(builder, 0);
    zen_BinaryEntityBuilder_emitInvokeStatic(builder, 0);
}

void zen_BinaryEntityBuilder_emitPushNull(zen_BinaryEntityBuilder_t* builder) {
    zen_BinaryEntityBuilder_emit(builder, ZEN_BYTE_CODE_PUSH_NULL);
}

void zen_BinaryEntityBuilder_emitPushUndefined(zen_BinaryEntityBuilder_t* builder) {
    zen_BinaryEntityBuilder_emit(builder, ZEN_BYTE_CODE_PUSH_UNDEFINED);
}

void zen_BinaryEntityBuilder_emitNewReferenceArray(zen_BinaryEntityBuilder_t* builder,
    int32_t index) {
    zen_BinaryEntityBuilder_emit(builder, ZEN_BYTE_CODE_NEW_ARRAY_A, index);
}

void zen_BinaryEntityBuilder_emitNewReferenceArrayEx(zen_BinaryEntityBuilder_t* builder,
    int32_t index, int32_t size) {

}

void zen_BinaryEntityBuilder_emitDuplicate(zen_BinaryEntityBuilder_t* builder) {
    zen_BinaryEntityBuilder_emit(builder, ZEN_BYTE_CODE_DUPLICATE);
}

void zen_BinaryEntityBuilder_emitStoreReferenceArray(zen_BinaryEntityBuilder_t* builder) {
    zen_BinaryEntityBuilder_emit(builder, ZEN_BYTE_CODE_STORE_AA);
}

void zen_BinaryEntityBuilder_emitPushByte(zen_BinaryEntityBuilder_t* builder, int8_t value) {
    zen_BinaryEntityBuilder_emit(builder, ZEN_BYTE_CODE_PUSH_B, value);
}



void zen_BinaryEntityBuilder_pushInteger(zen_BinaryEntityBuilder_t* builder, int64_t i) {
    // TODO
    zen_BinaryEntityBuilder_emitLoadCpr(builder, 0);
}

int32_t zen_BinaryEntityBuilder_findFieldDescriptorIndex(zen_BinaryEntityBuilder_t* builder, jtk_ArrayList_t* identifiers) {
    return 0;
}

int32_t zen_BinaryEntityBuilder_findLocalVariableIndex(zen_BinaryEntityBuilder_t* builder, const char* identifier) {
    return 0;
}












/* compilationUnit */

void zen_BinaryEntityBuilder_onEnterCompilationUnit(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
    zen_Scope_t* scope = zen_AstAnnotations_get(builder->m_scopes, node);
    zen_SymbolTable_setCurrentScope(builder->m_symbolTable, scope);

    zen_ChannelManager_add(builder->m_channelManager);
}

void zen_BinaryEntityBuilder_printChannel(zen_ByteCodeChannel_t* channel) {
    int32_t size = zen_ByteCodeChannel_getSize(channel);
    int32_t i;
    bool text = true;
    for (i = 0; i < size; i++) {
        zen_ByteCode_t byteCode = (zen_ByteCode_t)channel->m_bytes[i];
        zen_Instruction_t* instruction = zen_Instruction_getInstance(byteCode);
        int32_t argumentCount = instruction->m_argumentCount;
        if (text) {
            fprintf(stdout, "%s", zen_Instruction_getText(instruction));
        }
        else {
            fprintf(stdout, "%c", byteCode);
        }
        if (argumentCount > 0) {
            int32_t j;
            int32_t limit = i + argumentCount;
            for (j = i + 1; j <= argumentCount; j++) {
                uint8_t nextByte = channel->m_bytes[j];
                if (text) {
                    fprintf(stdout, " %d", nextByte);
                }
                else {
                    fprintf(stdout, "%c", nextByte);
                }
            }
            i += argumentCount;
        }
        if (text) {
            fprintf(stdout, "\n");
        }
        //fprintf(stdout, "%X ", );
    }
}

void zen_BinaryEntityBuilder_onExitCompilationUnit(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
    zen_SymbolTable_invalidateCurrentScope(builder->m_symbolTable);

    // Do something with the generated byte codes.

    zen_BinaryEntityBuilder_printChannel(zen_ChannelManager_peek(builder->m_channelManager));

    zen_ChannelManager_remove(builder->m_channelManager, 0);
}

void zen_BinaryEntityBuilder_onEnterFunctionDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
    zen_FunctionDeclarationContext_t* context = (zen_FunctionDeclarationContext_t*)node->m_context;

    zen_Scope_t* scope = zen_AstAnnotations_get(builder->m_scopes, node);
    zen_SymbolTable_setCurrentScope(builder->m_symbolTable, scope);
}

void zen_BinaryEntityBuilder_onExitFunctionDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
    zen_SymbolTable_invalidateCurrentScope(builder->m_symbolTable);
}

void zen_BinaryEntityBuilder_onEnterStatementSuite(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;

    zen_Scope_t* scope = zen_AstAnnotations_get(builder->m_scopes, node);
    zen_SymbolTable_setCurrentScope(builder->m_symbolTable, scope);
}

void zen_BinaryEntityBuilder_onExitStatementSuite(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;

    zen_SymbolTable_invalidateCurrentScope(builder->m_symbolTable);
}

// BUG: Looks like we are not setting the current scope in this function.
void zen_BinaryEntityBuilder_onEnterClassDeclaration(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");
}

// BUG
void zen_BinaryEntityBuilder_onExitClassDeclaration(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;

    zen_SymbolTable_invalidateCurrentScope(builder->m_symbolTable);
}










void zen_BinaryEntityBuilder_onEnterWhileStatement(zen_BinaryEntityBuilder_t* builder) {
    zen_ChannelManager_add(builder->m_channelManager);
}

void zen_BinaryEntityBuilder_onExitWhileStatement(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;

    zen_ByteCodeChannel_t* statementSuiteChannel = zen_ChannelManager_peek(builder->m_channelManager);
    uint32_t index = zen_ByteCodeChannel_getSize(statementSuiteChannel);
    zen_BinaryEntityBuilder_emitJump(builder, index);
    zen_ChannelManager_merge(builder->m_channelManager);
}

void zen_BinaryEntityBuilder_onEnterVariableDeclaration(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    /*for () {
        if (variableDeclarator->m_expression != NULL) {
            zen_BinaryEntityBuilder_storeReference(variableDeclarator);
        }
    }*/
}

void zen_BinaryEntityBuilder_onEnterExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
    zen_ExpressionContext_t* context = (zen_AssignmentExpressionContext_t*)node->m_context;

    zen_AstAnnotation_t* annotation = zen_AstAnnotations_get(builder->m_annotations, node);
    if (annotation != NULL) {
        if (zen_AstAnnotation_getType(annotation) == ZEN_AST_ANNOTATION_TYPE_ASYMETRICAL_CHANNEL_MANAGEMENT) {
            zen_ChannelManager_add(builder->m_channelManager);
        }
    }
}

// How to differentiate between function calls

/*
 *    ...
 *    invoke_virtual #function_descriptor_index
 *    store_a #x
 */
void zen_BinaryEntityBuilder_onEnterAssignmentExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
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

    // zen_Symbol_t* symbol = zen_SymbolTable_resolve(builder->m_symbolTable, identifierText);
    // if (zen_Symbol_isVariable(symbol)) {
       //  int32_t targetIndex;
        // bool localVariable = false;

        // zen_Scope_t* enclosingScope = zen_Symbol_getEnclosingScope(symbol);
        // if (zen_Scope_isClassScope(enclosingScope)) {
           //  targetIndex = zen_BinaryEntityBuilder_findFieldDescriptorIndex(builder, children);
        // }
        // else {
           /* int32_t size = zen_ArrayList_getSize(children);
            if (size == 1) {
                targetIndex = zen_BinaryEntityBuilder_findLocalVariableIndex(builder, identifierText);
                localVariable = true;
            }
            else {
                targetIndex = zen_BinaryEntityBuilder_findFieldDescriptorIndex(builder, children);
            }
        }

        zen_BinaryEntityBuilder_emitLoadReference(builder, targetIndex);

        zen_ASTNode_t* assignmentOperator = context->m_assignmentOperator;
        zen_Token_t* operatorToken = (zen_Token_t*)assignmentOperator->m_context;
        switch (operatorToken->m_type) {
            case ZEN_TOKEN_EQUAL: {
                break;
            }

            case ZEN_TOKEN_PLUS_EQUAL: {
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, "Czen.base.Object;plusOperator(Czen.base.Object;)Czen.base.Object;");
                break;
            }

            case ZEN_TOKEN_DASH_EQUAL: {
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, "Czen.base.Object;minusOperator(Czen.base.Object;)Czen.base.Object;");
                break;
            }

            default: {
                // [internal error]
            }
        }

        void** state = zen_Memory_allocate(void*, ZEN_ASSIGNMENT_EXPRESSION_STATE_SIZE);
        state[ZEN_ASSIGNMENT_EXPRESSION_STATE_TARGET_INDEX] = (void*)targetIndex;
        state[ZEN_ASSIGNMENT_EXPRESSION_STATE_LOCAL_VARIABLE] = (void*)localVariable;
        zen_AstAnnotations_put(builder->m_annotations, node, state);

    }
    else if (zen_Symbol_isConstant(symbol)) {
        // error: Constant cannot be assigned
    }
    else {
        // error: LHS is not variable
    }*/

    // zen_ArrayList_delete(children);
}

void zen_BinaryEntityBuilder_onExitAssignmentExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
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
        zen_BinaryEntityBuilder_emitStoreReference(builder, 10);
    }
}

/* Conditional Expression */

void zen_BinaryEntityBuilder_onEnterConditionalExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)listener->m_context;
    zen_ConditionalExpressionContext_t* context = (zen_ConditionalExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityBuilder_onExitConditionalExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)listener->m_context;
    zen_ConditionalExpressionContext_t* context = (zen_ConditionalExpressionContext_t*)node->m_context;

    /* TODO */
}

/* Logical Or Expression */

void zen_BinaryEntityBuilder_onEnterLogicalOrExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)listener->m_context;
    zen_LogicalOrExpressionContext_t* context = (zen_LogicalOrExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityBuilder_onExitLogicalOrExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)listener->m_context;
    zen_LogicalOrExpressionContext_t* context = (zen_LogicalOrExpressionContext_t*)node->m_context;

    /* TODO */
}

/* Logical And Expression */

void zen_BinaryEntityBuilder_onEnterLogicalAndExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)listener->m_context;
    zen_LogicalAndExpressionContext_t* context = (zen_LogicalAndExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityBuilder_onExitLogicalAndExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)listener->m_context;
    zen_LogicalAndExpressionContext_t* context = (zen_LogicalAndExpressionContext_t*)node->m_context;

    /* TODO */
}

/* Inclusive Or Expression */

void zen_BinaryEntityBuilder_onEnterInclusiveOrExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)listener->m_context;
    zen_InclusiveOrExpressionContext_t* context = (zen_InclusiveOrExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityBuilder_onExitInclusiveOrExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)listener->m_context;
    zen_InclusiveOrExpressionContext_t* context = (zen_InclusiveOrExpressionContext_t*)node->m_context;

    if (context->m_inclusiveOrExpression != NULL) {
        zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
    }
}

/* Exclusive Or Expression */


void zen_BinaryEntityBuilder_onEnterExclusiveOrExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)listener->m_context;
    zen_ExclusiveOrExpressionContext_t* context = (zen_ExclusiveOrExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityBuilder_onExitExclusiveOrExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)listener->m_context;
    zen_ExclusiveOrExpressionContext_t* context = (zen_ExclusiveOrExpressionContext_t*)node->m_context;

    if (context->m_exclusiveOrExpression != NULL) {
        zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
    }
}

/* And Expression */

void zen_BinaryEntityBuilder_onEnterAndExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)listener->m_context;
    zen_AndExpressionContext_t* context = (zen_AndExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityBuilder_onExitAndExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)listener->m_context;
    zen_AndExpressionContext_t* context = (zen_AndExpressionContext_t*)node->m_context;

    if (context->m_andExpression != NULL) {
        zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
    }
}

/* Equality Expression */

void zen_BinaryEntityBuilder_onEnterEqualityExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)listener->m_context;
    zen_EqualityExpressionContext_t* context = (zen_EqualityExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityBuilder_onExitEqualityExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)listener->m_context;
    zen_EqualityExpressionContext_t* context = (zen_EqualityExpressionContext_t*)node->m_context;

    zen_ASTNode_t* equalityOperator = context->m_equalityOperator;
    if (equalityOperator != NULL) {
        zen_Token_t* equalityOperatorToken = (zen_Token_t*)(equalityOperator->m_context);
        zen_TokenType_t equalityOperatorTokenType = zen_Token_getType(equalityOperatorToken);

        switch (equalityOperatorTokenType) {
            case ZEN_TOKEN_EQUAL_2: {
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                break;
            }

            case ZEN_TOKEN_EXCLAMATION_MARK_EQUAL: {
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                break;
            }
        }
    }
}

/* Relational Expression */

void zen_BinaryEntityBuilder_onEnterRelationalExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)listener->m_context;
    zen_RelationalExpressionContext_t* context = (zen_RelationalExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityBuilder_onExitRelationalExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)listener->m_context;
    zen_RelationalExpressionContext_t* context = (zen_RelationalExpressionContext_t*)node->m_context;

    zen_ASTNode_t* relationalOperator = context->m_relationalOperator;
    if (relationalOperator != NULL) {
        zen_Token_t* relationalOperatorToken = (zen_Token_t*)(relationalOperator->m_context);
        zen_TokenType_t relationalOperatorTokenType = zen_Token_getType(relationalOperatorToken);
        switch (relationalOperatorTokenType) {
            case ZEN_TOKEN_LEFT_ANGLE_BRACKET: {
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                break;
            }

            case ZEN_TOKEN_RIGHT_ANGLE_BRACKET: {
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                break;
            }

            case ZEN_TOKEN_LEFT_ANGLE_BRACKET_EQUAL: {
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                break;
            }

            case ZEN_TOKEN_RIGHT_ANGLE_BRACKET_EQUAL: {
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                break;
            }

            case ZEN_TOKEN_KEYWORD_IS: {
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                break;
            }
        }
    }
}

/* Shift Expression */

void zen_BinaryEntityBuilder_onEnterShiftExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
    zen_ShiftExpressionContext_t* context = (zen_ShiftExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityBuilder_onExitShiftExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
    zen_ShiftExpressionContext_t* context = (zen_ShiftExpressionContext_t*)node->m_context;

    zen_ASTNode_t* shiftOperator = context->m_shiftOperator;
    if (shiftOperator != NULL) {
        zen_Token_t* shiftOperatorToken = (zen_Token_t*)(shiftOperator->m_context);
        zen_TokenType_t shiftOperatorTokenType = zen_Token_getType(shiftOperatorToken);
        switch (shiftOperatorTokenType) {
            case ZEN_TOKEN_LEFT_ANGLE_BRACKET_2: {
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                break;
            }

            case ZEN_TOKEN_RIGHT_ANGLE_BRACKET_2: {
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                break;
            }

            case ZEN_TOKEN_RIGHT_ANGLE_BRACKET_3: {
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                break;
            }
        }
    }
}

/* Additive Expression */

void zen_BinaryEntityBuilder_onEnterAdditiveExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
    zen_AdditiveExpressionContext_t* context = (zen_AdditiveExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityBuilder_onExitAdditiveExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
    zen_AdditiveExpressionContext_t* context = (zen_AdditiveExpressionContext_t*)node->m_context;

    zen_ASTNode_t* additiveOperator = context->m_additiveOperator;
    if (additiveOperator != NULL) {
        zen_Token_t* additiveOperatorToken = (zen_Token_t*)(additiveOperator->m_context);
        zen_TokenType_t additiveOperatorTokenType = zen_Token_getType(additiveOperatorToken);
        switch (additiveOperatorTokenType) {
            case ZEN_TOKEN_PLUS: {
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, /* "..." */ 0);
                break;
            }

            case ZEN_TOKEN_DASH: {
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                break;
            }

            default: {
                fprintf(stderr, "[internal error] Control should not reach here.\n");
                break;
            }
        }
    }
}

/* Multiplicative Expression */

void zen_BinaryEntityBuilder_onEnterMultiplicativeExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)listener->m_context;
    zen_MultiplicativeExpressionContext_t* context = (zen_MultiplicativeExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityBuilder_onExitMultiplicativeExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)listener->m_context;
    zen_MultiplicativeExpressionContext_t* context = (zen_MultiplicativeExpressionContext_t*)node->m_context;

    zen_ASTNode_t* multiplicativeOperator = context->m_multiplicativeOperator;
    if (multiplicativeOperator != NULL) {
        zen_Token_t* multiplicativeOperatorToken = (zen_Token_t*)(multiplicativeOperator->m_context);
        zen_TokenType_t multiplicativeOperatorTokenType = zen_Token_getType(multiplicativeOperatorToken);
        switch (multiplicativeOperatorTokenType) {
            case ZEN_TOKEN_ASTERISK: {
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                break;
            }

            case ZEN_TOKEN_FORWARD_SLASH: {
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                break;
            }

            case ZEN_TOKEN_MODULUS: {
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                break;
            }
        }
    }
}

/* Unary Expression */

void zen_BinaryEntityBuilder_onEnterUnaryExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)listener->m_context;
    zen_UnaryExpressionContext_t* context = (zen_UnaryExpressionContext_t*)node->m_context;
}

void zen_BinaryEntityBuilder_onExitUnaryExpression(zen_ASTListener_t* listener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)listener->m_context;
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
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                break;
            }

            case ZEN_TOKEN_DASH: {
                /* The UnaryPlusOperator.applyUnaryPlus() function is invoked against the object
                 * whose "negative" internal state is required. It returns an object with its
                 * internal state "negative".
                 */
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                break;
            }

            case ZEN_TOKEN_TILDE: {
                /* The UnaryPlusOperator.applyUnaryPlus() function is invoked against the object
                 * whose "deeply toggled" internal state is required. It returns an object with its
                 * internal state "deeply toggled".
                 */
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                break;
            }

            case ZEN_TOKEN_EXCLAMATION_MARK: {
                /* The UnaryPlusOperator.applyUnaryPlus() function is invoked against the object
                 * whose "toggled" internal state is required. It returns an object with its
                 * internal state "toggled".
                 */
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                break;
            }

            case ZEN_TOKEN_PLUS_2:
            case ZEN_TOKEN_DASH_2: {
                if (unaryOperatorType == ZEN_TOKEN_PLUS_2) {
                    /* The onPreIncrement() function is invoked against the object whose internal
                     * state has to be "incremented by 1". It returns an object with its internal
                     * state "incremented by 1".
                     */
                    zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                }
                else {
                    /* The onPreDecrement() function is invoked against the object whose internal
                     * state has to be "incremented by 1". It returns an object with its internal
                     * state "incremented by 1".
                     */
                    zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                }
                /* A copy of the "incremented" object is required on the operand stack for
                 * assignment.
                 */
                zen_BinaryEntityBuilder_emitDuplicate(builder);
                /* Assign the variable the object which represents the new state.
                 *
                 * TODO: Change store_a to store_a1 (and friends) and
                 *       store_field when necessary.
                 */
                zen_BinaryEntityBuilder_emitStoreReference(builder, 0);

                break;
            }
        }
    }
}

void zen_BinaryEntityBuilder_onEnterPostfixExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
}

void zen_BinaryEntityBuilder_onExitPostfixExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
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
                zen_BinaryEntityBuilder_emitLoadArray(builder);
                break;
            }
            */

            case ZEN_AST_NODE_POSTFIX_OPERATOR: {
                zen_PostfixOperatorContext_t* postfixOperatorContext = (zen_PostfixOperatorContext_t*)postfixPart->m_context;
                zen_Token_t* postfixOperator = (zen_Token_t*)(postfixOperatorContext->m_postfixOperator->m_context);

                /* The virtual onPostIncrement() function is invoked against the object.
                 * Therefore, a duplicate of it is necessary on the operand stack.
                 */
                zen_BinaryEntityBuilder_emitDuplicate(builder);
                if (zen_Token_getType(postfixOperator) == ZEN_TOKEN_PLUS_2) {
                    /* The onPostIncrement() function is invoked against the object whose internal
                     * state has to be "incremented by 1". It returns an object with its internal
                     * state "incremented by 1".
                     */
                    // TODO: Emit an invocation to PostIncrementOperator.applyPostDecrement()
                    zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                }
                else {
                    /* The onPostDecrement() function is invoked against the object whose internal
                     * state has to be "incremented by 1". It returns an object with its internal
                     * state "incremented by 1".
                     */
                    // TODO: Emit an invocation to PostDecrementOperator.applyPostDecrement()
                    zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                }
                /* Assign the variable the object which represents the new state.
                 *
                 * TODO: Change store_a to store_a1 (and friends) and
                 *       store_field when necessary.
                 */
                zen_BinaryEntityBuilder_emitStoreReference(builder, 0);

                break;
            }
        }
    }
}

void zen_BinaryEntityBuilder_onEnterPrimaryExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
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
                zen_Symbol_t* symbol = zen_SymbolTable_resolve(builder->m_symbolTable, identifierText);
                zen_Scope_t* enclosingScope = zen_Symbol_getEnclosingScope(symbol);
                if (zen_Scope_isClassScope(enclosingScope)) {
                    zen_BinaryEntityBuilder_emitLoadReference(builder, 0);
                    zen_BinaryEntityBuilder_emitLoadInstanceField(builder, 10);
                }
                else {
                    zen_BinaryEntityBuilder_emitLoadReference(builder, 10);
                }
                break;
            }

            case ZEN_TOKEN_INTEGER_LITERAL: {
                const uint8_t* integerText = zen_Token_getText(token);
                zen_BinaryEntityBuilder_emitPushIntegerEx(builder, integerText);
                zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0);
                break;
            }

            case ZEN_TOKEN_KEYWORD_TRUE: {
                zen_BinaryEntityBuilder_emitPushTrue(builder);
                break;
            }

            case ZEN_TOKEN_KEYWORD_FALSE: {
                zen_BinaryEntityBuilder_emitPushFalse(builder);
                break;
            }

            case ZEN_TOKEN_STRING_LITERAL: {
                zen_BinaryEntityBuilder_emitLoadCpr(builder, 0);
                break;
            }

            case ZEN_TOKEN_KEYWORD_NULL: {
                zen_BinaryEntityBuilder_emitPushNull(builder);
                break;
            }

            case ZEN_TOKEN_KEYWORD_UNDEFINED: {
                zen_BinaryEntityBuilder_emitPushUndefined(builder);
                break;
            }

            default: {
                /* [internal error] */
            }
        }
    }
}

void zen_BinaryEntityBuilder_onEnterMapExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
    zen_MapExpressionContext_t* context = (zen_MapExpressionContext_t*)node->m_context;

    zen_BinaryEntityBuilder_emitPushByte(builder, 0); // TODO: With proper size
    zen_BinaryEntityBuilder_emitNewReferenceArray(builder, 0); // TODO: new_array_n
}

void zen_BinaryEntityBuilder_onExitMapExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
    zen_MapExpressionContext_t* context = (zen_MapExpressionContext_t*)node->m_context;

    zen_BinaryEntityBuilder_emitInvokeVirtual(builder, 0); // TODO
}

void zen_BinaryEntityBuilder_onEnterListExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
    zen_ListExpressionContext_t* context = (zen_ListExpressionContext_t*)node->m_context;
    zen_ExpressionsContext_t* expressionsContext = (zen_ExpressionsContext_t*)context->m_expressions->m_context;

    int32_t size = zen_ArrayList_getSize(expressionsContext->m_expressions);

    zen_BinaryEntityBuilder_emitPushByte(builder, size);
    zen_BinaryEntityBuilder_emitNewReferenceArray(builder, 0);

    int32_t i;
    for (i = 0; i < size; i++) {
        zen_ASTNode_t* expression = (zen_ASTNode_t*)zen_ArrayList_get(expressionsContext->m_expressions, i);
        zen_AstAnnotation_t* expressionAnnotation = zen_AstAnnotation_new(ZEN_AST_ANNOTATION_TYPE_ASYMETRICAL_CHANNEL_MANAGEMENT, NULL);
        zen_AstAnnotations_put(builder->m_annotations, expression, expressionAnnotation);
    }
}

void zen_BinaryEntityBuilder_onExitListExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
    zen_ListExpressionContext_t* context = (zen_ListExpressionContext_t*)node->m_context;
    zen_ExpressionsContext_t* expressionsContext = (zen_ExpressionsContext_t*)context->m_expressions->m_context;

    int32_t size = zen_ArrayList_getSize(expressionsContext->m_expressions);
    /* Currently, an expression channel is on top of the stack. Whatever,
     * byte codes are emitted will be placed on it. Therefore, we need to
     * swap the expression channel with the local channel.
     */
    zen_ChannelManager_swap(builder->m_channelManager, 0, size);

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
        zen_BinaryEntityBuilder_emitDuplicate(builder);
        zen_BinaryEntityBuilder_emitPushByte(builder, i);

        int32_t expressionChannelIndex = ((i + 1) == size)? 1 : (size - i - 1);
        zen_ChannelManager_append(builder->m_channelManager, expressionChannelIndex, 0);
        zen_ChannelManager_remove(builder->m_channelManager, expressionChannelIndex);

        zen_BinaryEntityBuilder_emitStoreReferenceArray(builder);

        zen_ASTNode_t* expression = (zen_ASTNode_t*)zen_ArrayList_get(expressionsContext->m_expressions, i);
        zen_AstAnnotation_t* expressionAnnotation = zen_AstAnnotations_get(builder->m_annotations, expression);
        zen_AstAnnotation_delete(expressionAnnotation);
    }
}

void zen_BinaryEntityBuilder_onEnterMapEntries(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
    zen_MapEntriesContext_t* context = (zen_MapEntriesContext_t*)node->m_context;

    /*
    int32_t i;
    for (i = 0; i < size; i++) {
        zen_ASTNode_t* mapEntry = (zen_ASTNode_t*)zen_ArrayList_get(context->m_mapEntries, i);
        zen_AstAnnotation_t* mapEntryAnnotation = zen_AstAnnotation_new(ZEN_AST_ANNOTATION_TYPE_ASYMETRICAL_CHANNEL_MANAGEMENT, NULL);
        zen_AstAnnotations_put(builder->m_annotations, mapEntryAnnotation);
    }
    */
}

void zen_BinaryEntityBuilder_onExitMapEntries(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
    zen_MapEntriesContext_t* context = (zen_MapEntriesContext_t*)node->m_context;

    /* TODO: Apparently, storing integers as void* is dangerous. Find an alternative. */

    int32_t size = zen_ArrayList_getSize(context->m_mapEntries);
    // TODO: Filter out map with 0 entries!
    /* Currently, a map entry channel is on top of the stack. Whatever,
     * byte codes are emitted will be placed on it. Therefore, we need to
     * swap the map entry channel with the local channel.
     */
    zen_ChannelManager_swap(builder->m_channelManager, 0, size);

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
        zen_BinaryEntityBuilder_emitDuplicate(builder);
        zen_BinaryEntityBuilder_emitPushByte(builder, i);

        int32_t entryChannelIndex = ((i + 1) == size)? 1 : (size - i - 1);
        zen_ChannelManager_append(builder->m_channelManager, entryChannelIndex, 0);
        zen_ChannelManager_remove(builder->m_channelManager, entryChannelIndex);

        zen_BinaryEntityBuilder_emitStoreReferenceArray(builder);
    }
}

void zen_BinaryEntityBuilder_onEnterMapEntry(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
    zen_MapEntryContext_t* context = (zen_MapEntryContext_t*)node->m_context;

    /* The byte codes generated for every map entry is redirected on a new
     * channel. Thus, an oppurtunity to insert some arbitrary instructions
     * between the map entries is provided.
     */
    zen_ChannelManager_add(builder->m_channelManager);

    zen_BinaryEntityBuilder_emitPushByte(builder, 2);
    zen_BinaryEntityBuilder_emitNewReferenceArray(builder, 0);

    zen_AstAnnotation_t* keyExpressionAnnotation = zen_AstAnnotation_new(ZEN_AST_ANNOTATION_TYPE_ASYMETRICAL_CHANNEL_MANAGEMENT, NULL);
    zen_AstAnnotations_put(builder->m_annotations, context->m_keyExpression, keyExpressionAnnotation);

    zen_AstAnnotation_t* valueExpressionAnnotation = zen_AstAnnotation_new(ZEN_AST_ANNOTATION_TYPE_ASYMETRICAL_CHANNEL_MANAGEMENT, NULL);
    zen_AstAnnotations_put(builder->m_annotations, context->m_valueExpression, valueExpressionAnnotation);
}

void zen_BinaryEntityBuilder_onExitMapEntry(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_BinaryEntityBuilder_t* builder = (zen_BinaryEntityBuilder_t*)astListener->m_context;
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
    zen_ChannelManager_swap(builder->m_channelManager, 0, 2);

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
    zen_BinaryEntityBuilder_emitDuplicate(builder);
    zen_BinaryEntityBuilder_emitPushByte(builder, 0); // TODO: push_i0
    zen_ChannelManager_append(builder->m_channelManager, 1, 0);
    zen_ChannelManager_remove(builder->m_channelManager, 1);
    zen_BinaryEntityBuilder_emitStoreReferenceArray(builder);

    /* The following section generates instructions which insert the value into
     * the array in second dimension.
     */
    /* A new array was previously created for this map entry. Create a duplicate
     * of its reference on the operand stack for calling the {@code store_aa}
     * instruction.
     */
    zen_BinaryEntityBuilder_emitDuplicate(builder);
    zen_BinaryEntityBuilder_emitPushByte(builder, 1);
    zen_ChannelManager_append(builder->m_channelManager, 1, 0);
    zen_ChannelManager_remove(builder->m_channelManager, 1);
    zen_BinaryEntityBuilder_emitStoreReferenceArray(builder);

    zen_AstAnnotation_t* keyExpressionAnnotation = zen_AstAnnotations_get(builder->m_annotations, context->m_keyExpression);
    zen_AstAnnotation_t* valueExpressionAnnotation = zen_AstAnnotations_get(builder->m_annotations, context->m_valueExpression);

    zen_AstAnnotation_delete(keyExpressionAnnotation);
    zen_AstAnnotation_delete(valueExpressionAnnotation);
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