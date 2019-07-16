#include <string.h>
#include <com/onecube/zen/LexerError.h>

zen_LexerError_t* zen_LexerError_new(const char* message, const char* path, int32_t line, int32_t column) {
    zen_LexerError_t* error = zen_Memory_allocate(zen_LexerError_t, 1);
    error->m_message = (message == NULL)? strdup("No error message") : strdup(message);
    error->m_path = strdup(path);
    error->m_line = line;
    error->m_column = column;
    
    return error;
}

void zen_LexerError_delete(zen_LexerError_t* error) {
    jtk_Assert_assertObject(error, "The specified lexer error is null.");
    free(error->m_message);
    zen_Memory_deallocate(error);
}