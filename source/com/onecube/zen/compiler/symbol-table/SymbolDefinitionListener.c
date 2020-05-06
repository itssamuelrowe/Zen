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

#include <jtk/core/CString.h>
#include <jtk/collection/stack/LinkedStack.h>

#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/lexer/Token.h>
#include <com/onecube/zen/compiler/symbol-table/FunctionSignature.h>
#include <com/onecube/zen/compiler/symbol-table/FunctionSymbol.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolDefinitionListener.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolTable.h>

/*
 * As new features are implemented, the source code of a project grows in
 * complexity. It becomes hard to maintain the source code when everything is
 * crammed into a single source file. Therefore, an intuitive feature of a
 * programming language is to allow programmers to split their source code
 * into multiple files. The symbols defined in these source files can be imported
 * and exported outside a given source file or compilation unit as needed.
 *
 * In Zen, the import statement allows a compilation unit to refer to external
 * entities such as functions and classes. Without the use of the import statement,
 * the only way to refer to an entity outside the current compilatiin unit is to
 * use a fully qualified name. Further, all top-level entities declared in Zen
 * are exported by default. As of this writing, there is no way to override this
 * behaviour.
 *
 * Albeit the import/export mechanism is complicated, it is an interesting topic.
 * The following article describes the import/export mechanism employed by the
 * Zen compiler behind the scenes.
 *
 * ## Entity Forms
 *
 * A compilation unit can import entities that are either compiled or non-compiled.
 * (Note that the design assumes that the form of an entity is transparent
 * to the end user, i.e., the programmer.)
 *  1. Entities whose equivalent ".feb" files exist on the filsystem in the
 *     entity lookup directories specified to the compiler. These entities are
 *     referred to as compiled entities.
 *  2. Entities that are part of the current compilation batch. These entites
 *     are referred to as non-compiled entities.
 *
 * ## The Global Symbol Cache
 *
 * Given we are dealing with two forms of entities (compiled and non-compiled),
 * the design should allow for an abstraction which allows layers above the
 * import mechanism to work seamlessly without having to worry about an entities
 * form. In order to implement such an abstraction, a central repository known
 * as the global symbol cache is maintained by the compiler.
 *
 * The global symbol cache holds symbols which represent external entities. Here,
 * an external entity refers to any entity that is defined outside a compilation
 * unit. Internally, the global symbol cache uses a hash map to keep track of
 * the registered symbols. The various phases of the compiler can request the
 * global symbol cache to acquire symbols corresponding to entities. However,
 * symbols exported from compilation units that are part of the current compilation
 * batch are not available until the definition phase of the compiler is over.
 * In other words, non-compiled entities are not available until the definition
 * phase is complete.
 *
 * The global symbol cache satifies a symbol request in one of the following ways:
 * 1. If the requested symbol is found in the internal hash map, it is
 *    immediately returned.
 * 2. If the requested symbol is not found in the internal hash map, the cache
 *    searches the entity lookup directories to find a binary entity. If found,
 *    it loads the binary entity using the embedded binary entity loader and a
 *    corresponding symbol is created, inserted in the internal hash map, and
 *    returned. Otherwise, the request fails and null is returned. This allows
 *    the compiler to load external entities from their binary counterpart without
 *    the requesting party to manually deal with external entities.
 *
 *    What about non-compiled external entities? We know that such entities are
 *    part of the current compilation batch. The compiler can take advantage of
 *    this fact. During the definition phase, the compiler registers any symbol
 *    that is considered as an external symbol to the global symbol cache. This allows
 *    compilation units in the current compilation batch to reference symbols
 *    declared in another compilation unit. For example, consider two compilation
 *    units `BirdWatch.zen` and `Sparrow.zen` in the current compilation batch which
 *    declare the `BirdWatch` and `Sparrow` classes, respectively.
 *    Further, assume that `BirdWatch.zen` imports `Sparrow`. During the definition
 *    phase corresponding to `Sparrow.zen`, a class symbol for `Sparrow` is registered
 *    in the global cache. This allows `BirdWatch` to refer to the external entity
 *    `Sparrow` which is part of the current compilation batch.
 *
 * ## Phases of the Compiler
 *
 * For multiple input source files, the compiler takes a source file and subjects
 * it to various phases in a parallel sequence, where the phases include, lexical
 * analysis, syntax analysis, semantic analysis (which is divided into the
 * defintion phase and the resolution phase), optimization, and code generation.
 * However, the same flow might not be suitable for the mechanism described here.
 * Why? If you think about it, the previous flow does not provide an oppurutunity
 * to the compiler to register non-compiled entities. In other words, the phases
 * of the compiler are specific  to each compilation unit which prevents exchange
 * of symbols between compilation units.
 *
 * Therefore, in order to accomodate the mechanism described here, the flow of
 * the compiler has to be altered. In the older flow, a source file was not
 * processed until the previous source file was completely processed, i.e.,
 * subjected to all the phases from lexical analysis to code generation. In the
 * new flow, all the input source files are subjected to a single phase before
 * moving on to the next phase. This allows the compiler to mediate the exchange
 * of symbols between compilation units.
 */

zen_SymbolDefinitionListener_t* zen_SymbolDefinitionListener_new(
    zen_Compiler_t* compiler) {
    zen_SymbolDefinitionListener_t* listener = zen_Memory_allocate(zen_SymbolDefinitionListener_t, 1);
    listener->m_compiler = compiler;
    listener->m_astListener = zen_ASTListener_newWithContext(listener);
    listener->m_symbolTable = NULL;
    listener->m_scopes = NULL;
    listener->m_package = NULL;
    listener->m_packageSize = -1;
    listener->m_mainComponent = ZEN_AST_NODE_TYPE_UNKNOWN;
    listener->m_classPrepared = false;
    listener->m_className = NULL;
    listener->m_classNameSize = 0;

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

    zen_ASTListener_delete(listener->m_astListener);
    jtk_Memory_deallocate(listener);
}

zen_ASTListener_t* zen_SymbolDefinitionListener_getASTListener(zen_SymbolDefinitionListener_t* listener) {
    jtk_Assert_assertObject(listener, "The specified listener is null.");
    return listener->m_astListener;
}

void zen_SymbolDefinitionListener_reset(
    zen_SymbolDefinitionListener_t* listener,
    zen_SymbolTable_t* symbolTable, zen_ASTAnnotations_t* scopes,
    const uint8_t* package, int32_t packageSize) {
    listener->m_symbolTable = symbolTable;
    listener->m_scopes = scopes;
    listener->m_package = package;
    listener->m_packageSize = packageSize;
    listener->m_mainComponent = ZEN_AST_NODE_TYPE_UNKNOWN;
    listener->m_classPrepared = false;

    if (listener->m_className != NULL) {
        jtk_CString_delete(listener->m_className);
        listener->m_className = NULL;
        listener->m_classNameSize = 0;
    }
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
    zen_Scope_t* scope = zen_Scope_forCompilationUnit();
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

// TODO: constructorArguments

// TODO: Declare function even if it has no parameters.

void zen_SymbolDefinitionListener_declareOverloadedFunction(
    zen_SymbolDefinitionListener_t* listener, zen_Symbol_t* symbol,
    jtk_ArrayList_t* fixedParameters, zen_ASTNode_t* variableParameter,
    uint32_t modifiers, zen_Token_t* reference) {
    zen_Compiler_t* compiler = listener->m_compiler;
    zen_ErrorHandler_t* errorHandler = compiler->m_errorHandler;
    zen_FunctionSymbol_t* functionSymbol = &symbol->m_context.m_asFunction;
    zen_Token_t* identifierToken = (zen_Token_t*)symbol->m_identifier->m_context;

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
            zen_ErrorHandler_handleSemanticalError(errorHandler,
                listener, ZEN_ERROR_CODE_MULTIPLE_FUNCTION_OVERLOADS_WITH_VARIABLE_PARAMETER,
                (zen_Token_t*)(variableParameter->m_context));

            error = true;
        }
        else {
            int32_t fixedParameterCount0 = jtk_ArrayList_getSize(signature->m_fixedParameters);
            /* Determine whether the function being declared was duplicately
             * overloaded, or not.
             */
            if ((fixedParameterCount0 == fixedParameterCount) &&
                (signature->m_variableParameter == NULL)) {
                zen_ErrorHandler_handleSemanticalError(errorHandler, listener,
                    ZEN_ERROR_CODE_DUPLICATE_FUNCTION_OVERLOAD, identifierToken);
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
                        /* NOTE: The error report does not point to the actual error.
                         * Instead it points to the function currently being declared.
                         */
                        zen_ErrorHandler_handleSemanticalError(errorHandler, listener,
                            ZEN_ERROR_CODE_FUNCTION_DECLARATION_CAUSES_ANOTHER_FUNCTION_TO_EXCEED_PARAMETER_THRESHOLD,
                            reference);
                        error = true;
                    }
                }
                else {
                    /* The function being declared did not cause the update of parameter threshold.
                     * A parameter threshold may have existed before this event was fired. Make sure
                     * that the function obliges the parameter threshold.
                     */
                    if ((parameterThreshold >= 0) && (fixedParameterCount >= parameterThreshold)) {
                        zen_ErrorHandler_handleSemanticalError(errorHandler, listener,
                            ZEN_ERROR_CODE_FUNCTION_DECLARATION_EXCEEDS_PARAMETER_THRESHOLD,
                            reference);
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
        zen_FunctionSignature_t* signature = zen_FunctionSignature_new(fixedParameters, variableParameter, modifiers);
        zen_FunctionSymbol_addSignature(functionSymbol, signature);
    }
}

zen_Symbol_t* zen_SymbolDefinitionListener_declareFunction(zen_SymbolTable_t* symbolTable,
    zen_ASTNode_t* identifier, jtk_ArrayList_t* fixedParameters,
    zen_ASTNode_t* variableParameter, uint16_t modifiers) {
    /* Create a member function symbol to store in the symbol table. */
    zen_Symbol_t* symbol = zen_Symbol_forFunction(identifier, symbolTable->m_currentScope);
    zen_FunctionSymbol_t* functionSymbol = &symbol->m_context.m_asFunction;
    if (variableParameter != NULL) {
        int32_t parameterThreshold = jtk_ArrayList_getSize(fixedParameters);
        zen_FunctionSymbol_setParameterThreshold(functionSymbol, parameterThreshold);
    }
    /* In order to enable the symbol table to store overloaded functions,
     * we employ function signatures. Add the function signature
     * corresponding to the function being declared to the newly
     * created member function symbol.
     */
    zen_FunctionSignature_t* signature = zen_FunctionSignature_new(fixedParameters,
        variableParameter, modifiers);
    zen_FunctionSymbol_addSignature(functionSymbol, signature);

    /* Define the symbol in the symbol table. */
    zen_SymbolTable_define(symbolTable, symbol);

    return symbol;
}

void zen_SymbolDefinitionListener_initializeClassName(zen_SymbolDefinitionListener_t* listener) {
    zen_Compiler_t* compiler = listener->m_compiler;
    const uint8_t* fileName = (const uint8_t*)jtk_ArrayList_getValue(
        compiler->m_inputFiles, compiler->m_currentFileIndex);
    int32_t size = jtk_CString_getSize(fileName);
    int32_t slashIndex = jtk_CString_findLast_c(fileName, size, '/');
    int32_t dotIndex = jtk_CString_findLast_c(fileName, size, '.');
    listener->m_className = jtk_CString_substringEx(fileName, size, slashIndex + 1,
        dotIndex);
    listener->m_classNameSize = dotIndex - (slashIndex + 1);
}

void zen_SymbolDefinitionListener_defineClass(zen_SymbolDefinitionListener_t* listener) {
    zen_Scope_t* scope = zen_Scope_forClass(listener->m_symbolTable->m_currentScope);

    zen_Symbol_t* symbol = zen_Symbol_forClass(NULL,
        listener->m_symbolTable->m_currentScope, scope,
        listener->m_className, listener->m_classNameSize,
        listener->m_package, listener->m_packageSize);
    zen_ClassSymbol_t* classSymbol = &symbol->m_context;
    scope->m_symbol = symbol;

    zen_Scope_defineEx(listener->m_symbolTable->m_currentScope, listener->m_className,
        listener->m_classNameSize, symbol);
    zen_Compiler_registerSymbol(listener->m_compiler, classSymbol->m_qualifiedName,
        classSymbol->m_qualifiedNameSize, symbol);

    // TODO: Is the symbol defined here destroyed?
    zen_SymbolTable_setCurrentScope(listener->m_symbolTable, scope);

    listener->m_classPrepared = true;
}

void zen_SymbolDefinitionListener_onEnterFunctionDeclaration(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_Compiler_t* compiler = listener->m_compiler;
    zen_ErrorHandler_t* errorHandler = compiler->m_errorHandler;
    zen_FunctionDeclarationContext_t* functionDeclarationContext = (zen_FunctionDeclarationContext_t*)node->m_context;

    /* Retrieve the identifier associated with the function declaration. */
    zen_ASTNode_t* identifier = functionDeclarationContext->m_identifier;
    zen_Token_t* identifierToken = (zen_Token_t*)identifier->m_context;

    if ((listener->m_mainComponent != ZEN_AST_NODE_TYPE_CLASS_DECLARATION)
        && !listener->m_classPrepared) {
        /* Pure static classes do not inherit any superclasses. */
        zen_SymbolDefinitionListener_initializeClassName(listener);
        zen_SymbolDefinitionListener_defineClass(listener);
    }

    zen_FunctionParametersContext_t* functionParametersContext =
        (zen_FunctionParametersContext_t*)functionDeclarationContext->m_functionParameters->m_context;
    /* Retrieve the fixed parameters associated with the context of the
     * function arguments.
     */
    jtk_ArrayList_t* fixedParameters = functionParametersContext->m_fixedParameters;
    /* Retrieve the variable parameter associated with the context of the
     * function arguments, if any.
     */
    zen_ASTNode_t* variableParameter = functionParametersContext->m_variableParameter;
    int32_t fixedParameterCount = jtk_ArrayList_getSize(fixedParameters);

    /* Retrieve the symbol table associated with this listener. */
    zen_SymbolTable_t* symbolTable = listener->m_symbolTable;
    /* Retrieve the current scope of the symbol table. */
    zen_Scope_t* currentScope = zen_SymbolTable_getCurrentScope(symbolTable);

    if ((zen_Token_getType(identifierToken) == ZEN_TOKEN_KEYWORD_STATIC) &&
        ((fixedParameterCount > 0) || (variableParameter != NULL))) {
        zen_ErrorHandler_handleSemanticalError(errorHandler, listener,
            ZEN_ERROR_CODE_STATIC_INITIALIZER_WITH_PARAMETERS, identifierToken);
    }
    else {
        if (zen_Scope_isClassScope(currentScope)) {
            /* Resolve the identifier within the scope of the compilation unit. */
            zen_Symbol_t* symbol = zen_SymbolTable_resolve(symbolTable, identifierToken->m_text);
            uint32_t modifiers = 0;

            /* When functions are declared in the compilation unit, a
             * synthetic class symbol is generated. Therefore, make sure
             * that the class scope is not synthetic.
             */
            if (node->m_parent->m_type == ZEN_AST_NODE_TYPE_CLASS_MEMBER) {
                zen_ClassMemberContext_t* classMemberContext = (zen_ClassMemberContext_t*)node->m_parent->m_context;
                int32_t modifierCount = jtk_ArrayList_getSize(classMemberContext->m_modifiers);
                int32_t i;
                for (i = 0; i < modifierCount; i++) {
                    zen_ASTNode_t* modifier =
                        (zen_ASTNode_t*)jtk_ArrayList_getValue(classMemberContext->m_modifiers, i);
                    zen_Token_t* token = (zen_Token_t*)modifier->m_context;
                    modifiers |= zen_TokenType_toModifiers(token->m_type);
                }
            }
            else {
                /* NOTE: Functions declared in the compilation unit cannot have modifiers
                 * specified explicitly. However, by default they are all static.
                 */
                modifiers |= ZEN_TOKEN_KEYWORD_STATIC;
            }

            if (symbol != NULL) {
                /* If a symbol with the given identifier exists, make sure it is
                 * a function before overloading.
                 */
                if (zen_Symbol_isFunction(symbol)) {
                    /* The resolved symbol is a function. Retrieve the corresponding
                     * function symbol.
                     */
                    zen_FunctionSymbol_t* functionSymbol = &symbol->m_context.m_asFunction;
                    zen_SymbolDefinitionListener_declareOverloadedFunction(listener,
                        symbol, fixedParameters, variableParameter, modifiers,
                        identifierToken);
                }
                else {
                    zen_ErrorHandler_handleSemanticalError(errorHandler,
                        listener, ZEN_ERROR_CODE_REDECLARATION_OF_SYMBOL_AS_FUNCTION,
                        identifierToken);
                }
            }
            else {
                zen_SymbolDefinitionListener_declareFunction(listener->m_symbolTable, identifier,
                    fixedParameters, variableParameter, modifiers);
            }
        }
        else {
            printf("[internal error] Declaring function in unsuitable scope.\n");
        }
    }

    /* A function scope is pushed regardless of the declaration being erroneous. */

    zen_Scope_t* scope = zen_Scope_forFunction(symbolTable->m_currentScope);
    zen_SymbolTable_setCurrentScope(listener->m_symbolTable, scope);
    zen_ASTAnnotations_put(listener->m_scopes, node, scope);

    int32_t i;
    for (i = 0; i < fixedParameterCount; i++) {
        zen_ASTNode_t* parameter = (zen_ASTNode_t*)jtk_ArrayList_getValue(fixedParameters, i);
        const uint8_t* parameterText = zen_Token_getText((zen_Token_t*)(parameter->m_context));
        zen_Symbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, parameterText);
        if (symbol != NULL) {
            zen_ErrorHandler_handleSemanticalError(errorHandler,
                listener, ZEN_ERROR_CODE_REDECLARATION_OF_SYMBOL_AS_PARAMETER,
                (zen_Token_t*)(parameter->m_context));
        }
        else {
            zen_Symbol_t* symbol = zen_Symbol_forConstant(parameter,
                symbolTable->m_currentScope);
            zen_SymbolTable_define(listener->m_symbolTable, symbol);
        }
    }

    if (variableParameter != NULL) {
        const uint8_t* parameterText = zen_Token_getText((zen_Token_t*)(variableParameter->m_context));
        zen_Symbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, parameterText);
        if (symbol != NULL) {
            zen_ErrorHandler_handleSemanticalError(errorHandler,
                listener, ZEN_ERROR_CODE_REDECLARATION_OF_SYMBOL_AS_VARIABLE_PARAMETER,
                (zen_Token_t*)(variableParameter->m_context));
        }
        else {
            symbol = zen_Symbol_forConstant(variableParameter, symbolTable->m_currentScope);
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

void zen_SymbolDefinitionListener_onEnterStatementSuite(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_Scope_t* scope = zen_Scope_forLocal(listener->m_symbolTable->m_currentScope);
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
    zen_Compiler_t* compiler = listener->m_compiler;
    zen_ErrorHandler_t* errorHandler = compiler->m_errorHandler;
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
            zen_ErrorHandler_handleSemanticalError(errorHandler,
                listener, ZEN_ERROR_CODE_REDECLARATION_OF_SYMBOL_AS_VARIABLE,
                (zen_Token_t*)identifier->m_context);
        }
        else {
            zen_Symbol_t* symbol = zen_Symbol_forVariable(identifier,
                listener->m_symbolTable->m_currentScope);
            zen_SymbolTable_define(listener->m_symbolTable, symbol);
        }
    }
}

void zen_SymbolDefinitionListener_onEnterConstantDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_Compiler_t* compiler = listener->m_compiler;
    zen_ErrorHandler_t* errorHandler = compiler->m_errorHandler;
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
            zen_ErrorHandler_handleSemanticalError(errorHandler,
                listener, ZEN_ERROR_CODE_REDECLARATION_OF_SYMBOL_AS_CONSTANT,
                (zen_Token_t*)identifier->m_context);
        }
        else {
            zen_Symbol_t* symbol = zen_Symbol_forConstant(identifier, listener->m_symbolTable->m_currentScope);
            zen_SymbolTable_define(listener->m_symbolTable, symbol);
        }
    }
}

void zen_SymbolDefinitionListener_onEnterLabelClause(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    zen_Compiler_t* compiler = listener->m_compiler;
    zen_ErrorHandler_t* errorHandler = compiler->m_errorHandler;
    zen_LabelClauseContext_t* LabelClauseContext = (zen_LabelClauseContext_t*)node->m_context;

    zen_ASTNode_t* identifier = LabelClauseContext->m_identifier;
    const uint8_t* identifierText = zen_Token_getText((zen_Token_t*)identifier->m_context);

    zen_Symbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, identifierText);
    if (symbol != NULL) {
        zen_ErrorHandler_handleSemanticalError(errorHandler,
            listener, ZEN_ERROR_CODE_REDECLARATION_OF_SYMBOL_AS_LABEL,
            (zen_Token_t*)identifier->m_context);
    }
    else {
        zen_Symbol_t* symbol = zen_Symbol_forLabel(identifier,
            listener->m_symbolTable->m_currentScope);
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
    zen_Compiler_t* compiler = listener->m_compiler;
    zen_ErrorHandler_t* errorHandler = compiler->m_errorHandler;
    zen_ForParameterContext_t* forParametersContext = (zen_ForParameterContext_t*)node->m_context;

    if (forParametersContext->m_declaration) {
        zen_ASTNode_t* identifier = forParametersContext->m_identifier;
        const uint8_t* identifierText = zen_Token_getText((zen_Token_t*)identifier->m_context);

        zen_Symbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, identifierText);
        if (symbol != NULL) {
        zen_ErrorHandler_handleSemanticalError(errorHandler,
            listener, ZEN_ERROR_CODE_REDECLARATION_OF_SYMBOL_AS_LOOP_PARAMETER,
            (zen_Token_t*)identifier->m_context);
        }
        else {
            if (forParametersContext->m_variable) {
                symbol = zen_Symbol_forVariable(identifier,
                    listener->m_symbolTable->m_currentScope);
            }
            else {
                symbol = zen_Symbol_forConstant(identifier,
                    listener->m_symbolTable->m_currentScope);
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
    zen_Compiler_t* compiler = listener->m_compiler;
    zen_ErrorHandler_t* errorHandler = compiler->m_errorHandler;
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
        zen_Scope_t* scope = zen_Scope_forLocal(listener->m_symbolTable->m_currentScope);
        zen_SymbolTable_setCurrentScope(listener->m_symbolTable, scope);
        zen_ASTAnnotations_put(listener->m_scopes, catchClause, scope);

        zen_ASTNode_t* identifier = catchClauseContext->m_identifier;
        int32_t identifierSize;
        uint8_t* identifierText = zen_ASTNode_toCString(identifier, &identifierSize);

        zen_Symbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, identifierText);
        if (symbol != NULL) {
        zen_ErrorHandler_handleSemanticalError(errorHandler,
            listener, ZEN_ERROR_CODE_REDECLARATION_OF_SYMBOL_AS_CATCH_PARAMETER,
            (zen_Token_t*)identifier->m_context);
        }
        else {
            symbol = zen_Symbol_forVariable(identifier, scope);
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
    zen_Compiler_t* compiler = listener->m_compiler;
    zen_ErrorHandler_t* errorHandler = compiler->m_errorHandler;
    zen_ClassDeclarationContext_t* classDeclarationContext = (zen_ClassDeclarationContext_t*)node->m_context;

    zen_ASTNode_t* identifier = classDeclarationContext->m_identifier;
    zen_Token_t* identifierToken = (zen_Token_t*)identifier->m_context;
    const uint8_t* identifierText = zen_Token_getText(identifierToken);

    zen_Symbol_t* classSymbol = zen_SymbolTable_resolve(listener->m_symbolTable, identifierText);
    if (classSymbol != NULL) {
        zen_ErrorHandler_handleSemanticalError(errorHandler,
            listener, ZEN_ERROR_CODE_REDECLARATION_OF_SYMBOL_AS_CLASS,
            (zen_Token_t*)identifier->m_context);
    }
    else {
        zen_Scope_t* scope = zen_Scope_forClass(listener->m_symbolTable->m_currentScope);

        zen_Symbol_t* symbol = zen_Symbol_forClass(identifier,
            listener->m_symbolTable->m_currentScope, scope,
            identifierToken->m_text, identifierToken->m_length,
            listener->m_package, listener->m_packageSize);
        zen_ClassSymbol_t* classSymbol = &symbol->m_context;
        scope->m_symbol = symbol;

        zen_SymbolTable_define(listener->m_symbolTable, symbol);
        zen_Compiler_registerSymbol(compiler, classSymbol->m_qualifiedName,
            classSymbol->m_qualifiedNameSize, symbol);

        zen_SymbolTable_setCurrentScope(listener->m_symbolTable, scope);
        zen_ASTAnnotations_put(listener->m_scopes, node, scope);

        zen_ASTWalker_walk(astListener, classDeclarationContext->m_classSuite);

        zen_SymbolTable_invalidateCurrentScope(listener->m_symbolTable);
    }

    /* If the definition of a class is duplicate, then do not visit the children
     * nodes.
     */
    zen_ASTListener_skipChildren(astListener);
}

void zen_SymbolDefinitionListener_onExitClassDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");
}

void zen_SymbolDefinitionListener_onEnterEnumerationDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {

    // zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    // zen_EnumerationDeclarationContext_t* enumerationDeclarationContext = (zen_EnumerationDeclarationContext_t*)node->m_context;

    // zen_ASTNode_t* identifier = enumerationDeclarationContext->m_identifier;
    // zen_Token_t* identifierToken = (zen_Token_t*)identifier->m_context;
    // const uint8_t* identifierText = zen_Token_getText(identifierToken);

    // zen_Symbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, identifierText);
    // if (symbol != NULL) {
    //     zen_ErrorHandler_reportError(NULL, "Redeclaration of symbol as enumeration", (zen_Token_t*)identifier->m_context);
    // }
    // else {
    //     zen_EnumerationScope_t* enumerationScope = zen_EnumerationScope_new(listener->m_symbolTable->m_currentScope);

    //     zen_EnumerationSymbol_t* enumerationSymbol = zen_EnumerationSymbol_new(identifier, listener->m_symbolTable->m_currentScope, zen_EnumerationScope_getScope(enumerationScope));
    //     symbol = zen_EnumerationSymbol_getSymbol(enumerationSymbol);
    //     zen_SymbolTable_define(listener->m_symbolTable, symbol);

    //     enumerationScope->m_enumerationSymbol = symbol;

    //     zen_Scope_t* scope = zen_EnumerationScope_getScope(enumerationScope);

    //     zen_SymbolTable_setCurrentScope(listener->m_symbolTable, scope);
    //     zen_ASTAnnotations_put(listener->m_scopes, node, scope);
    // }
}

void zen_SymbolDefinitionListener_onExitEnumerationDeclaration(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {

    // zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    // zen_EnumerationDeclarationContext_t* enumerationDeclarationContext = (zen_EnumerationDeclarationContext_t*)node->m_context;

    // zen_SymbolTable_invalidateCurrentScope(listener->m_symbolTable);
}

void zen_SymbolDefinitionListener_onEnterEnumerate(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {

    // zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    // zen_EnumerationDeclarationContext_t* enumerationDeclarationContext = (zen_EnumerationDeclarationContext_t*)node->m_context;

    // zen_ASTNode_t* identifier = enumerationDeclarationContext->m_identifier;
    // zen_Token_t* identifierToken = (zen_Token_t*)identifier->m_context;
    // const uint8_t* identifierText = zen_Token_getText(identifierToken);

    // zen_Scope_t* currentScope = zen_SymbolTable_getCurrentScope(listener->m_symbolTable);
    // if (zen_Scope_isEnumerationScope(currentScope)) {
    //     zen_Symbol_t* symbol = zen_Scope_resolve(currentScope, identifierText);
    //     if (symbol != NULL) {
    //         zen_ErrorHandler_reportError(NULL, "Duplicate enumerate", (zen_Token_t*)identifier->m_context);
    //     }
    //     else {
    //         zen_EnumerateSymbol_t* enumerateSymbol = zen_EnumerateSymbol_new(identifier, listener->m_symbolTable->m_currentScope);
    //         symbol = zen_EnumerateSymbol_getSymbol(enumerateSymbol);
    //         zen_SymbolTable_define(listener->m_symbolTable, symbol);
    //     }
    // }
    // else {
    //     fprintf(stderr, "[internal error] Declaration of enumerate in unsuitable scope.\n");
    // }
}

void zen_SymbolDefinitionListener_onExitEnumerate(
    zen_ASTListener_t* astListener, zen_ASTNode_t* node) {

    // zen_SymbolDefinitionListener_t* listener = (zen_SymbolDefinitionListener_t*)astListener->m_context;
    // zen_EnumerationDeclarationContext_t* enumerationDeclarationContext = (zen_EnumerationDeclarationContext_t*)node->m_context;
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