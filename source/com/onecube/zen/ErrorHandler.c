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

// Monday, March 11, 2018

#include <com/onecube/zen/ErrorHandler.h>

struct zen_ErrorHandler_t {
    void* m_context;
    zen_ErrorHandler_OnSyntaxErrorFunction_t m_onSyntaxError;
    zen_ErrorHandler_OnLexicalErrorFunction_t m_onLexicalError;
    zen_ErrorHandler_OnSemanticErrorFunction_t m_onSemanticError;
    bool m_active;
    jtk_ArrayList_t* m_syntaxErrors;
    jtk_ArrayList_t* m_lexicalErrors;
    jtk_ArrayList_t* m_semanticErrors;
};

zen_ErrorHandler_t* zen_ErrorHandler_new(void* context) {
    zen_ErrorHandler_t* handler = zen_Memory_allocate(zen_ErrorHandler_t, 1);
    handler->m_context = context;
    handler->m_onSyntaxError = NULL;
    handler->m_onSemanticError = NULL;
    handler->m_onLexicalError = NULL;
    handler->m_active = false;
    handler->m_syntaxErrors = jtk_ArrayList_newWithCapacity(ZEN_ERROR_HANDLER_PREFERED_LIST_CAPACITY);
    handler->m_lexicalErrors = jtk_ArrayList_newWithCapacity(ZEN_ERROR_HANDLER_PREFERED_LIST_CAPACITY);
    handler->m_semanticErrors = jtk_ArrayList_newWithCapacity(ZEN_ERROR_HANDLER_PREFERED_LIST_CAPACITY);

    return handler;
}

void zen_ErrorHandler_delete(zen_ErrorHandler_t* handler);
void zen_ErrorHandler_setOnSyntaxError(zen_ErrorHandler_t* handler, zen_ErrorHandler_OnSyntaxErrorFunction_t m_onSyntaxError);
zen_ErrorHandler_OnSyntaxErrorFunction_t zen_ErrorHandler_getOnSyntaxError(zen_ErrorHandler_t* handler);
void zen_ErrorHandler_setOnLexicalError(zen_ErrorHandler_t* handler, zen_ErrorHandler_OnLexicalErrorFunction_t m_onLexicalError);
zen_ErrorHandler_OnLexicalErrorFunction_t zen_ErrorHandler_getOnLexicalError(zen_ErrorHandler_t* handler);
void zen_ErrorHandler_setOnSemanticError(zen_ErrorHandler_t* handler, zen_ErrorHandler_OnSemanticErrorFunction_t m_onSemanticError);
zen_ErrorHandler_OnSemanticErrorFunction_t zen_ErrorHandler_getOnSemanticError(zen_ErrorHandler_t* handler);
void zen_ErrorHandler_setActive(zen_ErrorHandler_t* handler, bool active);
bool zen_ErrorHandler_isActive(zen_ErrorHandler_t* handler);
void zen_ErrorHandler_reportLexicalError(zen_ErrorHandler_t* handler, const char* message, zen_ErrorCode_t code, zen_Token_t* offendingToken);
void zen_ErrorHandler_reportSyntaxError(zen_ErrorHandler_t* handler, const char* message, zen_ErrorCode_t code, zen_Token_t* offendingToken);
void zen_ErrorHandler_reportSemanticError(zen_ErrorHandler_t* handler, , const char* message, zen_ErrorCode_t code, zen_Token_t* offendingToken);
jtk_ArrayList_t* zen_ErrorHandler_getLexicalErrors(zen_ErrorHandler_t* handler);
jtk_ArrayList_t* zen_ErrorHandler_getSyntaxErrors(zen_ErrorHandler_t* handler);
jtk_ArrayList_t* zen_ErrorHandler_getSemanticErrors(zen_ErrorHandler_t* handler);
void zen_ErrorHandler_notifyAllErrorListeners(zen_ErrorHandler_t* handler);
void zen_ErrorHandler_notifyLexicalErrorListeners(zen_ErrorHandler_t* handler);
void zen_ErrorHandler_notifySemanticErrorListeners(zen_ErrorHandler_t* handler);
void zen_ErrorHandler_notifySyntaxErrorListeners(zen_ErrorHandler_t* handler);
