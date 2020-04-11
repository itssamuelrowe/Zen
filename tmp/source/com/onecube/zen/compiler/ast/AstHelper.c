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

// Saturday, April 28, 2018

#include <jtk/collection/stack/LinkedStack.h>

#include <com/onecube/zen/compiler/lexer/Token.h>
#include <com/onecube/zen/compiler/ast/ASTHelper.h>

/*******************************************************************************
 * ASTHelper                                                                   *
 *******************************************************************************/

bool zen_ASTHelper_isAncestor(zen_ASTNode_t* node, zen_ASTNode_t* subject) {
    jtk_Assert_assertObject(node, "The specified node is null.");
    jtk_Assert_assertObject(subject, "The specified subject is null.");

    zen_ASTNode_t* parent = zen_ASTNode_getParent(subject);
    bool result = false;
    while (parent != NULL) {
        if (parent == node) {
            result = true;
            break;
        }
        parent = zen_ASTNode_getParent(parent);
    }
    return result;
}

zen_ASTNode_t* zen_ASTHelper_getAncestor(zen_ASTNode_t* node, zen_ASTNodeType_t type) {
    jtk_Assert_assertObject(node, "The specified node is null.");

    zen_ASTNode_t* parent = zen_ASTNode_getParent(node);
    zen_ASTNode_t* result = NULL;
    while (parent != NULL) {
        if (zen_ASTNode_getType(parent) == type) {
            result = parent;
            break;
        }
        parent = zen_ASTNode_getParent(parent);
    }
    return result;
}

bool zen_ASTHelper_isDescendant(zen_ASTNode_t* node, zen_ASTNode_t* subject) {
    return zen_ASTHelper_isAncestor(subject, node);
}

void zen_ASTHelper_getTokens(zen_ASTNode_t* node, jtk_ArrayList_t* list) {
    zen_ASTHelper_getNodes(node, list, -1, true, true);
}

void zen_ASTHelper_getFilteredTokens(zen_ASTNode_t* node, jtk_ArrayList_t* list, zen_TokenType_t type) {
    zen_ASTHelper_getNodes(node, list, (int32_t)type, true, true);
}

void zen_ASTHelper_getTerminalNodes(zen_ASTNode_t* node, jtk_ArrayList_t* list) {
    zen_ASTHelper_getNodes(node, list, -1, true, false);
}

void zen_ASTHelper_getFilteredTerminalNodes(zen_ASTNode_t* node, jtk_ArrayList_t* list, zen_TokenType_t type) {
    zen_ASTHelper_getNodes(node, list, (int32_t)type, true, false);
}

void zen_ASTHelper_getNodes(zen_ASTNode_t* node, jtk_ArrayList_t* list,
    int32_t filter, bool captureTerminals, bool strip) {
    jtk_LinkedStack_t* stack = jtk_LinkedStack_new();
    jtk_LinkedStack_push(stack, node);

    while (!jtk_LinkedStack_isEmpty(stack)) {
        zen_ASTNode_t* currentNode = (zen_ASTNode_t*)jtk_LinkedStack_pop(stack);

        if (zen_ASTNode_isTerminal(currentNode)) {
            if (captureTerminals) {
                zen_Token_t* token = (zen_Token_t*)currentNode->m_context;

                if ((filter >= 0) && (zen_Token_getType(token) != (zen_TokenType_t)filter)) {
                    continue;
                }

                if (strip) {
                    jtk_ArrayList_add(list, token);
                }
                else {
                    jtk_ArrayList_add(list, currentNode);
                }
            }
        }
        else if (zen_ASTNode_isRule(currentNode)) {
            if (!captureTerminals) {
                if ((filter >= 0) && (zen_ASTNode_getType(currentNode) != (zen_ASTNodeType_t)filter)) {
                    continue;
                }

                if (strip) {
                    void* context = currentNode->m_context;
                    jtk_ArrayList_add(list, context);
                }
                else {
                    jtk_ArrayList_add(list, currentNode);
                }
            }

            jtk_ArrayList_t* children = zen_ASTNode_getChildren(currentNode);
            int32_t size = jtk_ArrayList_getSize(children);
            int32_t i;
            for (i = 0; i < size; i++) {
                zen_ASTNode_t* child = (zen_ASTNode_t*)jtk_ArrayList_getValue(children, i);
                jtk_LinkedStack_push(stack, child);
            }
        }
        // What happens to erroneous nodes?
    }
}