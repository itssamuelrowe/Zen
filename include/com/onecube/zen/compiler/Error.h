// Wedensday, October 18, 2017

#ifndef ZEN_ERROR_H
#define ZEN_ERROR_H

/*******************************************************************************
 * Error                                                                       *
 *******************************************************************************/

/**
 * Represents a syntax, semantic, or lexical error.
 *
 * @author Samuel Rowe
 * @since  zen 1.0
 */
struct zen_Error_t {
    zen_ErrorCode_t m_code;
    const uint8_t* m_message;
};

typedef struct zen_Error_t zen_Error_t;

#endif /* ZEN_ERROR_H */