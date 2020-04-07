/*
 * Copyright 2017-2020 Samuel Rowe
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
    ZEN_ERROR_CODE_EXPECTED_DIGIT_AFTER_UNDERSCORE,

    // Syntactical Errors

    ZEN_ERROR_CODE_UNEXPECTED_TOKEN,
    ZEN_ERROR_CODE_TRY_STATEMENT_EXPECTS_CATCH_OR_FINALLY,

    // Semantical Errors

    ZEN_ERROR_CODE_UNDECLARED_CLASS,
    ZEN_ERROR_CODE_INVALID_LVALUE,
    ZEN_ERROR_CODE_INSTANTIATION_OF_NON_CLASS_SYMBOL,
    ZEN_ERROR_CODE_NO_SUITABLE_CONSTRUCTOR,
    ZEN_ERROR_CODE_UNDECLARED_IDENTIFIER,
    ZEN_ERROR_CODE_VARIABLE_TREATED_AS_FUNCTION,
    ZEN_ERROR_CODE_STATIC_INITIALIZER_WITH_PARAMETERS,
    ZEN_ERROR_CODE_REDECLARATION_OF_SYMBOL_AS_FUNCTION,
    ZEN_ERROR_CODE_REDECLARATION_OF_SYMBOL_AS_PARAMETER,
    ZEN_ERROR_CODE_REDECLARATION_OF_SYMBOL_AS_VARIABLE_PARAMETER,
    ZEN_ERROR_CODE_MULTIPLE_FUNCTION_OVERLOADS_WITH_VARIABLE_PARAMETER,
    ZEN_ERROR_CODE_DUPLICATE_FUNCTION_OVERLOAD,
    ZEN_ERROR_CODE_FUNCTION_DECLARATION_EXCEEDS_PARAMETER_THRESHOLD,
    ZEN_ERROR_CODE_REDECLARATION_OF_SYMBOL_AS_VARIABLE,
    ZEN_ERROR_CODE_REDECLARATION_OF_SYMBOL_AS_CONSTANT,
    ZEN_ERROR_CODE_REDECLARATION_OF_SYMBOL_AS_LABEL,
    ZEN_ERROR_CODE_REDECLARATION_OF_SYMBOL_AS_LOOP_PARAMETER,
    ZEN_ERROR_CODE_REDECLARATION_OF_SYMBOL_AS_CATCH_PARAMETER,
    ZEN_ERROR_CODE_REDECLARATION_OF_SYMBOL_AS_CLASS,
    ZEN_ERROR_CODE_UNKNOWN_CLASS,

    ZEN_ERROR_CODE_COUNT
};

typedef enum zen_ErrorCode_t zen_ErrorCode_t;

#endif /* COM_ONECUBE_ZEN_COMPILER_SUPPORT_ERROR_CODE_H */