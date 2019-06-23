// Sunday, December 03, 2017

#ifndef ZEN_PARSER_ERROR_H
#define ZEN_PARSER_ERROR_H

/*******************************************************************************
 * ParserError                                                                 *
 *******************************************************************************/

struct zen_ParserError_t {
    char* m_message;
};

typedef struct zen_ParserError_t zen_ParserError_t;

zen_ParserError_t* zen_ParserError_new();
void zen_ParserError_delete(zen_ParserError_t* error);

#endif /* ZEN_PARSER_ERROR_H */