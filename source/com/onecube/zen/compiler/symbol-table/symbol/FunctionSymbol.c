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

// Saturday, February 24, 2018

#include <com/onecube/zen/compiler/symbol-table/FunctionSymbol.h>

/*******************************************************************************
 * FunctionSymbol                                                              *
 *******************************************************************************/

void zen_FunctionSymbol_initialize(zen_FunctionSymbol_t* symbol) {
    symbol->m_signatures = jtk_ArrayList_new();
    symbol->m_parameterThreshold = -1;
}

void zen_FunctionSymbol_destroy(zen_FunctionSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    int32_t size = jtk_ArrayList_getSize(symbol->m_signatures);
    int32_t i;
    for (i = 0; i < size; i++) {
        zen_FunctionSignature_t* signature =
            (zen_FunctionSignature_t*)jtk_ArrayList_getValue(symbol->m_signatures, i);
        zen_FunctionSignature_delete(signature);
    }
    jtk_ArrayList_delete(symbol->m_signatures);
}

jtk_ArrayList_t* zen_FunctionSymbol_getSignatures(zen_FunctionSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    return symbol->m_signatures;
}

void zen_FunctionSymbol_addSignature(zen_FunctionSymbol_t* symbol, zen_FunctionSignature_t* signature) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    jtk_ArrayList_add(symbol->m_signatures, signature);
}

int32_t zen_FunctionSymbol_getParameterThreshold(zen_FunctionSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    return symbol->m_parameterThreshold;
}

bool zen_FunctionSymbol_hasParameterThreshold(zen_FunctionSymbol_t* symbol) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    printf("has pt: %d\n", symbol->m_parameterThreshold != -1);
    return symbol->m_parameterThreshold != -1;
}

void zen_FunctionSymbol_setParameterThreshold(zen_FunctionSymbol_t* symbol,
    int32_t parameterThreshold) {
    jtk_Assert_assertObject(symbol, "The specified symbol is null.");

    symbol->m_parameterThreshold = parameterThreshold;
}