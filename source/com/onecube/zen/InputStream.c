// Saturday, November 04, 2017

#include <string.h>
#include <zen/InputStream.h>

/*******************************************************************************
 * InputStream                                                                 *
 *******************************************************************************/

zen_InputStream_t* zen_InputStream_new(const char* path) {
    zen_InputStream_t* stream = zen_Memory_allocate(zen_InputStream_t, 1);
    stream->m_file = fopen(path, "r");
    stream->m_path = strdup(path);
    stream->m_available = 1;
    return stream;
}

void zen_InputStream_delete(zen_InputStream_t* stream) {
    jtk_Assert_assertObject(stream, "The specified input stream is null.");
    free(stream->m_path);
    fclose(stream->m_file);
    zen_Memory_deallocate(stream);
}

int32_t zen_InputStream_read(zen_InputStream_t* stream) {
    jtk_Assert_assertObject(stream, "The specified input stream is null.");
    if (stream->m_available != 0) {
        char c = fgetc(stream->m_file);
        if (c == EOF) {
            stream->m_available = 0;
        }
        return c;
    }
    return 0;
}

int32_t zen_InputStream_isAvailable(zen_InputStream_t* stream) {
    jtk_Assert_assertObject(stream, "The specified input stream is null.");
    return stream->m_available;
}