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

// Sunday, March 11, 2018

#include <jtk/collection/stack/LinkedStack.h>

#include <com/onecube/zen/compiler/lexer/Token.h>
#include <com/onecube/zen/compiler/ast/context/Context.h>
#include <com/onecube/zen/compiler/symbol-table/ClassSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/FunctionSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolResolutionListener.h>

zen_SymbolResolutionListener_t* zen_SymbolResolutionListener_new(zen_SymbolTable_t* symbolTable, zen_ASTAnnotations_t* scopes) {
    zen_SymbolResolutionListener_t* listener = zen_Memory_allocate(zen_SymbolResolutionListener_t, 1);
    listener->m_astListener = zen_ASTListener_newWithContext(listener);
    listener->m_symbolTable = symbolTable;
    listener->m_scopes = scopes;

    zen_ASTListener_t* astListener = listener->m_astListener;
    astListener->m_onEnterCompilationUnit = zen_SymbolResolutionListener_onEnterCompilationUnit;
    astListener->m_onExitCompilationUnit = zen_SymbolResolutionListener_onExitCompilationUnit;
    astListener->m_onEnterClassDeclaration = zen_SymbolResolutionListener_onEnterClassDeclaration;
    astListener->m_onExitClassDeclaration = zen_SymbolResolutionListener_onExitClassDeclaration;
    astListener->m_onEnterFunctionDeclaration = zen_SymbolResolutionListener_onEnterFunctionDeclaration;
    astListener->m_onExitFunctionDeclaration = zen_SymbolResolutionListener_onExitFunctionDeclaration;
    astListener->m_onEnterStatementSuite = zen_SymbolResolutionListener_onEnterStatementSuite;
    astListener->m_onExitStatementSuite = zen_SymbolResolutionListener_onExitStatementSuite;
    astListener->m_onEnterPrimaryExpression = zen_SymbolResolutionListener_onEnterPrimaryExpression;

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

/* compilationUnit */

void zen_SymbolResolutionListener_onEnterCompilationUnit(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_Scope_t* scope = zen_ASTAnnotations_get(listener->m_scopes, node);
    zen_SymbolTable_setCurrentScope(listener->m_symbolTable, scope);
}

void zen_SymbolResolutionListener_onExitCompilationUnit(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_SymbolTable_invalidateCurrentScope(listener->m_symbolTable);
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
//        zen_Symbol_t* currentScopeSymbol = (zen_Symbol_t*)zen_HashMap_getValue(classScope->m_symbols, identifierText);
//
//        while (!zen_LinkedStack_isEmpty(stack)) {
//            zen_ClassSymbol_t* classSymbol = (zen_ClassSymbol_t*)zen_LinkedStack_pop(stack);
//            zen_Scope_t* ts = zen_ClassSymbol_getClassScope(classSymbol);
//            classScope = (zen_ClassScope_t*)ts->m_context;
//
//            zen_Symbol_t* superScopeSymbol = (zen_Symbol_t*)zen_HashMap_getValue(classScope->m_symbols, identifierText);
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

// BUG: Looks like we are not setting the current scope in this function.
// Also, update the BinaryEntityBuilder.c file.
// Further, the counter function of this function invalidates the current scope.
void zen_SymbolResolutionListener_onEnterClassDeclaration(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_ClassDeclarationContext_t* classDeclarationContext = (zen_ClassDeclarationContext_t*)node->m_context;
    if (classDeclarationContext->m_classExtendsClause != NULL) {
        zen_ClassExtendsClauseContext_t* classExtendsClauseContext = classExtendsClauseContext = (zen_ClassExtendsClauseContext_t*)classDeclarationContext->m_classExtendsClause->m_context;

        // TODO: Use a node annotation

        zen_Symbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, zen_Token_getText((zen_Token_t*)classDeclarationContext->m_identifier->m_context));
        zen_ClassSymbol_t* classSymbol = (zen_ClassSymbol_t*)symbol->m_context;
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
}

void zen_SymbolResolutionListener_onExitClassDeclaration(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;

    zen_SymbolTable_invalidateCurrentScope(listener->m_symbolTable);
}

void zen_SymbolResolutionListener_onEnterPrimaryExpression(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {

    zen_SymbolResolutionListener_t* listener = (zen_SymbolResolutionListener_t*)astListener->m_context;
    zen_PrimaryExpressionContext_t* primaryExpressionContext = (zen_PrimaryExpressionContext_t*)node->m_context;

    if (primaryExpressionContext->m_expression->m_type == ZEN_AST_NODE_TYPE_TERMINAL) {
        zen_Token_t* identifierToken = (zen_Token_t*)primaryExpressionContext->m_expression->m_context;
        if (zen_Token_getType(identifierToken) == ZEN_TOKEN_IDENTIFIER) {
            const uint8_t* identifierText = zen_Token_getText(identifierToken);
            zen_Symbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, identifierText);
            
            if (symbol != NULL) {
                zen_Scope_t* scope = zen_Symbol_getEnclosingScope(symbol);
                
                if (zen_Scope_isLocalScope(scope)) {
                    zen_Token_t* symbolToken = (zen_Token_t*)symbol->m_identifier->m_context;
                    if (identifierToken->m_startIndex <= symbolToken->m_startIndex) {
                        zen_ErrorHandler_reportError(NULL, "Undeclared identifier", identifierToken);
                    }
                }
            }
            else {
                zen_ErrorHandler_reportError(NULL, "Undeclared identifier", identifierToken);
            }
        }
    }
}