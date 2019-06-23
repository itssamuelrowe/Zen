// Saturday, November 04, 2017

#ifndef ZEN_FILE_INPUT_STREAM_H
#define ZEN_FILE_INPUT_STREAM_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * InputStream                                                                 *
 *******************************************************************************/

struct zen_InputStream_t {
    FILE* m_file;
    char* m_path;
    int32_t m_available;
};

typedef struct zen_InputStream_t zen_InputStream_t;

zen_InputStream_t* zen_InputStream_new(const char* path);
void zen_InputStream_delete(zen_InputStream_t* stream);
void zen_InputStream_consume(zen_InputStream_t* stream);
int32_t zen_InputStream_isAvailable(zen_InputStream_t* stream);
int32_t zen_InputStream_read(zen_InputStream_t* stream);
uint8_t* zen_InputStream_getSourceName(zen_InputStream_t* stream, int32_t* size);


#endif /* ZEN_FILE_INPUT_STREAM_H */