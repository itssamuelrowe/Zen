// Monday, March 12, 2018

#ifndef ZEN_ERROR_HANDLER_H
#define ZEN_ERROR_HANDLER_H

/*******************************************************************************
 * ErrorHandler                                                                *
 *******************************************************************************/

typedef void (*zen_ErrorHandler_OnSyntaxErrorFunction_t)(void* context, zen_Error_t* error);
typedef void (*zen_ErrorHandler_OnLexicalErrorFunction_t)(void* context, zen_Error_t* error);
 
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

typedef struct zen_ErrorHandler_t zen_ErrorHandler_t;

zen_ErrorHandler_t* zen_ErrorHandler_new();
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

#endif /* ZEN_ERROR_HANDLER_H */