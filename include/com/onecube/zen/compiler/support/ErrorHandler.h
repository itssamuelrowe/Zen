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

// Monday, March 12, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SUPPORT_ERROR_HANDLER_H
#define COM_ONECUBE_ZEN_COMPILER_SUPPORT_ERROR_HANDLER_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/support/Error.h>
#include <com/onecube/zen/compiler/support/ErrorCode.h>

/*******************************************************************************
 * ErrorHandler                                                                *
 *******************************************************************************/

struct zen_Lexer_t;
struct zen_Parser_t;
typedef struct zen_Lexer_t zen_Lexer_t;
typedef struct zen_Parser_t zen_Parser_t;

/* NOTE: The handlers should not maintain any reference to the origin
 * object that they receive. Because errors may persist beyond the lifetime
 * of an origin object.
 */

/**
 * @memberof ErrorHandler
 */
typedef void (*zen_ErrorHandler_HandleLexicalErrorFunction_t)(void* context, zen_Lexer_t* lexer, zen_Error_t* error);

/**
 * @memberof ErrorHandler
 */
typedef void (*zen_ErrorHandler_HandleSyntacticalErrorFunction_t)(void* context, zen_Parser_t* parser, zen_Error_t* error);

/**
 * @memberof ErrorHandler
 */
typedef void (*zen_ErrorHandler_HandleSemanticalErrorFunction_t)(void* context, void* origin, zen_Error_t* error);

/**
 * @memberof ErrorHandler
 */
typedef void (*zen_ErrorHandler_OnLexicalErrorFunction_t)(void* context, zen_Lexer_t* lexer, zen_Error_t* error);

/**
 * @memberof ErrorHandler
 */
typedef void (*zen_ErrorHandler_OnSyntacticalErrorFunction_t)(void* context, zen_Parser_t* parser, zen_Error_t* error, zen_TokenType_t expected);

/**
 * @memberof ErrorHandler
 */
typedef void (*zen_ErrorHandler_OnSemanticalErrorFunction_t)(void* context, void* origin, zen_Error_t* error);

/**
 * @memberof ErrorHandler
 */
typedef void (*zen_ErrorHandler_OnGeneralErrorFunction_t)(void* context, void* origin, zen_Error_t* error);

/**
 * @class ErrorHandler
 * @author Samuel Rowe
 * @ingroup zen_compiler
 * @since zen 1.0
 */
struct zen_ErrorHandler_t {
    zen_ErrorHandler_HandleLexicalErrorFunction_t m_handleLexicalError;
    zen_ErrorHandler_HandleSyntacticalErrorFunction_t m_handleSyntacticalError;
    zen_ErrorHandler_HandleSemanticalErrorFunction_t m_handleSemanticalError;

    zen_ErrorHandler_OnLexicalErrorFunction_t m_onLexicalError;
    zen_ErrorHandler_OnSyntacticalErrorFunction_t m_onSyntacticalError;
    zen_ErrorHandler_OnSemanticalErrorFunction_t m_onSemanticalError;
    zen_ErrorHandler_OnGeneralErrorFunction_t m_onGeneralError;

    jtk_ArrayList_t* m_errors;
    bool m_active;
    void* m_context;
};

/**
 * @memberof ErrorHandler
 */
typedef struct zen_ErrorHandler_t zen_ErrorHandler_t;

// Constructor

/**
 * @memberof ErrorHandler
 */
zen_ErrorHandler_t* zen_ErrorHandler_new();

// Destructor

/**
 * @memberof ErrorHandler
 */
void zen_ErrorHandler_delete(zen_ErrorHandler_t* handler);

// Active

/**
 * @memberof ErrorHandler
 */
void zen_ErrorHandler_setActive(zen_ErrorHandler_t* handler, bool active);

/**
 * @memberof ErrorHandler
 */
bool zen_ErrorHandler_isActive(zen_ErrorHandler_t* handler);

// Syntactical Error

/**
 * @memberof ErrorHandler
 */
void zen_ErrorHandler_setOnSyntacticalError(zen_ErrorHandler_t* handler,
    zen_ErrorHandler_OnSyntacticalErrorFunction_t onSyntacticalError);

/**
 * @memberof ErrorHandler
 */
zen_ErrorHandler_OnSyntacticalErrorFunction_t zen_ErrorHandler_getOnSyntacticalError(
    zen_ErrorHandler_t* handler);

/**
 * @memberof ErrorHandler
 */
void zen_ErrorHandler_handleSyntacticalError(zen_ErrorHandler_t* handler,
    zen_Parser_t* parser, zen_ErrorCode_t errorCode, zen_Token_t* token,
    zen_TokenType_t expected);

// Lexical Error

/**
 * @memberof ErrorHandler
 */
void zen_ErrorHandler_setOnLexicalError(zen_ErrorHandler_t* handler,
    zen_ErrorHandler_OnLexicalErrorFunction_t onLexicalError);

/**
 * @memberof ErrorHandler
 */
zen_ErrorHandler_OnLexicalErrorFunction_t zen_ErrorHandler_getOnLexicalError(
    zen_ErrorHandler_t* handler);

/**
 * @memberof ErrorHandler
 */
void zen_ErrorHandler_handleLexicalError(zen_ErrorHandler_t* handler,
    zen_Lexer_t* lexer, zen_ErrorCode_t errorCode, zen_Token_t* token);

// Semantic Error

/**
 * @memberof ErrorHandler
 */
void zen_ErrorHandler_setOnSemanticalError(zen_ErrorHandler_t* handler,
    zen_ErrorHandler_OnSemanticalErrorFunction_t onSemanticalError);

/**
 * @memberof ErrorHandler
 */
zen_ErrorHandler_OnSemanticalErrorFunction_t zen_ErrorHandler_getOnSemanticalError(
    zen_ErrorHandler_t* handler);

/**
 * @memberof ErrorHandler
 */
void zen_ErrorHandler_handleSemanticalError(zen_ErrorHandler_t* handler,
    void* origin, zen_ErrorCode_t errorCode, zen_Token_t* token);

/**
 * @memberof ErrorHandler
 */
void zen_ErrorHandler_handleGeneralError(zen_ErrorHandler_t* handler,
    void* origin, zen_ErrorCode_t errorCode);

// Errors

/**
 * @memberof ErrorHandler
 */
jtk_ArrayList_t* zen_ErrorHandler_getErrors(zen_ErrorHandler_t* handler);

int32_t zen_ErrorHandler_getErrorCount(zen_ErrorHandler_t* handler);

bool zen_ErrorHandler_hasErrors(zen_ErrorHandler_t* handler);

#endif /* COM_ONECUBE_ZEN_COMPILER_SUPPORT_ERROR_HANDLER_H */