/*
 * Copyright 2018-2019 OneCube
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef COM_ONECUBE_ZEN_COMPILER_SUPPORT_ERROR_CODE_H
#define COM_ONECUBE_ZEN_COMPILER_SUPPORT_ERROR_CODE_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ErrorCode                                                                   *
 *******************************************************************************/

/**
 * @class ErrorCode
 * @ingroup zen_compiler_support
 * @author Samuel Rowe
 * @since Zen 1.0
 */
enum zen_ErrorCode_t {
    ZEN_ERROR_CODE_NONE = 0,

    // Lexcial Errors

    ZEN_ERROR_CODE_UNTERMINATED_STRING_LITERAL,
    ZEN_ERROR_CODE_UNTERMINATED_MULTI_LINE_COMMENT,
    ZEN_ERROR_CODE_MALFORMED_UNICODE_CHARACTER_SEQUENCE,
    ZEN_ERROR_CODE_INVALID_ESCAPE_SEQUENCE,
    ZEN_ERROR_CODE_UNKNOWN_CHARACTER,
    ZEN_ERROR_CODE_INVALID_INTEGER_LITERAL_PREFIX,

    // Syntactical Errors

    ZEN_ERROR_CODE_NO_VIABLE_ALTERNATIVE,

    // Semantical Errors

    ZEN_ERROR_CODE_UNDECLARED_CLASS,
    ZEN_ERROR_CODE_INVALID_LVALUE,

    ZEN_ERROR_CODE_COUNT
};

typedef enum zen_ErrorCode_t zen_ErrorCode_t;

#endif /* COM_ONECUBE_ZEN_COMPILER_SUPPORT_ERROR_CODE_H */