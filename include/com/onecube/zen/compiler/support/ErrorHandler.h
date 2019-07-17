// Monday, March 12, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SUPPORT_ERROR_HANDLER_H
#define COM_ONECUBE_ZEN_COMPILER_SUPPORT_ERROR_HANDLER_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ErrorHandler                                                                *
 *******************************************************************************/

/**
 * @memberof ErrorHandler
 */
typedef void (*zen_ErrorHandler_OnSyntaxErrorFunction_t)(void* context, zen_Error_t* error);

/**
 * @memberof ErrorHandler
 */
typedef void (*zen_ErrorHandler_OnLexicalErrorFunction_t)(void* context, zen_Error_t* error);
 
/**
 * @class ErrorHandler
 * @author Samuel Rowe
 * @ingroup zen_compiler
 * @since zen 1.0
 */
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

// Syntax Error

/**
 * @memberof ErrorHandler
 */
void zen_ErrorHandler_setOnSyntaxError(zen_ErrorHandler_t* handler, zen_ErrorHandler_OnSyntaxErrorFunction_t m_onSyntaxError);

/**
 * @memberof ErrorHandler
 */
zen_ErrorHandler_OnSyntaxErrorFunction_t zen_ErrorHandler_getOnSyntaxError(zen_ErrorHandler_t* handler);

/**
 * @memberof ErrorHandler
 */
jtk_ArrayList_t* zen_ErrorHandler_getSyntaxErrors(zen_ErrorHandler_t* handler);

/**
 * @memberof ErrorHandler
 */
void zen_ErrorHandler_notifySyntaxErrorListeners(zen_ErrorHandler_t* handler);

/**
 * @memberof ErrorHandler
 */
void zen_ErrorHandler_reportSyntaxError(zen_ErrorHandler_t* handler, const char* message, zen_ErrorCode_t code, zen_Token_t* offendingToken);

// Lexical Error

/**
 * @memberof ErrorHandler
 */
void zen_ErrorHandler_setOnLexicalError(zen_ErrorHandler_t* handler, zen_ErrorHandler_OnLexicalErrorFunction_t m_onLexicalError);

/**
 * @memberof ErrorHandler
 */
zen_ErrorHandler_OnLexicalErrorFunction_t zen_ErrorHandler_getOnLexicalError(zen_ErrorHandler_t* handler);

/**
 * @memberof ErrorHandler
 */
void zen_ErrorHandler_notifyLexicalErrorListeners(zen_ErrorHandler_t* handler);

/**
 * @memberof ErrorHandler
 */
void zen_ErrorHandler_reportLexicalError(zen_ErrorHandler_t* handler, const char* message, zen_ErrorCode_t code, zen_Token_t* offendingToken);

/**
 * @memberof ErrorHandler
 */
jtk_ArrayList_t* zen_ErrorHandler_getLexicalErrors(zen_ErrorHandler_t* handler);

// Semantic Error

/**
 * @memberof ErrorHandler
 */
void zen_ErrorHandler_setOnSemanticError(zen_ErrorHandler_t* handler, zen_ErrorHandler_OnSemanticErrorFunction_t m_onSemanticError);

/**
 * @memberof ErrorHandler
 */
zen_ErrorHandler_OnSemanticErrorFunction_t zen_ErrorHandler_getOnSemanticError(zen_ErrorHandler_t* handler);

/**
 * @memberof ErrorHandler
 */
void zen_ErrorHandler_notifySemanticErrorListeners(zen_ErrorHandler_t* handler);

/**
 * @memberof ErrorHandler
 */
void zen_ErrorHandler_reportSemanticError(zen_ErrorHandler_t* handler, , const char* message, zen_ErrorCode_t code, zen_Token_t* offendingToken);

/**
 * @memberof ErrorHandler
 */
jtk_ArrayList_t* zen_ErrorHandler_getSemanticErrors(zen_ErrorHandler_t* handler);

// Notify

/**
 * @memberof ErrorHandler
 */
void zen_ErrorHandler_notifyAllErrorListeners(zen_ErrorHandler_t* handler);

#endif /* COM_ONECUBE_ZEN_COMPILER_SUPPORT_ERROR_HANDLER_H */