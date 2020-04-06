// Sunday, November 24, 2019

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

#include <com/onecube/zen/compiler/lexer/TokenType.h>
#include <com/onecube/zen/compiler/symbol-table/Modifier.h>

/*******************************************************************************
 * TokenType                                                                   *
 *******************************************************************************/

// Modifiers

uint32_t zen_TokenType_toModifiers(zen_TokenType_t type) {
    uint32_t modifiers = 0;
    switch (type) {
        case ZEN_TOKEN_KEYWORD_PUBLIC: {
            modifiers |= ZEN_MODIFIER_PUBLIC;
            break;
        }

        case ZEN_TOKEN_KEYWORD_PRIVATE: {
            modifiers |= ZEN_MODIFIER_PRIVATE;
            break;
        }

        case ZEN_TOKEN_KEYWORD_SECRET: {
            modifiers |= ZEN_MODIFIER_SECRET;
            break;
        }

        case ZEN_TOKEN_KEYWORD_ABSTRACT: {
            modifiers |= ZEN_MODIFIER_ABSTRACT;
            break;
        }

        /* NOTE: Functions in Zen cannot be declared as final. In other words,
         * a function declared in a superclass cannot be overriden in a subclass.
         */
        /*
        case ZEN_TOKEN_KEYWORD_FINAL: {
            modifiers |= ZEN_MODIFIER_FINAL;
            break;
        }
        */

        case ZEN_TOKEN_KEYWORD_STATIC: {
            modifiers |= ZEN_MODIFIER_STATIC;
            break;
        }

        case ZEN_TOKEN_KEYWORD_NATIVE: {
            modifiers |= ZEN_MODIFIER_NATIVE;
            break;
        }
    }
    return modifiers;
}

// Assignment Operator

bool zen_TokenType_isAssignmentOperator(zen_TokenType_t type) {
    return (type == ZEN_TOKEN_EQUAL) ||
           (type == ZEN_TOKEN_MODULUS_EQUAL) ||
           (type == ZEN_TOKEN_AMPERSAND_EQUAL) ||
           (type == ZEN_TOKEN_ASTERISK_2_EQUAL) ||
           (type == ZEN_TOKEN_ASTERISK_EQUAL) ||
           (type == ZEN_TOKEN_PLUS_EQUAL) ||
           (type == ZEN_TOKEN_DASH_EQUAL) ||
           (type == ZEN_TOKEN_LEFT_ANGLE_BRACKET_2_EQUAL) ||
           (type == ZEN_TOKEN_LEFT_ANGLE_BRACKET_2_EQUAL) ||
           (type == ZEN_TOKEN_RIGHT_ANGLE_BRACKET_3_EQUAL) ||
           (type == ZEN_TOKEN_RIGHT_ANGLE_BRACKET_2_EQUAL) ||
           (type == ZEN_TOKEN_VERTICAL_BAR_EQUAL);
}