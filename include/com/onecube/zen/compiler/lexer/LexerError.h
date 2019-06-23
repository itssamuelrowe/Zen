// Friday, November 04, 2017

#ifndef ZEN_LEXER_ERROR_H
#define ZEN_LEXER_ERROR_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * LexerError                                                                  *
 *******************************************************************************/
 
struct zen_LexerError_t {
    char* m_message;
    char* m_path;
    int32_t m_line;
    int32_t m_column;
};

typedef struct zen_LexerError_t zen_LexerError_t;

zen_LexerError_t* zen_LexerError_new(const char* message, const char* path, int32_t line, int32_t column);
void zen_LexerError_delete(zen_LexerError_t* error);

#endif /* ZEN_LEXER_ERROR_H */