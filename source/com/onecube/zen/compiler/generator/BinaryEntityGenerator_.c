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

// Saturday, October 05, 2019

/* The binary entity generator accepts an abstract syntax tree (AST). The binary
 * entity is generated as each node is visited.
 */

void zen_BinaryEntityGenerator_onEnterCompilationUnit(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    jtk_Assert_assertObject(astListener, "The specified AST listener is null.");
    jtk_Assert_assertObject(node, "The specified AST node is null.");

    /* Retrieve the generator associated with the AST listener. */
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    /* Retrieve the scope associated with the current AST node. */
    zen_Scope_t* scope = zen_ASTAnnotations_get(generator->m_scopes, node);
    /* Activate the scope associated with the current AST node. */
    zen_SymbolTable_setCurrentScope(generator->m_symbolTable, scope);
}

void zen_BinaryEntityGenerator_onExitCompilationUnit(zen_ASTListener_t* astListener,
    zen_ASTNode_t* node) {
    /* Retrieve the generator associated with the AST listener. */
    zen_BinaryEntityGenerator_t* generator = (zen_BinaryEntityGenerator_t*)astListener->m_context;
    
    /* Activate the scope associated with the current AST node. */
    zen_SymbolTable_invalidateCurrentScope(generator->m_symbolTable);

    /* Push a data channel, where the bytes generated will be written. */
    zen_BinaryEntityBuilder_pushChannel(generator->m_builder);

    /* Write magic number, major version, and minor version on the main channel. */
    zen_BinaryEntityBuilder_writeMagicNumber(generator->m_builder);
    /* Write the major version of the binary entity file format the stream is encoded in. */
    zen_BinaryEntityBuilder_writeMajorVersion(generator->m_builder, entityFile->m_version.m_majorVersion);
    /* Write the minor version of the binary entity file format the stream is encoded in. */
    zen_BinaryEntityBuilder_writeMinorVersion(generator->m_builder, entityFile->m_version.m_minorVersion);
    /* Write additional flags on how the binary entity file should be loaded. */
    zen_BinaryEntityBuilder_writeStreamFlags(generator->m_builder, entityFile->m_flags);

    /* At this point, all the constant pool entries required by the binary entity
     * file should be available to the constant pool builder. The constant pool
     * can now be built.
     */
    int32_t entryCount = zen_ConstantPoolBuilder_countEntries(constantPoolBuilder);
    zen_BinaryEntityBuilder_writeConstantPoolHeader(entryCount);
    int32_t i;
    for (i = 0; i < entryCount; i++) {
        zen_ConstantPoolEntry_t* entry = zen_ConstantPoolBuilder_getEntry(constantPoolBuilder, i);
        zen_BinaryEntityBuilder_writeConstantPoolEntry(builder, entry);
    }

    /* Write the entity header. */
    zen_BinaryEntityBuilder_writeEntityHeader(builder, entity->m_type, entity->m_flags,
        entity->m_reference);
}

// annotatedComponentDeclaration

void zen_BinaryEntityGenerator_onEnterAnnotatedComponentDeclaration(
    zen_ASTListener_t* listener, zen_ASTNode_t* node) {
    
}

void zen_BinaryEntityGenerator_onExitAnnotatedComponentDeclaration(
    zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// annotations

void zen_BinaryEntityGenerator_onEnterAnnotations(
    zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitAnnotations(
    zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

// annotation

void zen_BinaryEntityGenerator_onEnterAnnotation(
    zen_ASTListener_t* listener, zen_ASTNode_t* node) {
}

void zen_BinaryEntityGenerator_onExitAnnotation(
    zen_ASTListener_t* listener, zen_ASTNode_t* node) {
    zen_AnnotationContext_t* context = node->m_context;
    
    context->m_annotationType
    
    /* An annotation type can only be declared as a top-level entity. Resolve
     * the annotation type and retrieve its corresponding symbol.
     */
    zen_AnnotationSymbol_t* symbol = zen_SymbolTable_resolve(listener->m_symbolTable, annotationText);
    if (symbol == NULL) {
        // [error] Unknown annotation symbol.
        // [internal error] The previous error type should not originate at this point.
    }
}
