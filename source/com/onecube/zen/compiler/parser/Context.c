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

// Saturday, November 25, 2017

/*
 * The children list expected by the AST walker contains no null elements.
 * The enumeration function filters out nodes which are null and relevant to
 * the grammar. Unfiltered null values appear only within an erroneous
 * context. In order to avoid undefined behaviour due to such contexts,
 * the tree walker distinguishes between erroneously and correctly formed
 * constructs. Where corresponding listener callbacks are invoked for
 * correctly formed constructs, a special callback is invoked for erroneous
 * contexts.
 *
 * The AST destructor anticipates null values in the children list.
 * Thus, appropriately destroys only non-null nodes.
 *
 * All children are instances of zen_ASTNode_t. This applies to tokens, too.
 * Tokens are marked as ZEN_AST_NODE_TYPE_TERMINAL.
 */

#include <jtk/core/Object.h>
#include <jtk/collection/Pair.h>

#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/ast/context/Context.h>

/*******************************************************************************
 * Context                                                                     *
 *******************************************************************************/

void zen_Context_initializeNode(
    zen_ASTNode_t* node,
    zen_ASTNodeType_t type,
    void* context,
    zen_ContextDestructorFunction_t contextDestructor,
    zen_EnumerateContextChildrenFunction_t enumerateContextChildren) {
    node->m_type = type;
    node->m_context = context;
    node->m_contextDestructor = contextDestructor;
    node->m_enumerateContextChildren = enumerateContextChildren;
}

/*******************************************************************************
 * CompilationUnitContext                                                      *
 *******************************************************************************/

zen_CompilationUnitContext_t* zen_CompilationUnitContext_new(zen_ASTNode_t* node) {
    zen_CompilationUnitContext_t* context = zen_Memory_allocate(zen_CompilationUnitContext_t, 1);
    context->m_node = node;
    context->m_importDeclarations = jtk_ArrayList_new();
    context->m_annotatedComponentDeclarations = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_COMPILATION_UNIT, context,
        (zen_ContextDestructorFunction_t)zen_CompilationUnitContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_CompilationUnitContext_getChildren);

    return context;
}

void zen_CompilationUnitContext_delete(zen_CompilationUnitContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_importDeclarations);
    jtk_ArrayList_delete(context->m_annotatedComponentDeclarations);
    jtk_Memory_deallocate(context);
}

void zen_CompilationUnitContext_getChildren(zen_CompilationUnitContext_t* context, jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_importDeclarations);
    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_annotatedComponentDeclarations);
}

/*******************************************************************************
 * ImportDeclarationContext                                                    *
 *******************************************************************************/

zen_ImportDeclarationContext_t* zen_ImportDeclarationContext_new(zen_ASTNode_t* node) {
    zen_ImportDeclarationContext_t* context = zen_Memory_allocate(zen_ImportDeclarationContext_t, 1);
    context->m_node = node;
    context->m_identifiers = jtk_ArrayList_new();
    context->m_wildcard = false;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_IMPORT_DECLARATION, context,
        (zen_ContextDestructorFunction_t)zen_ImportDeclarationContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ImportDeclarationContext_getChildren);

    return context;
}

void zen_ImportDeclarationContext_delete(zen_ImportDeclarationContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_identifiers);
    jtk_Memory_deallocate(context);
}

void zen_ImportDeclarationContext_getChildren(zen_ImportDeclarationContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_identifiers);
}

/*******************************************************************************
 * AnnotatedComponentDeclaration                                               *
 *******************************************************************************/

zen_AnnotatedComponentDeclarationContext_t* zen_AnnotatedComponentDeclarationContext_new(zen_ASTNode_t* node) {
    zen_AnnotatedComponentDeclarationContext_t* context = zen_Memory_allocate(zen_AnnotatedComponentDeclarationContext_t, 1);
    context->m_node = node;
    context->m_annotations = NULL;
    context->m_componentDeclaration = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_ANNOTATED_COMPONENT_DECLARATION, context,
        (zen_ContextDestructorFunction_t)zen_AnnotatedComponentDeclarationContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_AnnotatedComponentDeclarationContext_getChildren);

    return context;
}

void zen_AnnotatedComponentDeclarationContext_delete(zen_AnnotatedComponentDeclarationContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Memory_deallocate(context);
}

void zen_AnnotatedComponentDeclarationContext_getChildren(zen_AnnotatedComponentDeclarationContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    // if (context->m_annotations != NULL) {
        // jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_annotations);
    // }


    jtk_ArrayList_add(children, context->m_annotations);
    jtk_ArrayList_add(children, context->m_componentDeclaration);
}

/*******************************************************************************
 * AnnotationsContext                                                          *
 *******************************************************************************/

zen_AnnotationsContext_t* zen_AnnotationsContext_new(zen_ASTNode_t* node) {
    zen_AnnotationsContext_t* context = zen_Memory_allocate(zen_AnnotationsContext_t, 1);
    context->m_node = node;
    context->m_annotations = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_ANNOTATIONS, context,
        (zen_ContextDestructorFunction_t)zen_AnnotationsContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_AnnotationsContext_getChildren);

    return context;
}

void zen_AnnotationsContext_delete(zen_AnnotationsContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_annotations);
    jtk_Memory_deallocate(context);
}

void zen_AnnotationsContext_getChildren(zen_AnnotationsContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_annotations);
}

/*******************************************************************************
 * AnnotationContext                                                           *
 *******************************************************************************/

zen_AnnotationContext_t* zen_AnnotationContext_new(zen_ASTNode_t* node) {
    zen_AnnotationContext_t* context = zen_Memory_allocate(zen_AnnotationContext_t, 1);
    context->m_node = node;
    context->m_annotationType = NULL;
    context->m_annotationAttributes = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_ANNOTATION, context,
        (zen_ContextDestructorFunction_t)zen_AnnotationContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_AnnotationContext_getChildren);

    return context;
}

void zen_AnnotationContext_delete(zen_AnnotationContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_annotationAttributes);
    jtk_Memory_deallocate(context);
}

void zen_AnnotationContext_getChildren(zen_AnnotationContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_annotationType);
    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_annotationAttributes);
}

/*******************************************************************************
 * AnnotationTypeContext                                                       *
 *******************************************************************************/

zen_AnnotationTypeContext_t* zen_AnnotationTypeContext_new(zen_ASTNode_t* node) {
    zen_AnnotationTypeContext_t* context = zen_Memory_allocate(zen_AnnotationTypeContext_t, 1);
    context->m_node = node;
    context->m_identifiers = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_ANNOTATION_TYPE, context,
        (zen_ContextDestructorFunction_t)zen_AnnotationTypeContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_AnnotationTypeContext_getChildren);

    return context;
}

void zen_AnnotationTypeContext_delete(zen_AnnotationTypeContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_identifiers);
    jtk_Memory_deallocate(context);
}

void zen_AnnotationTypeContext_getChildren(zen_AnnotationTypeContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_identifiers);
}

/*******************************************************************************
 * AnnotationAttributeContext                                                  *
 *******************************************************************************/

zen_AnnotationAttributeContext_t* zen_AnnotationAttributeContext_new(zen_ASTNode_t* node) {
    zen_AnnotationAttributeContext_t* context = zen_Memory_allocate(zen_AnnotationAttributeContext_t, 1);
    context->m_node = node;
    context->m_identifier = NULL;
    context->m_literal = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_ANNOTATION_ATTRIBUTE, context,
        (zen_ContextDestructorFunction_t)zen_AnnotationAttributeContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_AnnotationAttributeContext_getChildren);

    return context;
}

void zen_AnnotationAttributeContext_delete(zen_AnnotationAttributeContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Memory_deallocate(context);
}

void zen_AnnotationAttributeContext_getChildren(zen_AnnotationAttributeContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_identifier);
    jtk_ArrayList_add(children, context->m_literal);
}

/*******************************************************************************
 * ComponentDeclarationContext                                                 *
 *******************************************************************************/

zen_ComponentDeclarationContext_t* zen_ComponentDeclarationContext_new(zen_ASTNode_t* node) {
    zen_ComponentDeclarationContext_t* context = zen_Memory_allocate(zen_ComponentDeclarationContext_t, 1);
    context->m_node = node;
    context->m_component = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_COMPONENT_DECLARATION, context,
        (zen_ContextDestructorFunction_t)zen_ComponentDeclarationContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ComponentDeclarationContext_getChildren);

    return context;
}

void zen_ComponentDeclarationContext_delete(zen_ComponentDeclarationContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Memory_deallocate(context);
}

void zen_ComponentDeclarationContext_getChildren(zen_ComponentDeclarationContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_component);
}

/*******************************************************************************
 * FunctionDeclarationContext                                                  *
 *******************************************************************************/

zen_FunctionDeclarationContext_t* zen_FunctionDeclarationContext_new(zen_ASTNode_t* node) {
    zen_FunctionDeclarationContext_t* context = zen_Memory_allocate(zen_FunctionDeclarationContext_t, 1);
    context->m_node = node;
    context->m_identifier = NULL;
    context->m_functionParameters = NULL;
    context->m_functionBody = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_FUNCTION_DECLARATION, context,
        (zen_ContextDestructorFunction_t)zen_FunctionDeclarationContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_FunctionDeclarationContext_getChildren);

    return context;
}

void zen_FunctionDeclarationContext_delete(zen_FunctionDeclarationContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Memory_deallocate(context);
}

void zen_FunctionDeclarationContext_getChildren(zen_FunctionDeclarationContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_identifier);
    if (context->m_functionParameters != NULL) {
        jtk_ArrayList_add(children, context->m_functionParameters);
    }
    jtk_ArrayList_add(children, context->m_functionBody);
}

/*******************************************************************************
 * FunctionParametersContext                                                   *
 *******************************************************************************/

zen_FunctionParametersContext_t* zen_FunctionParametersContext_new(zen_ASTNode_t* node) {
    zen_FunctionParametersContext_t* context = zen_Memory_allocate(zen_FunctionParametersContext_t, 1);
    context->m_node = node;
    context->m_fixedParameters = jtk_ArrayList_new();
    context->m_variableParameter = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_FUNCTION_PARAMETERS, context,
        (zen_ContextDestructorFunction_t)zen_FunctionParametersContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_FunctionParametersContext_getChildren);

    return context;
}

void zen_FunctionParametersContext_delete(zen_FunctionParametersContext_t* context) {
    jtk_Assert_assertObject(context, "The specified object is null.");

    jtk_ArrayList_delete(context->m_fixedParameters);
    jtk_Memory_deallocate(context);
}

void zen_FunctionParametersContext_getChildren(zen_FunctionParametersContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_fixedParameters);
    if (context->m_variableParameter != NULL) {
        jtk_ArrayList_add(children, context->m_variableParameter);
    }
}

/*******************************************************************************
 * FunctionBodyContext                                                         *
 *******************************************************************************/

zen_FunctionBodyContext_t* zen_FunctionBodyContext_new(zen_ASTNode_t* node) {
    zen_FunctionBodyContext_t* context = zen_Memory_allocate(zen_FunctionBodyContext_t, 1);
    context->m_node = node;
    context->m_statementSuite = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_FUNCTION_BODY, context,
        (zen_ContextDestructorFunction_t)zen_FunctionBodyContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_FunctionBodyContext_getChildren);

    return context;
}

void zen_FunctionBodyContext_delete(zen_FunctionBodyContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Memory_deallocate(context);
}

void zen_FunctionBodyContext_getChildren(zen_FunctionBodyContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_statementSuite);
}

/*******************************************************************************
 * StatementSuiteContext                                                       *
 *******************************************************************************/

zen_StatementSuiteContext_t* zen_StatementSuiteContext_new(zen_ASTNode_t* node) {
    zen_StatementSuiteContext_t* context = zen_Memory_allocate(zen_StatementSuiteContext_t, 1);
    context->m_node = node;
    // context->m_simpleStatement = NULL;
    context->m_statements = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_STATEMENT_SUITE, context,
        (zen_ContextDestructorFunction_t)zen_StatementSuiteContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_StatementSuiteContext_getChildren);

    return context;
}

void zen_StatementSuiteContext_delete(zen_StatementSuiteContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_statements);

    jtk_Memory_deallocate(context);
}

void zen_StatementSuiteContext_getChildren(zen_StatementSuiteContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    /*if (context->m_simpleStatement != NULL) {
        jtk_ArrayList_add(children, context->m_simpleStatement);
    }
    else {*/
        jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_statements);
    /* } */
}

/*******************************************************************************
 * SimpleStatementContext                                                      *
 *******************************************************************************/

zen_SimpleStatementContext_t* zen_SimpleStatementContext_new(zen_ASTNode_t* node) {
    zen_SimpleStatementContext_t* context = zen_Memory_allocate(zen_SimpleStatementContext_t, 1);
    context->m_node = node;
    context->m_statement = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_SIMPLE_STATEMENT, context,
        (zen_ContextDestructorFunction_t)zen_SimpleStatementContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_SimpleStatementContext_getChildren);

    return context;
}

void zen_SimpleStatementContext_delete(zen_SimpleStatementContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Memory_deallocate(context);
}

void zen_SimpleStatementContext_getChildren(zen_SimpleStatementContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_statement);
}

/*******************************************************************************
 * StatementContext                                                            *
 *******************************************************************************/

zen_StatementContext_t* zen_StatementContext_new(zen_ASTNode_t* node) {
    zen_StatementContext_t* context = zen_Memory_allocate(zen_StatementContext_t, 1);
    context->m_node = node;
    context->m_simpleStatement = NULL;
    context->m_compoundStatement = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_STATEMENT, context,
        (zen_ContextDestructorFunction_t)zen_StatementContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_StatementContext_getChildren);

    return context;
}

void zen_StatementContext_delete(zen_StatementContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_StatementContext_getChildren(zen_StatementContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    if (context->m_simpleStatement != NULL) {
        jtk_ArrayList_add(children, context->m_simpleStatement);
    }
    else {
        jtk_ArrayList_add(children, context->m_compoundStatement);
    }
}

/*******************************************************************************
 * EmptyStatementContext                                                       *
 *******************************************************************************/

zen_EmptyStatementContext_t* zen_EmptyStatementContext_new(zen_ASTNode_t* node) {
    zen_EmptyStatementContext_t* context = zen_Memory_allocate(zen_EmptyStatementContext_t, 1);
    context->m_node = node;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_EMPTY_STATEMENT, context,
        (zen_ContextDestructorFunction_t)zen_EmptyStatementContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_EmptyStatementContext_getChildren);

    return context;
}

void zen_EmptyStatementContext_delete(zen_EmptyStatementContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Memory_deallocate(context);
}

void zen_EmptyStatementContext_getChildren(zen_EmptyStatementContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");
}

/*******************************************************************************
 * VariableDeclarationContext                                                  *
 *******************************************************************************/

/*
 * TODO: I have accidentally written zen_*Context_new() where I am supposed
 * to write zen_Memory_allocate(zen_*Context_t, 1). Please check for such
 * bugs.
 */
zen_VariableDeclarationContext_t* zen_VariableDeclarationContext_new(zen_ASTNode_t* node) {
    zen_VariableDeclarationContext_t* context = zen_Memory_allocate(zen_VariableDeclarationContext_t, 1);
    context->m_node = node;
    context->m_variableDeclarators = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_VARIABLE_DECLARATION, context,
        (zen_ContextDestructorFunction_t)zen_VariableDeclarationContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_VariableDeclarationContext_getChildren);

    return context;
}

void zen_VariableDeclarationContext_delete(zen_VariableDeclarationContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_variableDeclarators);
    jtk_Memory_deallocate(context);
}

/*
 * TODO: I have accidentally written add() instead of addAll(). Please look
 * for such bugs.
 */
void zen_VariableDeclarationContext_getChildren(zen_VariableDeclarationContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_variableDeclarators);
}

/*******************************************************************************
 * VariableDeclaratorContext                                                   *
 *******************************************************************************/

/*
 * TODO: Check if the constructors are returning.
 */
zen_VariableDeclaratorContext_t* zen_VariableDeclaratorContext_new(zen_ASTNode_t* node) {
    zen_VariableDeclaratorContext_t* context = zen_Memory_allocate(zen_VariableDeclaratorContext_t, 1);
    context->m_node = node;
    context->m_identifier = NULL;
    context->m_expression = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_VARIABLE_DECLARATOR, context,
        (zen_ContextDestructorFunction_t)zen_VariableDeclaratorContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_VariableDeclaratorContext_getChildren);

    return context;
}

void zen_VariableDeclaratorContext_delete(zen_VariableDeclaratorContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_VariableDeclaratorContext_getChildren(zen_VariableDeclaratorContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_identifier);
    if (context->m_expression != NULL) {
        jtk_ArrayList_add(children, context->m_expression);
    }
}

/*******************************************************************************
 * ConstantDeclarationContext                                                  *
 *******************************************************************************/

zen_ConstantDeclarationContext_t* zen_ConstantDeclarationContext_new(zen_ASTNode_t* node) {
    zen_ConstantDeclarationContext_t* context = zen_Memory_allocate(zen_ConstantDeclarationContext_t, 1);
    context->m_node = node;
    context->m_constantDeclarators = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_CONSTANT_DECLARATION, context,
        (zen_ContextDestructorFunction_t)zen_ConstantDeclarationContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ConstantDeclarationContext_getChildren);

    return context;
}

void zen_ConstantDeclarationContext_delete(zen_ConstantDeclarationContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_constantDeclarators);
    jtk_Memory_deallocate(context);
}

void zen_ConstantDeclarationContext_getChildren(zen_ConstantDeclarationContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_constantDeclarators);
}

/*******************************************************************************
 * ConstantDeclaratorContext                                                   *
 *******************************************************************************/

zen_ConstantDeclaratorContext_t* zen_ConstantDeclaratorContext_new(zen_ASTNode_t* node) {
    zen_ConstantDeclaratorContext_t* context = zen_Memory_allocate(zen_ConstantDeclaratorContext_t, 1);
    context->m_node = node;
    context->m_identifier = NULL;
    context->m_expression = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_CONSTANT_DECLARATOR, context,
        (zen_ContextDestructorFunction_t)zen_ConstantDeclaratorContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ConstantDeclaratorContext_getChildren);

    return context;
}

void zen_ConstantDeclaratorContext_delete(zen_ConstantDeclaratorContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_ConstantDeclaratorContext_getChildren(zen_ConstantDeclaratorContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_identifier);
    jtk_ArrayList_add(children, context->m_expression);
}

/*******************************************************************************
 * AssertStatementContext                                                      *
 *******************************************************************************/

zen_AssertStatementContext_t* zen_AssertStatementContext_new(zen_ASTNode_t* node) {
    zen_AssertStatementContext_t* context = zen_Memory_allocate(zen_AssertStatementContext_t, 1);
    context->m_node = node;
    context->m_conditionExpression = NULL;
    context->m_messageExpression = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_ASSERT_STATEMENT, context,
        (zen_ContextDestructorFunction_t)zen_AssertStatementContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_AssertStatementContext_getChildren);

    return context;
}

void zen_AssertStatementContext_delete(zen_AssertStatementContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_AssertStatementContext_getChildren(zen_AssertStatementContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_conditionExpression);
    jtk_ArrayList_add(children, context->m_messageExpression);
}

/*******************************************************************************
 * BreakStatementContext                                                       *
 *******************************************************************************/

zen_BreakStatementContext_t* zen_BreakStatementContext_new(zen_ASTNode_t* node) {
    zen_BreakStatementContext_t* context = zen_Memory_allocate(zen_BreakStatementContext_t, 1);
    context->m_node = node;
    context->m_identifier = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_BREAK_STATEMENT, context,
        (zen_ContextDestructorFunction_t)zen_BreakStatementContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_BreakStatementContext_getChildren);

    return context;
}

void zen_BreakStatementContext_delete(zen_BreakStatementContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Memory_deallocate(context);
}

void zen_BreakStatementContext_getChildren(zen_BreakStatementContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    if (context->m_identifier != NULL) {
        jtk_ArrayList_add(children, context->m_identifier);
    }
}

/*******************************************************************************
 * ContinueStatementContext                                                    *
 *******************************************************************************/

zen_ContinueStatementContext_t* zen_ContinueStatementContext_new(zen_ASTNode_t* node) {
    zen_ContinueStatementContext_t* context = zen_Memory_allocate(zen_ContinueStatementContext_t, 1);
    context->m_node = node;
    context->m_identifier = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_CONTINUE_STATEMENT, context,
        (zen_ContextDestructorFunction_t)zen_ContinueStatementContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ContinueStatementContext_getChildren);

    return context;
}

void zen_ContinueStatementContext_delete(zen_ContinueStatementContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Memory_deallocate(context);
}

void zen_ContinueStatementContext_getChildren(zen_ContinueStatementContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    if (context->m_identifier) {
        jtk_ArrayList_add(children, context->m_identifier);
    }
}

/*******************************************************************************
 * ReturnStatementContext                                                      *
 *******************************************************************************/

zen_ReturnStatementContext_t* zen_ReturnStatementContext_new(zen_ASTNode_t* node) {
    zen_ReturnStatementContext_t* context = zen_Memory_allocate(zen_ReturnStatementContext_t, 1);
    context->m_node = node;
    context->m_expression = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_RETURN_STATEMENT, context,
        (zen_ContextDestructorFunction_t)zen_ReturnStatementContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ReturnStatementContext_getChildren);

    return context;
}

void zen_ReturnStatementContext_delete(zen_ReturnStatementContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_ReturnStatementContext_getChildren(zen_ReturnStatementContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    if (context->m_expression != NULL) {
        jtk_ArrayList_add(children, context->m_expression);
    }
}

/*******************************************************************************
 * ThrowStatementContext                                                       *
 *******************************************************************************/

zen_ThrowStatementContext_t* zen_ThrowStatementContext_new(zen_ASTNode_t* node) {
    zen_ThrowStatementContext_t* context = zen_Memory_allocate(zen_ThrowStatementContext_t, 1);
    context->m_node = node;
    context->m_expression = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_THROW_STATEMENT, context,
        (zen_ContextDestructorFunction_t)zen_ThrowStatementContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ThrowStatementContext_getChildren);

    return context;
}

void zen_ThrowStatementContext_delete(zen_ThrowStatementContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_ThrowStatementContext_getChildren(zen_ThrowStatementContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    if (context->m_expression != NULL) {
        jtk_ArrayList_add(children, context->m_expression);
    }
}

/*******************************************************************************
 * CompoundStatementContext                                                    *
 *******************************************************************************/

zen_CompoundStatementContext_t* zen_CompoundStatementContext_new(zen_ASTNode_t* node) {
    zen_CompoundStatementContext_t* context = zen_Memory_allocate(zen_CompoundStatementContext_t, 1);
    context->m_node = node;
    context->m_statement = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_COMPOUND_STATEMENT, context,
        (zen_ContextDestructorFunction_t)zen_CompoundStatementContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_CompoundStatementContext_getChildren);

    return context;
}

void zen_CompoundStatementContext_delete(zen_CompoundStatementContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_CompoundStatementContext_getChildren(zen_CompoundStatementContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_statement);
}

/*******************************************************************************
 * IfStatementContext                                                          *
 *******************************************************************************/

zen_IfStatementContext_t* zen_IfStatementContext_new(zen_ASTNode_t* node) {
    zen_IfStatementContext_t* context = zen_Memory_allocate(zen_IfStatementContext_t, 1);
    context->m_node = node;
    context->m_ifClause = NULL;
    context->m_elseIfClauses = jtk_ArrayList_new();
    context->m_elseClause = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_IF_STATEMENT, context,
        (zen_ContextDestructorFunction_t)zen_IfStatementContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_IfStatementContext_getChildren);

    return context;
}

/*
 * TODO: At places, I have forgotten to delete the context. Please fix
 *       such bugs.
 */
void zen_IfStatementContext_delete(zen_IfStatementContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_elseIfClauses);
    jtk_Memory_deallocate(context);
}

void zen_IfStatementContext_getChildren(zen_IfStatementContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_ifClause);
    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_elseIfClauses);
    if (context->m_elseClause != NULL) {
        jtk_ArrayList_add(children, context->m_elseClause);
    }
}

/*******************************************************************************
 * IfClauseContext                                                             *
 *******************************************************************************/

zen_IfClauseContext_t* zen_IfClauseContext_new(zen_ASTNode_t* node) {
    zen_IfClauseContext_t* context = zen_Memory_allocate(zen_IfClauseContext_t, 1);
    context->m_node = node;
    context->m_expression = NULL;
    context->m_statementSuite = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_IF_CLAUSE, context,
        (zen_ContextDestructorFunction_t)zen_IfClauseContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_IfClauseContext_getChildren);

    return context;
}

void zen_IfClauseContext_delete(zen_IfClauseContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_IfClauseContext_getChildren(zen_IfClauseContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_expression);
    jtk_ArrayList_add(children, context->m_statementSuite);
}

/*******************************************************************************
 * ElseIfClauseContext                                                         *
 *******************************************************************************/

zen_ElseIfClauseContext_t* zen_ElseIfClauseContext_new(zen_ASTNode_t* node) {
    zen_ElseIfClauseContext_t* context = zen_Memory_allocate(zen_ElseIfClauseContext_t, 1);
    context->m_node = node;
    context->m_expression = NULL;
    context->m_statementSuite = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_ELSE_IF_CLAUSE, context,
        (zen_ContextDestructorFunction_t)zen_ElseIfClauseContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ElseIfClauseContext_getChildren);

    return context;
}

void zen_ElseIfClauseContext_delete(zen_ElseIfClauseContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_ElseIfClauseContext_getChildren(zen_ElseIfClauseContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_expression);
    jtk_ArrayList_add(children, context->m_statementSuite);
}

/*******************************************************************************
 * ElseClauseContext                                                           *
 *******************************************************************************/

zen_ElseClauseContext_t* zen_ElseClauseContext_new(zen_ASTNode_t* node) {
    zen_ElseClauseContext_t* context = zen_Memory_allocate(zen_ElseClauseContext_t, 1);
    context->m_node = node;
    context->m_statementSuite = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_ELSE_CLAUSE, context,
        (zen_ContextDestructorFunction_t)zen_ElseClauseContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ElseClauseContext_getChildren);

    return context;
}

void zen_ElseClauseContext_delete(zen_ElseClauseContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_ElseClauseContext_getChildren(zen_ElseClauseContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_statementSuite);
}

/*******************************************************************************
 * IterativeStatementContext                                                   *
 *******************************************************************************/

zen_IterativeStatementContext_t* zen_IterativeStatementContext_new(zen_ASTNode_t* node) {
    zen_IterativeStatementContext_t* context = zen_Memory_allocate(zen_IterativeStatementContext_t, 1);
    context->m_node = node;
    context->m_labelClause = NULL;
    context->m_statement = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_ITERATIVE_STATEMENT, context,
        (zen_ContextDestructorFunction_t)zen_IterativeStatementContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_IterativeStatementContext_getChildren);

    return context;
}

void zen_IterativeStatementContext_delete(zen_IterativeStatementContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_IterativeStatementContext_getChildren(zen_IterativeStatementContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    if (context->m_labelClause != NULL) {
        jtk_ArrayList_add(children, context->m_labelClause);
    }
    jtk_ArrayList_add(children, context->m_statement);
}

/*******************************************************************************
 * LabelClauseContext                                                          *
 *******************************************************************************/

zen_LabelClauseContext_t* zen_LabelClauseContext_new(zen_ASTNode_t* node) {
    zen_LabelClauseContext_t* context = zen_Memory_allocate(zen_LabelClauseContext_t, 1);
    context->m_node = node;
    context->m_identifier = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_LABEL_CLAUSE, context,
        (zen_ContextDestructorFunction_t)zen_LabelClauseContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_LabelClauseContext_getChildren);

    return context;
}

void zen_LabelClauseContext_delete(zen_LabelClauseContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Memory_deallocate(context);
}

void zen_LabelClauseContext_getChildren(zen_LabelClauseContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_identifier);
}

/*******************************************************************************
 * WhileStatementContext                                                       *
 *******************************************************************************/

zen_WhileStatementContext_t* zen_WhileStatementContext_new(zen_ASTNode_t* node) {
    zen_WhileStatementContext_t* context = zen_Memory_allocate(zen_WhileStatementContext_t, 1);
    context->m_node = node;
    context->m_expression = NULL;
    context->m_statementSuite = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_WHILE_STATEMENT, context,
        (zen_ContextDestructorFunction_t)zen_WhileStatementContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_WhileStatementContext_getChildren);

    return context;
}

void zen_WhileStatementContext_delete(zen_WhileStatementContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_WhileStatementContext_getChildren(zen_WhileStatementContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_expression);
    jtk_ArrayList_add(children, context->m_statementSuite);
}

/*******************************************************************************
 * ForStatementContext                                                         *
 *******************************************************************************/

zen_ForStatementContext_t* zen_ForStatementContext_new(zen_ASTNode_t* node) {
    zen_ForStatementContext_t* context = zen_Memory_allocate(zen_ForStatementContext_t, 1);
    context->m_node = node;
    context->m_forParameter = NULL;
    context->m_expression = NULL;
    context->m_statementSuite = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_FOR_STATEMENT, context,
        (zen_ContextDestructorFunction_t)zen_ForStatementContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ForStatementContext_getChildren);

    return context;
}

void zen_ForStatementContext_delete(zen_ForStatementContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_ForStatementContext_getChildren(zen_ForStatementContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_forParameter);
    jtk_ArrayList_add(children, context->m_expression);
    jtk_ArrayList_add(children, context->m_statementSuite);
}

/*******************************************************************************
 * ForParameterContext                                                         *
 *******************************************************************************/

zen_ForParameterContext_t* zen_ForParameterContext_new(zen_ASTNode_t* node) {
    zen_ForParameterContext_t* context = zen_Memory_allocate(zen_ForParameterContext_t, 1);
    context->m_node = node;
    context->m_declaration = false;
    context->m_variable = false;
    context->m_identifier = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_FOR_PARAMETER, context,
        (zen_ContextDestructorFunction_t)zen_ForParameterContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ForParameterContext_getChildren);

    return context;
}

void zen_ForParameterContext_delete(zen_ForParameterContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_ForParameterContext_getChildren(zen_ForParameterContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_add(children, context->m_identifier);
}

/*******************************************************************************
 * TryStatementContext                                                         *
 *******************************************************************************/

zen_TryStatementContext_t* zen_TryStatementContext_new(zen_ASTNode_t* node) {
    zen_TryStatementContext_t* context = zen_Memory_allocate(zen_TryStatementContext_t, 1);
    context->m_node = node;
    context->m_tryClause = NULL;
    context->m_catchClauses = jtk_ArrayList_new();
    context->m_finallyClause = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_TRY_STATEMENT, context,
        (zen_ContextDestructorFunction_t)zen_TryStatementContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_TryStatementContext_getChildren);

    return context;
}

void zen_TryStatementContext_delete(zen_TryStatementContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_catchClauses);
    jtk_Memory_deallocate(context);
}

void zen_TryStatementContext_getChildren(zen_TryStatementContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_tryClause);
    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_catchClauses);
    if (context->m_finallyClause != NULL) {
        jtk_ArrayList_add(children, context->m_finallyClause);
    }
}

/*******************************************************************************
 * TryClauseContext                                                            *
 *******************************************************************************/

zen_TryClauseContext_t* zen_TryClauseContext_new(zen_ASTNode_t* node) {
    zen_TryClauseContext_t* context = zen_Memory_allocate(zen_TryClauseContext_t, 1);
    context->m_node = node;
    context->m_statementSuite = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_TRY_CLAUSE, context,
        (zen_ContextDestructorFunction_t)zen_TryClauseContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_TryClauseContext_getChildren);

    return context;
}

void zen_TryClauseContext_delete(zen_TryClauseContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_TryClauseContext_getChildren(zen_TryClauseContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_statementSuite);
}

/*******************************************************************************
 * CatchClauseContext                                                          *
 *******************************************************************************/

zen_CatchClauseContext_t* zen_CatchClauseContext_new(zen_ASTNode_t* node) {
    zen_CatchClauseContext_t* context = zen_Memory_allocate(zen_CatchClauseContext_t, 1);
    context->m_node = node;
    context->m_catchFilter = NULL;
    context->m_identifier = NULL;
    context->m_statementSuite = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_CATCH_CLAUSE, context,
        (zen_ContextDestructorFunction_t)zen_CatchClauseContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_CatchClauseContext_getChildren);

    return context;
}

void zen_CatchClauseContext_delete(zen_CatchClauseContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_CatchClauseContext_getChildren(zen_CatchClauseContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_catchFilter);
    jtk_ArrayList_add(children, context->m_identifier);
    jtk_ArrayList_add(children, context->m_statementSuite);
}

/*******************************************************************************
 * CatchFilterContext                                                          *
 *******************************************************************************/

zen_CatchFilterContext_t* zen_CatchFilterContext_new(zen_ASTNode_t* node) {
    zen_CatchFilterContext_t* context = zen_Memory_allocate(zen_CatchFilterContext_t, 1);
    context->m_node = node;
    context->m_typeNames = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_CATCH_FILTER, context,
        (zen_ContextDestructorFunction_t)zen_CatchFilterContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_CatchFilterContext_getChildren);

    return context;
}

void zen_CatchFilterContext_delete(zen_CatchFilterContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_typeNames);
    jtk_Memory_deallocate(context);
}

void zen_CatchFilterContext_getChildren(zen_CatchFilterContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_typeNames);
}

/*******************************************************************************
 * TypeNameContext                                                             *
 *******************************************************************************/

zen_TypeNameContext_t* zen_TypeNameContext_new(zen_ASTNode_t* node) {
    zen_TypeNameContext_t* context = zen_Memory_allocate(zen_TypeNameContext_t, 1);
    context->m_node = node;
    context->m_identifiers = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_TYPE_NAME, context,
        (zen_ContextDestructorFunction_t)zen_TypeNameContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_TypeNameContext_getChildren);

    return context;
}

void zen_TypeNameContext_delete(zen_TypeNameContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_identifiers);
    jtk_Memory_deallocate(context);
}

void zen_TypeNameContext_getChildren(zen_TypeNameContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_identifiers);
}

/*******************************************************************************
 * FinallyClauseContext                                                        *
 *******************************************************************************/

zen_FinallyClauseContext_t* zen_FinallyClauseContext_new(zen_ASTNode_t* node) {
    zen_FinallyClauseContext_t* context = zen_Memory_allocate(zen_FinallyClauseContext_t, 1);
    context->m_node = node;
    context->m_statementSuite = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_FINALLY_CLAUSE, context,
        (zen_ContextDestructorFunction_t)zen_FinallyClauseContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_FinallyClauseContext_getChildren);

    return context;
}

void zen_FinallyClauseContext_delete(zen_FinallyClauseContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_FinallyClauseContext_getChildren(zen_FinallyClauseContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_statementSuite);
}

/*******************************************************************************
 * SynchronizeStatementContext                                                 *
 *******************************************************************************/

zen_SynchronizeStatementContext_t* zen_SynchronizeStatementContext_new(zen_ASTNode_t* node) {
    zen_SynchronizeStatementContext_t* context = zen_Memory_allocate(zen_SynchronizeStatementContext_t, 1);
    context->m_node = node;
    context->m_expression = NULL;
    context->m_statementSuite = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_SYNCHRONIZE_STATEMENT, context,
        (zen_ContextDestructorFunction_t)zen_SynchronizeStatementContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_SynchronizeStatementContext_getChildren);

    return context;
}

void zen_SynchronizeStatementContext_delete(zen_SynchronizeStatementContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_SynchronizeStatementContext_getChildren(zen_SynchronizeStatementContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_expression);
    jtk_ArrayList_add(children, context->m_statementSuite);
}

/*******************************************************************************
 * WithStatementContext                                                        *
 *******************************************************************************/

zen_WithStatementContext_t* zen_WithStatementContext_new(zen_ASTNode_t* node) {
    zen_WithStatementContext_t* context = zen_Memory_allocate(zen_WithStatementContext_t, 1);
    context->m_node = node;
    context->m_withParameters = NULL;
    context->m_statementSuite = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_WITH_STATEMENT, context,
        (zen_ContextDestructorFunction_t)zen_WithStatementContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_WithStatementContext_getChildren);

    return context;
}

void zen_WithStatementContext_delete(zen_WithStatementContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_WithStatementContext_getChildren(zen_WithStatementContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_withParameters);
    jtk_ArrayList_add(children, context->m_statementSuite);
}

/*******************************************************************************
 * WithParametersContext                                                       *
 *******************************************************************************/

zen_WithParametersContext_t* zen_WithParametersContext_new(zen_ASTNode_t* node) {
    zen_WithParametersContext_t* context = zen_Memory_allocate(zen_WithParametersContext_t, 1);
    context->m_node = node;
    context->m_withParameters = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_WITH_PARAMETERS, context,
        (zen_ContextDestructorFunction_t)zen_WithParametersContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_WithParametersContext_getChildren);

    return context;
}

void zen_WithParametersContext_delete(zen_WithParametersContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_withParameters);
    jtk_Memory_deallocate(context);
}

void zen_WithParametersContext_getChildren(zen_WithParametersContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_withParameters);
}

/*******************************************************************************
 * WithParameterContext                                                        *
 *******************************************************************************/

zen_WithParameterContext_t* zen_WithParameterContext_new(zen_ASTNode_t* node) {
    zen_WithParameterContext_t* context = zen_Memory_allocate(zen_WithParameterContext_t, 1);
    context->m_node = node;
    context->m_variable = true;
    context->m_identifier = NULL;
    context->m_expression = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_WITH_PARAMETER, context,
        (zen_ContextDestructorFunction_t)zen_WithParameterContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_WithParameterContext_getChildren);

    return context;
}

void zen_WithParameterContext_delete(zen_WithParameterContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_WithParameterContext_getChildren(zen_WithParameterContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_identifier);
    jtk_ArrayList_add(children, context->m_expression);
}

/*******************************************************************************
 * ClassDeclarationContext                                                     *
 *******************************************************************************/

zen_ClassDeclarationContext_t* zen_ClassDeclarationContext_new(zen_ASTNode_t* node) {
    zen_ClassDeclarationContext_t* context = zen_Memory_allocate(zen_ClassDeclarationContext_t, 1);
    context->m_node = node;
    context->m_identifier = NULL;
    context->m_classExtendsClause = NULL;
    context->m_classSuite = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_CLASS_DECLARATION, context,
        (zen_ContextDestructorFunction_t)zen_ClassDeclarationContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ClassDeclarationContext_getChildren);

    return context;
}

void zen_ClassDeclarationContext_delete(zen_ClassDeclarationContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_ClassDeclarationContext_getChildren(zen_ClassDeclarationContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_identifier);
    if (context->m_classExtendsClause != NULL) {
        jtk_ArrayList_add(children, context->m_classExtendsClause);
    }
    jtk_ArrayList_add(children, context->m_classSuite);
}

/*******************************************************************************
 * ClassExtendsClauseContext                                                        *
 *******************************************************************************/

zen_ClassExtendsClauseContext_t* zen_ClassExtendsClauseContext_new(zen_ASTNode_t* node) {
    zen_ClassExtendsClauseContext_t* context = zen_Memory_allocate(zen_ClassExtendsClauseContext_t, 1);
    context->m_node = node;
    context->m_typeNames = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_EXTENDS_CLAUSE, context,
        (zen_ContextDestructorFunction_t)zen_ClassExtendsClauseContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ClassExtendsClauseContext_getChildren);

    return context;
}

void zen_ClassExtendsClauseContext_delete(zen_ClassExtendsClauseContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_typeNames);
    jtk_Memory_deallocate(context);
}

void zen_ClassExtendsClauseContext_getChildren(zen_ClassExtendsClauseContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_typeNames);
}

/*******************************************************************************
 * ClassSuiteContext                                                           *
 *******************************************************************************/

zen_ClassSuiteContext_t* zen_ClassSuiteContext_new(zen_ASTNode_t* node) {
    zen_ClassSuiteContext_t* context = zen_Memory_allocate(zen_ClassSuiteContext_t, 1);
    context->m_node = node;
    context->m_classMembers = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_CLASS_SUITE, context,
        (zen_ContextDestructorFunction_t)zen_ClassSuiteContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ClassSuiteContext_getChildren);

    return context;
}

void zen_ClassSuiteContext_delete(zen_ClassSuiteContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_classMembers);
    jtk_Memory_deallocate(context);
}

void zen_ClassSuiteContext_getChildren(zen_ClassSuiteContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_classMembers);
}

/*******************************************************************************
 * ClassMemberContext                                                          *
 *******************************************************************************/

zen_ClassMemberContext_t* zen_ClassMemberContext_new(zen_ASTNode_t* node) {
    zen_ClassMemberContext_t* context = zen_Memory_allocate(zen_ClassMemberContext_t, 1);
    context->m_node = node;
    context->m_annotations = NULL;
    context->m_modifiers = jtk_ArrayList_new();
    context->m_declaration = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_CLASS_MEMBER, context,
        (zen_ContextDestructorFunction_t)zen_ClassMemberContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ClassMemberContext_getChildren);

    return context;
}

void zen_ClassMemberContext_delete(zen_ClassMemberContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_modifiers);
    jtk_Memory_deallocate(context);
}

void zen_ClassMemberContext_getChildren(zen_ClassMemberContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_annotations);
    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_modifiers);
    jtk_ArrayList_add(children, context->m_declaration);
}

/*******************************************************************************
 * ConstructorDeclarationContext                                               *
 *******************************************************************************/

// zen_ConstructorDeclarationContext_t* zen_ConstructorDeclarationContext_new(zen_ASTNode_t* node) {
//    zen_ConstructorDeclarationContext_t* context = zen_Memory_allocate(zen_ConstructorDeclarationContext_t, 1);
//     context->m_node = node;
//     context->m_identifier = NULL;
//     context->m_functionParameters = NULL;
//     context->m_statementSuite = NULL;

//     zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_CONSTRUCTOR_DECLARATION, context,
//         (zen_ContextDestructorFunction_t)zen_ConstructorDeclarationContext_delete,
//         (zen_EnumerateContextChildrenFunction_t)zen_ConstructorDeclarationContext_getChildren);

//     return context;
// }

// void zen_ConstructorDeclarationContext_delete(zen_ConstructorDeclarationContext_t* context) {
//     jtk_Assert_assertObject(context, "The specified context is null.");

//     jtk_Memory_deallocate(context);
// }

// void zen_ConstructorDeclarationContext_getChildren(zen_ConstructorDeclarationContext_t* context,
//     jtk_ArrayList_t* children) {
//     jtk_Assert_assertObject(context, "The specified context is null.");
//     jtk_Assert_assertObject(children, "The specified children is null.");

//     jtk_ArrayList_add(children, context->m_identifier);
//     if (context->m_functionParameters != NULL) {
//         jtk_ArrayList_add(children, context->m_functionParameters);
//     }
//     jtk_ArrayList_add(children, context->m_statementSuite);
// }

/*******************************************************************************
 * EnumerationDeclarationContext                                               *
 *******************************************************************************/

/* TODO: Check if all the delete functions deallocate the context. */
// zen_EnumerationDeclarationContext_t* zen_EnumerationDeclarationContext_new(zen_ASTNode_t* node) {
//     zen_EnumerationDeclarationContext_t* context = zen_Memory_allocate(zen_EnumerationDeclarationContext_t, 1);
//     context->m_node = node;
//     context->m_identifier = NULL;
//     context->m_enumerationBaseClause = NULL;
//     context->m_enumerationSuite = NULL;

//     zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_ENUMERATION_DECLARATION, context,
//         (zen_ContextDestructorFunction_t)zen_EnumerationDeclarationContext_delete,
//         (zen_EnumerateContextChildrenFunction_t)zen_EnumerationDeclarationContext_getChildren);

//     return context;
// }

// void zen_EnumerationDeclarationContext_delete(zen_EnumerationDeclarationContext_t* context) {
//     jtk_Assert_assertObject(context, "The specified context is null.");

//     jtk_Memory_deallocate(context);
// }

// void zen_EnumerationDeclarationContext_getChildren(zen_EnumerationDeclarationContext_t* context,
//     jtk_ArrayList_t* children) {
//     jtk_Assert_assertObject(context, "The specified context is null.");
//     jtk_Assert_assertObject(children, "The specified children is null.");

//     jtk_ArrayList_add(children, context->m_identifier);
//     if (context->m_enumerationBaseClause != NULL) {
//         jtk_ArrayList_add(children, context->m_enumerationBaseClause);
//     }
//     jtk_ArrayList_add(children, context->m_enumerationSuite);
// }

/*******************************************************************************
 * EnumerationBaseClauseContext                                                *
 *******************************************************************************/

// zen_EnumerationBaseClauseContext_t* zen_EnumerationBaseClauseContext_new(zen_ASTNode_t* node) {
//     zen_EnumerationBaseClauseContext_t* context = zen_Memory_allocate(zen_EnumerationBaseClauseContext_t, 1);
//     context->m_node = node;
//     context->m_typeName = NULL;

//     zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_ENUMERATION_BASE_CLAUSE, context,
//         (zen_ContextDestructorFunction_t)zen_EnumerationBaseClauseContext_delete,
//         (zen_EnumerateContextChildrenFunction_t)zen_EnumerationBaseClauseContext_getChildren);

//     return context;
// }

// void zen_EnumerationBaseClauseContext_delete(zen_EnumerationBaseClauseContext_t* context) {
//     jtk_Assert_assertObject(context, "The specified context is null.");

//     jtk_Memory_deallocate(context);
// }

// void zen_EnumerationBaseClauseContext_getChildren(zen_EnumerationBaseClauseContext_t* context,
//     jtk_ArrayList_t* children) {
//     jtk_Assert_assertObject(context, "The specified context is null.");
//     jtk_Assert_assertObject(children, "The specified children is null.");

//     jtk_ArrayList_add(children, context->m_typeName);
// }

/*******************************************************************************
 * EnumerationSuiteContext                                                     *
 *******************************************************************************/

// zen_EnumerationSuiteContext_t* zen_EnumerationSuiteContext_new(zen_ASTNode_t* node) {
//     zen_EnumerationSuiteContext_t* context = zen_Memory_allocate(zen_EnumerationSuiteContext_t, 1);
//     context->m_node = node;
//     context->m_enumerates = jtk_ArrayList_new();

//     zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_ENUMERATION_SUITE, context,
//         (zen_ContextDestructorFunction_t)zen_EnumerationSuiteContext_delete,
//         (zen_EnumerateContextChildrenFunction_t)zen_EnumerationSuiteContext_getChildren);

//     return context;
// }

// void zen_EnumerationSuiteContext_delete(zen_EnumerationSuiteContext_t* context) {
//     jtk_Assert_assertObject(context, "The specified context is null.");

//     jtk_ArrayList_delete(context->m_enumerates);
//     jtk_Memory_deallocate(context);
// }

// void zen_EnumerationSuiteContext_getChildren(zen_EnumerationSuiteContext_t* context,
//     jtk_ArrayList_t* children) {
//     jtk_Assert_assertObject(context, "The specified context is null.");
//     jtk_Assert_assertObject(children, "The specified children is null.");

//     jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_enumerates);
// }

/*******************************************************************************
 * EnumerateContext                                                            *
 *******************************************************************************/

// zen_EnumerateContext_t* zen_EnumerateContext_new(zen_ASTNode_t* node) {
//     zen_EnumerateContext_t* context = zen_Memory_allocate(zen_EnumerateContext_t, 1);
//     context->m_node = node;
//     context->m_identifier = NULL;
//     context->m_functionArguments = NULL;

//     zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_ENUMERATE, context,
//         (zen_ContextDestructorFunction_t)zen_EnumerateContext_delete,
//         (zen_EnumerateContextChildrenFunction_t)zen_EnumerateContext_getChildren);

//     return context;
// }

// void zen_EnumerateContext_delete(zen_EnumerateContext_t* context) {
//     jtk_Assert_assertObject(context, "The specified context is null.");

//     jtk_Memory_deallocate(context);
// }

// void zen_EnumerateContext_getChildren(zen_EnumerateContext_t* context,
//     jtk_ArrayList_t* children) {
//     jtk_Assert_assertObject(context, "The specified context is null.");
//     jtk_Assert_assertObject(children, "The specified children is null.");

//     jtk_ArrayList_add(children, context->m_identifier);
//     if (context->m_functionArguments != NULL) {
//         jtk_ArrayList_add(children, context->m_functionArguments);
//     }
// }

/*******************************************************************************
 * ExpressionsContext                                                          *
 *******************************************************************************/

zen_ExpressionsContext_t* zen_ExpressionsContext_new(zen_ASTNode_t* node) {
    zen_ExpressionsContext_t* context = zen_Memory_allocate(zen_ExpressionsContext_t, 1);
    context->m_node = node;
    context->m_expressions = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_EXPRESSIONS, context,
        (zen_ContextDestructorFunction_t)zen_ExpressionsContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ExpressionsContext_getChildren);

    return context;
}

void zen_ExpressionsContext_delete(zen_ExpressionsContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_expressions);
    jtk_Memory_deallocate(context);
}

void zen_ExpressionsContext_getChildren(zen_ExpressionsContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_expressions);
}

/*******************************************************************************
 * ExpressionContext                                                           *
 *******************************************************************************/

zen_ExpressionContext_t* zen_ExpressionContext_new(zen_ASTNode_t* node) {
    zen_ExpressionContext_t* context = zen_Memory_allocate(zen_ExpressionContext_t, 1);
    context->m_node = node;
    context->m_assignmentExpression = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_EXPRESSION, context,
        (zen_ContextDestructorFunction_t)zen_ExpressionContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ExpressionContext_getChildren);

    return context;
}

void zen_ExpressionContext_delete(zen_ExpressionContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_ExpressionContext_getChildren(zen_ExpressionContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_assignmentExpression);
}

/*******************************************************************************
 * AssignmentExpressionContext                                                 *
 *******************************************************************************/

zen_AssignmentExpressionContext_t* zen_AssignmentExpressionContext_new(zen_ASTNode_t* node) {
    zen_AssignmentExpressionContext_t* context = zen_Memory_allocate(zen_AssignmentExpressionContext_t, 1);
    context->m_node = node;
    context->m_conditionalExpression = NULL;
    context->m_assignmentOperator = NULL;
    context->m_assignmentExpression = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_ASSIGNMENT_EXPRESSION, context,
        (zen_ContextDestructorFunction_t)zen_AssignmentExpressionContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_AssignmentExpressionContext_getChildren);

    return context;
}

void zen_AssignmentExpressionContext_delete(zen_AssignmentExpressionContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_AssignmentExpressionContext_getChildren(zen_AssignmentExpressionContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_conditionalExpression);
    if (context->m_assignmentOperator != NULL) {
        jtk_ArrayList_add(children, context->m_assignmentOperator);
        jtk_ArrayList_add(children, context->m_assignmentExpression);
    }
}

/*******************************************************************************
 * ConditionalExpressionContext                                                *
 *******************************************************************************/

zen_ConditionalExpressionContext_t* zen_ConditionalExpressionContext_new(zen_ASTNode_t* node) {
    zen_ConditionalExpressionContext_t* context = zen_Memory_allocate(zen_ConditionalExpressionContext_t, 1);
    context->m_node = node;
    context->m_logicalOrExpression = NULL;
    context->m_thenExpression = NULL;
    context->m_elseExpression = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_CONDITIONAL_EXPRESSION, context,
        (zen_ContextDestructorFunction_t)zen_ConditionalExpressionContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ConditionalExpressionContext_getChildren);

    return context;
}

void zen_ConditionalExpressionContext_delete(zen_ConditionalExpressionContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_ConditionalExpressionContext_getChildren(zen_ConditionalExpressionContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_logicalOrExpression);
    if (context->m_thenExpression != NULL) {
        jtk_ArrayList_add(children, context->m_thenExpression);
        jtk_ArrayList_add(children, context->m_elseExpression);
    }
}

/*******************************************************************************
 * LogicalOrExpressionContext                                                  *
 *******************************************************************************/

zen_LogicalOrExpressionContext_t* zen_LogicalOrExpressionContext_new(zen_ASTNode_t* node) {
    zen_LogicalOrExpressionContext_t* context = zen_Memory_allocate(zen_LogicalOrExpressionContext_t, 1);
    context->m_node = node;
    context->m_logicalAndExpression = NULL;
    context->m_logicalAndExpressions = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_LOGICAL_OR_EXPRESSION, context,
        (zen_ContextDestructorFunction_t)zen_LogicalOrExpressionContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_LogicalOrExpressionContext_getChildren);

    return context;
}

void zen_LogicalOrExpressionContext_delete(zen_LogicalOrExpressionContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_logicalAndExpressions);
    jtk_Memory_deallocate(context);
}

void zen_LogicalOrExpressionContext_getChildren(zen_LogicalOrExpressionContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_logicalAndExpression);
    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_logicalAndExpressions);
}

/*******************************************************************************
 * LogicalAndExpressionContext                                                 *
 *******************************************************************************/

zen_LogicalAndExpressionContext_t* zen_LogicalAndExpressionContext_new(zen_ASTNode_t* node) {
    zen_LogicalAndExpressionContext_t* context = zen_Memory_allocate(zen_LogicalAndExpressionContext_t, 1);
    context->m_node = node;
    context->m_inclusiveOrExpression = NULL;
    context->m_inclusiveOrExpressions = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_LOGICAL_AND_EXPRESSION, context,
        (zen_ContextDestructorFunction_t)zen_LogicalAndExpressionContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_LogicalAndExpressionContext_getChildren);

    return context;
}

void zen_LogicalAndExpressionContext_delete(zen_LogicalAndExpressionContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_inclusiveOrExpressions);
    jtk_Memory_deallocate(context);
}

void zen_LogicalAndExpressionContext_getChildren(zen_LogicalAndExpressionContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_inclusiveOrExpression);
    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_inclusiveOrExpressions);
}

/*******************************************************************************
 * InclusiveOrExpressionContext                                                *
 *******************************************************************************/

zen_InclusiveOrExpressionContext_t* zen_InclusiveOrExpressionContext_new(zen_ASTNode_t* node) {
    zen_InclusiveOrExpressionContext_t* context = zen_Memory_allocate(zen_InclusiveOrExpressionContext_t, 1);
    context->m_node = node;
    context->m_exclusiveOrExpression = NULL;
    context->m_exclusiveOrExpressions = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_INCLUSIVE_OR_EXPRESSION, context,
        (zen_ContextDestructorFunction_t)zen_InclusiveOrExpressionContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_InclusiveOrExpressionContext_getChildren);

    return context;
}

void zen_InclusiveOrExpressionContext_delete(zen_InclusiveOrExpressionContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_exclusiveOrExpressions);
    jtk_Memory_deallocate(context);
}

void zen_InclusiveOrExpressionContext_getChildren(zen_InclusiveOrExpressionContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_exclusiveOrExpression);
    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_exclusiveOrExpressions);
}

/*******************************************************************************
 * ExclusiveOrExpressionContext                                                *
 *******************************************************************************/

zen_ExclusiveOrExpressionContext_t* zen_ExclusiveOrExpressionContext_new(zen_ASTNode_t* node) {
    zen_ExclusiveOrExpressionContext_t* context = zen_Memory_allocate(zen_ExclusiveOrExpressionContext_t, 1);
    context->m_node = node;
    context->m_andExpression = NULL;
    context->m_andExpressions = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_EXCLUSIVE_OR_EXPRESSION, context,
        (zen_ContextDestructorFunction_t)zen_ExclusiveOrExpressionContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ExclusiveOrExpressionContext_getChildren);

    return context;
}

void zen_ExclusiveOrExpressionContext_delete(zen_ExclusiveOrExpressionContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_andExpressions);
    jtk_Memory_deallocate(context);
}

void zen_ExclusiveOrExpressionContext_getChildren(zen_ExclusiveOrExpressionContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_andExpression);
    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_andExpressions);
}

/*******************************************************************************
 * AndExpressionContext                                                        *
 *******************************************************************************/

zen_AndExpressionContext_t* zen_AndExpressionContext_new(zen_ASTNode_t* node) {
    zen_AndExpressionContext_t* context = zen_Memory_allocate(zen_AndExpressionContext_t, 1);
    context->m_node = node;
    context->m_equalityExpression = NULL;
    context->m_equalityExpressions = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_AND_EXPRESSION, context,
        (zen_ContextDestructorFunction_t)zen_AndExpressionContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_AndExpressionContext_getChildren);

    return context;
}

void zen_AndExpressionContext_delete(zen_AndExpressionContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_equalityExpressions);
    jtk_Memory_deallocate(context);
}

void zen_AndExpressionContext_getChildren(zen_AndExpressionContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_equalityExpression);
    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_equalityExpressions);
}

/*******************************************************************************
 * EqualityExpressionContext                                                   *
 *******************************************************************************/

zen_EqualityExpressionContext_t* zen_EqualityExpressionContext_new(zen_ASTNode_t* node) {
    zen_EqualityExpressionContext_t* context = zen_Memory_allocate(zen_EqualityExpressionContext_t, 1);
    context->m_node = node;
    context->m_relationalExpression = NULL;
    context->m_relationalExpressions = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_EQUALITY_EXPRESSION, context,
        (zen_ContextDestructorFunction_t)zen_EqualityExpressionContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_EqualityExpressionContext_getChildren);

    return context;
}

void zen_EqualityExpressionContext_delete(zen_EqualityExpressionContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    int32_t size = jtk_ArrayList_getSize(context->m_relationalExpressions);
    int32_t i;
    for (i = 0; i < size; i++) {
        jtk_Pair_t* pair = (jtk_Pair_t*)jtk_ArrayList_getValue(context->m_relationalExpressions, i);
        jtk_Pair_delete(pair);
    }
    jtk_ArrayList_delete(context->m_relationalExpressions);
    jtk_Memory_deallocate(context);
}

void zen_EqualityExpressionContext_getChildren(zen_EqualityExpressionContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_relationalExpression);

    int32_t size = jtk_ArrayList_getSize(context->m_relationalExpressions);
    int32_t i;
    for (i = 0; i < size; i++) {
        jtk_Pair_t* pair = (jtk_Pair_t*)jtk_ArrayList_getValue(context->m_relationalExpressions, i);
        jtk_ArrayList_add(children, pair->m_left);
        jtk_ArrayList_add(children, pair->m_right);
    }
}

/*******************************************************************************
 * RelationalExpressionContext                                                 *
 *******************************************************************************/

zen_RelationalExpressionContext_t* zen_RelationalExpressionContext_new(zen_ASTNode_t* node) {
    zen_RelationalExpressionContext_t* context = zen_Memory_allocate(zen_RelationalExpressionContext_t, 1);
    context->m_node = node;
    context->m_shiftExpression = NULL;
    context->m_shiftExpressions = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_RELATIONAL_EXPRESSION, context,
        (zen_ContextDestructorFunction_t)zen_RelationalExpressionContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_RelationalExpressionContext_getChildren);

    return context;
}

void zen_RelationalExpressionContext_delete(zen_RelationalExpressionContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    int32_t size = jtk_ArrayList_getSize(context->m_shiftExpressions);
    int32_t i;
    for (i = 0; i < size; i++) {
        jtk_Pair_t* pair = (jtk_Pair_t*)jtk_ArrayList_getValue(
            context->m_shiftExpressions, i);
        jtk_Pair_delete(pair);
    }
    jtk_ArrayList_delete(context->m_shiftExpressions);
    jtk_Memory_deallocate(context);
}

void zen_RelationalExpressionContext_getChildren(zen_RelationalExpressionContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_shiftExpression);

    int32_t size = jtk_ArrayList_getSize(context->m_shiftExpressions);
    int32_t i;
    for (i = 0; i < size; i++) {
        jtk_Pair_t* pair = (jtk_Pair_t*)jtk_ArrayList_getValue(context->m_shiftExpressions, i);
        jtk_ArrayList_add(children, pair->m_left);
        jtk_ArrayList_add(children, pair->m_right);
    }
}

/*******************************************************************************
 * ShiftExpressionContext                                                      *
 *******************************************************************************/

zen_ShiftExpressionContext_t* zen_ShiftExpressionContext_new(zen_ASTNode_t* node) {
    zen_ShiftExpressionContext_t* context = zen_Memory_allocate(zen_ShiftExpressionContext_t, 1);
    context->m_node = node;
    context->m_additiveExpression = NULL;
    context->m_additiveExpressions = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_SHIFT_EXPRESSION, context,
        (zen_ContextDestructorFunction_t)zen_ShiftExpressionContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ShiftExpressionContext_getChildren);

    return context;
}

void zen_ShiftExpressionContext_delete(zen_ShiftExpressionContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    int32_t size = jtk_ArrayList_getSize(context->m_additiveExpressions);
    int32_t i;
    for (i = 0; i < size; i++) {
        jtk_Pair_t* pair = (jtk_Pair_t*)jtk_ArrayList_getValue(
            context->m_additiveExpressions, i);
        jtk_Pair_delete(pair);
    }
    jtk_ArrayList_delete(context->m_additiveExpressions);
    jtk_Memory_deallocate(context);
}

void zen_ShiftExpressionContext_getChildren(zen_ShiftExpressionContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_additiveExpression);

    int32_t size = jtk_ArrayList_getSize(context->m_additiveExpressions);
    int32_t i;
    for (i = 0; i < size; i++) {
        jtk_Pair_t* pair = (jtk_Pair_t*)jtk_ArrayList_getValue(context->m_additiveExpressions, i);
        jtk_ArrayList_add(children, pair->m_left);
        jtk_ArrayList_add(children, pair->m_right);
    }
}

/*******************************************************************************
 * AdditiveExpressionContext                                                   *
 *******************************************************************************/

zen_AdditiveExpressionContext_t* zen_AdditiveExpressionContext_new(zen_ASTNode_t* node) {
    zen_AdditiveExpressionContext_t* context = zen_Memory_allocate(zen_AdditiveExpressionContext_t, 1);
    context->m_node = node;
    context->m_multiplicativeExpression = NULL;
    context->m_multiplicativeExpressions = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_ADDITIVE_EXPRESSION, context,
        (zen_ContextDestructorFunction_t)zen_AdditiveExpressionContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_AdditiveExpressionContext_getChildren);

    return context;
}

void zen_AdditiveExpressionContext_delete(zen_AdditiveExpressionContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    int32_t size = jtk_ArrayList_getSize(context->m_multiplicativeExpressions);
    int32_t i;
    for (i = 0; i < size; i++) {
        jtk_Pair_t* pair = (jtk_Pair_t*)jtk_ArrayList_getValue(
            context->m_multiplicativeExpressions, i);
        jtk_Pair_delete(pair);
    }
    jtk_ArrayList_delete(context->m_multiplicativeExpressions);
    jtk_Memory_deallocate(context);
}

void zen_AdditiveExpressionContext_getChildren(zen_AdditiveExpressionContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_multiplicativeExpression);

    int32_t size = jtk_ArrayList_getSize(context->m_multiplicativeExpressions);
    int32_t i;
    for (i = 0; i < size; i++) {
        jtk_Pair_t* pair = (jtk_Pair_t*)jtk_ArrayList_getValue(context->m_multiplicativeExpressions, i);
        jtk_ArrayList_add(children, pair->m_left);
        jtk_ArrayList_add(children, pair->m_right);
    }
}

/*******************************************************************************
 * MultiplicativeExpressionContext                                             *
 *******************************************************************************/

zen_MultiplicativeExpressionContext_t* zen_MultiplicativeExpressionContext_new(zen_ASTNode_t* node) {
    zen_MultiplicativeExpressionContext_t* context = zen_Memory_allocate(zen_MultiplicativeExpressionContext_t, 1);
    context->m_node = node;
    context->m_unaryExpression = NULL;
    context->m_unaryExpressions = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_MULTIPLICATIVE_EXPRESSION, context,
        (zen_ContextDestructorFunction_t)zen_MultiplicativeExpressionContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_MultiplicativeExpressionContext_getChildren);

    return context;
}

void zen_MultiplicativeExpressionContext_delete(zen_MultiplicativeExpressionContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    int32_t size = jtk_ArrayList_getSize(context->m_unaryExpressions);
    int32_t i;
    for (i = 0; i < size; i++) {
        jtk_Pair_t* pair = (jtk_Pair_t*)jtk_ArrayList_getValue(context->m_unaryExpressions, i);
        jtk_Pair_delete(pair);
    }
    jtk_ArrayList_delete(context->m_unaryExpressions);
    jtk_Memory_deallocate(context);
}

void zen_MultiplicativeExpressionContext_getChildren(zen_MultiplicativeExpressionContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_unaryExpression);

    int32_t size = jtk_ArrayList_getSize(context->m_unaryExpressions);
    int32_t i;
    for (i = 0; i < size; i++) {
        jtk_Pair_t* pair = (jtk_Pair_t*)jtk_ArrayList_getValue(context->m_unaryExpressions, i);
        jtk_ArrayList_add(children, pair->m_left);
        jtk_ArrayList_add(children, pair->m_right);
    }
}

/*******************************************************************************
 * UnaryExpressionContext                                                      *
 *******************************************************************************/

zen_UnaryExpressionContext_t* zen_UnaryExpressionContext_new(zen_ASTNode_t* node) {
    zen_UnaryExpressionContext_t* context = zen_Memory_allocate(zen_UnaryExpressionContext_t, 1);
    context->m_node = node;
    context->m_unaryOperator = NULL;
    context->m_unaryExpression = NULL;
    context->m_postfixExpression = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_UNARY_EXPRESSION, context,
        (zen_ContextDestructorFunction_t)zen_UnaryExpressionContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_UnaryExpressionContext_getChildren);

    return context;
}

void zen_UnaryExpressionContext_delete(zen_UnaryExpressionContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_UnaryExpressionContext_getChildren(zen_UnaryExpressionContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    if (context->m_unaryOperator != NULL) {
        jtk_ArrayList_add(children, context->m_unaryOperator);
        jtk_ArrayList_add(children, context->m_unaryExpression);
    }
    else {
        jtk_ArrayList_add(children, context->m_postfixExpression);
    }
}

/*******************************************************************************
 * PostfixExpressionContext                                                    *
 *******************************************************************************/

zen_PostfixExpressionContext_t* zen_PostfixExpressionContext_new(zen_ASTNode_t* node) {
    zen_PostfixExpressionContext_t* context = zen_Memory_allocate(zen_PostfixExpressionContext_t, 1);
    context->m_node = node;
    context->m_primaryExpression = NULL;
    context->m_postfixParts = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_POSTFIX_EXPRESSION, context,
        (zen_ContextDestructorFunction_t)zen_PostfixExpressionContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_PostfixExpressionContext_getChildren);

    return context;
}

void zen_PostfixExpressionContext_delete(zen_PostfixExpressionContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_postfixParts);
    jtk_Memory_deallocate(context);
}

void zen_PostfixExpressionContext_getChildren(zen_PostfixExpressionContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_primaryExpression);
    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_postfixParts);
}

/*******************************************************************************
 * SubscriptContext                                                            *
 *******************************************************************************/

zen_SubscriptContext_t* zen_SubscriptContext_new(zen_ASTNode_t* node) {
    zen_SubscriptContext_t* context = zen_Memory_allocate(zen_SubscriptContext_t, 1);
    context->m_node = node;
    context->m_expression = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_SUBSCRIPT, context,
        (zen_ContextDestructorFunction_t)zen_SubscriptContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_SubscriptContext_getChildren);

    return context;
}

void zen_SubscriptContext_delete(zen_SubscriptContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_SubscriptContext_getChildren(zen_SubscriptContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_expression);
}

/*******************************************************************************
 * FunctionArgumentsContext                                                   *
 *******************************************************************************/

zen_FunctionArgumentsContext_t* zen_FunctionArgumentsContext_new(zen_ASTNode_t* node) {
    zen_FunctionArgumentsContext_t* context = zen_Memory_allocate(zen_FunctionArgumentsContext_t, 1);
    context->m_node = node;
    context->m_expressions = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_FUNCTION_ARGUMENTS, context,
        (zen_ContextDestructorFunction_t)zen_FunctionArgumentsContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_FunctionArgumentsContext_getChildren);

    return context;
}

void zen_FunctionArgumentsContext_delete(zen_FunctionArgumentsContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_FunctionArgumentsContext_getChildren(zen_FunctionArgumentsContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    if (context->m_expressions != NULL) {
        jtk_ArrayList_add(children, context->m_expressions);
    }
}

/*******************************************************************************
 * MemberAccessContext                                                         *
 *******************************************************************************/

zen_MemberAccessContext_t* zen_MemberAccessContext_new(zen_ASTNode_t* node) {
    zen_MemberAccessContext_t* context = zen_Memory_allocate(zen_MemberAccessContext_t, 1);
    context->m_node = node;
    context->m_identifier = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_MEMBER_ACCESS, context,
        (zen_ContextDestructorFunction_t)zen_MemberAccessContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_MemberAccessContext_getChildren);

    return context;
}

void zen_MemberAccessContext_delete(zen_MemberAccessContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_MemberAccessContext_getChildren(zen_MemberAccessContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_identifier);
}

/*******************************************************************************
 * PostfixOperatorContext                                                      *
 *******************************************************************************/

zen_PostfixOperatorContext_t* zen_PostfixOperatorContext_new(zen_ASTNode_t* node) {
    zen_PostfixOperatorContext_t* context = zen_Memory_allocate(zen_PostfixOperatorContext_t, 1);
    context->m_node = node;
    context->m_postfixOperator = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_POSTFIX_OPERATOR, context,
        (zen_ContextDestructorFunction_t)zen_PostfixOperatorContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_PostfixOperatorContext_getChildren);

    return context;
}

void zen_PostfixOperatorContext_delete(zen_PostfixOperatorContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Memory_deallocate(context);
}

void zen_PostfixOperatorContext_getChildren(zen_PostfixOperatorContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_postfixOperator);
}

/*******************************************************************************
 * PrimaryExpressionContext                                                    *
 *******************************************************************************/

zen_PrimaryExpressionContext_t* zen_PrimaryExpressionContext_new(zen_ASTNode_t* node) {
    zen_PrimaryExpressionContext_t* context = zen_Memory_allocate(zen_PrimaryExpressionContext_t, 1);
    context->m_node = node;
    context->m_expression = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_PRIMARY_EXPRESSION, context,
        (zen_ContextDestructorFunction_t)zen_PrimaryExpressionContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_PrimaryExpressionContext_getChildren);
    return context;
}

void zen_PrimaryExpressionContext_delete(zen_PrimaryExpressionContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Memory_deallocate(context);
}

void zen_PrimaryExpressionContext_getChildren(zen_PrimaryExpressionContext_t* context, jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_expression);
}

/*******************************************************************************
 * MapExpressionContext                                                        *
 *******************************************************************************/

zen_MapExpressionContext_t* zen_MapExpressionContext_new(zen_ASTNode_t* node) {
    zen_MapExpressionContext_t* context = zen_Memory_allocate(zen_MapExpressionContext_t, 1);
    context->m_node = node;
    context->m_mapEntries = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_MAP_EXPRESSION, context,
        (zen_ContextDestructorFunction_t)zen_MapExpressionContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_MapExpressionContext_getChildren);

    return context;
}

void zen_MapExpressionContext_delete(zen_MapExpressionContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_MapExpressionContext_getChildren(zen_MapExpressionContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_mapEntries);
}

/*******************************************************************************
 * MapEntriesContext                                                           *
 *******************************************************************************/

zen_MapEntriesContext_t* zen_MapEntriesContext_new(zen_ASTNode_t* node) {
    zen_MapEntriesContext_t* context = zen_Memory_allocate(zen_MapEntriesContext_t, 1);
    context->m_node = node;
    context->m_mapEntries = jtk_ArrayList_new();

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_MAP_ENTRIES, context,
        (zen_ContextDestructorFunction_t)zen_MapEntriesContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_MapEntriesContext_getChildren);

    return context;
}

void zen_MapEntriesContext_delete(zen_MapEntriesContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_ArrayList_delete(context->m_mapEntries);
    jtk_Memory_deallocate(context);
}

void zen_MapEntriesContext_getChildren(zen_MapEntriesContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_addAll(children, JTK_COLLECTION_ARRAY_LIST, context->m_mapEntries);
}

/*******************************************************************************
 * MapEntryContext                                                             *
 *******************************************************************************/

zen_MapEntryContext_t* zen_MapEntryContext_new(zen_ASTNode_t* node) {
    zen_MapEntryContext_t* context = zen_Memory_allocate(zen_MapEntryContext_t, 1);
    context->m_node = node;
    context->m_keyExpression = NULL;
    context->m_valueExpression = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_MAP_ENTRY, context,
        (zen_ContextDestructorFunction_t)zen_MapEntryContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_MapEntryContext_getChildren);

    return context;
}

void zen_MapEntryContext_delete(zen_MapEntryContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_MapEntryContext_getChildren(zen_MapEntryContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_keyExpression);
    jtk_ArrayList_add(children, context->m_valueExpression);
}

/*******************************************************************************
 * ListExpressionContext                                                       *
 *******************************************************************************/

zen_ListExpressionContext_t* zen_ListExpressionContext_new(zen_ASTNode_t* node) {
    zen_ListExpressionContext_t* context = zen_Memory_allocate(zen_ListExpressionContext_t, 1);
    context->m_node = node;
    context->m_expressions = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_LIST_EXPRESSION, context,
        (zen_ContextDestructorFunction_t)zen_ListExpressionContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_ListExpressionContext_getChildren);

    return context;
}

void zen_ListExpressionContext_delete(zen_ListExpressionContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_ListExpressionContext_getChildren(zen_ListExpressionContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_expressions);
}

/*******************************************************************************
 * NewExpressionContext                                                        *
 *******************************************************************************/

zen_NewExpressionContext_t* zen_NewExpressionContext_new(zen_ASTNode_t* node) {
    zen_NewExpressionContext_t* context = zen_Memory_allocate(zen_NewExpressionContext_t, 1);
    context->m_node = node;
    context->m_typeName = NULL;
    context->m_functionArguments = NULL;

    zen_Context_initializeNode(node, ZEN_AST_NODE_TYPE_NEW_EXPRESSION, context,
        (zen_ContextDestructorFunction_t)zen_NewExpressionContext_delete,
        (zen_EnumerateContextChildrenFunction_t)zen_NewExpressionContext_getChildren);

    return context;
}

void zen_NewExpressionContext_delete(zen_NewExpressionContext_t* context) {
    jtk_Assert_assertObject(context, "The specified context is null.");

    jtk_Memory_deallocate(context);
}

void zen_NewExpressionContext_getChildren(zen_NewExpressionContext_t* context,
    jtk_ArrayList_t* children) {
    jtk_Assert_assertObject(context, "The specified context is null.");
    jtk_Assert_assertObject(children, "The specified children is null.");

    jtk_ArrayList_add(children, context->m_typeName);
    if (context->m_functionArguments != NULL) {
        jtk_ArrayList_add(children, context->m_functionArguments);
    }
}