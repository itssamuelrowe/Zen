    
    /* The resolution phase ensures that the expression on the left hand
     * side of the assignment operator evaluates to an identifier or
     * field access.
     */
    zen_ArrayList_t* children = zen_ASTNode_getChildren(context->m_conditionalExpression);
    zen_ASTNode_t* identifier = zen_ArrayList_get(children, 0);
    zen_Token_t* identifierToken = (zen_Token_t*)identifier->m_context;
    const uint8_t* identifierText = zen_Token_getText(identifierToken);
    // zen_Symbol_t* symbol = zen_SymbolTable_resolve(builder->m_symbolTable, identifierText);
    if (zen_Symbol_isVariable(symbol)) {
        

        void** state = (void**)zen_AstAnnotations_get(builder->m_annotations, node);
        int32_t targetIndex = (int32_t)state[ZEN_ASSIGNMENT_EXPRESSION_STATE_TARGET_INDEX];
        bool localVariable = (bool)state[ZEN_ASSIGNMENT_EXPRESSION_STATE_LOCAL_VARIABLE];

        if (localVariable) {
            zen_BinaryEntityBuilder_emitStoreReference(builder, targetIndex);
        }
        else {
            zen_BinaryEntityBuilder_emitStoreField(builder, targetIndex);
        }

        zen_Memory_deallocate(state);
    }