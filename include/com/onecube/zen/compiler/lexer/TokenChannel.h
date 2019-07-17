// Monday, November 27, 2017

#ifndef COM_ONECUBE_ZEN_COMPILER_LEXER_TOKEN_CHANNEL_H
#define COM_ONECUBE_ZEN_COMPILER_LEXER_TOKEN_CHANNEL_H

/*******************************************************************************
 * TokenChannel                                                                *
 *******************************************************************************/

/**
 * @class TokenChannel
 * @ingroup zen_compiler_lexer
 * @author Samuel Rowe
 * @since zen 1.0
 */
enum zen_TokenChannel_t {
    ZEN_TOKEN_CHANNEL_DEFAULT,
    ZEN_TOKEN_CHANNEL_HIDDEN,
};

/**
 * @memberof TokenChannel
 */
typedef enum zen_TokenChannel_t zen_TokenChannel_t;

#endif /* COM_ONECUBE_ZEN_COMPILER_LEXER_TOKEN_CHANNEL_H */