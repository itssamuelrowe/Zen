// Monday, 24 June, 2019

#include <jtk/collection/list/ArrayList.h>
#include <com/onecube/zen/collection/queue/LinkedQueue.h>
#include <com/onecube/zen/collection/stack/ArrayStack.h>
#include <jtk/core/CString.h>
#include <jtk/core/StringBuilder.h>
#include <jtk/io/InputStream.h>

#include <com/onecube/zen/assembler/lexer/AssemblyLexer.h>
#include <com/onecube/zen/onecube/zen/assembler/lexer/AssemblyToken.h>
#include <com/onecube/zen/onecube/zen/assembler/AssemblyTokenType.h>

#warning "AssemblyLexer does not recognize decimal values!"

/*******************************************************************************
 * AssemblyLexer                                                                       *
 *******************************************************************************/

uint8_t zen_AssemblyLexer_literalNames[][25] = {
    { '<', 'u', 'n', 'k', 'n', 'o', 'w', 'n', '>', '\0' },

    { '<', 'i', 'n', 'd', 'e', 'n', 't', 'a', 't', 'i', 'o', 'n', '>', '\0' },
    { '<', 'd', 'e', 'd', 'e', 'n', 't', 'a', 't', 'i', 'o', 'n', '>', '\0' },

    { '<', 'w', 'h', 'i', 't', 'e', '_', 's', 'p', 'a', 'c', 'e', '>', '\0' },
    { '<', 'n', 'e', 'w', 'l', 'i', 'n', 'e', '>', '\0' },
    { '<', 'e', 'n', 'd', '_', 'o', 'f', '_', 's', 't', 'r', 'e', 'a', 'm', '>', '\0' },

    { '.', '\0' },

    { 's', 'i', 'n', 'g', 'l', 'e', '_', 'l', 'i', 'n', 'e', '_', 'c', 'o', 'm', 'm', 'e', 'n', 't', '\0' },
    { 'm', 'u', 'l', 't', 'i', '_', 'l', 'i', 'n', 'e', '_', 'c', 'o', 'm', 'm', 'e', 'n', 't', '\0' },

    { '<', 'i', 'd', 'e', 'n', 't', 'i', 'f', 'i', 'e', 'r', '>', '\0' },

    { 'n', 'o', 'p', '\0' },
    { 'a', 'd', 'd', '_', 'i', '\0' },
    { 'a', 'd', 'd', '_', 'l', '\0' },
    { 'a', 'd', 'd', '_', 'f', '\0' },
    { 'a', 'd', 'd', '_', 'd', '\0' },
    { 'a', 'n', 'd', '_', 'i', '\0' },
    { 'a', 'n', 'd', '_', 'l', '\0' },
    { 'o', 'r', '_', 'i', '\0' },
    { 'o', 'r', '_', 'l', '\0' },
    { 's', 'h', 'i', 'f', 't', '_', 'l', 'e', 'f', 't', '_', 'i', '\0' },
    { 's', 'h', 'i', 'f', 't', '_', 'l', 'e', 'f', 't', '_', 'l', '\0' },
    { 's', 'h', 'i', 'f', 't', '_', 'r', 'i', 'g', 'h', 't', '_', 'i', '\0' },
    { 's', 'h', 'i', 'f', 't', '_', 'r', 'i', 'g', 'h', 't', '_', 'l', '\0' },
    { 's', 'h', 'i', 'f', 't', '_', 'r', 'i', 'g', 'h', 't', '_', 'u', 'i', '\0' },
    { 's', 'h', 'i', 'f', 't', '_', 'r', 'i', 'g', 'h', 't', '_', 'u', 'l', '\0' },
    { 'x', 'o', 'r', '_', 'i', '\0' },
    { 'x', 'o', 'r', '_', 'l', '\0' },
    { 'c', 'a', 's', 't', '_', 'i', 't', 'l', '\0' },
    { 'c', 'a', 's', 't', '_', 'i', 't', 'f', '\0' },
    { 'c', 'a', 's', 't', '_', 'i', 't', 'd', '\0' },
    { 'c', 'a', 's', 't', '_', 'l', 't', 'i', '\0' },
    { 'c', 'a', 's', 't', '_', 'l', 't', 'f', '\0' },
    { 'c', 'a', 's', 't', '_', 'l', 't', 'd', '\0' },
    { 'c', 'a', 's', 't', '_', 'f', 't', 'i', '\0' },
    { 'c', 'a', 's', 't', '_', 'f', 't', 'l', '\0' },
    { 'c', 'a', 's', 't', '_', 'f', 't', 'd', '\0' },
    { 'c', 'a', 's', 't', '_', 'd', 't', 'i', '\0' },
    { 'c', 'a', 's', 't', '_', 'd', 't', 'l', '\0' },
    { 'c', 'a', 's', 't', '_', 'd', 't', 'f', '\0' },
    { 'c', 'a', 's', 't', '_', 'i', 't', 'b', '\0' },
    { 'c', 'a', 's', 't', '_', 'i', 't', 's', '\0' },
    { 'c', 'h', 'e', 'c', 'k', '_', 'c', 'a', 's', 't', '\0' },
    { 'c', 'o', 'm', 'p', 'a', 'r', 'e', '_', 'l', '\0' },
    { 'c', 'o', 'm', 'p', 'a', 'r', 'e', '_', 'l', 't', '_', 'f', '\0' },
    { 'c', 'o', 'm', 'p', 'a', 'r', 'e', '_', 'g', 't', '_', 'f', '\0' },
    { 'c', 'o', 'm', 'p', 'a', 'r', 'e', '_', 'l', 't', '_', 'd', '\0' },
    { 'c', 'o', 'm', 'p', 'a', 'r', 'e', '_', 'g', 't', '_', 'd', '\0' },
    { 'd', 'i', 'v', 'i', 'd', 'e', '_', 'i', '\0' },
    { 'd', 'i', 'v', 'i', 'd', 'e', '_', 'l', '\0' },
    { 'd', 'i', 'v', 'i', 'd', 'e', '_', 'f', '\0' },
    { 'd', 'i', 'v', 'i', 'd', 'e', '_', 'd', '\0' },
    { 'd', 'u', 'p', 'l', 'i', 'c', 'a', 't', 'e', '\0' },
    { 'd', 'u', 'p', 'l', 'i', 'c', 'a', 't', 'e', '_', 'x', '1', '\0' },
    { 'd', 'u', 'p', 'l', 'i', 'c', 'a', 't', 'e', '_', 'x', '2', '\0' },
    { 'd', 'u', 'p', 'l', 'i', 'c', 'a', 't', 'e', '2', '\0' },
    { 'd', 'u', 'p', 'l', 'i', 'c', 'a', 't', 'e', '2', '_', 'x', '1', '\0' },
    { 'd', 'u', 'p', 'l', 'i', 'c', 'a', 't', 'e', '2', '_', 'x', '2', '\0' },
    { 'j', 'u', 'm', 'p', '_', 'e', 'q', '0', '_', 'i', '\0' },
    { 'j', 'u', 'm', 'p', '_', 'n', 'e', '0', '_', 'i', '\0' },
    { 'j', 'u', 'm', 'p', '_', 'l', 't', '0', '_', 'i', '\0' },
    { 'j', 'u', 'm', 'p', '_', 'g', 't', '0', '_', 'i', '\0' },
    { 'j', 'u', 'm', 'p', '_', 'l', 'e', '0', '_', 'i', '\0' },
    { 'j', 'u', 'm', 'p', '_', 'g', 'e', '0', '_', 'i', '\0' },
    { 'j', 'u', 'm', 'p', '_', 'e', 'q', '_', 'i', '\0' },
    { 'j', 'u', 'm', 'p', '_', 'n', 'e', '_', 'i', '\0' },
    { 'j', 'u', 'm', 'p', '_', 'l', 't', '_', 'i', '\0' },
    { 'j', 'u', 'm', 'p', '_', 'g', 't', '_', 'i', '\0' },
    { 'j', 'u', 'm', 'p', '_', 'l', 'e', '_', 'i', '\0' },
    { 'j', 'u', 'm', 'p', '_', 'g', 'e', '_', 'i', '\0' },
    { 'j', 'u', 'm', 'p', '_', 'e', 'q', '_', 'a', '\0' },
    { 'j', 'u', 'm', 'p', '_', 'n', 'e', '_', 'a', '\0' },
    { 'j', 'u', 'm', 'p', '_', 'e', 'q', 'n', '_', 'a', '\0' },
    { 'j', 'u', 'm', 'p', '_', 'n', 'e', 'n', '_', 'a', '\0' },
    { 'i', 'n', 'c', 'r', 'e', 'm', 'e', 'n', 't', '_', 'i', '\0' },
    { 'i', 'n', 'v', 'o', 'k', 'e', '_', 's', 'p', 'e', 'c', 'i', 'a', 'l', '\0' },
    { 'i', 'n', 'v', 'o', 'k', 'e', '_', 'v', 'i', 'r', 't', 'u', 'a', 'l', '\0' },
    { 'i', 'n', 'v', 'o', 'k', 'e', '_', 'd', 'y', 'n', 'a', 'm', 'i', 'c', '\0' },
    { 'i', 'n', 'v', 'o', 'k', 'e', '_', 's', 't', 'a', 't', 'i', 'c', '\0' },
    { 'i', 'n', 'v', 'o', 'k', 'e', '_', 'f', 'r', 'a', 'g', 'm', 'e', 'n', 't', '\0' },
    { 'j', 'u', 'm', 'p', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'i', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'l', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'f', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'd', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'a', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'i', '0', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'i', '1', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'i', '2', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'i', '3', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'l', '0', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'l', '1', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'l', '2', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'l', '3', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'f', '0', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'f', '1', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'f', '2', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'f', '3', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'd', '0', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'd', '1', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'd', '2', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'd', '3', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'a', '0', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'a', '1', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'a', '2', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'a', '3', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'a', 'b', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'a', 'c', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'a', 's', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'a', 'i', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'a', 'l', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'a', 'f', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'a', 'd', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'a', 'a', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'i', 'n', 's', 't', 'a', 'n', 'c', 'e', '_', 'f', 'i', 'e', 'l', 'd', '\0' },
    { 'l', 'o', 'a', 'd', '_', 's', 't', 'a', 't', 'i', 'c', '_', 'f', 'i', 'e', 'l', 'd', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'c', 'p', 'r', '\0' },
    { 'l', 'o', 'a', 'd', '_', 'a', 'r', 'r', 'a', 'y', '_', 's', 'i', 'z', 'e', '\0' },
    { 'm', 'o', 'd', 'u', 'l', 'o', '_', 'i', '\0' },
    { 'm', 'o', 'd', 'u', 'l', 'o', '_', 'l', '\0' },
    { 'm', 'o', 'd', 'u', 'l', 'o', '_', 'f', '\0' },
    { 'm', 'o', 'd', 'u', 'l', 'o', '_', 'd', '\0' },
    { 'm', 'u', 'l', 't', 'i', 'p', 'l', 'y', '_', 'i', '\0' },
    { 'm', 'u', 'l', 't', 'i', 'p', 'l', 'y', '_', 'l', '\0' },
    { 'm', 'u', 'l', 't', 'i', 'p', 'l', 'y', '_', 'f', '\0' },
    { 'm', 'u', 'l', 't', 'i', 'p', 'l', 'y', '_', 'd', '\0' },
    { 'n', 'e', 'g', 'a', 't', 'e', '_', 'i', '\0' },
    { 'n', 'e', 'g', 'a', 't', 'e', '_', 'l', '\0' },
    { 'n', 'e', 'g', 'a', 't', 'e', '_', 'f', '\0' },
    { 'n', 'e', 'g', 'a', 't', 'e', '_', 'd', '\0' },
    { 'n', 'e', 'w', '\0' },
    { 'n', 'e', 'w', '_', 'a', 'r', 'r', 'a', 'y', '\0' },
    { 'n', 'e', 'w', '_', 'a', 'r', 'r', 'a', 'y', '_', 'n', '\0' },
    { 'n', 'e', 'w', '_', 'a', 'r', 'r', 'a', 'y', '_', 'a', '\0' },
    { 'p', 'o', 'p', '\0' },
    { 'p', 'o', 'p', '2', '\0' },
    { 'p', 'u', 's', 'h', '_', 'n', 'u', 'l', 'l', '\0' },
    { 'p', 'u', 's', 'h', '_', 'i', 'n', '1', '\0' },
    { 'p', 'u', 's', 'h', '_', 'i', '0', '\0' },
    { 'p', 'u', 's', 'h', '_', 'i', '1', '\0' },
    { 'p', 'u', 's', 'h', '_', 'i', '2', '\0' },
    { 'p', 'u', 's', 'h', '_', 'i', '3', '\0' },
    { 'p', 'u', 's', 'h', '_', 'i', '4', '\0' },
    { 'p', 'u', 's', 'h', '_', 'i', '5', '\0' },
    { 'p', 'u', 's', 'h', '_', 'l', '0', '\0' },
    { 'p', 'u', 's', 'h', '_', 'l', '1', '\0' },
    { 'p', 'u', 's', 'h', '_', 'l', '2', '\0' },
    { 'p', 'u', 's', 'h', '_', 'f', '0', '\0' },
    { 'p', 'u', 's', 'h', '_', 'f', '1', '\0' },
    { 'p', 'u', 's', 'h', '_', 'f', '2', '\0' },
    { 'p', 'u', 's', 'h', '_', 'd', '0', '\0' },
    { 'p', 'u', 's', 'h', '_', 'd', '1', '\0' },
    { 'p', 'u', 's', 'h', '_', 'd', '2', '\0' },
    { 'r', 'e', 't', 'u', 'r', 'n', '\0' },
    { 'r', 'e', 't', 'u', 'r', 'n', '_', 'i', '\0' },
    { 'r', 'e', 't', 'u', 'r', 'n', '_', 'l', '\0' },
    { 'r', 'e', 't', 'u', 'r', 'n', '_', 'f', '\0' },
    { 'r', 'e', 't', 'u', 'r', 'n', '_', 'd', '\0' },
    { 'r', 'e', 't', 'u', 'r', 'n', '_', 'a', '\0' },
    { 'r', 'e', 't', 'u', 'r', 'n', '_', 'f', 'r', 'a', 'g', 'm', 'e', 'n', 't', '\0' },
    { 'r', 't', 't', 'i', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'i', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'i', '0', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'i', '1', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'i', '2', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'i', '3', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'l', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'l', '0', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'l', '1', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'l', '2', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'l', '3', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'f', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'f', '0', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'f', '1', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'f', '2', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'f', '3', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'd', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'd', '0', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'd', '1', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'd', '2', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'd', '3', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'a', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'a', '0', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'a', '1', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'a', '2', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'a', '3', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'a', 'b', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'a', 'c', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'a', 's', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'a', 'i', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'a', 'l', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'a', 'f', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'a', 'd', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'a', 'a', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 's', 't', 'a', 't', 'i', 'c', '_', 'f', 'i', 'e', 'l', 'd', '\0' },
    { 's', 't', 'o', 'r', 'e', '_', 'i', 'n', 's', 't', 'a', 'n', 'c', 'e', '_', 'f', 'i', 'e', 'l', 'd', '\0' },
    { 's', 'u', 'b', 't', 'r', 'a', 'c', 't', '_', 'i', '\0' },
    { 's', 'u', 'b', 't', 'r', 'a', 'c', 't', '_', 'l', '\0' },
    { 's', 'u', 'b', 't', 'r', 'a', 'c', 't', '_', 'f', '\0' },
    { 's', 'u', 'b', 't', 'r', 'a', 'c', 't', '_', 'd', '\0' },
    { 's', 'w', 'a', 'p', '\0' },
    { 's', 'w', 'i', 't', 'c', 'h', '_', 't', 'a', 'b', 'l', 'e', '\0' },
    { 's', 'w', 'i', 't', 'c', 'h', '_', 's', 'e', 'a', 'r', 'c', 'h', '\0' },
    { 't', 'h', 'r', 'o', 'w', '\0' },
    { 'w', 'i', 'd', 'e', '\0' },

    { 'c', 'o', 'd', 'e', '\0' },
    { 'c', 'o', 'n', 's', 't', 'a', 'n', 't', '_', 'p', 'o', 'o', 'l', '\0' },
    { 'f', 'u', 'n', 'c', 't', 'i', 'o', 'n', '\0' },

    { '<', 'i', 'n', 't', 'e', 'g', 'e', 'r', '_', 'l', 'i', 't', 'e', 'r', 'a', 'l', '>', '\0' },

    { '<', 's', 't', 'r', 'i', 'n', 'g', '_', 'l', 'i', 't', 'e', 'r', 'a', 'l', '>', '\0' },

    { '<', 'f', 'l', 'o', 'a', 't', 'i', 'n', 'g', '_', 'p', 'o', 'i', 'n', 't', '_', 'l', 'i', 't', 'e', 'r', 'a', 'l', '>', '\0' },
};

/* Literal Name */

const uint8_t* zen_AssemblyLexer_getLiteralName(zen_TokenType_t type) {
    return zen_AssemblyLexer_literalNames[(int32_t)type];
}

/* Constructor */

zen_AssemblyLexer_t* zen_AssemblyLexer_new(zen_InputStream_t* inputStream) {
    /* The constructor invokes zen_AssemblyLexer_consume() to initialize
     * the LA(1) character. Therefore, we assign negative values
     * to certain attributes.
     */

    zen_AssemblyLexer_t* lexer = zen_Memory_allocate(zen_AssemblyLexer_t, 1);
    lexer->m_inputStream = inputStream;
    lexer->m_la1 = 0;
    lexer->m_index = -1;
    lexer->m_line = 1;
    lexer->m_column = -1;
    lexer->m_startIndex = 0;
    lexer->m_startLine = 0;
    lexer->m_startColumn = 0;
    lexer->m_hitEndOfStream = false;
    lexer->m_token = NULL;
    lexer->m_channel = ZEN_TOKEN_CHANNEL_DEFAULT;
    lexer->m_text = zen_StringBuilder_new();
    lexer->m_type = ZEN_TOKEN_UNKNOWN;
    lexer->m_tokens = zen_ArrayQueue_new();
    lexer->m_indentations = zen_ArrayStack_new();
    lexer->m_enclosures = 0;
    lexer->m_errors = zen_ArrayList_new();

    zen_AssemblyLexer_consume(lexer);

    return lexer;
}

/* Destructor */

void zen_AssemblyLexer_delete(zen_AssemblyLexer_t* lexer) {
    jtk_Assert_assertObject(lexer, "The specified lexer is null.");

    zen_StringBuilder_delete(lexer->m_text);

    /* The lexer may have unretrieved tokens in the buffer.
     * This destructor is responsible for the destruction of
     * such tokens.
     */
    int32_t size = zen_ArrayList_getSize(lexer->m_tokens->m_list);
    int32_t i;
    for (i = 0; i < size; i++) {
        zen_Token_t* token = (zen_Token_t*)zen_ArrayList_get(lexer->m_tokens->m_list, i);
        zen_Token_delete(token);
    }
    zen_ArrayQueue_delete(lexer->m_tokens);

    zen_ArrayStack_delete(lexer->m_indentations);

    size = zen_ArrayList_getSize(lexer->m_errors);
    for (i = 0; i < size; i++) {
        zen_AssemblyLexerError_t* error = (zen_AssemblyLexerError_t*)zen_ArrayList_get(lexer->m_errors, i);
        zen_AssemblyLexerError_delete(error);
    }
    zen_ArrayList_delete(lexer->m_errors);

    zen_Memory_deallocate(lexer);
}

/* Create Token */

zen_Token_t* zen_AssemblyLexer_createToken(zen_AssemblyLexer_t* lexer) {
    int8_t* text = zen_StringBuilder_toString(lexer->m_text);
    int32_t length = zen_StringBuilder_getSize(lexer->m_text);

    zen_Token_t* token =
        zen_Token_new(
            lexer->m_channel,
            lexer->m_type,
            text,
            length,
            lexer->m_startIndex,    /* inclusive */
            lexer->m_index,         /* exclusive */
            lexer->m_startLine,     /* inclusive */
            lexer->m_line,          /* inclusive */
            lexer->m_startColumn,   /* inclusive */
            lexer->m_column         /* inclusive */
        );

    /* Destroy the text; not required anymore. */
    zen_String_delete(text);

    return token;
}

void zen_AssemblyLexer_onNewline(zen_AssemblyLexer_t* lexer) {
    lexer->m_line++;
    lexer->m_column = 1;
}

zen_AssemblyLexerError_t* zen_AssemblyLexer_createError(zen_AssemblyLexer_t* lexer, const char* message) {
    zen_AssemblyLexerError_t* error = zen_AssemblyLexerError_new(message, lexer->m_inputStream->m_path, lexer->m_startLine, lexer->m_startColumn);
    return error;
}

bool zen_AssemblyLexer_isInputStart(zen_AssemblyLexer_t* lexer) {
    return (lexer->m_startLine == 0) && (lexer->m_startColumn == 0);
}

void zen_AssemblyLexer_consume(zen_AssemblyLexer_t* lexer) {
    zen_StringBuilder_appendCodePoint(lexer->m_text, lexer->m_la1);

    lexer->m_index++;
    lexer->m_column++;
    /* NOTE: We could have used
     * lexer->index >= "length of input stream"
     * Unfortunately, the flexible design of both the
     * lexer and input stream fails to provide a method
     * to determine the stream length in advance.
     */
    if (zen_InputStream_isAvailable(lexer->m_inputStream) == 0) {
        lexer->m_la1 = ZEN_END_OF_STREAM;
    }
    else {
        lexer->m_la1 = zen_InputStream_read(lexer->m_inputStream);
    }
}

void zen_AssemblyLexer_emit(zen_AssemblyLexer_t* lexer, zen_Token_t* token) {
    lexer->m_token = token;
    zen_ArrayQueue_enqueue(lexer->m_tokens, token);
}

/*
 * ALGORITHM
 * ---------
 *
 * Check for a buffered token. If found, remove it from the buffer
 * and return it to the user.
 *
 * The lexer checks if an end of stream was encountered within
 * a block, indicated when the end of stream character is encountered
 * when the indentation depth is greater than 0. If so, a newline
 * token is emitted. This arbitrary token serves as the end of
 * a statement. After which, the lexer emits dedentation tokens
 * as needed. In order to retrieve all the tokens generated in the
 * above steps multiple calls to nextToken() are required. Therefore,
 * the lexer buffers up tokens. The buffered tokens are appropriately
 * produced before the user as described previously.
 *
 * A single lexical recognition may result in multiple errors. For example,
 * when recognizing a string literal we may encounter a number of malformed
 * escape sequences and an unexpected end of stream.
 */
zen_Token_t* zen_AssemblyLexer_nextToken(zen_AssemblyLexer_t* lexer) {
    jtk_Assert_assertObject(lexer, "The specified lexer is null.");

    /* The lexer does not bother to recognize a token
     * from the input stream unless necessary.
     */
    if (zen_ArrayQueue_isEmpty(lexer->m_tokens)) {
        /* We don't exit the loop until
         * -- We have token.
         * -- We have reached the end of the stream.
         * -- We have encountered an error. (Interestingly, this condition
         *    is not explicitly checked because errorneous token recognition
         *    too generate tokens!)
         */
        loopEntry : {
            lexer->m_token = NULL;
            lexer->m_type = ZEN_TOKEN_UNKNOWN;
            zen_StringBuilder_clear(lexer->m_text);
            lexer->m_channel = ZEN_TOKEN_CHANNEL_DEFAULT;
            lexer->m_startIndex = lexer->m_index;
            lexer->m_startLine = lexer->m_line;
            lexer->m_startColumn = lexer->m_column;

            switch (lexer->m_la1) {

                case ZEN_END_OF_STREAM : {
                    if (!zen_ArrayStack_isEmpty(lexer->m_indentations)) {
                        /* It appears that the lexer has reached the end of
                         * the stream inside a block. In order to prevent sytax
                         * errors occuring because of a "missing newline" we emit
                         * an extra newline token. It may serve as the end of a
                         * statement. After which, the lexer emits dedentation
                         * tokens as needed.
                         *
                         * NOTE: The lexer is creating an imaginary token here.
                         *       Therefore, we directly invoke zen_Token_new().
                         */
                        zen_Token_t* newlineToken = zen_Token_new(
                            ZEN_TOKEN_CHANNEL_DEFAULT,
                            ZEN_TOKEN_NEWLINE,
                            "\n",
                            1,
                            lexer->m_startIndex,    /* inclusive */
                            lexer->m_index,         /* exclusive */
                            lexer->m_startLine,     /* inclusive */
                            lexer->m_line,          /* inclusive */
                            lexer->m_startColumn,   /* inclusive */
                            lexer->m_column         /* inclusive */
                        );
                        zen_AssemblyLexer_emit(lexer, newlineToken);

                        while (!zen_ArrayStack_isEmpty(lexer->m_indentations)) {
                            /*
                             * NOTE: The lexer is creating an imaginary token here.
                             *       Therefore, we directly invoke zen_Token_new().
                             */
                            zen_Token_t* dedentationToken = zen_Token_new(
                                    ZEN_TOKEN_CHANNEL_DEFAULT,
                                    ZEN_TOKEN_DEDENTATION,
                                    "",
                                    0,
                                    lexer->m_startIndex,    /* inclusive */
                                    lexer->m_index,         /* exclusive */
                                    lexer->m_startLine,     /* inclusive */
                                    lexer->m_line,          /* inclusive */
                                    lexer->m_startColumn,   /* inclusive */
                                    lexer->m_column         /* inclusive */
                                );
                            zen_AssemblyLexer_emit(lexer, dedentationToken);
                            zen_ArrayStack_pop(lexer->m_indentations);
                        }
                    }
                    /* The data required for the creating the end-of-stream token.
                     */
                    lexer->m_type = ZEN_TOKEN_END_OF_STREAM;
                    lexer->m_hitEndOfStream = true;
                    break;
                }

                case ' '  :
                case '\r' :
                case '\n' : {
                    int32_t indentation = 0;
                    if (lexer->m_la1 == ' ') {
                        do {
                            indentation++;
                            zen_AssemblyLexer_consume(lexer);
                        }
                        while (lexer->m_la1 == ' ');

                        if (!zen_AssemblyLexer_isInputStart(lexer)) {
                            /* This token belongs to the WHITESPACE rule. */
                            lexer->m_type = ZEN_TOKEN_WHITESPACE;
                            lexer->m_channel = ZEN_TOKEN_CHANNEL_HIDDEN;
                        }
                    }
                    else {
                        if (lexer->m_la1 == '\r') {
                            zen_AssemblyLexer_consume(lexer);
                            /* Optionally, the carriage return character may be
                             * followed by a newline character.
                             */
                            if (lexer->m_la1 == '\n') {
                                zen_AssemblyLexer_consume(lexer);

                                /* Update information such as the current line,
                                 * current column, etc.
                                 */
                                zen_AssemblyLexer_onNewline(lexer);

                                /* Update information such as the current line,
                                 * current column, etc.
                                 */
                                zen_AssemblyLexer_onNewline(lexer);
                            }
                        }
                        else {
                            zen_AssemblyLexer_consume(lexer);

                            /* Update information such as the current line,
                             * current column, etc.
                             */
                            zen_AssemblyLexer_onNewline(lexer);
                        }

                        while (lexer->m_la1 == ' ') {
                            indentation++;
                            zen_AssemblyLexer_consume(lexer);
                        }
                    }

                    if (lexer->m_type != ZEN_TOKEN_WHITESPACE) {
                        int32_t la1 = lexer->m_la1;
                        if ((lexer->m_enclosures > 0) || (la1 == '\r') || (la1 == '\n')) {
                            /* If the lexer is within the context of a collection
                             * or a blank line it ignores all the indentations,
                             * dedentations, and newlines.
                             */
                            zen_Logger_log(ZEN_LOG_PRIORITY_INFO, "Skipping a character (line=%d, column=%d)", lexer->m_startIndex + 1, lexer->m_startColumn + 1);
                            goto loopEntry;
                        }
                        else {
                            /*
                             * NOTE: The lexer is creating a custom token here.
                             *       Therefore, we directly invoke zen_Token_new().
                             */
                            zen_Token_t* newlineToken = zen_Token_new(
                                ZEN_TOKEN_CHANNEL_DEFAULT,
                                ZEN_TOKEN_NEWLINE,
                                "\n",
                                1,
                                lexer->m_startIndex,    /* inclusive */
                                lexer->m_index,         /* exclusive */
                                lexer->m_startLine,     /* inclusive */
                                lexer->m_line,          /* inclusive */
                                lexer->m_startColumn,   /* inclusive */
                                lexer->m_column         /* inclusive */
                            );
                            zen_AssemblyLexer_emit(lexer, newlineToken);

                            int32_t previous = zen_ArrayStack_isEmpty(lexer->m_indentations)?
                                0 : (int32_t)zen_ArrayStack_peek(lexer->m_indentations);

                            if (indentation == previous) {
                                /* The lexer does not generate an INDENTETATION
                                 * token if the current indentation is the same
                                 * as the previous indentation.
                                 */
                                goto loopEntry;
                            }
                            else if (indentation > previous) {
                                /* The lexer generates a new INDENTETATION token
                                 * if the current indentation is greater than the
                                 * previous indentation, indicating a deeper block
                                 * nest.
                                 */
                                zen_ArrayStack_push(lexer->m_indentations, (void*)indentation);
                                /*
                                 * NOTE: The lexer is creating a custom token here.
                                 *       Therefore, we directly invoke zen_Token_new().
                                 */
                                zen_Token_t* indentationToken = zen_Token_new(
                                        ZEN_TOKEN_CHANNEL_DEFAULT,
                                        ZEN_TOKEN_INDENTATION,
                                        "",
                                        0,
                                        lexer->m_startIndex,    /* inclusive */
                                        lexer->m_index,         /* exclusive */
                                        lexer->m_startLine,     /* inclusive */
                                        lexer->m_line,          /* inclusive */
                                        lexer->m_startColumn,   /* inclusive */
                                        lexer->m_column         /* inclusive */
                                    );
                                zen_AssemblyLexer_emit(lexer, indentationToken);
                            }
                            else {
                                /* The lexer generates one or more DEDENTATION tokens
                                 * if the current indentation is lesser than the
                                 * previous indentation, indicating a shallower
                                 * block.
                                 *
                                 * Interestingly, the dedentation does not require
                                 * the exact number of whitespaces as seen in the
                                 * indentation.
                                 */
                                while (!zen_ArrayStack_isEmpty(lexer->m_indentations) &&
                                       ((int32_t)zen_ArrayStack_peek(lexer->m_indentations) > indentation)) {
                                    /*
                                     * NOTE: The lexer is creating a custom token here.
                                     *       Therefore, we directly invoke zen_Token_new().
                                     */
                                    zen_Token_t* dedentationToken = zen_Token_new(
                                        ZEN_TOKEN_CHANNEL_DEFAULT,
                                        ZEN_TOKEN_DEDENTATION,
                                        "",
                                        0,
                                        lexer->m_startIndex,    /* inclusive */
                                        lexer->m_index,         /* exclusive */
                                        lexer->m_startLine,     /* inclusive */
                                        lexer->m_line,          /* inclusive */
                                        lexer->m_startColumn,   /* inclusive */
                                        lexer->m_column         /* inclusive */
                                    );
                                    zen_AssemblyLexer_emit(lexer, dedentationToken);
                                    zen_ArrayStack_pop(lexer->m_indentations);
                                }
                            }
                        }
                        /* The rule action has taken care of generating
                         * tokens. The lexer can confidently skip any other
                         * token producing operations.
                         */
                        goto loopEntry;
                    }
                    break;
                }

                /* AT
                 * :    '@'
                 * ;
                 */
                case '@' : {
                    /* Consume and discard the '@' character. */
                    zen_AssemblyLexer_consume(lexer);
                    /* The lexer has recognized the '@' token. */
                    lexer->m_type = ZEN_TOKEN_AT;
                    break;
                }

                /* HASH
                 * :    '#'
                 * ;
                 */
                case '#' : {
                    /* Consume and discard the '#' character. */
                    zen_AssemblyLexer_consume(lexer);
                    /* The lexer has recognized the '#' token. */
                    lexer->m_type = ZEN_TOKEN_HASH;

                    break;
                }

                /* COMMA
                 * :    ','
                 * ;
                 */
                case ',' : {
                    /* Consume and discard the ',' character. */
                    zen_AssemblyLexer_consume(lexer);
                    /* The lexer has recognized the ',' token. */
                    lexer->m_type = ZEN_TOKEN_COMMA;
                    break;
                }

                /* SINGLE_LINE_COMMENT
                 * :    '//' ~[\r\n]* -> channel(hidden)
                 * ;
                 *
                 * MULTI_LINE_COMMENT
                 * :    '/*' .*? '*''/' -> channel(hidden)
                 * ;
                 *
                 */
                case '/' : {
                    /* Consume and discard the '/' character. */
                    zen_AssemblyLexer_consume(lexer);

                    if (lexer->m_la1 == '/') {
                        /* Consume and discard the '/' character. */
                        zen_AssemblyLexer_consume(lexer);

                        while ((lexer->m_la1 != '\n') && (lexer->m_la1 != ZEN_END_OF_STREAM)) {
                            /* Consume and discard the unknown character. */
                            zen_AssemblyLexer_consume(lexer);
                        }

                        /* We consume the terminating character, regardless
                         * of it being newline or end-of-stream.
                         */
                        zen_AssemblyLexer_consume(lexer);

                        /* Update information such as the current line,
                         * current column, etc.
                         */
                        if (lexer->m_la1 == '\n') {
                            zen_AssemblyLexer_onNewline(lexer);
                        }

                        /* The lexer has recognized a single line comment. */
                        lexer->m_type = ZEN_TOKEN_SINGLE_LINE_COMMENT;
                        /* The single-line comment token should be produced on the
                         * hidden channel. Otherwise, the parser will have a hard
                         * time ignoring redundant single-line comment tokens.
                         */
                        lexer->m_channel = ZEN_TOKEN_CHANNEL_HIDDEN;
                    }
                    else if (lexer->m_la1 == '*') {
                        /* Consume and discard the '*' token. */
                        zen_AssemblyLexer_consume(lexer);

                        do {
                            while (lexer->m_la1 != '*') {
                                if (lexer->m_la1 == ZEN_END_OF_STREAM) {
                                    zen_AssemblyLexer_t* error = zen_AssemblyLexer_createError(lexer, "Unterminated multi-line comment");
                                    zen_ArrayList_add(lexer->m_errors, error);
                                    break;
                                }

                                /* Update information such as the current line,
                                 * current column, etc.
                                 */
                                if (lexer->m_la1 == '\n') {
                                    zen_AssemblyLexer_onNewline(lexer);
                                }

                                /* Consume and discard the unknown character. */
                                zen_AssemblyLexer_consume(lexer);
                            }

                            /* The following condition is checked to exit the
                             * outer loop. We do not have to report the error.
                             * Because the error was reported in the inner loop.
                             */
                            if (lexer->m_la1 == ZEN_END_OF_STREAM) {
                                break;
                            }

                            /* Here, we are currently processing the '*' character.
                             * Therefore, we consume it.
                             */
                            zen_AssemblyLexer_consume(lexer);
                        }
                        while (lexer->m_la1 != '/');

                        if (lexer->m_la1 == '/') {
                            /* At this point, we are indeed processing the '/'
                             * character. Therefore, we consume it.
                             */
                            zen_AssemblyLexer_consume(lexer);
                        }
                        /*
                        else {
                            ... UNTERMINATED COMMENT ...
                        }
                        */

                        /* The lexer has recognized the multi-line comment. */
                        lexer->m_type = ZEN_TOKEN_MULTI_LINE_COMMENT;
                        /* The multi-line comment token should be produced on the
                         * hidden channel. Otherwise, the parser will have a hard
                         * time ignoring redundant multi-line comment tokens.
                         */
                        lexer->m_channel = ZEN_TOKEN_CHANNEL_HIDDEN;
                    }

                    break;
                }

                /* STRING_LITERAL
                 * :    '"' STRING_CHARACTER* '"'
                 * ;
                 *
                 * STRING_CHARACTER
                 * :    ~["'\\]
                 * |    ESCAPE_SEQUENCE
                 * ;
                 *
                 * ESCAPE_SEQUENCE
                 * :    '\\' [btnfr"'\\]
                 * |    '\\' HEXADECIMAL_DIGIT HEXADECIMAL_DIGIT HEXADECIMAL_DIGIT HEXADECIMAL_DIGIT
                 * ;
                 *
                 * HEXADECIMAL_DIGIT
                 * :    [0-9a-fA-F]
                 * ;
                 */
                case '"': {
                    /* Consume and discard the character which marks the
                     * beginning of this string.
                     */
                    zen_AssemblyLexer_consume(lexer);

                    while (lexer->m_la1 != '"') {
                        if (lexer->m_la1 == ZEN_END_OF_STREAM) {
                            zen_AssemblyLexerError_t* error = zen_AssemblyLexer_createError(lexer, "Unexpected end of stream in string literal");
                            zen_ArrayList_add(lexer->m_errors, error);
                            break;
                        }
                        else if (lexer->m_la1 == '\n') {
                            zen_AssemblyLexerError_t* error = zen_AssemblyLexer_createError(lexer, "Unexpected end of line in string literal");
                            zen_ArrayList_add(lexer->m_errors, error);
                            break;
                        }
                        else if (lexer->m_la1 == '\\') {
                            /* Consume and discard the '\' character. */
                            zen_AssemblyLexer_consume(lexer);

                            if (zen_AssemblyLexer_isBasicEscapeSequence(lexer->m_la1)) {
                                /* Consume and discard the character which represents
                                 * a basic escape sequence.
                                 */
                                zen_AssemblyLexer_consume(lexer);
                            }
                            else if (lexer->m_la1 == 'u') {
                                /* Consume and discard the 'u' character. */
                                zen_AssemblyLexer_consume(lexer);

                                int32_t i;
                                for (i = 0; i < 4; i++) {
                                    if (zen_AssemblyLexer_isHexadecimalDigit(lexer->m_la1)) {
                                        /* Consume and discard the hexadecimal digit character. */
                                        zen_AssemblyLexer_consume(lexer);
                                    }
                                    else {
                                        zen_AssemblyLexerError_t* error = zen_AssemblyLexer_createError(lexer, "Expected four hexadecimal digits");
                                        zen_ArrayList_add(lexer->m_errors, error);
                                        break;
                                    }
                                }
                            }
                            else {
                                zen_AssemblyLexerError_t* error = zen_AssemblyLexer_createError(lexer, "Unknown escape sequence");
                                zen_ArrayList_add(lexer->m_errors, error);

                                /* Consume and discard the unknown escape sequence. */
                                zen_AssemblyLexer_consume(lexer);
                            }
                        }
                        else {
                            /* Consume and discard a character in the string literal. */
                            zen_AssemblyLexer_consume(lexer);
                        }
                    }

                    if (lexer->m_la1 == terminator) {
                        /* At this point, we are processing the terminating
                         * double or single quote character. Therefore,
                         * consume and discard it.
                         */
                        zen_AssemblyLexer_consume(lexer);
                    }
                    else {
                        /* Most likely, we encountered an immature end of line or stream. */
                    }

                    /* The lexer has recognized a string literal. */
                    lexer->m_type = ZEN_TOKEN_STRING_LITERAL;

                    break;
                }

                default : {

                    /* IDENTIFIER
                     * :    LETTER LETTER_OR_DIGIT*
                     * ;
                     */
                    if (zen_AssemblyLexer_isIdentifierStart(lexer->m_la1)) {
                        /* Consume and discard the first letter. */
                        zen_AssemblyLexer_consume(lexer);

                        while (zen_AssemblyLexer_isIdentifierPart(lexer->m_la1)) {
                            /* Consume and discard the consecutive letter
                             * or digit character.
                             */
                            zen_AssemblyLexer_consume(lexer);
                        }

                        uint8_t* text = zen_StringBuilder_toString(lexer->m_text);
                        int32_t length = lexer->m_index - lexer->m_startIndex;

                        /* TODO: Find a better solution.
                         *
                         * Another solution, would be to use a hash map with token
                         * text as key and token type as values. This is obviously
                         * more efficient than the current algorithm. Unfortunately,
                         * I neither have patience nor time to implement this.
                         *
                         * The following algorithm tries to prevent redundant comparisons
                         * between strings (as many as possible).
                         * Basically, the code checks if the token length is known.
                         * If true, then the keywords with the correspoding length
                         * are compared against the token. The keywords are always
                         * arranged in ascending lexicographical order. Thus, we
                         * reduce the worst case scenario for number of comparisons
                         * from 198 to 20.
                         * The algorithm described here was not hand written.
                         * It was generated by a program.
                         */
                        lexer->m_type = ZEN_TOKEN_IDENTIFIER;
                        switch (length) {
                            case 3 : {
                                if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_NEW], 3)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_NEW;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_NOP], 3)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_NOP;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_POP], 3)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_POP;
                                }
                                break;
                            }
                            case 4 : {
                                if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_JUMP], 4)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_JUMP;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_OR_I], 4)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_OR_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_OR_L], 4)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_OR_L;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_POP2], 4)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_POP2;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_RTTI], 4)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_RTTI;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_SWAP], 4)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_SWAP;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_WIDE], 4)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_WIDE;
                                }
                                break;
                            }
                            case 5 : {
                                if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_ADD_F], 5)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_ADD_F;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_ADD_I], 5)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_ADD_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_ADD_L], 5)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_ADD_L;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_ADD_d], 5)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_ADD_d;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_AND_I], 5)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_AND_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_AND_L], 5)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_AND_L;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_THROW], 5)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_THROW;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_XOR_I], 5)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_XOR_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_XOR_L], 5)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_XOR_L;
                                }
                                break;
                            }
                            case 6 : {
                                if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_A], 6)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_A;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_D], 6)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_D;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_F], 6)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_F;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_I], 6)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_L], 6)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_L;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_PUSH_B], 6)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_PUSH_B;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_PUSH_S], 6)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_PUSH_S;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_RETURN], 6)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_RETURN;
                                }
                                break;
                            }
                            case 7 : {
                                if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_A0], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_A0;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_A1], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_A1;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_A2], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_A2;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_A3], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_A3;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_AA], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_AA;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_AB], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_AB;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_AC], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_AC;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_AD], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_AD;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_AF], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_AF;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_AI], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_AI;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_AL], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_AL;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_AS], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_AS;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_D0], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_D0;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_D1], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_D1;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_D2], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_D2;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_D3], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_D3;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_F0], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_F0;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_F1], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_F1;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_F2], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_F2;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_F3], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_F3;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_I0], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_I0;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_I1], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_I1;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_I2], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_I2;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_I3], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_I3;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_L0], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_L0;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_L1], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_L1;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_L2], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_L2;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_L3], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_L3;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_PUSH_D0], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_PUSH_D0;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_PUSH_D1], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_PUSH_D1;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_PUSH_D2], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_PUSH_D2;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_PUSH_F0], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_PUSH_F0;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_PUSH_F1], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_PUSH_F1;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_PUSH_F2], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_PUSH_F2;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_PUSH_I0], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_PUSH_I0;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_PUSH_I1], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_PUSH_I1;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_PUSH_I2], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_PUSH_I2;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_PUSH_I3], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_PUSH_I3;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_PUSH_I4], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_PUSH_I4;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_PUSH_I5], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_PUSH_I5;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_PUSH_L0], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_PUSH_L0;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_PUSH_L1], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_PUSH_L1;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_PUSH_L2], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_PUSH_L2;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_A], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_A;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_D], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_D;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_F], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_F;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_I], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_L], 7)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_L;
                                }
                                break;
                            }
                            case 8 : {
                                if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_CAST_DTF], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_CAST_DTF;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_CAST_DTI], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_CAST_DTI;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_CAST_DTL], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_CAST_DTL;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_CAST_FTD], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_CAST_FTD;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_CAST_FTI], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_CAST_FTI;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_CAST_FTL], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_CAST_FTL;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_CAST_ITB], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_CAST_ITB;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_CAST_ITC], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_CAST_ITC;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_CAST_ITD], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_CAST_ITD;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_CAST_ITF], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_CAST_ITF;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_CAST_ITL], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_CAST_ITL;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_CAST_ITS], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_CAST_ITS;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_CAST_LTD], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_CAST_LTD;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_CAST_LTF], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_CAST_LTF;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_CAST_LTI], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_CAST_LTI;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_DIVIDE_D], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_DIVIDE_D;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_DIVIDE_F], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_DIVIDE_F;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_DIVIDE_I], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_DIVIDE_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_DIVIDE_L], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_DIVIDE_L;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_CPR], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_CPR;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_MODULO_D], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_MODULO_D;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_MODULO_F], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_MODULO_F;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_MODULO_I], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_MODULO_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_MODULO_L], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_MODULO_L;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_NEGATE_D], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_NEGATE_D;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_NEGATE_F], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_NEGATE_F;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_NEGATE_I], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_NEGATE_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_NEGATE_L], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_NEGATE_L;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_PUSH_IN1], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_PUSH_IN1;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_RETURN_A], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_RETURN_A;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_RETURN_D], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_RETURN_D;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_RETURN_F], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_RETURN_F;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_RETURN_I], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_RETURN_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_RETURN_L], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_RETURN_L;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_A0], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_A0;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_A1], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_A1;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_A2], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_A2;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_A3], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_A3;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_AA], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_AA;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_AB], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_AB;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_AC], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_AC;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_AD], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_AD;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_AF], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_AF;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_AI], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_AI;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_AL], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_AL;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_AS], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_AS;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_D0], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_D0;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_D1], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_D1;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_D2], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_D2;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_D3], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_D3;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_F0], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_F0;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_F1], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_F1;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_F2], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_F2;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_F3], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_F3;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_I0], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_I0;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_I1], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_I1;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_I2], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_I2;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_I3], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_I3;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_L0], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_L0;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_L1], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_L1;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_L2], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_L2;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_L3], 8)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_L3;
                                }
                                break;
                            }
                            case 9 : {
                                if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_COMPARE_L], 9)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_COMPARE_L;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_DUPLICATE], 9)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_DUPLICATE;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_JUMP_EQ_A], 9)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_JUMP_EQ_A;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_JUMP_EQ_I], 9)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_JUMP_EQ_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_JUMP_GE_I], 9)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_JUMP_GE_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_JUMP_GT_I], 9)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_JUMP_GT_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_JUMP_LE_I], 9)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_JUMP_LE_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_JUMP_LT_I], 9)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_JUMP_LT_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_JUMP_NE_A], 9)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_JUMP_NE_A;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_JUMP_NE_I], 9)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_JUMP_NE_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_NEW_ARRAY], 9)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_NEW_ARRAY;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_PUSH_NULL], 9)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_PUSH_NULL;
                                }
                                break;
                            }
                            case 10 : {
                                if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_CHECK_CAST], 10)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_CHECK_CAST;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_DUPLICATE2], 10)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_DUPLICATE2;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_JUMP_EQ0_I], 10)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_JUMP_EQ0_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_JUMP_EQN_A], 10)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_JUMP_EQN_A;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_JUMP_GE0_I], 10)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_JUMP_GE0_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_JUMP_GT0_I], 10)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_JUMP_GT0_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_JUMP_LE0_I], 10)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_JUMP_LE0_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_JUMP_LT0_I], 10)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_JUMP_LT0_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_JUMP_NE0_I], 10)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_JUMP_NE0_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_JUMP_NEN_A], 10)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_JUMP_NEN_A;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_MULTIPLY_D], 10)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_MULTIPLY_D;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_MULTIPLY_F], 10)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_MULTIPLY_F;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_MULTIPLY_I], 10)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_MULTIPLY_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_MULTIPLY_L], 10)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_MULTIPLY_L;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_SUBTRACT_D], 10)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_SUBTRACT_D;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_SUBTRACT_F], 10)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_SUBTRACT_F;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_SUBTRACT_I], 10)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_SUBTRACT_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_SUBTRACT_L], 10)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_SUBTRACT_L;
                                }
                                break;
                            }
                            case 11 : {
                                if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_INCREMENT_I], 11)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_INCREMENT_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_NEW_ARRAY_A], 11)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_NEW_ARRAY_A;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_NEW_ARRAY_N], 11)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_NEW_ARRAY_N;
                                }
                                break;
                            }
                            case 12 : {
                                if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_COMPARE_GT_D], 12)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_COMPARE_GT_D;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_COMPARE_GT_F], 12)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_COMPARE_GT_F;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_COMPARE_LT_D], 12)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_COMPARE_LT_D;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_COMPARE_LT_F], 12)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_COMPARE_LT_F;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_DUPLICATE_X1], 12)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_DUPLICATE_X1;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_DUPLICATE_X2], 12)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_DUPLICATE_X2;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_SHIFT_LEFT_I], 12)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_SHIFT_LEFT_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_SHIFT_LEFT_I], 12)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_SHIFT_LEFT_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_SHIFT_LEFT_L], 12)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_SHIFT_LEFT_L;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_SWITCH_TABLE], 12)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_SWITCH_TABLE;
                                }
                                break;
                            }
                            case 13 : {
                                if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_DUPLICATE2_X1], 13)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_DUPLICATE2_X1;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_DUPLICATE2_X2], 13)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_DUPLICATE2_X2;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_INVOKE_STATIC], 13)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_INVOKE_STATIC;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_SHIFT_RIGHT_I], 13)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_SHIFT_RIGHT_I;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_SHIFT_RIGHT_L], 13)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_SHIFT_RIGHT_L;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_SWITCH_SEARCH], 13)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_SWITCH_SEARCH;
                                }
                                break;
                            }
                            case 14 : {
                                if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_INVOKE_DYNAMIC], 14)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_INVOKE_DYNAMIC;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_INVOKE_SPECIAL], 14)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_INVOKE_SPECIAL;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_INVOKE_VIRTUAL], 14)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_INVOKE_VIRTUAL;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_SHIFT_RIGHT_UI], 14)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_SHIFT_RIGHT_UI;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_SHIFT_RIGHT_UL], 14)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_SHIFT_RIGHT_UL;
                                }
                                break;
                            }
                            case 15 : {
                                if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_INVOKE_FRAGMENT], 15)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_INVOKE_FRAGMENT;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_ARRAY_SIZE], 15)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_ARRAY_SIZE;
                                }
                                else if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_RETURN_FRAGMENT], 15)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_RETURN_FRAGMENT;
                                }
                                break;
                            }
                            case 17 : {
                                if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_STATIC_FIELD], 17)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_STATIC_FIELD;
                                }
                                break;
                            }
                            case 18 : {
                                if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_STATIC_FIELD], 18)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_STATIC_FIELD;
                                }
                                break;
                            }
                            case 19 : {
                                if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_LOAD_INSTANCE_FIELD], 19)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_LOAD_INSTANCE_FIELD;
                                }
                                break;
                            }
                            case 20 : {
                                if (zen_String_equals(text, length, zen_AssemblyLexer_literalNames[(int32_t)ZEN_TOKEN_INSTRUCTION_STORE_INSTANCE_FIELD], 20)) {
                                    lexer->m_type = ZEN_TOKEN_INSTRUCTION_STORE_INSTANCE_FIELD;
                                }
                                break;
                            }
                        }

                        /* Destroy the text; not required anymore. */
                        zen_String_delete(text);
                    }
                    else if (zen_AssemblyLexer_isDecimalDigit(lexer->m_la1)) {
                        if (lexer->m_la1 == '0') {
                            /* Consume and discard the '0' character. */
                            zen_AssemblyLexer_consume(lexer);

                            if (zen_AssemblyLexer_isBinaryPrefix(lexer->m_la1)) {
                                /* Binary Integer Literal */

                                /* Consume and discard the binary prefix character. */
                                zen_AssemblyLexer_consume(lexer);

                                if (zen_AssemblyLexer_isBinaryDigit(lexer->m_la1)) {
                                    /* Consume and discard the binary digit character. */
                                    zen_AssemblyLexer_consume(lexer);

                                    if (zen_AssemblyLexer_isBinaryDigitOrUnderscore(lexer->m_la1)) {
                                        uint8_t previous = '\0';
                                        while (zen_AssemblyLexer_isBinaryDigitOrUnderscore(lexer->m_la1)) {
                                            previous = lexer->m_la1;

                                            /* Consume and discard a binary digit or an underscore
                                             * character.
                                             */
                                            zen_AssemblyLexer_consume(lexer);
                                        }

                                        if (previous == '_') {
                                            // error
                                        }
                                    }

                                    if (zen_AssemblyLexer_isLetter(lexer->m_la1)) {
                                        // Error: Invalid suffix
                                    }
                                }
                                else {
                                    // Error: Expected binary digit
                                }
                            }
                            else if (zen_AssemblyLexer_isHexadecimalPrefix(lexer->m_la1)) {
                                /* Hexadecimal Integer Literal */

                                /* Consume and discard the binary prefix character. */
                                zen_AssemblyLexer_consume(lexer);

                                if (zen_AssemblyLexer_isHexadecimalDigit(lexer->m_la1)) {
                                    /* Consume and discard the hexadecimal digit character. */
                                    zen_AssemblyLexer_consume(lexer);

                                    if (zen_AssemblyLexer_isHexadecimalDigitOrUnderscore(lexer->m_la1)) {
                                        uint8_t previous = '\0';
                                        while (zen_AssemblyLexer_isHexadecimalDigitOrUnderscore(lexer->m_la1)) {
                                            previous = lexer->m_la1;

                                            /* Consume and discard a binary digit or an underscore
                                             * character.
                                             */
                                            zen_AssemblyLexer_consume(lexer);
                                        }

                                        if (previous == '_') {
                                            // error
                                        }
                                    }

                                    if (zen_AssemblyLexer_isLetter(lexer->m_la1)) {
                                        // Error: Invalid suffix
                                    }
                                }
                                else {
                                    // Error: Expected binary digit
                                }
                            }
                            else {
                                if (zen_AssemblyLexer_isOctalDigitOrUnderscore(lexer->m_la1)) {
                                    /* Octal Integer Literal */

                                    if (lexer->m_la1 == '_') {
                                        do {
                                            /* Consume and discard the '_' character. */
                                            zen_AssemblyLexer_consume(lexer);
                                        }
                                        while (lexer->m_la1 == '_');

                                        if (zen_AssemblyLexer_isOctalDigit(lexer->m_la1)) {
                                            /* Consume and discard the octal digit character. */
                                            zen_AssemblyLexer_consume(lexer);

                                            if (zen_AssemblyLexer_isOctalDigitOrUnderscore(lexer->m_la1)) {
                                                uint8_t previous = '\0';
                                                while (zen_AssemblyLexer_isOctalDigitOrUnderscore(lexer->m_la1)) {
                                                    previous = lexer->m_la1;

                                                    /* Consume and discard a binary digit or an underscore
                                                     * character.
                                                     */
                                                    zen_AssemblyLexer_consume(lexer);
                                                }

                                                if (previous == '_') {
                                                    // error
                                                }
                                            }

                                            if (zen_AssemblyLexer_isLetter(lexer->m_la1)) {
                                                // Error: Invalid suffix
                                            }
                                        }
                                        else {
                                            // Error: Expected octal digit
                                        }
                                    }
                                }
                                else {
                                    if (zen_AssemblyLexer_isDecimalDigit(lexer->m_la1)) {
                                        // error
                                    }
                                }
                            }
                        }
                        else {
                            /* Decimal Integer Literal */
                            /* Consume and discard the decimal digit character. */
                            zen_AssemblyLexer_consume(lexer);

                            if (zen_AssemblyLexer_isDecimalDigit(lexer->m_la1)) {
                                /* Consume and discard the decimal digit character. */
                                zen_AssemblyLexer_consume(lexer);

                                if (zen_AssemblyLexer_isDecimalDigitOrUnderscore(lexer->m_la1)) {
                                    uint8_t previous = '\0';
                                    while (zen_AssemblyLexer_isDecimalDigitOrUnderscore(lexer->m_la1)) {
                                        previous = lexer->m_la1;

                                        /* Consume and discard the decimal digit or underscore
                                         * character.
                                         */
                                        zen_AssemblyLexer_consume(lexer);
                                    }

                                    if (previous == '_') {
                                        // error
                                    }
                                }
                            }
                            else if (lexer->m_la1 == '_') {
                                do {
                                    /* Consume and discard the '_' character. */
                                    zen_AssemblyLexer_consume(lexer);
                                }
                                while (lexer->m_la1 == '_');

                                if (zen_AssemblyLexer_isDecimalDigitOrUnderscore(lexer->m_la1)) {
                                    uint8_t previous = '\0';
                                    while (zen_AssemblyLexer_isDecimalDigitOrUnderscore(lexer->m_la1)) {
                                        previous = lexer->m_la1;

                                        /* Consume and discard the decimal digit or underscore
                                         * character.
                                         */
                                        zen_AssemblyLexer_consume(lexer);
                                    }

                                    if (previous == '_') {
                                        // Error: Expected digit after underscore
                                    }
                                }
                                else {
                                    // Error: Expected digit
                                }
                            }

                            if (zen_AssemblyLexer_isLetter(lexer->m_la1)) {
                                // Error: Invalid suffix
                            }
                        }

                        /* The lexer has recognized an integer literal. */
                        lexer->m_type = ZEN_TOKEN_INTEGER_LITERAL;
                    }
                    else {
                        zen_AssemblyLexerError_t* error = zen_AssemblyLexer_createError(lexer, "Unknown character");
                        zen_ArrayList_add(lexer->m_errors, error);
                        /* Consume and discard the unknown character. */
                        zen_AssemblyLexer_consume(lexer);
                        /* The lexer has encountered an unrecognized character. */
                        lexer->m_type = ZEN_TOKEN_UNKNOWN;
                    }
                    break;
                }
            }
        }

        zen_Token_t* newToken = zen_AssemblyLexer_createToken(lexer);
        zen_AssemblyLexer_emit(lexer, newToken);
    }

    zen_Token_t* next = (zen_Token_t*)zen_ArrayQueue_peek(lexer->m_tokens);
    zen_ArrayQueue_dequeue(lexer->m_tokens);
    return next;
}

bool zen_AssemblyLexer_isBinaryPrefix(int32_t codePoint) {
    return (codePoint == 'b') || (codePoint == 'B');
}

bool zen_AssemblyLexer_isBinaryDigit(int32_t codePoint) {
	return (codePoint == '0') || (codePoint == '1');
}

bool zen_AssemblyLexer_isBinaryDigitOrUnderscore(int32_t codePoint) {
	return (codePoint == '0') ||
           (codePoint == '1') ||
           (codePoint == '_');
}

bool zen_AssemblyLexer_isBasicEscapeSequence(int32_t codePoint) {
	return (codePoint == 'b') ||
           (codePoint == 'f') ||
           (codePoint == 'n') ||
           (codePoint == 'r') ||
           (codePoint == 't') ||
           (codePoint == '\\') ||
           (codePoint == '\"') ||
           (codePoint == '\'');
}

bool zen_AssemblyLexer_isDecimalDigit(int32_t codePoint) {
    return (codePoint >= '0') && (codePoint <= '9');
}

bool zen_AssemblyLexer_isDecimalDigitOrUnderscore(int32_t codePoint) {
    return zen_AssemblyLexer_isDecimalDigit(codePoint) || (codePoint == '_');
}

bool zen_AssemblyLexer_isIdentifierStart(int32_t codePoint) {
    return ((codePoint >= 'a') && (codePoint <= 'z')) ||
           ((codePoint >= 'A') && (codePoint <= 'Z'));
}

bool zen_AssemblyLexer_isIdentifierPart(int32_t codePoint) {
    return ((codePoint >= 'a') && (codePoint <= 'z')) ||
           ((codePoint >= 'A') && (codePoint <= 'Z')) ||
           ((codePoint >= '0') && (codePoint <= '9')) ||
           (codePoint == '_');
}

bool zen_AssemblyLexer_isLetter(int32_t codePoint) {
    return ((codePoint >= 'a') && (codePoint <= 'z')) ||
           ((codePoint >= 'A') && (codePoint <= 'Z'));
}

bool zen_AssemblyLexer_isLetterOrDigit(int32_t codePoint) {
    return zen_AssemblyLexer_isLetter(codePoint) || zen_AssemblyLexer_isDecimalDigit(codePoint);
}

bool zen_AssemblyLexer_isHexadecimalPrefix(int32_t codePoint) {
    return (codePoint == 'x') || (codePoint == 'X');
}

bool zen_AssemblyLexer_isHexadecimalDigit(int32_t codePoint) {
    return zen_AssemblyLexer_isDecimalDigit(codePoint) ||
           ((codePoint >= 'a') && (codePoint <= 'f')) ||
           ((codePoint >= 'A') && (codePoint <= 'F'));
}

bool zen_AssemblyLexer_isHexadecimalDigitOrUnderscore(int32_t codePoint) {
    return zen_AssemblyLexer_isHexadecimalDigit(codePoint) || (codePoint == '_');
}

bool zen_AssemblyLexer_isOctalDigit(int32_t codePoint) {
    return (codePoint >= '0') && (codePoint <= '7');
}

bool zen_AssemblyLexer_isOctalDigitOrUnderscore(int32_t codePoint) {
    return zen_AssemblyLexer_isOctalDigit(codePoint) || (codePoint == '_');
}