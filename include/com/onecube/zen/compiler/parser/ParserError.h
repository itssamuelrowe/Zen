// Sunday, December 03, 2017

#ifndef COM_ONECUBE_ZEN_COMPILER_PARSER_PARSER_ERROR_H
#define COM_ONECUBE_ZEN_COMPILER_PARSER_PARSER_ERROR_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ParserError                                                                 *
 *******************************************************************************/

/**
 * @class ParserError
 * @ingroup zen_compiler_parser
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ParserError_t {
    char* m_message;
};

/**
 * @memberof ParserError
 */
typedef struct zen_ParserError_t zen_ParserError_t;

// Constructor

/**
 * @memberof ParserError
 */
zen_ParserError_t* zen_ParserError_new();

// Destructor

/**
 * @memberof ParserError
 */
void zen_ParserError_delete(zen_ParserError_t* error);

#endif /* COM_ONECUBE_ZEN_COMPILER_PARSER_PARSER_ERROR_H */