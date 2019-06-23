#include <zen/Token.h>
#include <zen/CString.h>

zen_Token_t* zen_Token_new(
    zen_TokenChannel_t channel,
    zen_TokenType_t type,
    const uint8_t* text,
    int32_t length,
    int32_t startIndex,
    int32_t stopIndex,
    int32_t startLine,
    int32_t stopLine,
    int32_t startColumn,
    int32_t stopColumn) {
    zen_Token_t* token = zen_Memory_allocate(zen_Token_t, 1);
    token->m_channel = channel;
    token->m_type = type;
    token->m_text = zen_String_newWithSize(text, length);
    token->m_length = length; // This is the length of the text representation!
    token->m_startIndex = startIndex;
    token->m_stopIndex = stopIndex;
    token->m_startLine = startLine;
    token->m_stopLine = stopLine;
    token->m_startColumn = startColumn;
    token->m_stopColumn = stopColumn;

    return token;
}

void zen_Token_delete(zen_Token_t* token) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    zen_String_delete(token->m_text);
    zen_Memory_deallocate(token);
}

zen_TokenChannel_t zen_Token_getChannel(zen_Token_t* token) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    return token->m_channel;
}

void zen_Token_setChannel(zen_Token_t* token, zen_TokenChannel_t channel) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    token->m_channel = channel;
}

zen_TokenType_t zen_Token_getType(zen_Token_t* token) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    return token->m_type;
}

void zen_Token_setType(zen_Token_t* token, zen_TokenType_t type) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    token->m_type = type;
}


void zen_Token_setText(zen_Token_t* token, const uint8_t* text, int32_t length) {
    jtk_Assert_assertObject(token, "The specified token is null.");

    /* The text should not be null. */
    zen_String_delete(token->m_text);
    token->m_text = zen_String_newWithSize(text, length);
    token->m_length = length;
}

const uint8_t* zen_Token_getText(zen_Token_t* token) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    return token->m_text;
}


int32_t zen_Token_getLength(zen_Token_t* token) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    return token->m_length;
}


void zen_Token_setStartIndex(zen_Token_t* token, int32_t startIndex) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    token->m_startIndex = startIndex;
}

void zen_Token_setStopIndex(zen_Token_t* token, int32_t stopIndex) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    token->m_stopIndex = stopIndex;
}


void zen_Token_setStartLine(zen_Token_t* token, int32_t startLine) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    token->m_startLine = startLine;
}

int32_t zen_Token_getStartLine(zen_Token_t* token) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    return token->m_startLine;
}

void zen_Token_setStopLine(zen_Token_t* token, int32_t stopLine) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    token->m_startLine = stopLine;
}

int32_t zen_Token_getStopLine(zen_Token_t* token) {
    return token->m_stopLine;
}

void zen_Token_setStartColumn(zen_Token_t* token, int32_t startColumn) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    token->m_startColumn = startColumn;
}

int32_t zen_Token_getStartColumn(zen_Token_t* token) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    return token->m_startColumn;
}


void zen_Token_setStopColumn(zen_Token_t* token, int32_t stopColumn) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    token->m_stopColumn = stopColumn;
}

int32_t zen_Token_getStopColumn(zen_Token_t* token) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    return token->m_stopColumn;
}

void zen_Token_setIndex(zen_Token_t* token, int32_t index) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    token->m_index = index;
}

int32_t zen_Token_getIndex(zen_Token_t* token) {
    jtk_Assert_assertObject(token, "The specified token is null.");
    return token->m_index;
}
