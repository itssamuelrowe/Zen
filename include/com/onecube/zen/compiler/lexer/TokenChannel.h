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

// Monday, November 27, 2017

#ifndef COM_ONECUBE_ZEN_COMPILER_LEXER_TOKEN_CHANNEL_H
#define COM_ONECUBE_ZEN_COMPILER_LEXER_TOKEN_CHANNEL_H

/*******************************************************************************
 * TokenChannel                                                                *
 *******************************************************************************/

/**
 * @class TokenChannel
 * @ingroup zen_compiler_lexer
 * @author Samuel Rowe
 * @since zen 1.0
 */
enum zen_TokenChannel_t {
    ZEN_TOKEN_CHANNEL_DEFAULT,
    ZEN_TOKEN_CHANNEL_HIDDEN,
};

/**
 * @memberof TokenChannel
 */
typedef enum zen_TokenChannel_t zen_TokenChannel_t;

#endif /* COM_ONECUBE_ZEN_COMPILER_LEXER_TOKEN_CHANNEL_H */