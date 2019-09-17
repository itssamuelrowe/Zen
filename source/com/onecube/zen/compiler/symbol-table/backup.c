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

#include <com/onecube/zen/compiler/astNode.h>
#include <com/onecube/zen/Token.h>
#include <com/onecube/zen/st/ClassScope.h>
#include <com/onecube/zen/st/FunctionSignature.h>
#include <com/onecube/zen/st/FunctionSymbol.h>
#include <com/onecube/zen/st/SymbolDefinitionListener.h>
#include <com/onecube/zen/st/SymbolTable.h>

zen_SymbolDefinitionListener_t* zen_SymbolDefinitionListener_new(zen_SymbolTable_t* symbolTable) {
    zen_SymbolDefinitionListener_t* listener = zen_Memory_allocate(zen_SymbolDefinitionListener_t, 1);
    listener->m_symbolTable = symbolTable;
    listener->m_astListener = zen_ASTListener_newWithContext(listener);
    listener->m_functionDeclarationContext = NULL;
    
    zen_ASTListener_t* astListener = listener->m_astListener;

    astListener->m_onEnterFunctionDeclaration = zen_SymbolDefinitionListener_onEnterFunctionDeclaration;
    astListener->m_onExitFunctionDeclaration = zen_SymbolDefinitionListener_onExitFunctionDeclaration;

    astListener->m_onEnterFunctionArguments = zen_SymbolDefinitionListener_onEnterFunctionArguments;

    return listener;
}

/* functionDeclaration */

void zen_SymbolDefinitionListener_onEnterFunctionDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_FunctionDeclarationContext_t* functionDeclarationContext = (zen_FunctionDeclarationContext_t*)node->m_context;

    listener->m_functionDeclarationContext = functionDeclarationContext;
}

void zen_SymbolDefinitionListener_onExitFunctionDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;

    listener->m_functionDeclarationContext = NULL;
}

/* functionParameters */

// TODO: constructorArguments

void zen_SymbolDefinitionListener_onEnterFunctionParameters(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_FunctionParametersContext_t* functionParametersContext = (zen_FunctionParametersContext_t*)node->m_context;

    zen_FunctionDeclarationContext_t* functionDeclarationContext = listener->m_functionDeclarationContext;

    /* Retrieve the identifier associated with the function declaration. */
    zen_ASTNode_t* identifier = functionDeclarationContext->m_identifier;
    /* Retrieve the text representation of the identifier. */
    uint8_t* const identifierText = ((zen_Token_t*)identifier->m_context)->m_text;
    /* Retrieve the variableParameter associated with the context of the
     * function arguments.
     */
    jtk_ArrayList_t* fixedParameters = functionParametersContext->m_fixedParameters;
    /* Retrieve the variableParameter associated with the context of the
     * function arguments.
     */
    zen_ASTNode_t* variableParameter = functionParametersContext->m_variableParameter;
    /* Retrieve the symbol table associated with this listener. */
    zen_SymbolTable_t* symbolTable = listener->m_symbolTable;
    /* Retrieve the current scope of the symbol table. */
    zen_Scope_t* currentScope = zen_SymbolTable_getCurrentScope(symbolTable);

    if (zen_Scope_isClassScope(currentScope)) {
        /* If the current scope is a class scope, retrieve the class scope
         * corresponding to the current scope for further examination.
         */
        zen_ClassScope_t* classScope = (zen_ClassScope_t*)currentScope->m_context;
        /* Resolve the identifier within the scope of the class and the
         * scopes of its superclasses.
         */
        zen_Symbol_t* symbol = zen_SymbolTable_resolveClassMember(symbolTable,
            classScope, identifierText);
        if (symbol != NULL) {
            /* If a symbol with the given identifier exists, make sure it is
             * a function.
             */
            if (zen_Symbol_isMemberFunction(symbol)) {
                /* Determines whether the function definition is an overloaded
                 * version, or not.
                 */
                bool duplicateOverload = false;
                /* Determines whether the function definition ignores the
                 * parameter threshold.
                 */
                bool ignoresParameterThreshold = false;
                /* The resolved symbol is a member function. Retrieve the corresponding
                 * member function symbol.
                 */
                zen_FunctionSymbol_t* memberFunctionSymbol = (zen_FunctionSymbol_t*)symbol->m_context;
                /* Retrieve the scope within which the symbol was defined. */
                zen_Scope_t* enclosingScope = zen_Symbol_getEnclosingScope(symbol);
                if (enclosingScope == currentScope) {
                    /* If the enclosing scope is same as the current scope, we
                     * need to verify the signature of the function being declared
                     * to be unique.
                     */
                    /* Retrieve the overloaded signatures of the function symbol. */
                    jtk_ArrayList_t* signatures = zen_FunctionSymbol_getSignatures(memberFunctionSymbol);
                    /* Let X be a function. If an overload of function X, say Xi, has k
                     * fixed parameters and a variable parameter, then the maximum number
                     * of fixed parameters for any overload of function X is limited to k.
                     * Where, k is known as the parameter threshold of function X.
                     */
                    int32_t parameterThreshold = zen_FunctionSymbol_getParameterThreshold(memberFunctionSymbol);
                    
                    /* If currently there is no parameter threshold, try to update the
                     * threshold.
                     */
                    int32_t fixedParameterCount = jtk_ArrayList_getSize(fixedParameters);
                    if (parameterThreshold < 0) {
                        /* Update the threshold if the function being declared has a variable
                         * parameter.
                         */
                        if (variableParameter != NULL) {
                            parameterThreshold = fixedParameterCount;
                            zen_FunctionSymbol_setParameterThreshold(memberFunctionSymbol, parameterThreshold);
                        }
                    }
                    
                    /* Determine the number of signatures. */
                    int32_t size = jtk_ArrayList_getSize(signatures);
                    /* Iterate over the signatures to determine if the signature of the
                     * function being declared to be unique, or not.
                     */
                    int32_t i;
                    for (i = 0; i < size; i++) {
                        zen_FunctionSignature_t* signature = (zen_FunctionSignature_t*)jtk_ArrayList_getValue(signatures, i);
                        if ((signature->m_variableParameter != NULL) && (variableParameter != NULL)) {
                            // semantic error: Multiple overloads cannot have variable parameter.
                        }
                        else {
                            int32_t fixedParameterCount0 = jtk_ArrayList_getSize(signature->m_fixedParameters);
                            /* Determine whether the function being declared was duplicately
                             * overloaded, or not.
                             */
                            if ((fixedParameterCount0 == fixedParameterCount) &&
                                ((signature->m_variableParameter == NULL) &&
                                 (variableParameter == NULL))) {
                                // semantic error: Duplicate overload
                                /* Flag the section that is concerned with handling duplicate
                                 * overloads.
                                 */
                                duplicateOverload = true;
                                break;
                            }
                            else {
                                if (variableParameter != NULL) {
                                    /* The function being declared caused the update of parameter
                                     * threshold. Make sure that the number of parameters is
                                     * less than the parameter threshold for previously defined
                                     * functions.
                                     */
                                    if (fixedParameterCount0 > parameterThreshold) {
                                        // semantic error: More parameters than parameter threshold.
                                    }
                                }
                                else {
                                    /* The function being declared did not cause the update of
                                     * parameter threshold. Apparently, a parameter threshold
                                     * existed before this event was fired. Make sure that
                                     * the function obliges the parameter threshold.
                                     */
                                    if (fixedParameterCount > parameterThreshold) {
                                        // semantic error: More parameters than parameter threshold.
                                        /* Flag the section that is concerened with handling definition
                                         * of a function which ignores parameter threshold.
                                         */
                                        ignoresParameterThreshold = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    if (duplicateOverload) {
                        /* semantic error: Duplicate function overload */
                    }
                    else if (ignoresParameterThreshold) {
                        /* semantic error: More parameters than parameter threshold. */
                    }
                    else {
                        /* The function being declared is an overload of an existing
                         * function. Add the signature of the function.
                         */
                        zen_FunctionSignature_t* signature = zen_FunctionSignature_new(fixedParameters, variableParameter);
                        zen_FunctionSymbol_addSignature(memberFunctionSymbol, signature);
                    }
                }
            }
            else {
                /* semantic error: Redeclaration of a symbol as function */
            }
        }
        else {
            /* Create a member function symbol to store in the symbol table. */
            zen_FunctionSymbol_t* memberFunctionSymbol = zen_FunctionSymbol_new(identifier);
            /* In order to enable the symbol table to store overloaded functions,
             * we employe function signatures. Add the function signature
             * corresponding to the function being declared to the newly
             * created member function symbol.
             */
            zen_FunctionSignature_t* signature = zen_FunctionSignature_new(fixedParameters, variableParameter);
            zen_FunctionSymbol_addSignature(memberFunctionSymbol, signature);
            /* Retrieve the symbol corresponding to the member function symbol. */
            symbol = zen_FunctionSymbol_getSymbol(memberFunctionSymbol);
            /* Define the symbol in the symbol table. */
            zen_SymbolTable_defineClassMember(symbolTable, currentScope, symbol);
        }
    }
}

/*
void zen_SymbolDefinitionListener_onEnter(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");
}
*/