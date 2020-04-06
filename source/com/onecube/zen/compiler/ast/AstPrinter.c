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

// Sunday, March 11, 2018

#include <com/onecube/zen/compiler/ast/ASTPrinter.h>
#include <com/onecube/zen/compiler/parser/Parser.h>

zen_ASTPrinter_t* zen_ASTPrinter_new() {
    zen_ASTPrinter_t* listener = zen_Memory_allocate(zen_ASTPrinter_t, 1);
    listener->m_astListener = zen_ASTListener_newWithContext(listener);
    listener->m_depth = 0;

    zen_ASTListener_t* astListener = listener->m_astListener;
    astListener->m_onEnterEveryRule = zen_ASTPrinter_onEnterEveryRule;
    astListener->m_onExitEveryRule = zen_ASTPrinter_onExitEveryRule;
    astListener->m_onVisitTerminal = zen_ASTPrinter_onVisitTerminal;

    // TODO: Initialize astListener

    return listener;
}

void zen_ASTPrinter_delete(zen_ASTPrinter_t* listener) {
    jtk_Assert_assertObject(listener, "The specified listener is null.");

    zen_ASTListener_delete(listener->m_astListener);
    jtk_Memory_deallocate(listener);
}

zen_ASTListener_t* zen_ASTPrinter_getASTListener(zen_ASTPrinter_t* listener) {
    jtk_Assert_assertObject(listener, "The specified listener is null.");
    return listener->m_astListener;
}

void zen_ASTPrinter_indent(int32_t depth) {
    int32_t i;
    for (i = 0; i < depth; i++) {
        printf("    ");
    }
}

void zen_ASTPrinter_onVisitTerminal(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_ASTPrinter_t* listener = (zen_ASTPrinter_t*)astListener->m_context;

    listener->m_depth++;
    zen_ASTPrinter_indent(listener->m_depth);
    printf("<%s>\n", zen_Token_getText((zen_Token_t*)node->m_context));
    listener->m_depth--;
}

void zen_ASTPrinter_onEnterEveryRule(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_ASTPrinter_t* listener = (zen_ASTPrinter_t*)astListener->m_context;

    listener->m_depth++;
    zen_ASTPrinter_indent(listener->m_depth);
    printf("%s\n", zen_Parser_getRuleName(node->m_type));
}

void zen_ASTPrinter_onExitEveryRule(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_ASTPrinter_t* listener = (zen_ASTPrinter_t*)astListener->m_context;
    listener->m_depth--;
}
