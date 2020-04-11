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

// Wednesday, July 17, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ENUMERATION_DECLARATION_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ENUMERATION_DECLARATION_CONTEXT_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * EnumerationDeclarationContext                                               *
 *******************************************************************************/

/**
 * @class EnumerationDeclarationContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EnumerationDeclarationContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_enumerationBaseClause;
    zen_ASTNode_t* m_enumerationSuite;
};

/**
 * @memberof EnumerationDeclarationContext
 */
typedef struct zen_EnumerationDeclarationContext_t zen_EnumerationDeclarationContext_t;

// Constructor

/**
 * @memberof EnumerationDeclarationContext
 */
zen_EnumerationDeclarationContext_t* zen_EnumerationDeclarationContext_new(zen_ASTNode_t* node);

// Destructor

/**
 * @memberof EnumerationDeclarationContext
 */
void zen_EnumerationDeclarationContext_delete(zen_EnumerationDeclarationContext_t* context);

// Children

/**
 * @memberof EnumerationDeclarationContext
 */
void zen_EnumerationDeclarationContext_getChildren(zen_EnumerationDeclarationContext_t* context, jtk_ArrayList_t* children);

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_CONTEXT_ENUMERATION_DECLARATION_CONTEXT_H */