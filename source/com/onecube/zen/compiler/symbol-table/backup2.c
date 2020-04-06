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

zen_FunctionSymbol_t* memberFunctionSymbol =
            jtk_HashMap_getValue(classScope->m_functions, (void*)identifier);
        if (memberFunctionSymbol != NULL) {
            symbol = zen_FunctionSymbol_getSymbol(memberFunctionSymbol);
        }
        else {
            /* Look for a variable within this scope and the scopes of the
             * super classes.
             */
            zen_MemberVariableSymbol_t* variableMemberSymbol =
                jtk_HashMap_getValue(classScope->m_variables, (void*)identifier);
            if (variableMemberSymbol != NULL) {
                symbol = zen_MemberVariableSymbol_getSymbol(variableMemberSymbol);
            }
            else {
                /* Look for a constant within this scope and the scopes of
                 * the super classes.
                 */
                zen_MemberConstantSymbol_t* constantMemberSymbol =
                    (zen_MemberConstantSymbol_t*)jtk_HashMap_getValue(classScope->m_constants, (void*)identifier);
                if (constantMemberSymbol != NULL) {
                    symbol = zen_MemberConstantSymbol_getSymbol(constantMemberSymbol);
                }
                else {
                    /* Look for a class within this scope and the scopes of
                     * the super classes.
                     */
                    zen_MemberClassSymbol_t* classMemberSymbol =
                        (zen_MemberClassSymbol_t*)jtk_HashMap_getValue(classScope->m_classes, (void*)identifier);
                    if (classMemberSymbol != NULL) {
                        symbol = zen_MemberClassSymbol_getSymbol(classMemberSymbol);
                    }
                    else {
                        /* Look for an enumeration within this scope and the scopes
                         * of the super class.
                         */
                        zen_MemberEnumerationSymbol_t* enumerationMemberSymbol =
                            (zen_MemberEnumerationSymbol_t*)jtk_HashMap_getValue(classScope->m_classes, (void*)identifier);
                        if (enumerationMemberSymbol != NULL) {
                            symbol = zen_MemberEnumerationSymbol_getSymbol(enumerationMemberSymbol);
                        }
                    }
                }
            }
        }