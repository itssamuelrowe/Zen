// Sunday, December 03, 2017

#include <zen/ParserError.h>
#include <zen/CString.h>

zen_ParserError_t* zen_ParserError_new() {
    zen_ParserError_t* error = zen_Memory_allocate(zen_ParserError_t, 1);
    error->m_message = NULL;
    return error;
}

void zen_ParserError_delete(zen_ParserError_t* error) {
    jtk_Assert_assertObject(error, "The specified error is null.");
    
    if (error->m_message != NULL) {
        zen_String_delete(error->m_message);
    }
    
    zen_Memory_deallocate(error);
}