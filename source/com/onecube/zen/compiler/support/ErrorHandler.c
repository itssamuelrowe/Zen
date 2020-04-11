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

// Monday, March 11, 2018

#include <com/onecube/zen/compiler/support/ErrorHandler.h>

/*******************************************************************************
 * ErrorHandler                                                                *
 *******************************************************************************/

// Constructor

zen_ErrorHandler_t* zen_ErrorHandler_new() {
    zen_ErrorHandler_t* handler = jtk_Memory_allocate(zen_ErrorHandler_t, 1);
    handler->m_handleLexicalError = NULL;
    handler->m_handleSyntacticalError = NULL;
    handler->m_handleSemanticalError = NULL;
    handler->m_onLexicalError = NULL;
    handler->m_onSyntacticalError = NULL;
    handler->m_onSemanticalError = NULL;
    handler->m_errors = jtk_ArrayList_new();
    handler->m_active = true;
    handler->m_context = NULL;

    return handler;
}

// Destructor

void zen_ErrorHandler_delete(zen_ErrorHandler_t* handler) {
    jtk_Assert_assertObject(handler, "The specified error handler is null.");

    int32_t errorCount = jtk_ArrayList_getSize(handler->m_errors);
    int32_t i;
    for (i = 0; i < errorCount; i++) {
        zen_Error_t* error = (zen_Error_t*)jtk_ArrayList_getValue(handler->m_errors, i);
        zen_Error_delete(error);
    }
    jtk_ArrayList_delete(handler->m_errors);
    jtk_Memory_deallocate(handler);
}

// Active

void zen_ErrorHandler_setActive(zen_ErrorHandler_t* handler, bool active) {
    jtk_Assert_assertObject(handler, "The specified error handler is null.");

    handler->m_active = active;
}

bool zen_ErrorHandler_isActive(zen_ErrorHandler_t* handler) {
    jtk_Assert_assertObject(handler, "The specified error handler is null.");

    return handler->m_active;
}

// Syntactical Error

void zen_ErrorHandler_setOnSyntacticalError(zen_ErrorHandler_t* handler,
    zen_ErrorHandler_OnSyntacticalErrorFunction_t onSyntacticalError) {
    jtk_Assert_assertObject(handler, "The specified error handler is null.");

    handler->m_onSyntacticalError = onSyntacticalError;
}

zen_ErrorHandler_OnSyntacticalErrorFunction_t zen_ErrorHandler_getOnSyntacticalError(
    zen_ErrorHandler_t* handler) {
    jtk_Assert_assertObject(handler, "The specified error handler is null.");

    return handler->m_onSemanticalError;
}

void zen_ErrorHandler_handleSyntacticalError(zen_ErrorHandler_t* handler,
    zen_Parser_t* parser, zen_ErrorCode_t errorCode, zen_Token_t* token,
    zen_TokenType_t expected) {
    jtk_Assert_assertObject(handler, "The specified error handler is null.");

    zen_Error_t* error = zen_Error_newEx(errorCode, token, expected);
    jtk_ArrayList_add(handler->m_errors, error);

    if (handler->m_handleSyntacticalError != NULL) {
        handler->m_handleSyntacticalError(handler->m_context, parser, error);
    }

    if (handler->m_active && (handler->m_onSyntacticalError != NULL)) {
        handler->m_onSyntacticalError(handler->m_context, parser, error, expected);
    }
}

// Lexical Error

void zen_ErrorHandler_setOnLexicalError(zen_ErrorHandler_t* handler,
    zen_ErrorHandler_OnLexicalErrorFunction_t onLexicalError) {
    jtk_Assert_assertObject(handler, "The specified error handler is null.");

    handler->m_onLexicalError = onLexicalError;
}

zen_ErrorHandler_OnLexicalErrorFunction_t zen_ErrorHandler_getOnLexicalError(
    zen_ErrorHandler_t* handler) {
    jtk_Assert_assertObject(handler, "The specified error handler is null.");

    return handler->m_onLexicalError;
}

void zen_ErrorHandler_handleLexicalError(zen_ErrorHandler_t* handler,
    zen_Lexer_t* lexer, zen_ErrorCode_t errorCode, zen_Token_t* token) {
    jtk_Assert_assertObject(handler, "The specified error handler is null.");

    zen_Error_t* error = zen_Error_new(errorCode, token);
    jtk_ArrayList_add(handler->m_errors, error);

    if (handler->m_handleLexicalError != NULL) {
        handler->m_handleLexicalError(handler->m_context, lexer, error);
    }

    if (handler->m_active && (handler->m_onLexicalError != NULL)) {
        handler->m_onLexicalError(handler->m_context, lexer, error);
    }
}

// Semantic Error

void zen_ErrorHandler_setOnSemanticalError(zen_ErrorHandler_t* handler,
    zen_ErrorHandler_OnSemanticalErrorFunction_t onSemanticalError) {
    jtk_Assert_assertObject(handler, "The specified error handler is null.");

    handler->m_onSemanticalError = onSemanticalError;
}

zen_ErrorHandler_OnSemanticalErrorFunction_t zen_ErrorHandler_getOnSemanticalError(
    zen_ErrorHandler_t* handler){
    jtk_Assert_assertObject(handler, "The specified error handler is null.");

    return handler->m_onSemanticalError;
}

void zen_ErrorHandler_handleSemanticalError(zen_ErrorHandler_t* handler,
    void* origin, zen_ErrorCode_t errorCode, zen_Token_t* token) {
    jtk_Assert_assertObject(handler, "The specified error handler is null.");

    zen_Error_t* error = zen_Error_new(errorCode, token);
    jtk_ArrayList_add(handler->m_errors, error);

    if (handler->m_handleSemanticalError != NULL) {
        handler->m_handleSemanticalError(handler->m_context, origin, error);
    }

    if (handler->m_active && (handler->m_onSemanticalError != NULL)) {
        handler->m_onSemanticalError(handler->m_context, origin, error);
    }
}

void zen_ErrorHandler_handleGeneralError(zen_ErrorHandler_t* handler,
    void* origin, zen_ErrorCode_t errorCode) {
    jtk_Assert_assertObject(handler, "The specified error handler is null.");

    zen_Error_t* error = zen_Error_new(errorCode, NULL);
    jtk_ArrayList_add(handler->m_errors, error);

    // if (handler->m_handleGeneralError != NULL) {
    //     handler->m_handleGeneralError(handler->m_context, origin, error);
    // }

    if (handler->m_active && (handler->m_onGeneralError != NULL)) {
        handler->m_onGeneralError(handler->m_context, origin, error);
    }
}

// Errors

jtk_ArrayList_t* zen_ErrorHandler_getErrors(zen_ErrorHandler_t* handler) {
    jtk_Assert_assertObject(handler, "The specified error handler is null.");

    return handler->m_errors;
}

int32_t zen_ErrorHandler_getErrorCount(zen_ErrorHandler_t* handler) {
    jtk_Assert_assertObject(handler, "The specified error handler is null.");

    return jtk_ArrayList_getSize(handler->m_errors);
}

bool zen_ErrorHandler_hasErrors(zen_ErrorHandler_t* handler) {
    jtk_Assert_assertObject(handler, "The specified error handler is null.");

    return !jtk_ArrayList_isEmpty(handler->m_errors);
}