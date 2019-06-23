zen_FunctionSymbol_t* memberFunctionSymbol =
            zen_HashMap_getValue(classScope->m_functions, (void*)identifier);
        if (memberFunctionSymbol != NULL) {
            symbol = zen_FunctionSymbol_getSymbol(memberFunctionSymbol);
        }
        else {
            /* Look for a variable within this scope and the scopes of the
             * super classes.
             */
            zen_MemberVariableSymbol_t* variableMemberSymbol =
                zen_HashMap_getValue(classScope->m_variables, (void*)identifier);
            if (variableMemberSymbol != NULL) {
                symbol = zen_MemberVariableSymbol_getSymbol(variableMemberSymbol);
            }
            else {
                /* Look for a constant within this scope and the scopes of
                 * the super classes.
                 */
                zen_MemberConstantSymbol_t* constantMemberSymbol =
                    (zen_MemberConstantSymbol_t*)zen_HashMap_getValue(classScope->m_constants, (void*)identifier);
                if (constantMemberSymbol != NULL) {
                    symbol = zen_MemberConstantSymbol_getSymbol(constantMemberSymbol);
                }
                else {
                    /* Look for a class within this scope and the scopes of
                     * the super classes.
                     */
                    zen_MemberClassSymbol_t* classMemberSymbol =
                        (zen_MemberClassSymbol_t*)zen_HashMap_getValue(classScope->m_classes, (void*)identifier);
                    if (classMemberSymbol != NULL) {
                        symbol = zen_MemberClassSymbol_getSymbol(classMemberSymbol);
                    }
                    else {
                        /* Look for an enumeration within this scope and the scopes
                         * of the super class.
                         */
                        zen_MemberEnumerationSymbol_t* enumerationMemberSymbol =
                            (zen_MemberEnumerationSymbol_t*)zen_HashMap_getValue(classScope->m_classes, (void*)identifier);
                        if (enumerationMemberSymbol != NULL) {
                            symbol = zen_MemberEnumerationSymbol_getSymbol(enumerationMemberSymbol);
                        }
                    }
                }
            }
        }