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

#include <jtk/collection/stack/LinkedStack.h>

#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/lexer/Token.h>
#include <com/onecube/zen/compiler/symbol-table/ClassScope.h>
#include <com/onecube/zen/compiler/symbol-table/FunctionSignature.h>
#include <com/onecube/zen/compiler/symbol-table/FunctionSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolDefinitionListener.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolTable.h>
#include <com/onecube/zen/compiler/symbol-table/FunctionScope.h>
#include <com/onecube/zen/compiler/symbol-table/ConstantSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/LocalScope.h>
#include <com/onecube/zen/compiler/symbol-table/VariableSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/LabelSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/ClassSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/EnumerationSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/EnumerationScope.h>
#include <com/onecube/zen/compiler/symbol-table/CompilationUnitScope.h>
#include <com/onecube/zen/compiler/symbol-table/EnumerateSymbol.h>

void zen_ErrorHandler_reportError(void* handler, const char* message, zen_Token_t* token) {
    fprintf(stderr, "[error] %d:%d-%d: %s\n", token->m_startLine, token->m_startColumn, token->m_stopColumn, message);
    fflush(stdout);
}

zen_SymbolDefinitionListener_t* zen_SymbolDefinitionListener_new(zen_SymbolTable_t* symbolTable, zen_ASTAnnotations_t* scopes) {
    zen_SymbolDefinitionListener_t* listener = zen_Memory_allocate(zen_SymbolDefinitionListener_t, 1);
    listener->m_astListener = zen_ASTListener_newWithContext(listener);
    listener->m_symbolTable = symbolTable;
    listener->m_scopes = scopes;
    listener->m_package = NULL;

    zen_ASTListener_t* astListener = listener->m_astListener;

    astListener->m_onEnterCompilationUnit = zen_SymbolDefinitionListener_onEnterCompilationUnit;
    astListener->m_onExitCompilationUnit = zen_SymbolDefinitionListener_onExitCompilationUnit;
    astListener->m_onEnterFunctionDeclaration = zen_SymbolDefinitionListener_onEnterFunctionDeclaration;
    astListener->m_onExitFunctionDeclaration = zen_SymbolDefinitionListener_onExitFunctionDeclaration;
    astListener->m_onEnterStatementSuite = zen_SymbolDefinitionListener_onEnterStatementSuite;
    astListener->m_onExitStatementSuite = zen_SymbolDefinitionListener_onExitStatementSuite;
    astListener->m_onEnterVariableDeclaration = zen_SymbolDefinitionListener_onEnterVariableDeclaration;
    astListener->m_onEnterConstantDeclaration = zen_SymbolDefinitionListener_onEnterConstantDeclaration;
    astListener->m_onEnterLabelClause = zen_SymbolDefinitionListener_onEnterLabelClause;
    astListener->m_onEnterForParameter = zen_SymbolDefinitionListener_onEnterForParameters;
    astListener->m_onEnterTryStatement = zen_SymbolDefinitionListener_onEnterTryStatement;
    astListener->m_onEnterClassDeclaration = zen_SymbolDefinitionListener_onEnterClassDeclaration;
    astListener->m_onExitClassDeclaration = zen_SymbolDefinitionListener_onExitClassDeclaration;
    astListener->m_onEnterEnumerationDeclaration = zen_SymbolDefinitionListener_onEnterEnumerationDeclaration;
    astListener->m_onExitEnumerationDeclaration = zen_SymbolDefinitionListener_onExitEnumerationDeclaration;
    astListener->m_onEnterEnumerate = zen_SymbolDefinitionListener_onEnterEnumerate;
    astListener->m_onExitEnumerate = zen_SymbolDefinitionListener_onExitEnumerate;

    return listener;
}

void zen_SymbolDefinitionListener_delete(zen_SymbolDefinitionListener_t* listener) {
    jtk_Assert_assertObject(listener, "The specified listener is null.");

    zen_SymbolDefinitionListener_destroy(listener->m_scopes);

    zen_ASTListener_delete(listener->m_astListener);
    jtk_Memory_deallocate(listener);
}

void zen_SymbolDefinitionListener_destroy(zen_ASTAnnotations_t* annotations) {
    jtk_Assert_assertObject(annotations, "The specified annotations is null.");

    /* There are three algorithms that can help us in the destruction of the
     * symbols and scopes.
     * 1. The symbols are destructed by their enclosing scopes when the scopes
     *    themselves are destroyed.
     * 2. The compilation unit scope can be queried for children. These children
     *    are destroyed in a recursive fashion. During each recursion a symbol
     *    is tested to see if it also represents a scope. In which case,
     *    the scope is queried for its children. The scope part of the symbol
     *    is also destroyed here. The cycle repeats until there are no symbols
     *    and scopes left to destroy.
     * 3. The AST annotations are iterated over to retrieve the scopes.
     *    In each iteration, the children symbols are retrieved and destroyed.
     *    It should be noted that the scopes are also destroyed here.
     *
     * Here, I have chosen to implement the third algorithm due to certain
     * circumstances.
     */

    jtk_ArrayList_t* temporary = jtk_ArrayList_new();
    jtk_Iterator_t* iterator = jtk_HashMap_getValueIterator(annotations->m_map);
    while (jtk_Iterator_hasNext(iterator)) {
        /* The scopes are created during the definition phase of the symbol table.
         * Therefore, we destroy them here before destroying the
         * jtk_SymbolDefinitionListener_t class instance itself.
         */
        zen_Scope_t* scope = (zen_Scope_t*)jtk_Iterator_getNext(iterator);

        /* Retrieve the children symbols declared in the current scope. */
        zen_Scope_getChildrenSymbols(scope, temporary);

        /* Iterate over the children symbols and destroy them. */
        int32_t limit = jtk_ArrayList_getSize(temporary);
        int32_t i;
        for (i = 0; i < limit; i++) {
            zen_Symbol_t* symbol = (zen_Symbol_t*)jtk_ArrayList_getValue(temporary, i);
            zen_SymbolDefinitionListener_destroySymbol(symbol);
        }

        /* At this point, the symbols retrieved form the scope are destroyed.
         * Therefore, remove references to them from the temporary list.
         */
        jtk_ArrayList_clear(temporary);

        /* Destroy the current scope. */
        zen_SymbolDefinitionListener_destroyScope(scope);
    }
    jtk_Iterator_delete(iterator);
    jtk_ArrayList_delete(temporary);
}

void zen_SymbolDefinitionListener_destroySymbol(zen_Symbol_t* symbol) {
    /* I could have designed the zen_Symbol_t and its "descendant" classes that
     * implement specific symbols like the jtk_Iterator_t and its "descendant"
     * classes such as jtk_ArrayListIterator_t. This would have helped
     * us delete specific symbols by simply invoking the zen_Symbol_delete()
     * function. However, to implement such a design would require me to
     * modify a few files for which I can spare no time. Further, the likelihood
     * of introducing new symbol categories is very rare. Therefore, I have chosen
     * to manually cast the context of the symbol before invoking a specific
     * destructor.
     *
     * NOTE: If a new symbol category is added and its instance is created during
     * the definition phase to the symbol table, then please add a respective
     * destructor call here.
     */
    zen_SymbolCategory_t category = zen_Symbol_getCategory(symbol);
    switch (category) {
        /* TODO: Should annotations simply be classes? In which case the
         * ZEN_SYMBOL_CATEGORY_ANNOTATION would be useless.
         */

        case ZEN_SYMBOL_CATEGORY_CLASS: {
            zen_ClassSymbol_t* classSymbol = (zen_ClassSymbol_t*)symbol->m_context;
            zen_ClassSymbol_delete(classSymbol);

            // printf("[debug] Destroyed a class symbol.\n");

            break;
        }

        case ZEN_SYMBOL_CATEGORY_ENUMERATE: {
            zen_EnumerateSymbol_t* enumerateSymbol = (zen_EnumerateSymbol_t*)symbol->m_context;
            zen_EnumerateSymbol_delete(enumerateSymbol);

            // printf("[debug] Destroyed a enumerate symbol.\n");

            break;
        }

        case ZEN_SYMBOL_CATEGORY_ENUMERATION: {
            zen_EnumerationSymbol_t* enumerationSymbol = (zen_EnumerationSymbol_t*)symbol->m_context;
            zen_EnumerationSymbol_delete(enumerationSymbol);

            // printf("[debug] Destroyed a enumeration symbol.\n");

            break;
        }

        case ZEN_SYMBOL_CATEGORY_FUNCTION: {
            zen_FunctionSymbol_t* functionSymbol = (zen_FunctionSymbol_t*)symbol->m_context;
            zen_FunctionSymbol_delete(functionSymbol);

            // printf("[debug] Destroyed a function symbol.\n");

            break;
        }

        case ZEN_SYMBOL_CATEGORY_CONSTANT: {
            zen_ConstantSymbol_t* constantSymbol = (zen_ConstantSymbol_t*)symbol->m_context;
            zen_ConstantSymbol_delete(constantSymbol);

            // printf("[debug] Destroyed a constant symbol.\n");

            break;
        }

        case ZEN_SYMBOL_CATEGORY_VARIABLE: {
            zen_VariableSymbol_t* variableSymbol = (zen_VariableSymbol_t*)symbol->m_context;
            zen_VariableSymbol_delete(variableSymbol);

            // printf("[debug] Destroyed a variable symbol.\n");

            break;
        }

        case ZEN_SYMBOL_CATEGORY_LABEL: {
            zen_LabelSymbol_t* labelSymbol = (zen_LabelSymbol_t*)symbol->m_context;
            zen_LabelSymbol_delete(labelSymbol);

            // printf("[debug] Destroyed a label symbol.\n");

            break;
        }
    }
}

void zen_SymbolDefinitionListener_destroyScope(zen_Scope_t* scope) {
    /* I could have designed the zen_Scope_t and its "descendant" classes that
     * implement specific scopes like the jtk_Iterator_t and its "descendant"
     * classes such as jtk_ArrayListIterator_t. This would have helped
     * us delete specific scopes by simply invoking the zen_Scope_delete()
     * function. However, to implement such a design would require me to
     * modify a few files for which I can spare no time. Further, the likelihood
     * of introducing new scope types is very rare. Therefore, I have chosen
     * to manually cast the context of the scope before invoking a specific
     * destructor.
     *
     * Since the jtk_HashMap_t class does not store the order in which the
     * scopes were inserted, the scopes are deleted in "random" order. With
     * the currently available scopes, this behavior should not really affect
     * the program in unexpected ways.
     *
     * NOTE: If a new scope type is added and its instance is created during
     * the definition phase to the symbol table, then please add a respective
     * destructor call here.
     */
    zen_ScopeType_t type = zen_Scope_getType(scope);
    switch (type) {
        /* TODO: Should annotations simply be classes? In which case the
         * ZEN_SCOPE_ANNOTATION would be useless.
         */

        case ZEN_SCOPE_COMPILATION_UNIT: {
            zen_CompilationUnitScope_t* compilationUnitScope =
                (zen_CompilationUnitScope_t*)scope->m_context;
            zen_CompilationUnitScope_delete(compilationUnitScope);

            // printf("[debug] Compilation unit scope deleted.\n");

            break;
        }


        case ZEN_SCOPE_ENUMERATION: {
            zen_EnumerationScope_t* enumerationScope =
                (zen_EnumerationScope_t*)scope->m_context;
            zen_EnumerationScope_delete(enumerationScope);

            // printf("[debug] Enumeration scope deleted.\n");

            break;
        }

        case ZEN_SCOPE_FUNCTION: {
            zen_FunctionScope_t* functionScope =
                (zen_FunctionScope_t*)scope->m_context;
            zen_FunctionScope_delete(functionScope);

            // printf("[debug] Function scope deleted.\n");

            break;
        }

        case ZEN_SCOPE_CLASS: {
            zen_ClassScope_t* classScope =
                (zen_ClassScope_t*)scope->m_context;
            zen_ClassScope_delete(classScope);

            // printf("[debug] Class scope deleted.\n");

            break;
        }

        case ZEN_SCOPE_LOCAL: {
            zen_LocalScope_t* localScope =
                (zen_LocalScope_t*)scope->m_context;
            zen_LocalScope_delete(localScope);

            // printf("[debug] Local scope deleted.\n");

            break;
        }
    }
}

zen_ASTListener_t* zen_SymbolDefinitionListener_getASTListener(zen_SymbolDefinitionListener_t* listener) {
    jtk_Assert_assertObject(listener, "The specified listener is null.");
    return listener->m_astListener;
}

/* compilationUnit */

void zen_SymbolDefinitionListener_onEnterCompilationUnit(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;

    /* The scopes are represented as an n-ary tree, where the root scope is
     * an instance of zen_CompilationUnitScope_t.
     */
    zen_CompilationUnitScope_t* compilationUnitScope = zen_CompilationUnitScope_new();
    zen_Scope_t* scope = zen_CompilationUnitScope_getScope(compilationUnitScope);
    zen_SymbolTable_setCurrentScope(listener->m_symbolTable, scope);
    zen_ASTAnnotations_put(listener->m_scopes, node, scope);
}

void zen_SymbolDefinitionListener_onExitCompilationUnit(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_SymbolTable_invalidateCurrentScope(listener->m_symbolTable);
}

/* functionDeclaration */

void zen_SymbolDefinitionListener_onEnterFunctionDeclaration(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_FunctionDeclarationContext_t* functionDeclarationContext = (zen_FunctionDeclarationContext_t*)node->m_context;
    zen_FunctionParametersContext_t* functionParametersContext = NULL;

    /* Retrieve the identifier associated with the function declaration. */
    zen_ASTNode_t* identifier = functionDeclarationContext->m_identifier;
    zen_Token_t* identifierToken = (zen_Token_t*)identifier->m_context;
    /* Retrieve the text representation of the identifier. */
    uint8_t* const identifierText = ((zen_Token_t*)identifier->m_context)->m_text;
    /* Retrieve the variableParameter associated with the context of the
     * function arguments.
     */
    jtk_ArrayList_t* fixedParameters = NULL;
    /* Retrieve the variableParameter associated with the context of the
     * function arguments.
     */
    zen_ASTNode_t* variableParameter = NULL;
    int32_t fixedParameterCount = 0;

    if (functionDeclarationContext->m_functionParameters != NULL) {
        functionParametersContext = (zen_FunctionParametersContext_t*)functionDeclarationContext->m_functionParameters->m_context;
        fixedParameters = functionParametersContext->m_fixedParameters;
        variableParameter = functionParametersContext->m_variableParameter;
        fixedParameterCount = jtk_ArrayList_getSize(fixedParameters);
    }

    /* Retrieve the symbol table associated with this listener. */
    zen_SymbolTable_t* symbolTable = listener->m_symbolTable;
    /* Retrieve the current scope of the symbol table. */
    zen_Scope_t* currentScope = zen_SymbolTable_getCurrentScope(symbolTable);

    if ((zen_Token_getType(identifierToken) == ZEN_TOKEN_KEYWORD_STATIC) && ((fixedParameterCount > 0) || (variableParameter != NULL))) {
        zen_ErrorHandler_reportError(NULL, "Static initializer with parameters", identifierToken);
    }
    else {
        if (zen_Scope_isCompilationUnitScope(currentScope) || zen_Scope_isClassScope(currentScope) || zen_Scope_isLocalScope(currentScope)) {
            /* Resolve the identifier within the scope of the compilation unit. */
            zen_Symbol_t* symbol = NULL;

            if (zen_Scope_isClassScope(currentScope)) {
                symbol = zen_Scope_resolve(currentScope, identifierText);
            }
            else {
                symbol = zen_SymbolTable_resolve(symbolTable, identifierText);
            }

            if (symbol != NULL) {
                /* If a symbol with the given identifier exists, make sure it is
                 * a function before overloading.
                 */
                if (zen_Symbol_isFunction(symbol)) {
                    /* The resolved symbol is a function. Retrieve the corresponding
                     * function symbol.
                     */
                    zen_FunctionSymbol_t* functionSymbol = (zen_FunctionSymbol_t*)symbol->m_context;
                    zen_SymbolDefinitionListener_declareOverloadedFunction(functionSymbol, fixedParameters, variableParameter);
                }
                else {
                    zen_ErrorHandler_reportError(NULL, "Redeclaration of symbol as function", identifierToken);
                }
            }
            else {
                symbol = zen_SymbolDefinitionListener_declareFunction(listener->m_symbolTable, identifier, fixedParameters, variableParameter);

                zen_ClassMemberContext_t* classMemberContext = (zen_ClassMemberContext_t*)node->m_parent->m_context;
                int32_t modifierCount = jtk_ArrayList_getSize(classMemberContext->m_modifiers);
                int32_t i;
                for (i = 0; i < modifierCount; i++) {
                    zen_ASTNode_t* modifier = (zen_ASTNode_t*)jtk_ArrayList_getValue(classMemberContext->m_modifiers, i);
                    zen_Token_t* token = (zen_Token_t*)modifier->m_context;
                    uint32_t modifiers = zen_TokenType_toModifiers(token->m_type);
                    zen_Symbol_addModifiers(symbol, modifiers, modifier);
                }
            }
        }
        else {
            fprintf(stderr, "[internal error] Declaring function in unsuitable scope\n");
        }
    }

    /* A function scope is pushed regardless of the declaration being erroneous. */

    zen_FunctionScope_t* functionScope = zen_FunctionScope_new(symbolTable->m_currentScope);
    zen_Scope_t* scope = zen_FunctionScope_getScope(functionScope);
    zen_SymbolTable_setCurrentScope(listener->m_symbolTable, scope);
    zen_ASTAnnotations_put(listener->m_scopes, node, scope);

    int32_t i;
    for (i = 0; i < fixedParameterCount; i++) {
        zen_ASTNode_t* parameter = (zen_ASTNode_t*)jtk_ArrayList_getValue(fixedParameters, i);
        const uint8_t* parameterText = zen_Token_getText((zen_Token_t*)(parameter->m_context));
        zen_Symbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, parameterText);
        if (symbol != NULL) {
            zen_ErrorHandler_reportError(NULL, "Redeclaration of symbol as parameter", (zen_Token_t*)(parameter->m_context));
        }
        else {
            zen_ConstantSymbol_t* constantSymbol = zen_ConstantSymbol_new(parameter, symbolTable->m_currentScope);
            symbol = zen_ConstantSymbol_getSymbol(constantSymbol);
            zen_SymbolTable_define(listener->m_symbolTable, symbol);
        }
    }

    if (variableParameter != NULL) {
        const uint8_t* parameterText = zen_Token_getText((zen_Token_t*)(variableParameter->m_context));
        zen_Symbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, parameterText);
        if (symbol != NULL) {
            zen_ErrorHandler_reportError(NULL, "Redeclaration of symbol as variable parameter", (zen_Token_t*)(variableParameter->m_context));
        }
        else {
            zen_ConstantSymbol_t* constantSymbol = zen_ConstantSymbol_new(variableParameter, symbolTable->m_currentScope);
            symbol = zen_ConstantSymbol_getSymbol(constantSymbol);
            zen_Symbol_addModifiers(symbol, ZEN_MODIFIER_VARIABLE_PARAMETER);
            zen_SymbolTable_define(listener->m_symbolTable, symbol);
        }
    }
}

void zen_SymbolDefinitionListener_onExitFunctionDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;

    zen_SymbolTable_invalidateCurrentScope(listener->m_symbolTable);
}

/* functionParameters */

// TODO: constructorArguments

// TODO: Declare function even if it has no parameters.

void zen_SymbolDefinitionListener_declareOverloadedFunction(zen_FunctionSymbol_t* functionSymbol,
    jtk_ArrayList_t* fixedParameters, zen_ASTNode_t* variableParameter) {

    zen_Token_t* identifierToken = (zen_Token_t*)functionSymbol->m_symbol->m_identifier->m_context;

    /* Determines whether the overloaded function is semantically erroneous. */
    bool error = false;

    /* If the enclosing scope is same as the current scope, we
     * need to verify the signature of the function being declared
     * to be unique.
     */

    /* Let X be a function. If an overload of function X, say Xi, has k
     * fixed parameters and a variable parameter, then the maximum number
     * of fixed parameters for any overload of function X is limited to k - 1.
     * Where, k is known as the parameter threshold of function X.
     */
    int32_t parameterThreshold = zen_FunctionSymbol_getParameterThreshold(functionSymbol);

    /* If currently there is no parameter threshold, try to update the
     * threshold.
     */
    int32_t fixedParameterCount = (fixedParameters != NULL)? jtk_ArrayList_getSize(fixedParameters) : 0;
    if (parameterThreshold < 0) {
        /* Update the threshold if the function being declared has a variable
         * parameter.
         */
        if (variableParameter != NULL) {
            parameterThreshold = fixedParameterCount;
            zen_FunctionSymbol_setParameterThreshold(functionSymbol, parameterThreshold);
        }
    }

    /* Retrieve the overloaded signatures of the function symbol. */
    jtk_ArrayList_t* signatures = zen_FunctionSymbol_getSignatures(functionSymbol);
    /* Determine the number of signatures. */
    int32_t size = jtk_ArrayList_getSize(signatures);
    /* Iterate over the signatures to determine if the signature of the
     * function being declared to be unique, or not.
     */
    int32_t i;
    for (i = 0; i < size; i++) {
        zen_FunctionSignature_t* signature = (zen_FunctionSignature_t*)jtk_ArrayList_getValue(signatures, i);
        if ((signature->m_variableParameter != NULL) && (variableParameter != NULL)) {
            zen_ErrorHandler_reportError(NULL, "Multiple overloads with variable parameter", (zen_Token_t*)(variableParameter->m_context));
            error = true;
        }
        else {
            int32_t fixedParameterCount0 = (signature->m_fixedParameters != NULL)? jtk_ArrayList_getSize(signature->m_fixedParameters) : 0;
            /* Determine whether the function being declared was duplicately
             * overloaded, or not.
             */
            if ((fixedParameterCount0 == fixedParameterCount) &&
                ((signature->m_variableParameter == NULL) &&
                 (variableParameter == NULL))) {
                zen_ErrorHandler_reportError(NULL, "Duplicate overload", identifierToken);
                error = true;
                break;
            }
            else {
                if (variableParameter != NULL) {
                    /* The function being declared caused the update of parameter
                     * threshold. Make sure that the number of parameters is
                     * less than the parameter threshold for previously defined
                     * functions.
                     */
                    if (fixedParameterCount0 >= parameterThreshold) {
                        // BUG: The error report does not point to the actual error.
                        // We should do something about FunctionSignature.
                        zen_ErrorHandler_reportError(NULL, "Exceeds parameter threshold", identifierToken);
                        error = true;
                        /* TODO: Flag the section that is concerened with handling identification
                         * of function violating the parameter threshold.
                         */
                    }
                }
                else {
                    /* The function being declared did not cause the update of parameter threshold.
                     * A parameter threshold may have existed before this event was fired. Make sure
                     * that the function obliges the parameter threshold.
                     */
                    if ((parameterThreshold >= 0) && (fixedParameterCount >= parameterThreshold)) {
                        zen_ErrorHandler_reportError(NULL, "Exceeds parameter threshold", identifierToken);
                        error = true;
                        break;
                    }
                }
            }
        }
    }

    if (!error) {
        /* The function being declared is an overload of an existing
         * function. Add the signature of the function.
         */
        zen_FunctionSignature_t* signature = zen_FunctionSignature_new(fixedParameters, variableParameter);
        zen_FunctionSymbol_addSignature(functionSymbol, signature);
    }
}

zen_Symbol_t* zen_SymbolDefinitionListener_declareFunction(zen_SymbolTable_t* symbolTable,
    zen_ASTNode_t* identifier, jtk_ArrayList_t* fixedParameters,
    zen_ASTNode_t* variableParameter) {
    /* Create a member function symbol to store in the symbol table. */
    zen_FunctionSymbol_t* functionSymbol = zen_FunctionSymbol_new(identifier, symbolTable->m_currentScope);
    if (variableParameter != NULL) {
        int32_t parameterThreshold = jtk_ArrayList_getSize(fixedParameters);
        zen_FunctionSymbol_setParameterThreshold(functionSymbol, parameterThreshold);
    }
    /* In order to enable the symbol table to store overloaded functions,
     * we employ function signatures. Add the function signature
     * corresponding to the function being declared to the newly
     * created member function symbol.
     */
    zen_FunctionSignature_t* signature = zen_FunctionSignature_new(fixedParameters, variableParameter);
    zen_FunctionSymbol_addSignature(functionSymbol, signature);
    /* Retrieve the symbol corresponding to the member function symbol. */
    zen_Symbol_t* symbol = zen_FunctionSymbol_getSymbol(functionSymbol);
    /* Define the symbol in the symbol table. */
    zen_SymbolTable_define(symbolTable, symbol);

    return symbol;
}

void zen_SymbolDefinitionListener_onEnterStatementSuite(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_LocalScope_t* localScope = zen_LocalScope_new(listener->m_symbolTable->m_currentScope);
    zen_Scope_t* scope = zen_LocalScope_getScope(localScope);
    zen_SymbolTable_setCurrentScope(listener->m_symbolTable, scope);
    zen_ASTAnnotations_put(listener->m_scopes, node, scope);
}

void zen_SymbolDefinitionListener_onExitStatementSuite(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_SymbolTable_invalidateCurrentScope(listener->m_symbolTable);
}

void zen_SymbolDefinitionListener_onEnterVariableDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_VariableDeclarationContext_t* variableDeclarationContext = (zen_VariableDeclarationContext_t*)node->m_context;

    int32_t size = jtk_ArrayList_getSize(variableDeclarationContext->m_variableDeclarators);
    int32_t i;
    for (i = 0; i < size; i++) {
        zen_ASTNode_t* variableDeclarator = jtk_ArrayList_getValue(variableDeclarationContext->m_variableDeclarators, i);
        zen_VariableDeclaratorContext_t* variableDeclaratorContext = (zen_VariableDeclaratorContext_t*)variableDeclarator->m_context;

        zen_ASTNode_t* identifier = variableDeclaratorContext->m_identifier;
        const uint8_t* identifierText = zen_Token_getText((zen_Token_t*)identifier->m_context);

        zen_Symbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, identifierText);
        if (symbol != NULL) {
            zen_ErrorHandler_reportError(NULL, "Redeclaration of symbol as variable", (zen_Token_t*)identifier->m_context);
        }
        else {
            zen_VariableSymbol_t* variableSymbol = zen_VariableSymbol_new(identifier, listener->m_symbolTable->m_currentScope);
            symbol = zen_VariableSymbol_getSymbol(variableSymbol);
            zen_SymbolTable_define(listener->m_symbolTable, symbol);
        }
    }
}

void zen_SymbolDefinitionListener_onEnterConstantDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_ConstantDeclarationContext_t* constantDeclarationContext = (zen_ConstantDeclarationContext_t*)node->m_context;

    int32_t size = jtk_ArrayList_getSize(constantDeclarationContext->m_constantDeclarators);
    int32_t i;
    for (i = 0; i < size; i++) {
        zen_ASTNode_t* constantDeclarator = jtk_ArrayList_getValue(constantDeclarationContext->m_constantDeclarators, i);
        zen_ConstantDeclaratorContext_t* constantDeclaratorContext = (zen_ConstantDeclaratorContext_t*)constantDeclarator->m_context;

        zen_ASTNode_t* identifier = constantDeclaratorContext->m_identifier;
        const uint8_t* identifierText = zen_Token_getText((zen_Token_t*)identifier->m_context);

        zen_Symbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, identifierText);
        if (symbol != NULL) {
            zen_ErrorHandler_reportError(NULL, "Redeclaration of symbol as constant", (zen_Token_t*)identifier->m_context);
        }
        else {
            zen_ConstantSymbol_t* constantSymbol = zen_ConstantSymbol_new(identifier, listener->m_symbolTable->m_currentScope);
            symbol = zen_ConstantSymbol_getSymbol(constantSymbol);
            zen_SymbolTable_define(listener->m_symbolTable, symbol);
        }
    }
}

void zen_SymbolDefinitionListener_onEnterLabelClause(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_LabelClauseContext_t* LabelClauseContext = (zen_LabelClauseContext_t*)node->m_context;

    zen_ASTNode_t* identifier = LabelClauseContext->m_identifier;
    const uint8_t* identifierText = zen_Token_getText((zen_Token_t*)identifier->m_context);

    zen_Symbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, identifierText);
    if (symbol != NULL) {
        zen_ErrorHandler_reportError(NULL, "Redeclaration of symbol as label", (zen_Token_t*)identifier->m_context);
    }
    else {
        zen_LabelSymbol_t* labelSymbol = zen_LabelSymbol_new(identifier, listener->m_symbolTable->m_currentScope);
        symbol = zen_LabelSymbol_getSymbol(labelSymbol);
        zen_SymbolTable_define(listener->m_symbolTable, symbol);
    }
}

// TODO: Each for statement should receive its own local scope to define
// its item variable.
void zen_SymbolDefinitionListener_onEnterForParameters(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_ForParameterContext_t* forParametersContext = (zen_ForParameterContext_t*)node->m_context;

    if (forParametersContext->m_declaration) {
        zen_ASTNode_t* identifier = forParametersContext->m_identifier;
        const uint8_t* identifierText = zen_Token_getText((zen_Token_t*)identifier->m_context);

        zen_Symbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, identifierText);
        if (symbol != NULL) {
            zen_ErrorHandler_reportError(NULL, "Redeclaration of symbol as loop parameter", (zen_Token_t*)identifier->m_context);
        }
        else {
            if (forParametersContext->m_variable) {
                zen_VariableSymbol_t* variableSymbol = zen_VariableSymbol_new(identifier, listener->m_symbolTable->m_currentScope);
                symbol = zen_VariableSymbol_getSymbol(variableSymbol);
            }
            else {
                zen_ConstantSymbol_t* constantSymbol = zen_ConstantSymbol_new(identifier, listener->m_symbolTable->m_currentScope);
                symbol = zen_ConstantSymbol_getSymbol(constantSymbol);
            }
            zen_SymbolTable_define(listener->m_symbolTable, symbol);
        }
    }
}

void zen_SymbolDefinitionListener_onEnterTryStatement(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_TryStatementContext_t* context = (zen_TryStatementContext_t*)node->m_context;
    zen_ASTWalker_walk(astListener, context->m_tryClause);

    int32_t catchClauseCount = jtk_ArrayList_getSize(context->m_catchClauses);
    int32_t i;
    for (i = 0; i < catchClauseCount; i++) {
        zen_ASTNode_t* catchClause = (zen_ASTNode_t*)jtk_ArrayList_getValue(context->m_catchClauses, i);
        zen_CatchClauseContext_t* catchClauseContext = (zen_CatchClauseContext_t*)catchClause->m_context;

        /* Each catch clause receives its own local scope to define its
         * catch parameter.
         */
        zen_LocalScope_t* localScope = zen_LocalScope_new(listener->m_symbolTable->m_currentScope);
        zen_Scope_t* scope = zen_LocalScope_getScope(localScope);
        zen_SymbolTable_setCurrentScope(listener->m_symbolTable, scope);
        zen_ASTAnnotations_put(listener->m_scopes, catchClause, scope);
        
        zen_ASTNode_t* identifier = catchClauseContext->m_identifier;
        int32_t identifierSize;
        uint8_t* identifierText = zen_ASTNode_toCString(identifier, &identifierSize);
        
        zen_Symbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, identifierText);
        if (symbol != NULL) {
            zen_ErrorHandler_reportError(NULL, "Redeclaration of symbol as catch parameter", (zen_Token_t*)identifier->m_context);
        }
        else {
            zen_VariableSymbol_t* variableSymbol = zen_VariableSymbol_new(identifier, scope);
            symbol = zen_VariableSymbol_getSymbol(variableSymbol);
            
            zen_SymbolTable_define(listener->m_symbolTable, symbol);
        }

        /* Visit the scopes of the statement suite specified to the
         * catch clause.
         */
        zen_ASTWalker_walk(astListener, catchClauseContext->m_statementSuite);

        zen_SymbolTable_invalidateCurrentScope(listener->m_symbolTable);

        /* Destroy the identifier text. It is not required anymore. */
        jtk_Memory_deallocate(identifierText);
    }

    zen_ASTListener_skipChildren(astListener);
}

void zen_SymbolDefinitionListener_onEnterClassDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_ClassDeclarationContext_t* classDeclarationContext = (zen_ClassDeclarationContext_t*)node->m_context;

    zen_ASTNode_t* identifier = classDeclarationContext->m_identifier;
    zen_Token_t* identifierToken = (zen_Token_t*)identifier->m_context;
    const uint8_t* identifierText = zen_Token_getText(identifierToken);

    zen_Symbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, identifierText);
    if (symbol != NULL) {
        zen_ErrorHandler_reportError(NULL, "Redeclaration of symbol as class", (zen_Token_t*)identifier->m_context);
    }
    else {
        jtk_String_t* qualifiedName = NULL;
        if (listener->m_package != NULL) {
            qualifiedName = jtk_String_newFromJoinEx(listener->m_package->m_value,
                listener->m_package->m_size, identifierToken->m_text, identifierToken->m_length);
        }
        else {
            qualifiedName = jtk_String_newEx(identifierToken->m_text, identifierToken->m_length);
        }

        zen_ClassScope_t* classScope = zen_ClassScope_new(listener->m_symbolTable->m_currentScope);

        zen_ClassSymbol_t* classSymbol = zen_ClassSymbol_new(identifier,
            listener->m_symbolTable->m_currentScope, zen_ClassScope_getScope(classScope),
            qualifiedName);
        symbol = zen_ClassSymbol_getSymbol(classSymbol);
        zen_SymbolTable_define(listener->m_symbolTable, symbol);

        classScope->m_classSymbol = symbol;

        zen_Scope_t* scope = zen_ClassScope_getScope(classScope);

        zen_SymbolTable_setCurrentScope(listener->m_symbolTable, scope);
        zen_ASTAnnotations_put(listener->m_scopes, node, scope);
    }
}

void zen_SymbolDefinitionListener_onExitClassDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_ClassDeclarationContext_t* classDeclarationContext = (zen_ClassDeclarationContext_t*)node->m_context;

    zen_SymbolTable_invalidateCurrentScope(listener->m_symbolTable);
}

void zen_SymbolDefinitionListener_onEnterEnumerationDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_EnumerationDeclarationContext_t* enumerationDeclarationContext = (zen_EnumerationDeclarationContext_t*)node->m_context;

    zen_ASTNode_t* identifier = enumerationDeclarationContext->m_identifier;
    zen_Token_t* identifierToken = (zen_Token_t*)identifier->m_context;
    const uint8_t* identifierText = zen_Token_getText(identifierToken);

    zen_Symbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, identifierText);
    if (symbol != NULL) {
        zen_ErrorHandler_reportError(NULL, "Redeclaration of symbol as enumeration", (zen_Token_t*)identifier->m_context);
    }
    else {
        zen_EnumerationScope_t* enumerationScope = zen_EnumerationScope_new(listener->m_symbolTable->m_currentScope);

        zen_EnumerationSymbol_t* enumerationSymbol = zen_EnumerationSymbol_new(identifier, listener->m_symbolTable->m_currentScope, zen_EnumerationScope_getScope(enumerationScope));
        symbol = zen_EnumerationSymbol_getSymbol(enumerationSymbol);
        zen_SymbolTable_define(listener->m_symbolTable, symbol);

        enumerationScope->m_enumerationSymbol = symbol;

        zen_Scope_t* scope = zen_EnumerationScope_getScope(enumerationScope);

        zen_SymbolTable_setCurrentScope(listener->m_symbolTable, scope);
        zen_ASTAnnotations_put(listener->m_scopes, node, scope);
    }
}

void zen_SymbolDefinitionListener_onExitEnumerationDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_EnumerationDeclarationContext_t* enumerationDeclarationContext = (zen_EnumerationDeclarationContext_t*)node->m_context;

    zen_SymbolTable_invalidateCurrentScope(listener->m_symbolTable);
}

void zen_SymbolDefinitionListener_onEnterEnumerate(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_EnumerationDeclarationContext_t* enumerationDeclarationContext = (zen_EnumerationDeclarationContext_t*)node->m_context;

    zen_ASTNode_t* identifier = enumerationDeclarationContext->m_identifier;
    zen_Token_t* identifierToken = (zen_Token_t*)identifier->m_context;
    const uint8_t* identifierText = zen_Token_getText(identifierToken);

    zen_Scope_t* currentScope = zen_SymbolTable_getCurrentScope(listener->m_symbolTable);
    if (zen_Scope_isEnumerationScope(currentScope)) {
        zen_Symbol_t* symbol = zen_Scope_resolve(currentScope, identifierText);
        if (symbol != NULL) {
            zen_ErrorHandler_reportError(NULL, "Duplicate enumerate", (zen_Token_t*)identifier->m_context);
        }
        else {
            zen_EnumerateSymbol_t* enumerateSymbol = zen_EnumerateSymbol_new(identifier, listener->m_symbolTable->m_currentScope);
            symbol = zen_EnumerateSymbol_getSymbol(enumerateSymbol);
            zen_SymbolTable_define(listener->m_symbolTable, symbol);
        }
    }
    else {
        fprintf(stderr, "[internal error] Declaration of enumerate in unsuitable scope.\n");
    }
}

void zen_SymbolDefinitionListener_onExitEnumerate(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_EnumerationDeclarationContext_t* enumerationDeclarationContext = (zen_EnumerationDeclarationContext_t*)node->m_context;
}

/* When an error occurs in a nested function, it prevents the listener from
 * creating a scope. But then, when exit event invalidates a scope which does
 * not exist. This causes the program to crash.
 * Fix this.
 * Further, a match parameter should be accepted by the invalidate function
 * to prevent such mismatches.
 */

/* When a function declaration is erroneous, it is not defined in the symbol
 * table. But a corresponding scope is openened anyway. Who deletes this scope?
 */

/* TODO: BUG!!!! When an error occurs in a class or enumeration declaration,
   a scope is not opened. But when the declaration context exits the listener
   automatically closes a scope which does not exist! This causes a segmentation
   fault. A possible fix is to use a state variable. But then it will
   get tampered a nested declaration occurs!
*/