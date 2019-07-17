// Friday, November 04, 2017

#ifndef COM_ONECUBE_ZEN_COMPILER_LEXER_LEXER_ERROR_H
#define COM_ONECUBE_ZEN_COMPILER_LEXER_LEXER_ERROR_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * LexerError                                                                  *
 *******************************************************************************/
 
/**
 * @class LexerError
 * @ingroup zen_compiler_lexer
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_LexerError_t {
    char* m_message;
    char* m_path;
    int32_t m_line;
    int32_t m_column;
};

/**
 * @memberof LexerError
 */
typedef struct zen_LexerError_t zen_LexerError_t;

// Constructor

/**
 * @memberof LexerError
 */
zen_LexerError_t* zen_LexerError_new(const char* message, const char* path,
    int32_t line, int32_t column);

// Destructor

/**
 * @memberof LexerError
 */
void zen_LexerError_delete(zen_LexerError_t* error);

#endif /* COM_ONECUBE_ZEN_COMPILER_LEXER_LEXER_ERROR_H */