// Wedensday, October 18, 2017

#ifndef ZEN_TOKEN_H
#define ZEN_TOKEN_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/TokenType.h>
#include <com/onecube/zen/TokenChannel.h>

/*******************************************************************************
 * Token                                                                       *
 *******************************************************************************/

/**
 * A token represents the smallest entity that appears
 * in a source code. Each token has two primary attributes:
 * a token type (symbol category) and the text associated
 * with it.
 *
 * @author Samuel Rowe
 * @since  zen 1.0
 */
struct zen_Token_t {
    zen_TokenChannel_t m_channel;
    zen_TokenType_t m_type;
    uint8_t* m_text;
    int32_t m_length;
    int32_t m_startIndex;
    int32_t m_stopIndex;
    int32_t m_startLine;
    int32_t m_stopLine;
    int32_t m_startColumn;
    int32_t m_stopColumn;
    int32_t m_index;
};

typedef struct zen_Token_t zen_Token_t;

zen_Token_t* zen_Token_new(zen_TokenChannel_t channel, zen_TokenType_t type, const uint8_t* text, int32_t length, int32_t startIndex, int32_t stopIndex, int32_t startLine, int32_t stopLine, int32_t startColumn, int32_t stopColumn);
void zen_Token_delete(zen_Token_t* token);

zen_TokenChannel_t zen_Token_getChannel(zen_Token_t* token);
void zen_Token_setChannel(zen_Token_t* token, zen_TokenChannel_t channel);

zen_TokenType_t zen_Token_getType(zen_Token_t* token);
void zen_Token_setType(zen_Token_t* token, zen_TokenType_t type);

void zen_Token_setText(zen_Token_t* token, const uint8_t* text, int32_t length);
const uint8_t* zen_Token_getText(zen_Token_t* token);

int32_t zen_Token_getLength(zen_Token_t* token);

void zen_Token_setStartIndex(zen_Token_t* token, int32_t startIndex);
void zen_Token_setStopIndex(zen_Token_t* token, int32_t stopIndex);
    
void zen_Token_setStartLine(zen_Token_t* token, int32_t startLine);
int32_t zen_Token_getStartLine(zen_Token_t* token);

void zen_Token_setStopLine(zen_Token_t* token, int32_t stopLine);
int32_t zen_Token_getStopLine(zen_Token_t* token);

void zen_Token_setStartColumn(zen_Token_t* token, int32_t startColumn);
int32_t zen_Token_getStartColumn(zen_Token_t* token);

void zen_Token_setStopColumn(zen_Token_t* token, int32_t stopColumn);
int32_t zen_Token_getStopColumn(zen_Token_t* token);

int32_t zen_Token_getIndex(zen_Token_t* token);
void zen_Token_setIndex(zen_Token_t* token, int32_t index);

#endif /* ZEN_TOKEN_H */