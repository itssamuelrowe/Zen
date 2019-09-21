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

else if () {
        /* Determines whether the overloaded function is semantically erroneous. */
        bool error = false;

        /* Let X be a function. If an overload of function X, say Xi, has k
         * fixed parameters and a variable parameter, then the maximum number
         * of fixed parameters for any overload of function X is limited to k - 1.
         * Where, k is known as the parameter threshold of function X.
         *
         * The parameter threshold is inheirted by subclasses only. For example,
         *
         * class A
         *     function foo(x, y, z, ...)
         *         ;
         *
         *     function bar(x, y, z)
         *         ;
         *
         * class B : A
         *     function foo(x, y)
         *         ;
         *
         *     function bar(x, y, z, ...)
         *         ;
         *
         * In the above example, foo() in subclass has a parameter threshold of 3.
         * The bar() in subclass generates a new parameter threshold which is
         * incompatible with the superclass version of the bar() function. Whereas,
         * a correct form would be:
         *
         *     function bar(w, x, y, z, ...)
         *         ;
         *
         * A valid parameter threshold will be generated for the above function.
         * In any case, the new parameter threshold does not affect the superclass
         * function.
         */
        int32_t parameterThreshold = -1;
        int32_t fixedParameterCount = jtk_ArrayList_getSize(fixedParameters);

        zen_ClassScope_t* classScope = (zen_ClassScope_t*)(currentScope->m_context);
        zen_ClassSymbol_t* classSymbol = (zen_ClassSymbol_t*)(zen_ClassScope_getClassSymbol(classScope)->m_context);
        zen_Symbol_t* oldSymbol = NULL;

        /* The superclasses are represented as an n-ary tree. Use a stack based
         * traversal to compare against every function signature the class inherits.
         */
        zen_LinkedStack_t* stack = zen_LinkedStack_new();
        zen_LinkedStack_push(stack, classSymbol);

        jtk_ArrayList_t* signatures = jtk_ArrayList_new();
        while (!zen_LinkedStack_isEmpty(stack)) {
            classSymbol = (zen_ClassSymbol_t*)zen_LinkedStack_pop(stack);
            classScope = (zen_ClassScope_t*)(zen_ClassSymbol_getClassScope(classSymbol)->m_context); // The context is unset

            oldSymbol = (zen_Symbol_t*)jtk_HashMap_getValue(classScope->m_symbols, identifierText);
            if (oldSymbol != NULL) {
                if (zen_Symbol_isFunction(oldSymbol)) {
                    zen_Scope_t* oldSymbolEnclosingScope = zen_Symbol_getEnclosingScope(oldSymbol);
                    zen_FunctionSymbol_t* oldFunctionSymbol = (zen_FunctionSymbol_t*)oldSymbol->m_context;
                    
                    if (oldSymbolEnclosingScope == currentScope) {
                        /* If currently there is no parameter threshold, try to update the
                         * threshold.
                         */
                        parameterThreshold = zen_FunctionSymbol_getParameterThreshold(oldFunctionSymbol);
                        if (parameterThreshold < 0) {
                            /* Update the threshold if the function being declared has a variable
                             * parameter.
                             */
                            if (variableParameter != NULL) {
                                parameterThreshold = fixedParameterCount;
                                zen_FunctionSymbol_setParameterThreshold(oldFunctionSymbol, parameterThreshold);
                            }
                        }
                    }
                    
                    jtk_ArrayList_t* oldSignatures = zen_FunctionSymbol_getSignatures(oldFunctionSymbol);
                    int32_t oldSignaturesSize = jtk_ArrayList_getSize(oldSignatures);
                    int32_t i;
                    for (i = 0; i < oldSignaturesSize; i++) {
                        zen_FunctionSignature_t* oldSignature = (zen_FunctionSignature_t*)jtk_ArrayList_getValue(oldSignatures, i);
                        int32_t fixedParameterCount0 = jtk_ArrayList_getSize(oldSignature->m_fixedParameters);
                        
                        if ((oldSignature->m_variableParameter != NULL) && (variableParameter != NULL)) {
                            if ((oldSymbolEnclosingScope == currentScope) || (fixedParameterCount != fixedParameterCount0))
                            zen_ErrorHandler_reportError(NULL, "Multiple overloads with variable parameter", (zen_Token_t*)(variableParameter->m_context));
                            error = true;
                        }
                        else {
                            /* Determine whether the function being declared was duplicately
                             * overloaded, or not.
                             */
                            if ((fixedParameterCount0 == fixedParameterCount) &&
                                ((oldSignature->m_variableParameter == NULL) &&
                                 (variableParameter == NULL))) {
                                if (oldSymbolEnclosingScope == currentScope) {
                                    zen_ErrorHandler_reportError(NULL, "Duplicate overload", identifierToken);
                                    error = true;
                                    // break;
                                }
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
                }
                else {
                    fprintf(stderr, "[semantic error] Redeclaration of symbol as function\n");
                    break;
                }
            }
            else {
                jtk_ArrayList_t* superClasses = zen_ClassSymbol_getSuperClasses(classSymbol);
                int32_t size = jtk_ArrayList_getSize(superClasses);
                int32_t i;
                for (i = 0; i < size; i++) {
                    // TODO: Check what getSuperclasses returns!! Symbol or ClassSymbol.
                    zen_ClassSymbol_t* superClass = (zen_ClassSymbol_t*)jtk_ArrayList_getValue(superClasses, i);
                    zen_LinkedStack_push(stack, superClass);
                }
            }
        }

        if (!error) {
            zen_ClassScope_t* classScope = (zen_ClassScope_t*)currentScope->m_context;
            zen_Symbol_t* symbol = (zen_Symbol_t*)jtk_HashMap_getValue(classScope->m_symbols, identifierText);
            zen_FunctionSymbol_t* functionSymbol = NULL;
            if (symbol == NULL) {
                functionSymbol = zen_FunctionSymbol_new(identifier, symbolTable->m_currentScope);
                zen_Symbol_t* symbol = zen_FunctionSymbol_getSymbol(functionSymbol);
                zen_SymbolTable_define(symbolTable, symbol);
            }
            else {
                functionSymbol = (zen_FunctionSymbol_t*)symbol->m_context;
            }
            zen_FunctionSignature_t* signature = zen_FunctionSignature_new(fixedParameters, variableParameter);
            zen_FunctionSymbol_addSignature(functionSymbol, signature);
        }
    }