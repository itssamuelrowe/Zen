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

#include <jtk/collection/Pair.h>

#include <com/onecube/zen/compiler/ast/context/Context.h>
#include <com/onecube/zen/compiler/parser/Parser.h>
#include <com/onecube/zen/compiler/lexer/TokenStream.h>
#include <com/onecube/zen/compiler/support/StackTrace.h>

/*******************************************************************************
 * Parser                                                                      *
 *******************************************************************************/

const char zen_Parser_ruleNames[][50] = {
    "<unknown>",
    "<terminal>",

    "compilationUnit",
    "importDeclaration",
    "annotatedComponentDeclaration",
    "annotations",
    "annotation",
    "annotationType",
    "annotationAttribute",
    "componentDeclaration",
    "functionDeclaration",
    "functionParameters",
    "functionBody",
    "statementSuite",
    "simpleStatement",
    "statement",
    "emptyStatement",
    "variableDeclaration",
    "variableDeclarator",
    "constantDeclaration",
    "constantDeclarator",
    "assertStatement",
    "breakStatement",
    "continueStatement",
    "returnStatement",
    "throwStatement",
    "compoundStatement",
    "ifStatement",
    "ifClause",
    "elseIfClause",
    "elseClause",
    "iterativeStatement",
    "labelClause",
    "whileStatement",
    "forStatement",
    "forParameters",
    "tryStatement",
    "tryClause",
    "catchClause",
    "catchFilter",
    "typeName",
    "finallyClause",
    "synchronizeStatement",
    "withStatement",
    "withParameters",
    "withParameter",
    "classDeclaration",
    "classClassExtendsClause",
    "classSuite",
    "classMember",
    // "constructorDeclaration",
    // "enumerationDeclaration",
    // "enumerationBaseClause",
    // "enumerationSuite",
    // "enumerate",
    "expressions",
    "expression",
    "assignmentExpression",
    "conditionalExpression",
    "logicalOrExpression",
    "logicalAndExpression",
    "inclusiveOrExpression",
    "exclusiveOrExpression",
    "andExpression",
    "equalityExpression",
    "relationalExpression",
    "shiftExpression",
    "additiveExpression",
    "multiplicativeExpression",
    "unaryExpression",
    "postfixExpression",
    "subscript",
    "functionArguments",
    "memberAccess",
    "postfixOperator",
    "primaryExpression",
    "mapExpression",
    "mapEntries",
    "mapEntry",
    "listExpression",
    "newExpression"
};

/* Constructor */

zen_Parser_t* zen_Parser_new(zen_Compiler_t* compiler, zen_TokenStream_t* tokens) {
    jtk_Assert_assertObject(compiler, "The specified compiler is null.");

    zen_Parser_t* parser = zen_Memory_allocate(zen_Parser_t, 1);
    parser->m_compiler = compiler;
    parser->m_tokens = tokens;
    parser->m_followSet = jtk_Memory_allocate(zen_TokenType_t, 128);
    parser->m_followSetSize = 0;
    parser->m_followSetCapacity = 128;
    parser->m_recovery = false;

    return parser;
}

/* Destructor */

void zen_Parser_delete(zen_Parser_t* parser) {
    jtk_Assert_assertObject(parser, "The specified parser is null.");

    jtk_Memory_deallocate(parser->m_followSet);
    jtk_Memory_deallocate(parser);
}

/* Rule Name */

const char* zen_Parser_getRuleName(zen_ASTNodeType_t type) {
    return zen_Parser_ruleNames[(int32_t)type];
}

/* Syntax Error */

// void zen_Parser_reportSyntaxError(zen_Parser_t* parser, zen_Token_t* token,
//     const char* message) {
//     fprintf(stderr, "[error] %d-%d:%d-%d: %s\n", token->m_startLine + 1,
//         token->m_stopLine + 1, token->m_startColumn + 1, token->m_stopColumn + 1,
//         message);
// }

/* Terminal Node */

zen_ASTNode_t* zen_Parser_newTerminalNode(zen_ASTNode_t* node, zen_Token_t* token) {
    zen_ASTNode_t* terminalNode = zen_ASTNode_new(node);
    terminalNode->m_type = ZEN_AST_NODE_TYPE_TERMINAL;
    terminalNode->m_context = token;
    terminalNode->m_contextDestructor = NULL;
    terminalNode->m_enumerateContextChildren = NULL;

    return terminalNode;
}

/* Recover */

/* When the parser encounters an invalid input, the current rule cannot continue,
 * so the parser recovers by skipping tokens until it a possible resynchronized
 * state is achived. The control is then returned to the calling rule.
 * This technique is known as the panic mode strategy.
 *
 * The trick here is to discard tokens only until the lookahead token is
 * something that the parent rule of the current rule expects. For example,
 * if there is a syntax error within a throw statement, the parser discards
 * tokens until a newline token or other relevant token is encountered.
 */
void zen_Parser_recover(zen_Parser_t* parser) {
    /* The parser is now in recovery mode; flag other parts of the parser. */
    parser->m_recovery = true;

    if (parser->m_followSetSize > 0) {
        zen_Token_t* lt1 = zen_TokenStream_lt(parser->m_tokens, 1);
        /* The parser tries to recover until a token from the follow set or
         * the end-of-stream token is encountered.
         */
        while (lt1->m_type != ZEN_TOKEN_END_OF_STREAM) {
            /* When searching for a follow token, the parser prioritizes tokens
             * that are expected by the nearest rule in the rule invocation
             * stack. This is why, the linear search algorithm is applied in a
             * reverse fashion over the follow set.
             */
            int32_t i;
            for (i = parser->m_followSetSize - 1; i >= 0; i--) {
                if (lt1->m_type == parser->m_followSet[i]) {
                    /* A token from the follow set was encountered. The parser
                     * may have resynchronized with the input.
                     */
                    goto afterDiscard;
                }
            }
            /* Consume and discard the current token. */
            zen_TokenStream_consume(parser->m_tokens);
            /* Update the lookahead token. */
            lt1 = zen_TokenStream_lt(parser->m_tokens, 1);
        }
        afterDiscard:
            ;
    }
}

bool zen_Parser_ensureFollowSetSpace(zen_Parser_t* parser, int32_t capacity) {
    jtk_Assert_assertObject(parser, "The specified parser is null.");

    bool result = false;
    if (capacity > 0) {
        int32_t currentCapacity = parser->m_followSetCapacity;
        int32_t minimumCapacity = (parser->m_followSetSize + capacity);
        int32_t requiredCapacity = minimumCapacity - currentCapacity;
        if (requiredCapacity > 0) {
            int32_t newCapacity = (currentCapacity * 2) + 2;
            if ((newCapacity - minimumCapacity) < 0) {
                newCapacity = minimumCapacity;
            }

            if (newCapacity < 0) {
                if (minimumCapacity < 0) {
                    /* Report an error, the requested capacity is too big. */
                    printf("[internal error] The requested capacity is too big. The parser has run out of memory.\n");
                }
                else {
                    /* Fall back, the new capacity was recommened by this function. */
                    newCapacity = JTK_INTEGER_MAX_VALUE;
                }
            }

            /* Do not allocate a new buffer if an out-of-memory error was
             * reported.
             */
            if (newCapacity > 0) {
                uint8_t* temporary = parser->m_followSet;
                parser->m_followSet = (uint8_t*)jtk_Arrays_copyOfEx_b(
                    parser->m_followSet, parser->m_followSet, newCapacity, 0,
                    false);
                parser->m_followSetCapacity = newCapacity;
                jtk_Memory_deallocate(temporary);
            }
        }
    }
    return result;
}

void zen_Parser_pushFollowToken(zen_Parser_t* parser, zen_TokenType_t type) {
    jtk_Assert_assertObject(parser, "The specified parser is null.");

    /* Make sure that the set is large enough to hold another token type. */
    zen_Parser_ensureFollowSetSpace(parser, parser->m_followSetSize + 1);
    /* Insert the follow token to the set. */
    parser->m_followSet[parser->m_followSetSize] = type;
    /* Increment the size of the follow set. */
    parser->m_followSetSize++;
}

void zen_Parser_popFollowToken(zen_Parser_t* parser) {
    jtk_Assert_assertTrue(parser->m_followSetSize > 0, "The follow set is empty.");

    parser->m_followSetSize--;
}

/* Match */

void zen_Parser_match(zen_Parser_t* parser, zen_TokenType_t type) {
    jtk_Assert_assertObject(parser, "The specified parser is null.");

    zen_Parser_matchAndYield(parser, type);
}

void zen_Parser_reportAndRecover(zen_Parser_t* parser, zen_TokenType_t expected) {
    /* Do not report the error if the parser is in recovery mode. Otherwise,
    * duplicate syntax errors will be reported to the end user.
    */
    if (!parser->m_recovery) {
        zen_Token_t* lt1 = zen_TokenStream_lt(parser->m_tokens, 1);
        zen_Compiler_t* compiler = parser->m_compiler;
        zen_ErrorHandler_t* errorHandler = compiler->m_errorHandler;
        zen_ErrorHandler_handleSyntacticalError(errorHandler, parser,
            ZEN_ERROR_CODE_UNEXPECTED_TOKEN, lt1, expected);
    }

    /* Try to resychronize the parser with the input. */
    zen_Parser_recover(parser);
}

zen_Token_t* zen_Parser_matchAndYield(zen_Parser_t* parser, zen_TokenType_t type) {
    jtk_Assert_assertObject(parser, "The specified parser is null.");

    zen_Token_t* lt1 = zen_TokenStream_lt(parser->m_tokens, 1);

    if (lt1->m_type == type) {
        /* The token expected by the parser was found. If we the parser is
         * in error recovery, turn it off.
         */
        parser->m_recovery = false;


        /* The token stream prohibts consumption of end-of-stream
         * token.
         */
        if (lt1->m_type != ZEN_TOKEN_END_OF_STREAM) {
            zen_TokenStream_consume(parser->m_tokens);
        }
    }
    else {
        zen_Parser_reportAndRecover(parser, type);
    }
    return lt1;
}

// Reset

void zen_Parser_reset(zen_Parser_t* parser, zen_TokenStream_t* tokens) {
    jtk_Assert_assertObject(parser, "The specified parser is null.");

    parser->m_tokens = tokens;
    parser->m_followSetSize = 0;
    parser->m_recovery = false;
    parser->m_mainComponent = ZEN_AST_NODE_TYPE_UNKNOWN;
}

/*
 * -----------------------------------------------------------------------------
 * How do we construct the abstract syntax tree?
 * -----------------------------------------------------------------------------
 *
 * Every parent rule allocates and passes an AST node to a child
 * rule, except for the entry rules. In this case, the user has to
 * manually allocate an AST node and pass. A node is associated
 * with a context. A context has specific data related to a given
 * rule. It is recognized by the m_type attribute within the
 * zen_ASTNode_t structure.
 *
 * -----------------------------------------------------------------------------
 * How are abstract syntax trees, contexts, and rule specific data
 * destroyed?
 * -----------------------------------------------------------------------------
 *
 * Every rule receives a node. Note that within the abstract
 * syntax tree this node represents the current rule and not
 * the parent rule. Any allocated object should be immediately
 * attached to the node or the rules context. Because the parser
 * long jumps when a syntax error is discovered. Failing to
 * attach a resource may result in dangling pointers, or worst
 * case scenario: undefined behaviour.
 *
 * A rule first creates its context and attachs it to the
 * given node. Further, any object allocated within a
 * rule is immediately attached to its context. Thus,
 * all allocated objects are destroyed even if the parser
 * long jumps using the m_contextDestructor function within
 * the zen_ASTNode_t structure.
 *
 * The special function zen_ASTNode_delete() should be invoked
 * on the root node. zen_ASTNode_delete() travereses down the
 * AST destroying each node it encounters and all its descendants.
 * The context of a node is destroyed by invoking the
 * m_contextDestructor, provided that it is non-null. Otherwise,
 * the context is ignored.
 */

/*
 * compilationUnit
 * :	NEWLINE*
 *      importDeclaration*
 *      annotatedComponentDeclaration*
 *      EOF
 * ;
 *
 */
void zen_Parser_compilationUnit(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

	/* Create and attach the context of this rule to the given node. */
	zen_CompilationUnitContext_t* context = zen_CompilationUnitContext_new(node);

    /* A newline token may optionally appear at the beginning of the source
     * code. Such arbitrary newline tokens are recognized here in order
     * to parse inputs with such construct.
     *
     * // This is a comment.
     * function main
     *     ...
     */
    while (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_NEWLINE) {
        /* Consume the newline token. */
        zen_TokenStream_consume(parser->m_tokens);
    }

	/* Zero or more import declarations may occur in the source code.
	 * Therefore, repeatedly parse import declarations as long as
	 * LA(1) is the 'import' keyword.
	 *
	 * The following code represents: importDeclaration*
	 */
    while (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_KEYWORD_IMPORT) {
		zen_ASTNode_t* importDeclaration = zen_ASTNode_new(node);
        zen_Parser_importDeclaration(parser, importDeclaration);

		jtk_ArrayList_add(context->m_importDeclarations, importDeclaration);
    }

	/* The following text describes the algorithm employed previously.
     * The implementation of the previous algorithm was commented out.
     * The current implementation extended the grammar to accomodate
     * arbitrary newlines at the beginning of the input, between components,
     * and end of the input. Albeit deprecated, this text has some valuable
     * information.
     *
     * Zero or more function declarations, class declarations, and enum
	 * declarations may occur in the source code. Interestingly, the parser
	 * imposes no restrictions in the order in which they appear.
	 * Things get complicated when each declaration may be annotated
	 * individually zero or more times. Therefore, the following code
	 * is not a direct translation of the grammar.
	 *
	 * The following actions are performed while parsing syntax
	 * of the form: (annotation (functionDeclaration | classDeclaration | enumerationDeclaration))*
	 * Firstly, we parse one or more annotations if LA(1) is the '@' token.
	 * Otherwise, we parse function declaration, class declaration, enum
	 * declaration if LA(1) is 'function', 'class', 'enum', respectively.
     * Annotations and declarations are paired within the AST.
	 *
	 * When an unknown token appears in the loop, we stop the loop
	 * immediately. After which the 'end-of-stream' token is expected.
	 * Moreover, a syntax error is reported if annotation(s) are not
	 * followed by a declaration.
	 */
    /*
    while (true) {
        jtk_Pair_t* declarationPair = jtk_Pair_new();
        jtk_ArrayList_add(context->m_declarations, declarationPair);

        if (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_AT) {
			zen_ASTNode_t* annotations = zen_ASTNode_new(node);
            declarationPair->m_leftElement = annotations;
            zen_Parser_annotations(parser, annotations);
        }

        switch (zen_TokenStream_la(parser->m_tokens, 1)) {
            case ZEN_TOKEN_KEYWORD_FUNCTION: {
				zen_ASTNode_t* functionDeclaration = zen_ASTNode_new(node);
                declarationPair->m_rightElement = functionDeclaration;
				zen_Parser_functionDeclaration(parser, functionDeclaration, 0);
                break;
            }

            case ZEN_TOKEN_KEYWORD_CLASS: {
				zen_ASTNode_t* classDeclaration = zen_ASTNode_new(node);
                declarationPair->m_rightElement = classDeclaration;
				zen_Parser_classDeclaration(parser, classDeclaration);
                break;
            }

            case ZEN_TOKEN_KEYWORD_ENUM: {
				zen_ASTNode_t* enumerationDeclaration = zen_ASTNode_new(node);
                declarationPair->m_rightElement = enumerationDeclaration;
				zen_Parser_enumerationDeclaration(parser, enumerationDeclaration);
                break;
            }

            default: {
				// Check if annotation(s) is not followed by a declaration.
				if ((declarationPair->m_leftElement != NULL) &&
                    (declarationPair->m_rightElement == NULL)) {
                    // Syntax Error: Expected function, class, or enum declaration after annotation
                }
                goto breakPoint;
            }
        }
    }
    breakPoint:
    */

    while (zen_Parser_isAnnotatedComponentDeclarationFollow(zen_TokenStream_la(parser->m_tokens, 1))) {
        zen_ASTNode_t* annotatedComponentDeclaration = zen_ASTNode_new(node);
        jtk_ArrayList_add(context->m_annotatedComponentDeclarations, annotatedComponentDeclaration);
        zen_Parser_annotatedComponentDeclaration(parser, annotatedComponentDeclaration);
    }

	/* We are expecting an 'end of stream' here. */
    zen_Parser_match(parser, ZEN_TOKEN_END_OF_STREAM);

    zen_StackTrace_exit();
}

/*
 * importDeclaration
 * :    'import' importTarget NEWLINE
 * ;
 *
 * TODO:
 *
 * importTarget
 * :    IDENTIFIER ('.' IDENTIFIER)* ('.' '*' | ('as' IDENTIFIER))?
 * ;
 *
 * The following function combines both the rules. This measure was
 * taken to avoid redundant nodes in the AST.
 */
void zen_Parser_importDeclaration(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

	zen_ImportDeclarationContext_t* context = zen_ImportDeclarationContext_new(node);

	/* An import statement begins with the 'import' keyword. Therefore,
	 * we are expecting it.
	 */
    zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_IMPORT);

	/* The user is required to specify the symbol he/she wants to
	 * import. The following code parses the import target.
	 */

    /* We are building an abstract syntax tree (AST). Therefore, we
	 * discard the '.' '*' tokens and use a boolean variable to mark
	 * wildcard imports. Thus, we uphold abstraction.
	 *
	 * By default, we assume that we have no wildcard.
	 */
	context->m_wildcard = false;

    /* If importTarget fails, discard tokens until the newline token is
     * encountered.
     */
    zen_Parser_pushFollowToken(parser, ZEN_TOKEN_NEWLINE);

	/* The user is expected to specify at least, one identifier.
	 * Consume it. The consumed identifier saved for later inspection.
	 */
    zen_Token_t* identifier = zen_Parser_matchAndYield(parser, ZEN_TOKEN_IDENTIFIER);
    zen_ASTNode_t* identifierNode = zen_Parser_newTerminalNode(node, identifier);

	jtk_ArrayList_add(context->m_identifiers, identifierNode);

	/* Optionally, the user may specify more identifiers (with each identifier
	 * separated by the '.' token. Therefore, we repeatedly consume the '.' and
	 * identifier tokens as long as LA(1) is the '.' token and LA(2) is the
	 * identifier token.
	 */
    while ((zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_DOT) &&
           (zen_TokenStream_la(parser->m_tokens, 2) == ZEN_TOKEN_IDENTIFIER)) {
		/* Consume and discard the '.' token. */
        zen_TokenStream_consume(parser->m_tokens);

		/* The consumed identifier is saved for later inspection.
		 */
        identifier = zen_TokenStream_lt(parser->m_tokens, 1);
        identifierNode = zen_Parser_newTerminalNode(node, identifier);
        zen_TokenStream_consume(parser->m_tokens);
		jtk_ArrayList_add(context->m_identifiers, identifierNode);
    }

	/* Optionally, the user may specify a wildcard; recognized when
	 * LA(1) is the '.' token and LA(2) is the '*' token.
	 */
    if (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_DOT) {
		zen_TokenStream_consume(parser->m_tokens);
		zen_Parser_match(parser, ZEN_TOKEN_ASTERISK);
        context->m_wildcard = true;
    }

    /* Pop the newline token from the follow set. */
    zen_Parser_popFollowToken(parser);

	/* The import declaration is terminated with a newline.
	 * Therefore, we are expecting it here.
	 */
    zen_Parser_match(parser, ZEN_TOKEN_NEWLINE);

    zen_StackTrace_exit();
}

bool zen_Parser_isAnnotatedComponentDeclarationFollow(zen_TokenType_t token) {
    return (token == ZEN_TOKEN_AT) ||
           (token == ZEN_TOKEN_KEYWORD_FUNCTION) ||
           (token == ZEN_TOKEN_KEYWORD_CLASS) ||
           (token == ZEN_TOKEN_KEYWORD_ENUM);
}

/*
 * annotatedComponentDeclaration
 * :    annotations? componentDeclaration
 * ;
 */
void zen_Parser_annotatedComponentDeclaration(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_AnnotatedComponentDeclarationContext_t* context = zen_AnnotatedComponentDeclarationContext_new(node);

    if (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_AT) {
        zen_ASTNode_t* annotations = zen_ASTNode_new(node);
        context->m_annotations = annotations;
        zen_Parser_annotations(parser, annotations);
    }

    zen_ASTNode_t* componentDeclaration = zen_ASTNode_new(node);
    context->m_componentDeclaration = componentDeclaration;
    zen_Parser_componentDeclaration(parser, componentDeclaration);

    zen_StackTrace_exit();
}

/*
 * annotations
 * :    annotation+
 * ;
 */
void zen_Parser_annotations(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

	zen_AnnotationsContext_t* context = zen_AnnotationsContext_new(node);

    /* This rule expects at least, one annotation. Further annotations
	 * are parsed if LA(1) is the '@' token.
	 */
	do {
		zen_ASTNode_t* annotation = zen_ASTNode_new(node);
		jtk_ArrayList_add(context->m_annotations, annotation);

		zen_Parser_annotation(parser, annotation);
    }
    while (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_AT);

    zen_StackTrace_exit();
}

/*
 * annotation
 * :    '@' annotationType annotationAttribute* NEWLINE
 * ;
 */
void zen_Parser_annotation(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

	zen_AnnotationContext_t* context = zen_AnnotationContext_new(node);

    /* We are expecting the '@' token here. */
    zen_Parser_match(parser, ZEN_TOKEN_AT);

    /* If annotationType fails, discard tokens until the newline token is
     * encountered.
     */
    zen_Parser_pushFollowToken(parser, ZEN_TOKEN_NEWLINE);

    /* We are expecting an annotation type here. */
    zen_ASTNode_t* annotationType = zen_ASTNode_new(node);
	context->m_annotationType = annotationType;
	zen_Parser_annotationType(parser, annotationType);

    /* An optional attribute list may be specified, recognized
	 * if LA(1) is an identifier.
	 */
    while (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_IDENTIFIER) {
		zen_ASTNode_t* annotationAttribute = zen_ASTNode_new(node);
		jtk_ArrayList_add(context->m_annotationAttributes, annotationAttribute);
        zen_Parser_annotationAttribute(parser, annotationAttribute);
    }

    /* Pop the newline token from the follow set. */
    zen_Parser_popFollowToken(parser);

    /* We are expecting a newline token here. The newline token marks the
	 * end of an annotation.
	 */
    zen_Parser_match(parser, ZEN_TOKEN_NEWLINE);

    zen_StackTrace_exit();
}

/*
 * annotationType
 * :    IDENTIFIER ('.' IDENTIFIER)*
 * ;
 */
void zen_Parser_annotationType(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_AnnotationTypeContext_t* context = zen_AnnotationTypeContext_new(node);

    /* We are expecting an IDENTIFIER token here. */
    zen_Token_t* identifier = zen_Parser_matchAndYield(parser, ZEN_TOKEN_IDENTIFIER);
    zen_ASTNode_t* identifierNode = zen_Parser_newTerminalNode(node, identifier);
    jtk_ArrayList_add(context->m_identifiers, identifierNode);

    /* Optionally, the user may specify more identifiers separated
     * by the '.' token.
     */
    while (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_DOT) {
        /* Consume and discard the '.' token. */
        zen_TokenStream_consume(parser->m_tokens);

        /* We are expecting an identifier here. The consumed token is
		 * saved for later inspection.
		 */
        identifier = zen_Parser_matchAndYield(parser, ZEN_TOKEN_IDENTIFIER);
        identifierNode = zen_Parser_newTerminalNode(node, identifier);
        jtk_ArrayList_add(context->m_identifiers, identifierNode);
    }

    zen_StackTrace_exit();
}

/*
 * annotationAttribute
 * :    IDENTIFIER '=' literal
 * ;
 */
void zen_Parser_annotationAttribute(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

	zen_AnnotationAttributeContext_t* context = zen_AnnotationAttributeContext_new(node);

	/* We are expecting an identifier here. The consumed token is
	 * saved for later inspection.
	 */
    zen_Token_t* identifier = zen_Parser_matchAndYield(parser, ZEN_TOKEN_IDENTIFIER);
	context->m_identifier = zen_Parser_newTerminalNode(node, identifier);

	/* The attribute key and value pair is separated by the '=' token.
	 * Match and discard the '=' token.
	 */
    zen_Parser_match(parser, ZEN_TOKEN_EQUAL);

	/* The value should always be a literal. As of now, expressions cannot
	 * be specified to attributes. Parse a literal.
	 */
    if (zen_Parser_isLiteralFollow(zen_TokenStream_la(parser->m_tokens, 1))) {
        zen_Token_t* literal = zen_TokenStream_lt(parser->m_tokens, 1);
        context->m_literal = zen_Parser_newTerminalNode(node, literal);
        /* Consume the literal token. */
        zen_TokenStream_consume(parser->m_tokens);
    }
    else {
        // TODO: string, integer, decimal, boolean, or null literal
        zen_Parser_reportAndRecover(parser, ZEN_TOKEN_STRING_LITERAL);
    }

    zen_StackTrace_exit();
}

/*
 * componentDeclaration
 * :    functionDeclaration
 * |    classDeclaration
 * // |    enumerationDeclaration
 * ;
 */
void zen_Parser_componentDeclaration(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_Compiler_t* compiler = parser->m_compiler;
    zen_ErrorHandler_t* errorHandler = compiler->m_errorHandler;
    zen_ComponentDeclarationContext_t* context = zen_ComponentDeclarationContext_new(node);

    zen_ASTNodeType_t component = ZEN_AST_NODE_TYPE_UNKNOWN;
    zen_Token_t* lt1 = zen_TokenStream_lt(parser->m_tokens, 1);
    switch (lt1->m_type) {
        case ZEN_TOKEN_KEYWORD_FUNCTION: {
            component = ZEN_AST_NODE_TYPE_FUNCTION_DECLARATION;

            zen_ASTNode_t* functionDeclaration = zen_ASTNode_new(node);
            context->m_component = functionDeclaration;
            zen_Parser_functionDeclaration(parser, functionDeclaration, 0);
            break;
        }

        case ZEN_TOKEN_KEYWORD_CLASS: {
            component = ZEN_AST_NODE_TYPE_CLASS_DECLARATION;

            zen_ASTNode_t* classDeclaration = zen_ASTNode_new(node);
            context->m_component = classDeclaration;
            zen_Parser_classDeclaration(parser, classDeclaration);
            break;
        }

        // case ZEN_TOKEN_KEYWORD_ENUM: {
        //     zen_ASTNode_t* enumerationDeclaration = zen_ASTNode_new(node);
        //     context->m_component = enumerationDeclaration;
        //     zen_Parser_enumerationDeclaration(parser, enumerationDeclaration);
        //     break;
        // }

        default: {
            // TODO: Expected function or class
            zen_Parser_reportAndRecover(parser, ZEN_TOKEN_KEYWORD_CLASS);
            break;
        }
    }

    if (parser->m_mainComponent == ZEN_AST_NODE_TYPE_UNKNOWN) {
        parser->m_mainComponent = component;
    }
    else {
        if (parser->m_mainComponent != component) {
            zen_ErrorHandler_handleSemanticalError(errorHandler, parser,
                ZEN_ERROR_CODE_CANNOT_DECLARE_MULTIPLE_TYPES_OF_COMPONENT, lt1);
        }
    }


    zen_StackTrace_exit();
}

/*
 * functionDeclaration
 * :    'function' functionIdentifier functionParameters (functionBody | NEWLINE)
 * ;
 *
 * functionIdentifier
 * :    IDENTIFIER
 * // |    'static'
 * |    'new'
 * ;
 */
void zen_Parser_functionDeclaration(zen_Parser_t* parser, zen_ASTNode_t* node,
    uint32_t modifiers) {
    zen_StackTrace_enter();

	zen_FunctionDeclarationContext_t* context = zen_FunctionDeclarationContext_new(node);

    zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_FUNCTION);

    switch (zen_TokenStream_la(parser->m_tokens, 1)) {
        case ZEN_TOKEN_IDENTIFIER:
        case ZEN_TOKEN_KEYWORD_NEW:
        case ZEN_TOKEN_KEYWORD_STATIC: {
            zen_Token_t* identifierOrKeyword = zen_TokenStream_lt(parser->m_tokens, 1);
            context->m_identifier = zen_Parser_newTerminalNode(node, identifierOrKeyword);
            zen_TokenStream_consume(parser->m_tokens);
            break;
        }

        default: {
            // TODO: Expected identifier, new, or static
            zen_Parser_reportAndRecover(parser, ZEN_TOKEN_IDENTIFIER);
        }
    }

    /* Previously, functions and constructors were two different entities.
     * The former construct has been removed.
     */
    // zen_Token_t* identifier = zen_Parser_matchAndYield(parser, ZEN_TOKEN_IDENTIFIER);
    // context->m_identifier = zen_Parser_newTerminalNode(node, identifier);

    /* If functionParameters fails, discard tokens until the newline token is
     * encountered.
     */
    zen_Parser_pushFollowToken(parser, ZEN_TOKEN_NEWLINE);

    /* TODO:
     * The parenthesis are considered as enclosures by the lexer. Any
     * newline token in between parenthesis are generated on the hidden
     * channel. Therefore, the error recovery strategy does not work
     * well unless the hidden channel is also considered.
     */
    zen_ASTNode_t* functionParameters = zen_ASTNode_new(node);
    context->m_functionParameters = functionParameters;
    zen_Parser_functionParameters(parser, functionParameters);

    /* Pop the newline token from the follow set. */
    zen_Parser_popFollowToken(parser);

    if (!zen_Modifier_hasNative(modifiers) && !zen_Modifier_hasAbstract(modifiers)) {
        zen_ASTNode_t* functionBody = zen_ASTNode_new(node);
        context->m_functionBody = functionBody;
        zen_Parser_functionBody(parser, functionBody);
    }
    else {
        zen_Parser_match(parser, ZEN_TOKEN_NEWLINE);
    }

    zen_StackTrace_exit();
}

/*
 *  functionParameters
 *  :   '(' actualFunctionParameters? ')'
 *  ;
 *
 * actualFunctionParameters
 * :    IDENTIFIER (',' IDENTIFIER)* (',' '...' IDENTIFIER)?
 * |    '...' IDENTIFIER
 * ;
 *
 * The following function combines both the rules. This measure was
 * taken to avoid redundant nodes in the AST.
 */
void zen_Parser_functionParameters(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

	zen_FunctionParametersContext_t* context = zen_FunctionParametersContext_new(node);

    /* Match and discard the '(' token. */
    zen_Parser_match(parser, ZEN_TOKEN_LEFT_PARENTHESIS);

    /* Optionally, the programmer may specify one or more parameters.
     * The following statements parse both fixed and variable parameters.
     */
    switch (zen_TokenStream_la(parser->m_tokens, 1)) {
        case ZEN_TOKEN_IDENTIFIER: {
            zen_Token_t* fixedParameter = zen_TokenStream_lt(parser->m_tokens, 1);
            zen_ASTNode_t* fixedParameterNode = zen_Parser_newTerminalNode(node, fixedParameter);
			jtk_ArrayList_add(context->m_fixedParameters, fixedParameterNode);
			zen_TokenStream_consume(parser->m_tokens);

            while (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_COMMA) {
                /* Consume and discard the ',' token. */
                zen_TokenStream_consume(parser->m_tokens);
                switch (zen_TokenStream_la(parser->m_tokens, 1)) {
                    case ZEN_TOKEN_IDENTIFIER: {
                        fixedParameter = zen_TokenStream_lt(parser->m_tokens, 1);
                        fixedParameterNode = zen_Parser_newTerminalNode(node, fixedParameter);
						jtk_ArrayList_add(context->m_fixedParameters, fixedParameterNode);
                        zen_TokenStream_consume(parser->m_tokens);
                        break;
                    }

                    case ZEN_TOKEN_ELLIPSIS: {
						/* Consume the '...' token. */
                        zen_TokenStream_consume(parser->m_tokens);
                        zen_Token_t* variableParameter = zen_Parser_matchAndYield(parser, ZEN_TOKEN_IDENTIFIER);
						context->m_variableParameter = zen_Parser_newTerminalNode(node, variableParameter);
                        goto loopExit;
                    }

					default: {
                        zen_Parser_reportAndRecover(parser, ZEN_TOKEN_IDENTIFIER);
                        break;
					}
                }
            }
            loopExit:
            break;
        }

        case ZEN_TOKEN_ELLIPSIS: {
			/* Consume and discard the '...' token. */
            zen_TokenStream_consume(parser->m_tokens);
            zen_Token_t* variableParameter = zen_Parser_matchAndYield(parser, ZEN_TOKEN_IDENTIFIER);
			context->m_variableParameter = zen_Parser_newTerminalNode(node, variableParameter);
            break;
        }
    }

    /* Match and discard the ')' token. */
    zen_Parser_match(parser, ZEN_TOKEN_RIGHT_PARENTHESIS);

    zen_StackTrace_exit();
}

/*
 * functionBody
 * :    statementSuite
 * ;
 */
void zen_Parser_functionBody(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

	zen_FunctionBodyContext_t* context = zen_FunctionBodyContext_new(node);

	zen_ASTNode_t* statementSuite = zen_ASTNode_new(node);
	context->m_statementSuite = statementSuite;
    zen_Parser_statementSuite(parser, statementSuite);

    zen_StackTrace_exit();
}

/* In order to help the users read code easily, the simple statements were
 * removed from statement suites. Previously, a statement suite was either
 * a simple statement or a block consisting of a newline at the begining,
 * indentation and dedentation.
 */
/*
 * statementSuite
 * |    NEWLINE INDENT statement+ DEDENT
 * ;
 */
void zen_Parser_statementSuite(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

	zen_StatementSuiteContext_t* context = zen_StatementSuiteContext_new(node);

    // zen_TokenType_t la1 = zen_TokenStream_la(parser->m_tokens, 1);

    /* if (zen_Parser_isSimpleStatementFollow(la1)) {
        zen_ASTNode_t* simpleStatement = zen_ASTNode_new(node);
        context->m_simpleStatement = simpleStatement;
        zen_Parser_simpleStatement(parser, simpleStatement);
    }
    */

    // if (la1 == ZEN_TOKEN_NEWLINE) {
        /* Consume and discard the newline token. */
        zen_Parser_match(parser, ZEN_TOKEN_NEWLINE);
        /* Consume and discard the dedent token. */
        zen_Parser_match(parser, ZEN_TOKEN_INDENTATION);

        /* If statement fails, discard tokens until the dedentation token is
         * encountered.
         */
        zen_Parser_pushFollowToken(parser, ZEN_TOKEN_DEDENTATION);

        do {
            zen_ASTNode_t* statement = zen_ASTNode_new(node);
            jtk_ArrayList_add(context->m_statements, statement);
            zen_Parser_statement(parser, statement);
        }
        while (zen_Parser_isStatementFollow(zen_TokenStream_la(parser->m_tokens, 1)));

        /* Pop the dedentation token from the follow set. */
        zen_Parser_popFollowToken(parser);

        /* Consume and discard the dedent token. */
        zen_Parser_match(parser, ZEN_TOKEN_DEDENTATION);
    // }

    zen_StackTrace_exit();
}

bool zen_Parser_isSimpleStatementFollow(zen_TokenType_t type) {
    bool result = false;
    switch (type){
        case ZEN_TOKEN_SEMICOLON: /* simpleStatement -> emptyStatement */
        case ZEN_TOKEN_KEYWORD_VAR: /* simpleStatement -> variableDeclaration */
        case ZEN_TOKEN_KEYWORD_FINAL: /* simpleStatement -> constantDeclaration */
        case ZEN_TOKEN_KEYWORD_ASSERT: /* simpleStatement -> assertStatement */
        case ZEN_TOKEN_KEYWORD_BREAK: /* simpleStatement -> breakStatement */
        case ZEN_TOKEN_KEYWORD_CONTINUE: /* simpleStatement -> continueStatement */
        case ZEN_TOKEN_KEYWORD_RETURN: /* simpleStatement -> returnStatement */
        case ZEN_TOKEN_KEYWORD_THROW: /* simpleStatement -> throwStatement */
        {
            result = true;
            break;
        }

        default: {
            result = zen_Parser_isExpressionFollow(type); /* simpleStatement -> expressionStatement */
            break;
        }
    }
    return result;
}

/*
 * simpleStatement
 * :    unterminatedSimpleStatement NEWLINE
 * ;
 *
 * unterminatedSimpleStatement
 * :    expressionStatement
 * |    emptyStatement
 * |    variableDeclaration
 * |    constantDeclaration
 * |    assertStatement
 * |    breakStatement
 * |    continueStatement
 * |    returnStatement
 * |    throwStatement
 * ;
 *
 * expressionStatement
 * :    expression
 * ;
 *
 *
 * The following function combines both the rules. This measure was
 * taken to avoid redundant nodes in the AST.
 */
void zen_Parser_simpleStatement(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

	zen_SimpleStatementContext_t* context = zen_SimpleStatementContext_new(node);

    /* If expressionStatement, emptyStatement, variableDeclaration,
     * constantDeclaration, assertStatement, breakStatement, continueStatement,
     * returnStatement, or throwStatement fails, discard tokens until the newline
     * token is encountered.
     */
    zen_Parser_pushFollowToken(parser, ZEN_TOKEN_NEWLINE);

    zen_TokenType_t la1 = zen_TokenStream_la(parser->m_tokens, 1);
    if (zen_Parser_isExpressionFollow(la1)) {
        zen_ASTNode_t* expression = zen_ASTNode_new(node);
        context->m_statement = expression;
        zen_Parser_expression(parser, expression);
    }
    else {
        switch (la1) {
            case ZEN_TOKEN_SEMICOLON: {
                zen_ASTNode_t* emptyStatement = zen_ASTNode_new(node);
                context->m_statement = emptyStatement;
                zen_Parser_emptyStatement(parser, emptyStatement);
                break;
            }

            case ZEN_TOKEN_KEYWORD_VAR: {
                zen_ASTNode_t* variableDeclaration = zen_ASTNode_new(node);
                context->m_statement = variableDeclaration;
                zen_Parser_variableDeclaration(parser, variableDeclaration);
                break;
            }

            case ZEN_TOKEN_KEYWORD_FINAL: {
                zen_ASTNode_t* constantDeclaration = zen_ASTNode_new(node);
                context->m_statement = constantDeclaration;
                zen_Parser_constantDeclaration(parser, constantDeclaration);
                break;
            }

            case ZEN_TOKEN_KEYWORD_ASSERT: {
                zen_ASTNode_t* assertStatement = zen_ASTNode_new(node);
                context->m_statement = assertStatement;
                zen_Parser_assertStatement(parser, assertStatement);
                break;
            }

            case ZEN_TOKEN_KEYWORD_BREAK: {
                zen_ASTNode_t* breakStatement = zen_ASTNode_new(node);
                context->m_statement = breakStatement;
                zen_Parser_breakStatement(parser, breakStatement);
                break;
            }

            case ZEN_TOKEN_KEYWORD_CONTINUE: {
                zen_ASTNode_t* continueStatement = zen_ASTNode_new(node);
                context->m_statement = continueStatement;
                zen_Parser_continueStatement(parser, continueStatement);
                break;
            }

            case ZEN_TOKEN_KEYWORD_RETURN: {
                zen_ASTNode_t* returnStatement = zen_ASTNode_new(node);
                context->m_statement = returnStatement;
                zen_Parser_returnStatement(parser, returnStatement);
                break;
            }

            case ZEN_TOKEN_KEYWORD_THROW: {
                zen_ASTNode_t* throwStatement = zen_ASTNode_new(node);
                context->m_statement = throwStatement;
                zen_Parser_throwStatement(parser, throwStatement);
                break;
            }
        }
    }

    /* Pop the newline token from the follow set. */
    zen_Parser_popFollowToken(parser);

    /* Match and discard the newline token. */
	zen_Parser_match(parser, ZEN_TOKEN_NEWLINE);

    zen_StackTrace_exit();
}

/*
 * statement
 * :    simpleStatement
 * |    compoundStatement
 * ;
 */
void zen_Parser_statement(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_StatementContext_t* context = zen_StatementContext_new(node);

    zen_TokenType_t la1 = zen_TokenStream_la(parser->m_tokens, 1);
    if (zen_Parser_isSimpleStatementFollow(la1)) {
        zen_ASTNode_t* simpleStatement = zen_ASTNode_new(node);
        context->m_simpleStatement = simpleStatement;
        zen_Parser_simpleStatement(parser, simpleStatement);
    }
    else if (zen_Parser_isCompoundStatementFollow(la1)) {
        zen_ASTNode_t* compoundStatement = zen_ASTNode_new(node);
        context->m_compoundStatement = compoundStatement;
        zen_Parser_compoundStatement(parser, compoundStatement);
    }
    else {
        // TODO: Expected simple or compound statement
        zen_Parser_reportAndRecover(parser, ZEN_TOKEN_KEYWORD_VAR);
    }

    zen_StackTrace_exit();
}

bool zen_Parser_isStatementFollow(zen_TokenType_t type) {
    return zen_Parser_isSimpleStatementFollow(type) || zen_Parser_isCompoundStatementFollow(type);
}

/*
 * emptyStatement
 * :    ';'
 * ;
 */
void zen_Parser_emptyStatement(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    /* zen_EmptyStatementContext_t* context = */ zen_EmptyStatementContext_new(node);

    /* The empty statement rule has no context. */
    /* Match and discard the ';' token. */
	zen_Parser_match(parser, ZEN_TOKEN_SEMICOLON);

    zen_StackTrace_exit();
}

/*
 * variableDeclaration
 * :    'var' variableDeclarator (',' variableDeclarator)*
 * ;
 */
void zen_Parser_variableDeclaration(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

	zen_VariableDeclarationContext_t* context = zen_VariableDeclarationContext_new(node);

    /* Match and discard the 'var' token. */
    zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_VAR);

	zen_ASTNode_t* variableDeclarator = zen_ASTNode_new(node);
    jtk_ArrayList_add(context->m_variableDeclarators, variableDeclarator);
	zen_Parser_variableDeclarator(parser, variableDeclarator);

	while (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_COMMA) {
        /* Consume and discard the ',' token. */
        zen_TokenStream_consume(parser->m_tokens);

		variableDeclarator = zen_ASTNode_new(node);
        jtk_ArrayList_add(context->m_variableDeclarators, variableDeclarator);
		zen_Parser_variableDeclarator(parser, variableDeclarator);
	}

    zen_StackTrace_exit();
}

/*
 * variableDeclarator
 * :    IDENTIFIER ('=' expression)?
 * ;
 */
void zen_Parser_variableDeclarator(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

	zen_VariableDeclaratorContext_t* context = zen_VariableDeclaratorContext_new(node);

    zen_Token_t* identifier = zen_Parser_matchAndYield(parser, ZEN_TOKEN_IDENTIFIER);
    context->m_identifier = zen_Parser_newTerminalNode(node, identifier);

	if (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_EQUAL) {
        /* Consume and discard the '=' token. */
		zen_TokenStream_consume(parser->m_tokens);

		zen_ASTNode_t* expression = zen_ASTNode_new(node);
        context->m_expression = expression;
		zen_Parser_expression(parser, expression);
	}

    zen_StackTrace_exit();
}

/* TODO: I have accidentally forwarded the node that we receive in the rule
 *       to child rules in many places. Please check and correct such bugs.
 */
/*
 * constantDeclaration
 * :    'final' constantDeclarator (',' constantDeclarator)*
 * ;
 */
void zen_Parser_constantDeclaration(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_ConstantDeclarationContext_t* context = zen_ConstantDeclarationContext_new(node);

    /* Match and discard the 'final' token. */
	zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_FINAL);

	zen_ASTNode_t* constantDeclarator = zen_ASTNode_new(node);
    jtk_ArrayList_add(context->m_constantDeclarators, constantDeclarator);
	zen_Parser_constantDeclarator(parser, constantDeclarator);

	while (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_COMMA) {
        /* Match and discard the ',' token. */
        zen_TokenStream_consume(parser->m_tokens);

		constantDeclarator = zen_ASTNode_new(node);
        jtk_ArrayList_add(context->m_constantDeclarators, constantDeclarator);
		zen_Parser_constantDeclarator(parser, constantDeclarator);
	}

    zen_StackTrace_exit();
}

/* TODO: Please check if all the rules have contexts.
 */
/*
 * constantDeclarator
 * :    IDENTIFIER '=' expression
 * ;
 */
void zen_Parser_constantDeclarator(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_ConstantDeclaratorContext_t* context = zen_ConstantDeclaratorContext_new(node);

	zen_Token_t* identifier = zen_Parser_matchAndYield(parser, ZEN_TOKEN_IDENTIFIER);
    context->m_identifier = zen_Parser_newTerminalNode(node, identifier);

    /* Match and discard the '=' token. */
	zen_Parser_match(parser, ZEN_TOKEN_EQUAL);

	zen_ASTNode_t* expression = zen_ASTNode_new(node);
    context->m_expression = expression;
	zen_Parser_expression(parser, expression);

    zen_StackTrace_exit();
}

/*
 * assertStatement
 * :    'assert' expression (':' expression)?
 * ;
 */
void zen_Parser_assertStatement(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_AssertStatementContext_t* context = zen_AssertStatementContext_new(node);

    /* Match and discard the 'assert' token. */
    zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_ASSERT);

    zen_ASTNode_t* conditionExpression = zen_ASTNode_new(node);
    context->m_conditionExpression = conditionExpression;
    zen_Parser_expression(parser, conditionExpression);

    if (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_IDENTIFIER) {
        /* Consume and discard the ':' token. */
        zen_TokenStream_consume(parser->m_tokens);

        zen_ASTNode_t* messageExpression = zen_ASTNode_new(node);
        context->m_messageExpression = messageExpression;
        zen_Parser_expression(parser, messageExpression);
    }

    zen_StackTrace_exit();
}

/*
 * breakStatement
 * :    'break' IDENTIFIER?
 * ;
 */
void zen_Parser_breakStatement(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_BreakStatementContext_t* context = zen_BreakStatementContext_new(node);

    /* Match and discard the 'break' token. */
    zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_BREAK);

    if (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_IDENTIFIER) {
        zen_Token_t* identifier = zen_TokenStream_lt(parser->m_tokens, 1);
        context->m_identifier = zen_Parser_newTerminalNode(node, identifier);
        zen_TokenStream_consume(parser->m_tokens);
    }

    zen_StackTrace_exit();
}

/*
 * continueStatement
 * :    'continue' IDENTIFIER?
 * ;
 */
void zen_Parser_continueStatement(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_ContinueStatementContext_t* context = zen_ContinueStatementContext_new(node);

    /* Match and discard the 'continue' token. */
    zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_CONTINUE);

    if (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_IDENTIFIER) {
        zen_Token_t* identifier = zen_TokenStream_lt(parser->m_tokens, 1);
        context->m_identifier = zen_Parser_newTerminalNode(node, identifier);
        zen_TokenStream_consume(parser->m_tokens);
    }

    zen_StackTrace_exit();
}

/*
 * returnStatement
 * :    'return' expression
 * ;
 */
void zen_Parser_returnStatement(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_ReturnStatementContext_t* context = zen_ReturnStatementContext_new(node);

    /* Match and discard the 'return' token. */
    zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_RETURN);

    /* An expression is mandatory after the 'return' keyword. */
    zen_ASTNode_t* expression = zen_ASTNode_new(node);
    context->m_expression = expression;
    zen_Parser_expression(parser, expression);

    zen_StackTrace_exit();
}

/*
 * throwStatement
 * :    'throw' expression?
 * ;
 */
void zen_Parser_throwStatement(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_ThrowStatementContext_t* context = zen_ThrowStatementContext_new(node);

    /* Match and discard the 'throw' token. */
    zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_THROW);

    if (zen_Parser_isExpressionFollow(zen_TokenStream_la(parser->m_tokens, 1))) {
        zen_ASTNode_t* expression = zen_ASTNode_new(node);
        context->m_expression = expression;
        zen_Parser_expression(parser, expression);
    }

    zen_StackTrace_exit();
}

/*
 * compoundStatement
 * :    ifStatement
 * |    iterativeStatement
 * |    tryStatement
 * |    synchronizeStatement
 * |    withStatement
 * // |    functionDeclaration
 * // |    classDeclaration
 * ;
 */
void zen_Parser_compoundStatement(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_CompoundStatementContext_t* context = zen_CompoundStatementContext_new(node);

	switch (zen_TokenStream_la(parser->m_tokens, 1)) {
		case ZEN_TOKEN_KEYWORD_IF: {
			zen_ASTNode_t* ifStatement = zen_ASTNode_new(node);
            context->m_statement = ifStatement;
			zen_Parser_ifStatement(parser, ifStatement);
			break;
		}

		case ZEN_TOKEN_HASH:
		case ZEN_TOKEN_KEYWORD_WHILE:
		case ZEN_TOKEN_KEYWORD_FOR: {
			zen_ASTNode_t* iterativeStatement = zen_ASTNode_new(node);
            context->m_statement = iterativeStatement;
			zen_Parser_iterativeStatement(parser, iterativeStatement);
			break;
		}

		case ZEN_TOKEN_KEYWORD_TRY: {
			zen_ASTNode_t* tryStatement = zen_ASTNode_new(node);
            context->m_statement = tryStatement;
			zen_Parser_tryStatement(parser, tryStatement);
			break;
		}

		case ZEN_TOKEN_KEYWORD_SYNCHRONIZE: {
			zen_ASTNode_t* synchronizeStatement = zen_ASTNode_new(node);
            context->m_statement = synchronizeStatement;
			zen_Parser_synchronizeStatement(parser, synchronizeStatement);
			break;
		}

		case ZEN_TOKEN_KEYWORD_WITH: {
			zen_ASTNode_t* withStatement = zen_ASTNode_new(node);
            context->m_statement = withStatement;
			zen_Parser_withStatement(parser, withStatement);
			break;
		}

		/*case ZEN_TOKEN_KEYWORD_FUNCTION: {
			zen_ASTNode_t* functionDeclaration = zen_ASTNode_new(node);
            context->m_statement = functionDeclaration;
			zen_Parser_functionDeclaration(parser, functionDeclaration);
			break;
		}

		case ZEN_TOKEN_KEYWORD_CLASS: {
			zen_ASTNode_t* classDeclaration = zen_ASTNode_new(node);
            context->m_statement = classDeclaration;
			zen_Parser_classDeclaration(parser, classDeclaration);
			break;
		}
        */

        /* NOTE: Enumerations are invalid within the scope of a function.
         * 1. Increases the complexity of the program. After all, we are
         *    looking to create the simplest general purpose scripting language!
         * 2. Once we exit the scope the enumeration becomes invalid. We are
         *    not interested giving birth to "anonymous enumerations" anyway.
         *
         * TODO: Add a grammar to report local enumeration declarations.
         */

		default: {
			printf("Control should not reach here.");

			break;
		}
	}

    zen_StackTrace_exit();
}

bool zen_Parser_isCompoundStatementFollow(zen_TokenType_t type) {
    bool result = false;
    switch (type) {
		case ZEN_TOKEN_KEYWORD_IF: /* compoundStatement -> ifStatement */
		case ZEN_TOKEN_HASH: /* compoundStatement -> iterativeStatement */
		case ZEN_TOKEN_KEYWORD_WHILE: /* compoundStatement -> ... -> whileStatement */
		case ZEN_TOKEN_KEYWORD_FOR: /* compoundStatement -> ... -> forStatement */
		case ZEN_TOKEN_KEYWORD_TRY: /* compoundStatement -> tryStatement */
		case ZEN_TOKEN_KEYWORD_SYNCHRONIZE: /* compoundStatement -> synchronizeStatement */
		case ZEN_TOKEN_KEYWORD_WITH: /* compoundStatement -> withStatement */
		// case ZEN_TOKEN_KEYWORD_FUNCTION: /* compoundStatement -> functionDeclaration */
		// case ZEN_TOKEN_KEYWORD_CLASS: /* compoundStatement -> classDeclaration */
		{
            result = true;
			break;
		}
	}
    return result;
}

/*
 * ifStatement
 * :    ifClause elseIfClause* elseClause?
 * ;
 */
void zen_Parser_ifStatement(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_IfStatementContext_t* context = zen_IfStatementContext_new(node);

	zen_ASTNode_t* ifClause = zen_ASTNode_new(node);
    context->m_ifClause = ifClause;
	zen_Parser_ifClause(parser, ifClause);

	while ((zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_KEYWORD_ELSE) &&
	       (zen_TokenStream_la(parser->m_tokens, 2) == ZEN_TOKEN_KEYWORD_IF)) {
		zen_ASTNode_t* elseIfClause = zen_ASTNode_new(node);
        jtk_ArrayList_add(context->m_elseIfClauses, elseIfClause);
		zen_Parser_elseIfClause(parser, elseIfClause);
	}

    if (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_KEYWORD_ELSE) {
        zen_ASTNode_t* elseClause = zen_ASTNode_new(node);
        context->m_elseClause = elseClause;
        zen_Parser_elseClause(parser, elseClause);
    }

    zen_StackTrace_exit();
}

/*
 * ifClause
 * :    'if' expression statementSuite
 * ;
 */
void zen_Parser_ifClause(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_IfClauseContext_t* context = zen_IfClauseContext_new(node);

    /* Match and discard the 'if' token. */
	zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_IF);

	zen_ASTNode_t* expression = zen_ASTNode_new(node);
    context->m_expression = expression;
	zen_Parser_expression(parser, expression);

	zen_ASTNode_t* statementSuite = zen_ASTNode_new(node);
    context->m_statementSuite = statementSuite;
	zen_Parser_statementSuite(parser, statementSuite);

    zen_StackTrace_exit();
}

/*
 * elseIfClause
 * :    'else' 'if' expression statementSuite
 * ;
 */
void zen_Parser_elseIfClause(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_ElseIfClauseContext_t* context = zen_ElseIfClauseContext_new(node);

    /* Match and discard the 'else' 'if' tokens. */
	zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_ELSE);
	zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_IF);

	zen_ASTNode_t* expression = zen_ASTNode_new(node);
    context->m_expression = expression;
	zen_Parser_expression(parser, expression);

	zen_ASTNode_t* statementSuite = zen_ASTNode_new(node);
    context->m_statementSuite = statementSuite;
	zen_Parser_statementSuite(parser, statementSuite);

    zen_StackTrace_exit();
}

/*
 * elseClause
 * :    'else' statementSuite
 * ;
 */
void zen_Parser_elseClause(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_ElseClauseContext_t* context = zen_ElseClauseContext_new(node);

    /* Match and discard the 'else' token. */
	zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_ELSE);

	zen_ASTNode_t* statementSuite = zen_ASTNode_new(node);
    context->m_statementSuite = statementSuite;
	zen_Parser_statementSuite(parser, statementSuite);

    zen_StackTrace_exit();
}

/*
 * iterativeStatement
 * :    labelClause? (whileStatement | forStatement)
 * ;
 */
void zen_Parser_iterativeStatement(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_IterativeStatementContext_t* context = zen_IterativeStatementContext_new(node);

	if (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_HASH) {
		zen_ASTNode_t* labelClause = zen_ASTNode_new(node);
        context->m_labelClause = labelClause;
		zen_Parser_labelClause(parser, labelClause);
	}

	switch (zen_TokenStream_la(parser->m_tokens, 1)) {
		case ZEN_TOKEN_KEYWORD_WHILE: {
			zen_ASTNode_t* whileStatement = zen_ASTNode_new(node);
            context->m_statement = whileStatement;
			zen_Parser_whileStatement(parser, whileStatement);
			break;
		}

		case ZEN_TOKEN_KEYWORD_FOR: {
			zen_ASTNode_t* forStatement = zen_ASTNode_new(node);
            context->m_statement = forStatement;
			zen_Parser_forStatement(parser, forStatement);
			break;
		}
	}

    zen_StackTrace_exit();
}

/*
 * labelClause
 * :    '#' IDENTIFIER
 * ;
 */
void zen_Parser_labelClause(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_LabelClauseContext_t* context = zen_LabelClauseContext_new(node);

    /* Match and discard the '#' token. */
	zen_Parser_match(parser, ZEN_TOKEN_HASH);

	zen_Token_t* identifier = zen_Parser_matchAndYield(parser, ZEN_TOKEN_IDENTIFIER);
    context->m_identifier = zen_Parser_newTerminalNode(node, identifier);

    zen_StackTrace_exit();
}

/*
 * whileStatement
 * :    'while' expression statementSuite
 * ;
 *
 * TODO: Remove elseClause!
 */
void zen_Parser_whileStatement(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_WhileStatementContext_t* context = zen_WhileStatementContext_new(node);

    /* Match and discard the 'while' token. */
	zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_WHILE);

	zen_ASTNode_t* expression = zen_ASTNode_new(node);
    context->m_expression = expression;
	zen_Parser_expression(parser, expression);

	zen_ASTNode_t* statementSuite = zen_ASTNode_new(node);
    context->m_statementSuite = statementSuite;
	zen_Parser_statementSuite(parser, statementSuite);

    /*
	if (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_KEYWORD_ELSE) {
		zen_ASTNode_t* elseClause = zen_ASTNode_new(node);
        context->m_elseClause = elseClause;
		zen_Parser_elseClause(parser, elseClause);
	}
    */

    zen_StackTrace_exit();
}


/*
 * forParameter
 * :    ('var' | 'final')? IDENTIFIER
 * ;
 */
void zen_Parser_forParameter(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_ForParameterContext_t* context = zen_ForParameterContext_new(node);

    zen_TokenType_t la1 = zen_TokenStream_la(parser->m_tokens, 1);
    if ((la1 == ZEN_TOKEN_KEYWORD_VAR) || (la1 == ZEN_TOKEN_KEYWORD_FINAL)) {
        context->m_declaration = true;
        context->m_variable = (la1 == ZEN_TOKEN_KEYWORD_VAR);

        /* Consume 'var' or 'final' token. */
        zen_TokenStream_consume(parser->m_tokens);
    }

    zen_Token_t* identifierToken = zen_Parser_matchAndYield(parser, ZEN_TOKEN_IDENTIFIER);
    zen_ASTNode_t* identifier = zen_Parser_newTerminalNode(parser, identifierToken);
    context->m_identifier = identifier;

    zen_StackTrace_exit();
}

/*
 * forStatement
 * :    'for' forParameter 'in' expression statementSuite
 * ;
 *
 * TODO: Remove elseClause!
 */
void zen_Parser_forStatement(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

	zen_ForStatementContext_t* context = zen_ForStatementContext_new(node);

    /* Match and discard the 'for' token. */
    zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_FOR);

    zen_ASTNode_t* forParameter = zen_ASTNode_new(node);
    context->m_forParameter = forParameter;
    zen_Parser_forParameter(parser, forParameter);

    /* Match and discard the 'in' token. */
	zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_IN);

	zen_ASTNode_t* expression = zen_ASTNode_new(node);
    context->m_expression = expression;
	zen_Parser_expression(parser, expression);

	zen_ASTNode_t* statementSuite = zen_ASTNode_new(node);
    context->m_statementSuite = statementSuite;
	zen_Parser_statementSuite(parser, statementSuite);

    /*
	if (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_KEYWORD_ELSE) {
		zen_ASTNode_t* elseClause = zen_ASTNode_new(node);
        context->m_elseClause = elseClause;
		zen_Parser_elseClause(parser, elseClause);
	}
    */

    zen_StackTrace_exit();
}

/*
 * tryStatement
 * :    tryClause catchClause* finallyClause?
 * ;
 */
void zen_Parser_tryStatement(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_TryStatementContext_t* context = zen_TryStatementContext_new(node);

	bool hasCatch = false;
	bool hasFinally = false;

    zen_Token_t* tryKeyword = zen_TokenStream_lt(parser->m_tokens, 1);
    zen_ASTNode_t* tryClause = zen_ASTNode_new(node);
    context->m_tryClause = tryClause;
    zen_Parser_tryClause(parser, tryClause);

	while ((zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_KEYWORD_CATCH)) {
		hasCatch = true;

		zen_ASTNode_t* catchClause = zen_ASTNode_new(node);
        jtk_ArrayList_add(context->m_catchClauses, catchClause);
		zen_Parser_catchClause(parser, catchClause);
	}

	if (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_KEYWORD_FINALLY) {
		hasFinally = true;
		zen_ASTNode_t* finallyClause = zen_ASTNode_new(node);
        context->m_finallyClause = finallyClause;
		zen_Parser_finallyClause(parser, finallyClause);
	}

	if (!hasCatch && !hasFinally) {
		/* Try clause without catch or finally. According to the grammar of Zen,
         * this is not an error. However, the Zen specification requires a try
         * clause to be followed by at least a catch or finally clause.
         */
        zen_ErrorHandler_handleSyntacticalError(parser->m_compiler->m_errorHandler,
            parser, ZEN_ERROR_CODE_TRY_STATEMENT_EXPECTS_CATCH_OR_FINALLY,
            tryKeyword, ZEN_TOKEN_UNKNOWN);
	}

    zen_StackTrace_exit();
}

/*
 * tryClause
 * :    'try' statementSuite
 * ;
 */
void zen_Parser_tryClause(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_TryClauseContext_t* context = zen_TryClauseContext_new(node);

    /* Match and discard the 'try' token. */
	zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_TRY);

	zen_ASTNode_t* statementSuite = zen_ASTNode_new(node);
    context->m_statementSuite = statementSuite;
	zen_Parser_statementSuite(parser, statementSuite);

    zen_StackTrace_exit();
}

/* I am in a dilemma. Which one of the following constructs is the best?!
 *
 * catch RuntimeException | NullPointerException exception      (This is the construct which we currently recognize.)
 * catch RuntimeException | NullPointerException in exception
 *
 * catch RuntimeException or NullPointerException exception
 * catch RuntimeException or NullPointerException in exception
 */
/*
 * catchClause
 * :	'catch' catchFilter IDENTIFIER statementSuite'
 * ;
 */
void zen_Parser_catchClause(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_CatchClauseContext_t* context = zen_CatchClauseContext_new(node);

    /* Match and discard the 'catch' token. */
    zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_CATCH);

    zen_ASTNode_t* catchFilter = zen_ASTNode_new(node);
    context->m_catchFilter = catchFilter;
    zen_Parser_catchFilter(parser, catchFilter);

    zen_Token_t* identifier = zen_Parser_matchAndYield(parser, ZEN_TOKEN_IDENTIFIER);
    context->m_identifier = zen_Parser_newTerminalNode(node, identifier);

    zen_ASTNode_t* statementSuite = zen_ASTNode_new(node);
    context->m_statementSuite = statementSuite;
    zen_Parser_statementSuite(parser, statementSuite);

    zen_StackTrace_exit();
}

/*
 * catchFilter
 * :	typeName ('|' typeName)*
 * ;
 */
void zen_Parser_catchFilter(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_CatchFilterContext_t* context = zen_CatchFilterContext_new(node);

	zen_ASTNode_t* typeName = zen_ASTNode_new(node);
    jtk_ArrayList_add(context->m_typeNames, typeName);
	zen_Parser_typeName(parser, typeName);

	while (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_VERTICAL_BAR) {
        /* Consume and discard the '|' token. */
		zen_TokenStream_consume(parser->m_tokens);

		typeName = zen_ASTNode_new(node);
        jtk_ArrayList_add(context->m_typeNames, typeName);
		zen_Parser_typeName(parser, typeName);
	}

    zen_StackTrace_exit();
}

/*
 * typeName
 * :    IDENTIFIER ('.' IDENTIFIER)*
 * ;
 */
void zen_Parser_typeName(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_TypeNameContext_t* context = zen_TypeNameContext_new(node);

    zen_Token_t* identifierToken = zen_Parser_matchAndYield(parser, ZEN_TOKEN_IDENTIFIER);
    zen_ASTNode_t* identifier = zen_Parser_newTerminalNode(node, identifierToken);
    jtk_ArrayList_add(context->m_identifiers, identifier);

    while (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_DOT) {
        /* Consume and discard the '.' token. */
        zen_TokenStream_consume(parser->m_tokens);

        identifierToken = zen_Parser_matchAndYield(parser, ZEN_TOKEN_IDENTIFIER);
        identifier = zen_Parser_newTerminalNode(node, identifierToken);
        jtk_ArrayList_add(context->m_identifiers, identifier);
    }

    zen_StackTrace_exit();
}

/* TODO: We are invoking consume() instead of match() at certain locations.
 *       Especially check at the starting of a rule.
 */
/*
 * finallyClause
 * :	'finally' statementSuite
 * ;
 */
void zen_Parser_finallyClause(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_FinallyClauseContext_t* context = zen_FinallyClauseContext_new(node);

    /* Match and discard the 'finally' token. */
	zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_FINALLY);

	zen_ASTNode_t* statementSuite = zen_ASTNode_new(node);
    context->m_statementSuite = statementSuite;
	zen_Parser_statementSuite(parser, statementSuite);

    zen_StackTrace_exit();
}

/*
 * synchronizeStatement
 * :	'synchronize' expression statementSuite
 * ;
 */
void zen_Parser_synchronizeStatement(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_SynchronizeStatementContext_t* context = zen_SynchronizeStatementContext_new(node);

    /* Match and discard the 'synchronize' token. */
    zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_SYNCHRONIZE);

    zen_ASTNode_t* expression = zen_ASTNode_new(node);
    context->m_expression = expression;
    zen_Parser_expression(parser, expression);

    zen_ASTNode_t* statementSuite = zen_ASTNode_new(node);
    context->m_statementSuite = statementSuite;
    zen_Parser_statementSuite(parser, statementSuite);

    zen_StackTrace_exit();
}

/*
 * withStatement
 * :	'with' withParameters statementSuite
 * ;
 */
void zen_Parser_withStatement(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_WithStatementContext_t* context = zen_WithStatementContext_new(node);

    /* Match and discard the 'with' token. */
    zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_WITH);

    zen_ASTNode_t* withParameters = zen_ASTNode_new(node);
    context->m_withParameters = withParameters;
    zen_Parser_withParameters(parser, withParameters);

    zen_ASTNode_t* statementSuite = zen_ASTNode_new(node);
    context->m_statementSuite = statementSuite;
    zen_Parser_statementSuite(parser, statementSuite);

    zen_StackTrace_exit();
}

/*
 * withParameters
 * :    withParameter (',' withParameter)*
 * ;
 */
void zen_Parser_withParameters(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_WithParametersContext_t* context = zen_WithParametersContext_new(node);

    zen_ASTNode_t* withParameter = zen_ASTNode_new(node);
    jtk_ArrayList_add(context->m_withParameters, withParameter);
    zen_Parser_withParameter(parser, withParameter);

    zen_TokenType_t la1 = zen_TokenStream_la(parser->m_tokens, 1);
    while (la1 == ZEN_TOKEN_COMMA) {
        /* Match and discard the ',' token. */
        zen_Parser_match(parser, ZEN_TOKEN_COMMA);

        zen_ASTNode_t* withParameter0 = zen_ASTNode_new(node);
        jtk_ArrayList_add(context->m_withParameters, withParameter0);
        zen_Parser_withParameter(parser, withParameter0);

        la1 = zen_TokenStream_la(parser->m_tokens, 1);
    }

    zen_StackTrace_exit();
}

/*
 * withParameter
 * :    (('var' | 'final') IDENTIFIER '=' )? expression
 * ;
 */
void zen_Parser_withParameter(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_WithParameterContext_t* context = zen_WithParameterContext_new(node);

    zen_TokenType_t la1 = zen_TokenStream_la(parser->m_tokens, 1);
    if ((la1 == ZEN_TOKEN_KEYWORD_VAR) || (la1 == ZEN_TOKEN_KEYWORD_FINAL)) {
        /* Consume 'var' or 'final' token. */
        zen_TokenStream_consume(parser->m_tokens);

        context->m_variable = (la1 == ZEN_TOKEN_KEYWORD_VAR);

        zen_Token_t* identifierToken = zen_Parser_matchAndYield(parser, ZEN_TOKEN_IDENTIFIER);
        zen_ASTNode_t* identifier = zen_Parser_newTerminalNode(parser, identifierToken);
        context->m_identifier = identifier;

        /* Match and discard the '=' token. */
        zen_Parser_match(parser, ZEN_TOKEN_EQUAL);
    }

	zen_ASTNode_t* expression = zen_ASTNode_new(node);
    context->m_expression = expression;
	zen_Parser_expression(parser, expression);

    zen_StackTrace_exit();
}

/* TODO: At many locations I have forgotten to consume and discard a token.
 *       Please search for such bugs. Look near la() and lt() calls.
 */
/*
 * classDeclaration
 * :	'class' IDENTIFIER classClassExtendsClause? classSuite
 * ;
 */
void zen_Parser_classDeclaration(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_ClassDeclarationContext_t* context = zen_ClassDeclarationContext_new(node);

    /* Match and discard the 'class' token. */
    zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_CLASS);

    zen_Token_t* identifier = zen_Parser_matchAndYield(parser, ZEN_TOKEN_IDENTIFIER);
    context->m_identifier = zen_Parser_newTerminalNode(node, identifier);

    if (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_LEFT_ANGLE_BRACKET_EQUAL) {
        zen_ASTNode_t* classClassExtendsClause = zen_ASTNode_new(node);
        context->m_classExtendsClause = classClassExtendsClause;
        zen_Parser_classExtendsClause(parser, classClassExtendsClause);
    }

    zen_ASTNode_t* classSuite = zen_ASTNode_new(node);
    context->m_classSuite = classSuite;
    zen_Parser_classSuite(parser, classSuite);

    zen_StackTrace_exit();
}

/*
 * classClassExtendsClause
 * :	'<=' typeName (',' typeName)*
 * ;
 */
void zen_Parser_classExtendsClause(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_ClassExtendsClauseContext_t* context = zen_ClassExtendsClauseContext_new(node);

    /* Consume and discard the '<=' token. */
    zen_Parser_match(parser, ZEN_TOKEN_LEFT_ANGLE_BRACKET_EQUAL);

    zen_ASTNode_t* typeName = zen_ASTNode_new(node);
    jtk_ArrayList_add(context->m_typeNames, typeName);
    zen_Parser_typeName(parser, typeName);

    while (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_COMMA) {
        /* Consume and discard the ',' token. */
        zen_TokenStream_consume(parser->m_tokens);

        typeName = zen_ASTNode_new(node);
        jtk_ArrayList_add(context->m_typeNames, typeName);
        zen_Parser_typeName(parser, typeName);
    }

    zen_StackTrace_exit();
}

/*
 * classSuite
 * :	NEWLINE INDENT classMember+ DEDENT
 * ;
 */
void zen_Parser_classSuite(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_ClassSuiteContext_t* context = zen_ClassSuiteContext_new(node);

    /* Match and discard the newline and indent tokens. */
    zen_Parser_match(parser, ZEN_TOKEN_NEWLINE);
    zen_Parser_match(parser, ZEN_TOKEN_INDENTATION);

    do {
        zen_ASTNode_t* classMember = zen_ASTNode_new(node);
        jtk_ArrayList_add(context->m_classMembers, classMember);
        zen_Parser_classMember(parser, classMember);
    }
    while (zen_Parser_isClassMemberFollow(zen_TokenStream_la(parser->m_tokens, 1)));

    /* Match and discard the dedent token. */
    zen_Parser_match(parser, ZEN_TOKEN_DEDENTATION);

    zen_StackTrace_exit();
}

bool zen_Parser_isClassMemberFollow(zen_TokenType_t type) {
    return zen_Parser_isClassMemberModifier(type) || /* classMember -> modifier */
           (type == ZEN_TOKEN_KEYWORD_VAR)        || /* classMember -> ... -> variableDeclaration */
           (type == ZEN_TOKEN_KEYWORD_FINAL)      || /* classMember -> ... -> constantDeclaration */
           (type == ZEN_TOKEN_KEYWORD_FUNCTION)   || /* classMember -> ... -> functionDeclaration */
           // (type == ZEN_TOKEN_IDENTIFIER)      || /* classMember -> ... -> constructorDeclaration */
           // (type == ZEN_TOKEN_KEYWORD_CLASS)      || /* classMember -> ... -> classDeclaration */
           // (type == ZEN_TOKEN_KEYWORD_ENUM)       || /* classMember -> ... -> enumerationDeclaration */
           (type == ZEN_TOKEN_AT);                   /* classMember -> ... -> annotatedClassMember */
}

/*
 * classMember
 * :	annotations? classMemberModifier* unmodifiedClassMember
 * ;
 *
 * unmodifiedClassMember
 * :	variableDeclaration
 * |	constantDeclaration
 * |	functionDeclaration
 * // |	constructorDeclaration
 * // |	classDeclaration
 * // |	enumerationDeclaration
 * ;
 *
 * The following function combines both the rules. This measure was
 * taken to avoid redundant nodes in the AST.
 */
void zen_Parser_classMember(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_ClassMemberContext_t* context = zen_ClassMemberContext_new(node);

    if (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_AT) {
        zen_ASTNode_t* annotations = zen_ASTNode_new(node);
        context->m_annotations = annotations;
        zen_Parser_annotations(parser, annotations);
    }

    uint32_t modifiers = 0;
    while (zen_Parser_isClassMemberModifier(zen_TokenStream_la(parser->m_tokens, 1))) {
        zen_Token_t* modifierToken = zen_TokenStream_lt(parser->m_tokens, 1);
        zen_ASTNode_t* modifier = zen_Parser_newTerminalNode(node, modifierToken);
        jtk_ArrayList_add(context->m_modifiers, modifier);
        zen_TokenStream_consume(parser->m_tokens);

        modifiers |= zen_TokenType_toModifiers(modifierToken->m_type);
    }

    switch (zen_TokenStream_la(parser->m_tokens, 1)) {
        case ZEN_TOKEN_KEYWORD_VAR: {
            /* If variableDeclaration fails, discard tokens until the newline
             * token is encountered.
             */
            zen_Parser_pushFollowToken(parser, ZEN_TOKEN_NEWLINE);

            zen_ASTNode_t* variableDeclaration = zen_ASTNode_new(node);
            context->m_declaration = variableDeclaration;
            zen_Parser_variableDeclaration(parser, variableDeclaration);

            /* Pop the newline token from the follow set. */
            zen_Parser_popFollowToken(parser);

            /* Match and discard the newline token. */
            zen_Parser_match(parser, ZEN_TOKEN_NEWLINE);

            break;
        }

        case ZEN_TOKEN_KEYWORD_FINAL: {
            /* If constantDeclaration fails, discard tokens until the newline
             * token is encountered.
             */
            zen_Parser_pushFollowToken(parser, ZEN_TOKEN_NEWLINE);

            zen_ASTNode_t* constantDeclaration = zen_ASTNode_new(node);
            context->m_declaration = constantDeclaration;
            zen_Parser_constantDeclaration(parser, constantDeclaration);

            /* Pop the newline token from the follow set. */
            zen_Parser_popFollowToken(parser);

            /* Match and discard the newline token. */
            zen_Parser_match(parser, ZEN_TOKEN_NEWLINE);

            break;
        }

        case ZEN_TOKEN_KEYWORD_FUNCTION: {
            zen_ASTNode_t* functionDeclaration = zen_ASTNode_new(node);
            context->m_declaration = functionDeclaration;
            zen_Parser_functionDeclaration(parser, functionDeclaration, modifiers);

            break;
        }

//         case ZEN_TOKEN_IDENTIFIER: {
//             zen_ASTNode_t* constructorDeclaration = zen_ASTNode_new(node);
//             context->m_declaration = constructorDeclaration;
//             zen_Parser_constructorDeclaration(parser, constructorDeclaration);

//             break;
//         }

        // case ZEN_TOKEN_KEYWORD_CLASS: {
        //     zen_ASTNode_t* classDeclaration = zen_ASTNode_new(node);
        //     context->m_declaration = classDeclaration;
        //     zen_Parser_classDeclaration(parser, classDeclaration);

        //     break;
        // }

        // case ZEN_TOKEN_KEYWORD_ENUM: {
        //     zen_ASTNode_t* enumerationDeclaration = zen_ASTNode_new(node);
        //     context->m_declaration = enumerationDeclaration;
        //     zen_Parser_enumerationDeclaration(parser, enumerationDeclaration);

        //     break;
        // }

        default: {
            /* TODO: Expected var, final, or function. */
            zen_Parser_reportAndRecover(parser, ZEN_TOKEN_KEYWORD_FUNCTION);

            break;
        }
    }

    zen_StackTrace_exit();
}

/*
 * classMemberModifier
 * :    'static'
 * |    'native'
 * |    'abstract'
 * |    'public'
 * |    'private'
 * |    'secret'
 * ;
 */
bool zen_Parser_isClassMemberModifier(zen_TokenType_t type) {
    return (type == ZEN_TOKEN_KEYWORD_STATIC) ||
           (type == ZEN_TOKEN_KEYWORD_NATIVE) ||
           (type == ZEN_TOKEN_KEYWORD_ABSTRACT) ||
           (type == ZEN_TOKEN_KEYWORD_PUBLIC) ||
           (type == ZEN_TOKEN_KEYWORD_PRIVATE) ||
           (type == ZEN_TOKEN_KEYWORD_SECRET);
}

/*
 * constructorDeclaration
 * :	IDENTIFIER functionParameters? statementSuite
 * ;
 *
 */
// void zen_Parser_constructorDeclaration(zen_Parser_t* parser, zen_ASTNode_t* node) {
//     zen_StackTrace_enter();
//
//     zen_ConstructorDeclarationContext_t* context = zen_ConstructorDeclarationContext_new(node);
//
//     zen_Token_t* identifier = zen_Parser_matchAndYield(parser, ZEN_TOKEN_IDENTIFIER);
//     context->m_identifier = zen_Parser_newTerminalNode(node, identifier);
//
//     if (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_LEFT_PARENTHESIS) {
//         zen_ASTNode_t* functionParameters = zen_ASTNode_new(node);
//         context->m_functionParameters = functionParameters;
//         zen_Parser_functionParameters(parser, functionParameters);
//     }
//
//     zen_ASTNode_t* statementSuite = zen_ASTNode_new(node);
//     context->m_statementSuite = statementSuite;
//     zen_Parser_statementSuite(parser, statementSuite);
//
//     zen_StackTrace_exit();
// }

/*
 * enumerationDeclaration
 * :   'enum' IDENTIFIER enumerationBaseClause? enumerationSuite
 * ;
 */
// void zen_Parser_enumerationDeclaration(zen_Parser_t* parser, zen_ASTNode_t* node) {
//     zen_StackTrace_enter();

//     zen_EnumerationDeclarationContext_t* context = zen_EnumerationDeclarationContext_new(node);

//     /* Match and discard the 'enum' token. */
//     zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_ENUM);

//     zen_Token_t* identifier = zen_Parser_matchAndYield(parser, ZEN_TOKEN_IDENTIFIER);
//     context->m_identifier = zen_Parser_newTerminalNode(node, identifier);

//     if (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_COLON) {
//         zen_ASTNode_t* enumerationBaseClause = zen_ASTNode_new(node);
//         context->m_enumerationBaseClause = enumerationBaseClause;
//         zen_Parser_enumerationBaseClause(parser, enumerationBaseClause);
//     }

//     zen_ASTNode_t* enumerationSuite = zen_ASTNode_new(node);
//     context->m_enumerationSuite = enumerationSuite;
//     zen_Parser_enumerationSuite(parser, enumerationSuite);

//     zen_StackTrace_exit();
// }

/*
 * enumerationBaseClause
 * :   ':' typeName
 * ;
 */
// void zen_Parser_enumerationBaseClause(zen_Parser_t* parser, zen_ASTNode_t* node) {
//     zen_StackTrace_enter();

//     zen_EnumerationBaseClauseContext_t* context = zen_EnumerationBaseClauseContext_new(node);

//     /* Match and discard the ':' token. */
//     zen_Parser_match(parser, ZEN_TOKEN_COLON);

//     zen_ASTNode_t* typeName = zen_ASTNode_new(node);
//     context->m_typeName = typeName;
//     zen_Parser_typeName(parser, typeName);

//     zen_StackTrace_exit();
// }

/*
 * enumerationSuite
 * :   NEWLINE INDENT enumerate+ DEDENT
 * ;
 */
// void zen_Parser_enumerationSuite(zen_Parser_t* parser, zen_ASTNode_t* node) {
//     zen_StackTrace_enter();

//     zen_EnumerationSuiteContext_t* context = zen_EnumerationSuiteContext_new(node);

//     /* Match and discard the newline token. */
//     zen_Parser_match(parser, ZEN_TOKEN_NEWLINE);
//     /* Match and discard the indent token. */
//     zen_Parser_match(parser, ZEN_TOKEN_INDENTATION);

//     do {
//         zen_ASTNode_t* enumerate = zen_ASTNode_new(node);
//         jtk_ArrayList_add(context->m_enumerates, enumerate);
//         zen_Parser_enumerate(parser, enumerate);
//     }
//     while (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_IDENTIFIER);

//     /* Match and discard the dedent token. */
//     zen_Parser_match(parser, ZEN_TOKEN_DEDENTATION);

//     zen_StackTrace_exit();
// }

/*
 * enumerate
 * :   IDENTIFIER functionArguments? NEWLINE
 * ;
 */
// void zen_Parser_enumerate(zen_Parser_t* parser, zen_ASTNode_t* node) {
//     zen_StackTrace_enter();

//     zen_EnumerateContext_t* context = zen_EnumerateContext_new(node);

//     zen_Token_t* identifier = zen_Parser_matchAndYield(parser, ZEN_TOKEN_IDENTIFIER);
//     context->m_identifier = zen_Parser_newTerminalNode(node, identifier);

//     if (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_LEFT_PARENTHESIS) {
//         zen_ASTNode_t* functionArguments = zen_ASTNode_new(node);
//         context->m_functionArguments = functionArguments;
//         zen_Parser_functionArguments(parser, functionArguments);
//     }

//     /* Match and discard the newline token. */
//     zen_Parser_match(parser, ZEN_TOKEN_NEWLINE);

//     zen_StackTrace_exit();
// }

/*
 * expression
 * :    expression (',' expression)*
 * ;
 */
void zen_Parser_expressions(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_ExpressionsContext_t* context = zen_ExpressionsContext_new(node);

    zen_ASTNode_t* expression = zen_ASTNode_new(node);
    jtk_ArrayList_add(context->m_expressions, expression);
    zen_Parser_expression(parser, expression);

    while (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_COMMA) {
        /* Consume and discard the ',' token. */
        zen_TokenStream_consume(parser->m_tokens);

        expression = zen_ASTNode_new(node);
        jtk_ArrayList_add(context->m_expressions, expression);
        zen_Parser_expression(parser, expression);
    }

    zen_StackTrace_exit();
}

/*
 * expression
 * :	assignmentExpression
 * ;
 *
 */
void zen_Parser_expression(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_ExpressionContext_t* context = zen_ExpressionContext_new(node);

    zen_ASTNode_t* assignmentExpression = zen_ASTNode_new(node);
    context->m_assignmentExpression = assignmentExpression;
    zen_Parser_assignmentExpression(parser, assignmentExpression);
}

bool zen_Parser_isExpressionFollow(zen_TokenType_t type) {
    return zen_Parser_isUnaryExpressionFollow(type);
}

/*
 * assignmentExpression
 * :	conditionalExpression (assignmentOperator assignmentExpression)?
 * ;
 */
void zen_Parser_assignmentExpression(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_AssignmentExpressionContext_t* context = zen_AssignmentExpressionContext_new(node);

    zen_ASTNode_t* conditionalExpression = zen_ASTNode_new(node);
    context->m_conditionalExpression = conditionalExpression;
    zen_Parser_conditionalExpression(parser, conditionalExpression);

    if (zen_Parser_isAssignmentOperator(zen_TokenStream_la(parser->m_tokens, 1))) {
        zen_Token_t* assignmentOperator = zen_TokenStream_lt(parser->m_tokens, 1);
        context->m_assignmentOperator = zen_Parser_newTerminalNode(node, assignmentOperator);
        /* Consume the assignment operator token. */
        zen_TokenStream_consume(parser->m_tokens);

        zen_ASTNode_t* assignmentExpression = zen_ASTNode_new(node);
        context->m_assignmentExpression = assignmentExpression;
        zen_Parser_assignmentExpression(parser, assignmentExpression);
    }

    zen_StackTrace_exit();
}

/*
 * assignmentOperator
 * :    '='
 * |    '*='
 * |    '/='
 * |    '%='
 * |    '+='
 * |    '-='
 * |    '<<='
 * |    '>>='
 * |    '>>>='
 * |    '&='
 * |    '^='
 * |    '|='
 * ;
 */
bool zen_Parser_isAssignmentOperator(zen_TokenType_t type) {
    return (type == ZEN_TOKEN_EQUAL) ||
           (type == ZEN_TOKEN_ASTERISK_EQUAL) ||
           (type == ZEN_TOKEN_FORWARD_SLASH_EQUAL) ||
           (type == ZEN_TOKEN_MODULUS_EQUAL) ||
           (type == ZEN_TOKEN_PLUS_EQUAL) ||
           (type == ZEN_TOKEN_DASH_EQUAL) ||
           (type == ZEN_TOKEN_LEFT_ANGLE_BRACKET_2_EQUAL) ||
           (type == ZEN_TOKEN_RIGHT_ANGLE_BRACKET_2_EQUAL) ||
           (type == ZEN_TOKEN_RIGHT_ANGLE_BRACKET_3_EQUAL) ||
           (type == ZEN_TOKEN_AMPERSAND_EQUAL) ||
           (type == ZEN_TOKEN_CARET_EQUAL) ||
           (type == ZEN_TOKEN_VERTICAL_BAR_EQUAL);
}

/*
 * conditionalExpression
 * :	logicalOrExpression ('then' expression 'else' conditionalExpression)?
 * ;
 */
void zen_Parser_conditionalExpression(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_ConditionalExpressionContext_t* context = zen_ConditionalExpressionContext_new(node);

    zen_ASTNode_t* logicalOrExpression = zen_ASTNode_new(node);
    context->m_logicalOrExpression = logicalOrExpression;
    zen_Parser_logicalOrExpression(parser, logicalOrExpression);

    if (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_HOOK) {
        /* Consume and discard the '?' token. */
        zen_TokenStream_consume(parser->m_tokens);

        zen_ASTNode_t* expression = zen_ASTNode_new(node);
        context->m_thenExpression = expression;
        zen_Parser_expression(parser, expression);

		/* Consume and discard the ':' token. */
        zen_Parser_match(parser, ZEN_TOKEN_COLON);

        zen_ASTNode_t* conditionalExpression = zen_ASTNode_new(node);
        context->m_elseExpression = conditionalExpression;
        zen_Parser_conditionalExpression(parser, conditionalExpression);
    }

    zen_StackTrace_exit();
}

/*
 * logicalOrExpression
 * :	logicalAndExpression ('or' logicalAndExpression)*
 * ;
 */
void zen_Parser_logicalOrExpression(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_LogicalOrExpressionContext_t* context = zen_LogicalOrExpressionContext_new(node);

    zen_ASTNode_t* logicalAndExpression = zen_ASTNode_new(node);
    context->m_logicalAndExpression = logicalAndExpression;
    zen_Parser_logicalAndExpression(parser, logicalAndExpression);

    while (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_KEYWORD_OR) {
        /* Consume and discard the 'or' token. */
        zen_TokenStream_consume(parser->m_tokens);

        zen_ASTNode_t* logicalAndExpression0 = zen_ASTNode_new(node);
        jtk_ArrayList_add(context->m_logicalAndExpressions, logicalAndExpression0);
        zen_Parser_logicalAndExpression(parser, logicalAndExpression0);
    }

    zen_StackTrace_exit();
}

/*
 * logicalAndExpression
 * :	inclusiveOrExpression ('and' logicalAndExpression)?
 * ;
 */
void zen_Parser_logicalAndExpression(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_LogicalAndExpressionContext_t* context = zen_LogicalAndExpressionContext_new(node);

    /* Parse the operand left of the operator. */
    zen_ASTNode_t* inclusiveOrExpression = zen_ASTNode_new(node);
    context->m_inclusiveOrExpression = inclusiveOrExpression;
    zen_Parser_inclusiveOrExpression(parser, inclusiveOrExpression);

    /* Parse the expression to the right of the operator, if any. */
    while (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_KEYWORD_AND) {
        /* Consume and discard the 'and' token. */
        zen_TokenStream_consume(parser->m_tokens);

        zen_ASTNode_t* inclusiveOrExpression0 = zen_ASTNode_new(node);
        jtk_ArrayList_add(context->m_inclusiveOrExpressions, inclusiveOrExpression0);
        zen_Parser_inclusiveOrExpression(parser, inclusiveOrExpression0);
    }

    zen_StackTrace_exit();
}

/*
 * inclusiveOrExpression
 * :	exclusiveOrExpression ('|' exclusiveOrExpression)*
 * ;
 */
void zen_Parser_inclusiveOrExpression(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_InclusiveOrExpressionContext_t* context = zen_InclusiveOrExpressionContext_new(node);

    /* Parse the operand left of the operator. */
    zen_ASTNode_t* exclusiveOrExpression = zen_ASTNode_new(node);
    context->m_exclusiveOrExpression = exclusiveOrExpression;
    zen_Parser_exclusiveOrExpression(parser, exclusiveOrExpression);

    /* Parse the expression to the right of the operator, if any. */
    if (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_VERTICAL_BAR) {
        /* Consume and discard the '|' token. */
        zen_TokenStream_consume(parser->m_tokens);

        zen_ASTNode_t* exclusiveOrExpression0 = zen_ASTNode_new(node);
        jtk_ArrayList_add(context->m_exclusiveOrExpressions, exclusiveOrExpression0);
        zen_Parser_exclusiveOrExpression(parser, exclusiveOrExpression0);
    }

    zen_StackTrace_exit();
}

/*
 * exclusiveOrExpression
 * :	andExpression ('^' andExpression)*
 * ;
 */
void zen_Parser_exclusiveOrExpression(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_ExclusiveOrExpressionContext_t* context = zen_ExclusiveOrExpressionContext_new(node);

    /* Parse the operand left of the operator. */
    zen_ASTNode_t* andExpression = zen_ASTNode_new(node);
    context->m_andExpression = andExpression;
    zen_Parser_andExpression(parser, andExpression);

    /* Parse the expression to the right of the operator, if any. */
    while (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_CARET) {
        /* Consume and discard the '^' token. */
        zen_TokenStream_consume(parser->m_tokens);

        zen_ASTNode_t* andExpression0 = zen_ASTNode_new(node);
        jtk_ArrayList_add(context->m_andExpressions, andExpression0);
        zen_Parser_andExpression(parser, andExpression0);
    }

    zen_StackTrace_exit();
}

/*
 * andExpression
 * :	equalityExpression ('&' equalityExpression)*
 * ;
 */
void zen_Parser_andExpression(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_AndExpressionContext_t* context = zen_AndExpressionContext_new(node);

    /* Parse the operand left of the operator. */
    zen_ASTNode_t* equalityExpression = zen_ASTNode_new(node);
    context->m_equalityExpression = equalityExpression;
    zen_Parser_equalityExpression(parser, equalityExpression);

    /* Parse the expression to the right of the operator, if any. */
    while (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_AMPERSAND) {

        /* Consume and discard the '&' token. */
        zen_TokenStream_consume(parser->m_tokens);

        zen_ASTNode_t* equalityExpression0 = zen_ASTNode_new(node);
        jtk_ArrayList_add(context->m_equalityExpressions, equalityExpression0);
        zen_Parser_equalityExpression(parser, equalityExpression0);
    }

    zen_StackTrace_exit();
}

/*
 * equalityExpression
 * :	relationalExpression (equalityOperator relationalExpression)*
 * ;
 */
void zen_Parser_equalityExpression(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_EqualityExpressionContext_t* context = zen_EqualityExpressionContext_new(node);

    /* Parse the operand left of the operator. */
    zen_ASTNode_t* relationalExpression = zen_ASTNode_new(node);
    context->m_relationalExpression = relationalExpression;
    zen_Parser_relationalExpression(parser, relationalExpression);

    /* Parse the expression to the right of the operator, if any. */
    while (zen_Parser_isEqualityOperator(zen_TokenStream_la(parser->m_tokens, 1))) {
        jtk_Pair_t* pair = jtk_Pair_new();
        jtk_ArrayList_add(context->m_relationalExpressions, pair);

        zen_Token_t* equalityOperatorToken = zen_TokenStream_lt(parser->m_tokens, 1);
        zen_ASTNode_t* equalityOperator = zen_Parser_newTerminalNode(node, equalityOperatorToken);
        pair->m_left = equalityOperator;
        /* Consume the equality operator. */
        zen_TokenStream_consume(parser->m_tokens);

        zen_ASTNode_t* relationalExpression0 = zen_ASTNode_new(node);
        pair->m_right = relationalExpression0;
        zen_Parser_relationalExpression(parser, relationalExpression0);
    }

    zen_StackTrace_exit();
}

/*
 * equalityOperator
 * :	'=='
 * |	'!='
 * ;
 */
bool zen_Parser_isEqualityOperator(zen_TokenType_t type) {
    return (type == ZEN_TOKEN_EQUAL_2) ||
           (type == ZEN_TOKEN_EXCLAMATION_MARK_EQUAL);
}

/*
 * relationalExpression
 * :	shiftExpression (relationalOperator shiftExpression)*
 * ;
 */
void zen_Parser_relationalExpression(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_RelationalExpressionContext_t* context = zen_RelationalExpressionContext_new(node);

    /* Parse the operand left of the operator. */
    zen_ASTNode_t* shiftExpression = zen_ASTNode_new(node);
    context->m_shiftExpression = shiftExpression;
    zen_Parser_shiftExpression(parser, shiftExpression);

    /* Parse the expression to the right of the operator, if any. */
    while (zen_Parser_isRelationalOperator(zen_TokenStream_la(parser->m_tokens, 1))) {
        jtk_Pair_t* pair = jtk_Pair_new();
        jtk_ArrayList_add(context->m_shiftExpressions, pair);

        zen_Token_t* relationalOperatorToken = zen_TokenStream_lt(parser->m_tokens, 1);
        zen_ASTNode_t* relationalOperator = zen_Parser_newTerminalNode(node, relationalOperatorToken);
        pair->m_left = relationalOperator;
        /* Consume the relational operator. */
        zen_TokenStream_consume(parser->m_tokens);

        zen_ASTNode_t* shiftExpression0 = zen_ASTNode_new(node);
        pair->m_right = shiftExpression0;
        zen_Parser_shiftExpression(parser, shiftExpression0);
    }

    zen_StackTrace_exit();
}

/*
 * relationalOperator
 * :	'<'
 * |	'>'
 * |	'<='
 * |	'>='
 * |	'is'
 * ;
 */
bool zen_Parser_isRelationalOperator(zen_TokenType_t type) {
    return (type == ZEN_TOKEN_LEFT_ANGLE_BRACKET) ||
           (type == ZEN_TOKEN_RIGHT_ANGLE_BRACKET) ||
           (type == ZEN_TOKEN_LEFT_ANGLE_BRACKET_EQUAL) ||
           (type == ZEN_TOKEN_RIGHT_ANGLE_BRACKET_EQUAL) ||
           (type == ZEN_TOKEN_KEYWORD_IS);
}

/*
 * shiftExpression
 * :	additiveExpression (shiftOperator additiveExpression)*
 * ;
 */
void zen_Parser_shiftExpression(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_ShiftExpressionContext_t* context = zen_ShiftExpressionContext_new(node);

    /* Parse the operand left of the operator. */
    zen_ASTNode_t* additiveExpression = zen_ASTNode_new(node);
    context->m_additiveExpression = additiveExpression;
    zen_Parser_additiveExpression(parser, additiveExpression);

    /* Parse the expression to the right of the operator, if any. */
    while (zen_Parser_isShiftOperator(zen_TokenStream_la(parser->m_tokens, 1))) {
        jtk_Pair_t* pair = jtk_Pair_new();
        jtk_ArrayList_add(context->m_additiveExpressions, pair);

        zen_Token_t* shiftOperatorToken = zen_TokenStream_lt(parser->m_tokens, 1);
        zen_ASTNode_t* shiftOperator = zen_Parser_newTerminalNode(node, shiftOperatorToken);
        pair->m_left = shiftOperator;
        /* Consume the shift operator token. */
        zen_TokenStream_consume(parser->m_tokens);

        zen_ASTNode_t* additiveExpression0 = zen_ASTNode_new(node);
        pair->m_right = additiveExpression0;
        zen_Parser_additiveExpression(parser, additiveExpression0);
    }

    zen_StackTrace_exit();
}

/*
 * shiftOperator
 * :	'<<'
 * |	'>>'
 * |	'>>>'
 * ;
 */
bool zen_Parser_isShiftOperator(zen_TokenType_t type) {
    return (type == ZEN_TOKEN_LEFT_ANGLE_BRACKET_2) ||
           (type == ZEN_TOKEN_RIGHT_ANGLE_BRACKET_2) ||
           (type == ZEN_TOKEN_RIGHT_ANGLE_BRACKET_3);
}

/*
 * additiveExpression
 * :	multiplicativeExpression (multiplicativeOperator multiplicativeExpression)*
 * ;
 */
void zen_Parser_additiveExpression(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_AdditiveExpressionContext_t* context = zen_AdditiveExpressionContext_new(node);

    /* Parse the operand left of the operator. */
    zen_ASTNode_t* multiplicativeExpression = zen_ASTNode_new(node);
    context->m_multiplicativeExpression = multiplicativeExpression;
    zen_Parser_multiplicativeExpression(parser, multiplicativeExpression);

    /* Parse the expression to the right of the operator, if any. */
    while (zen_Parser_isAdditiveOperator(zen_TokenStream_la(parser->m_tokens, 1))) {
        jtk_Pair_t* pair = jtk_Pair_new();
        jtk_ArrayList_add(context->m_multiplicativeExpressions, pair);

        zen_Token_t* additiveOperatorToken = zen_TokenStream_lt(parser->m_tokens, 1);
        zen_ASTNode_t* additiveOperator = zen_Parser_newTerminalNode(node, additiveOperatorToken);
        pair->m_left = additiveOperator;
        /* Consume the additive operator token. */
        zen_TokenStream_consume(parser->m_tokens);

        zen_ASTNode_t* multiplicativeExpression0 = zen_ASTNode_new(node);
        pair->m_right = multiplicativeExpression0;
        zen_Parser_multiplicativeExpression(parser, multiplicativeExpression0);
    }

    zen_StackTrace_exit();
}

/*
 * additiveOperator
 * :	'+'
 * |	'-'
 * ;
 */
bool zen_Parser_isAdditiveOperator(zen_TokenType_t type) {
    return (type == ZEN_TOKEN_PLUS) ||
           (type == ZEN_TOKEN_DASH);
}

/*
 * multiplicativeExpression
 * :	unaryExpression (multiplicativeOperator unaryExpression)*
 * ;
 */
void zen_Parser_multiplicativeExpression(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_MultiplicativeExpressionContext_t* context = zen_MultiplicativeExpressionContext_new(node);

    /* Parse the operand left of the operator. */
    zen_ASTNode_t* unaryExpression = zen_ASTNode_new(node);
    context->m_unaryExpression = unaryExpression;
    zen_Parser_unaryExpression(parser, unaryExpression);

    /* Parse the expression to the right of the operator, if any. */
    while (zen_Parser_isMultiplicativeOperator(zen_TokenStream_la(parser->m_tokens, 1))) {
        jtk_Pair_t* pair = jtk_Pair_new();
        jtk_ArrayList_add(context->m_unaryExpressions, pair);

        zen_Token_t* multiplicativeOperatorToken = zen_TokenStream_lt(parser->m_tokens, 1);
        zen_ASTNode_t* multiplicativeOperator = zen_Parser_newTerminalNode(node, multiplicativeOperatorToken);
        pair->m_left = multiplicativeOperator;
        /* Consume the multiplicative operator token. */
        zen_TokenStream_consume(parser->m_tokens);

        zen_ASTNode_t* unaryExpression0 = zen_ASTNode_new(node);
        pair->m_right = unaryExpression0;
        zen_Parser_unaryExpression(parser, unaryExpression0);
    }

    zen_StackTrace_exit();
}

/*
 * multiplicativeOperator
 * :	'*'
 * |	'/'
 * |	'%'
 * ;
 */
bool zen_Parser_isMultiplicativeOperator(zen_TokenType_t type) {
    return (type == ZEN_TOKEN_ASTERISK) ||
           (type == ZEN_TOKEN_FORWARD_SLASH) ||
           (type == ZEN_TOKEN_MODULUS);
}

/*
 * unaryExpression
 * :	unaryOperator unaryExpression
 * |	postfixExpression
 * ;
 */
void zen_Parser_unaryExpression(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_UnaryExpressionContext_t* context = zen_UnaryExpressionContext_new(node);

    zen_TokenType_t la1 = zen_TokenStream_la(parser->m_tokens, 1);
    if (zen_Parser_isUnaryOperator(la1)) {
        zen_Token_t* unaryOperator = zen_TokenStream_lt(parser->m_tokens, 1);
        context->m_unaryOperator = zen_Parser_newTerminalNode(node, unaryOperator);
        /* Consume the unary operator token. */
        zen_TokenStream_consume(parser->m_tokens);

        zen_ASTNode_t* unaryExpression = zen_ASTNode_new(node);
        context->m_unaryExpression = unaryExpression;
        zen_Parser_unaryExpression(parser, unaryExpression);
    }
    else if (zen_Parser_isPostfixExpressionFollow(la1)) {
        zen_ASTNode_t* postfixExpression = zen_ASTNode_new(node);
        context->m_postfixExpression = postfixExpression;
        zen_Parser_postfixExpression(parser, postfixExpression);
    }
    else {
        // Error: Expected unary operator or postfix expression follow
        zen_Parser_reportAndRecover(parser, ZEN_TOKEN_INTEGER_LITERAL);
    }

    zen_StackTrace_exit();
}

bool zen_Parser_isUnaryExpressionFollow(zen_TokenType_t type) {
    return zen_Parser_isUnaryOperator(type) || zen_Parser_isPostfixExpressionFollow(type);
}

/*
 * unaryOperator
 * :    '+'
 * |    '-'
 * |    '~'
 * |    '!'
 * // |    '++'
 * // |    '--'
 * ;
 */
bool zen_Parser_isUnaryOperator(zen_TokenType_t type) {
    return (type == ZEN_TOKEN_PLUS) ||
           (type == ZEN_TOKEN_DASH) ||
           (type == ZEN_TOKEN_TILDE) ||
           (type == ZEN_TOKEN_EXCLAMATION_MARK);
           /*
           ||
           (type == ZEN_TOKEN_PLUS_2) ||
           (type == ZEN_TOKEN_DASH_2);
           */
}

/*
 * postfixExpression
 * :	primaryExpression postfixPart*
 * ;
 *
 * postfixPart
 * :    subscript
 * |    functionArguments
 * |    memberAccess
 * |    postfixOperator
 * ;
 *
 * The following function combines both the rules. This measure was
 * taken to avoid redundant nodes in the AST.
 */
void zen_Parser_postfixExpression(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_PostfixExpressionContext_t* context = zen_PostfixExpressionContext_new(node);

    zen_ASTNode_t* primaryExpression = zen_ASTNode_new(node);
    context->m_primaryExpression = primaryExpression;
    zen_Parser_primaryExpression(parser, primaryExpression);

    zen_TokenType_t la1 = zen_TokenStream_la(parser->m_tokens, 1);
    while (zen_Parser_isPostfixPartFollow(la1)) {
        switch (la1) {
            case ZEN_TOKEN_LEFT_SQUARE_BRACKET: {
                zen_ASTNode_t* subscript = zen_ASTNode_new(node);
                jtk_ArrayList_add(context->m_postfixParts, subscript);
                zen_Parser_subscript(parser, subscript);
                break;
            }

            case ZEN_TOKEN_LEFT_PARENTHESIS: {
                zen_ASTNode_t* functionArguments = zen_ASTNode_new(node);
                jtk_ArrayList_add(context->m_postfixParts, functionArguments);

                zen_Parser_functionArguments(parser, functionArguments);

                break;
            }

            case ZEN_TOKEN_DOT: {
                zen_ASTNode_t* memberAccess = zen_ASTNode_new(node);
                jtk_ArrayList_add(context->m_postfixParts, memberAccess);
                zen_Parser_memberAccess(parser, memberAccess);
                break;
            }

            // case ZEN_TOKEN_PLUS_2:
            // case ZEN_TOKEN_DASH_2: {
            //     zen_ASTNode_t* postfixOperator = zen_ASTNode_new(node);
            //     jtk_ArrayList_add(context->m_postfixParts, postfixOperator);
            //     zen_Parser_postfixOperator(parser, postfixOperator);
            //     break;
            // }
        }
        la1 = zen_TokenStream_la(parser->m_tokens, 1);
    }

    zen_StackTrace_exit();
}

bool zen_Parser_isPostfixExpressionFollow(zen_TokenType_t type) {
    return zen_Parser_isPrimaryExpressionFollow(type);
}

bool zen_Parser_isPostfixPartFollow(zen_TokenType_t type) {
    return (type == ZEN_TOKEN_LEFT_SQUARE_BRACKET) ||
           (type == ZEN_TOKEN_LEFT_PARENTHESIS) ||
           (type == ZEN_TOKEN_DOT) ||
           (type == ZEN_TOKEN_PLUS_2) ||
           (type == ZEN_TOKEN_DASH_2);
}

/*
 * subscript
 * :	'[' expression ']'
 * ;
 */
void zen_Parser_subscript(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_SubscriptContext_t* context = zen_SubscriptContext_new(node);

    /* Match and discard the '[' token. */
    zen_Parser_match(parser, ZEN_TOKEN_LEFT_SQUARE_BRACKET);

    /* If expression fails, discard tokens until the ']' token is
     * encountered.
     */
    zen_Parser_pushFollowToken(parser, ZEN_TOKEN_RIGHT_SQUARE_BRACKET);

    zen_ASTNode_t* expression = zen_ASTNode_new(node);
    context->m_expression = expression;
    zen_Parser_expression(parser, expression);

    /* Pop the ']' token from the follow set. */
    zen_Parser_popFollowToken(parser);

    /* Match and discard the ']' token. */
    zen_Parser_match(parser, ZEN_TOKEN_RIGHT_SQUARE_BRACKET);

    zen_StackTrace_exit();
}

/*
 * functionArguments
 * :	'(' expressions? ')'
 * ;
 */
void zen_Parser_functionArguments(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_FunctionArgumentsContext_t* context = zen_FunctionArgumentsContext_new(node);

    /* Match and discard the '(' token. */
    zen_Parser_match(parser, ZEN_TOKEN_LEFT_PARENTHESIS);

    if (zen_Parser_isExpressionFollow(zen_TokenStream_la(parser->m_tokens, 1))) {
        /* If expression fails, discard tokens until the ')' token is
        * encountered.
        */
        zen_Parser_pushFollowToken(parser, ZEN_TOKEN_RIGHT_PARENTHESIS);

        zen_ASTNode_t* expressions = zen_ASTNode_new(node);
        context->m_expressions = expressions;
        zen_Parser_expressions(parser, expressions);

        /* Pop the ')' token from the follow set. */
        zen_Parser_popFollowToken(parser);
    }

    /* Match and discard the ')' token. */
    zen_Parser_match(parser, ZEN_TOKEN_RIGHT_PARENTHESIS);

    zen_StackTrace_exit();
}

/*
 * memberAccess
 * :	'.' IDENTIFIER
 * ;
 */
void zen_Parser_memberAccess(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_MemberAccessContext_t* context = zen_MemberAccessContext_new(node);

    /* Match and discard the '.' token. */
    zen_Parser_match(parser, ZEN_TOKEN_DOT);

    zen_Token_t* identifier = zen_Parser_matchAndYield(parser, ZEN_TOKEN_IDENTIFIER);
    context->m_identifier = zen_Parser_newTerminalNode(node, identifier);

    zen_StackTrace_exit();
}

/*
 * postfixOperator
 * :	'++'
 * |	'--'
 * ;
 */
void zen_Parser_postfixOperator(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_PostfixOperatorContext_t* context = zen_PostfixOperatorContext_new(node);

    switch (zen_TokenStream_la(parser->m_tokens, 1)) {
        case ZEN_TOKEN_PLUS_2:
        case ZEN_TOKEN_DASH_2: {
            zen_Token_t* postfixOperator = zen_TokenStream_lt(parser->m_tokens, 1);
            context->m_postfixOperator = zen_Parser_newTerminalNode(node, postfixOperator);
            /* Consume the '++' or '--' token. */
            zen_TokenStream_consume(parser->m_tokens);

            break;
        }

        default: {
            // Syntax error
        }
    }

    zen_StackTrace_exit();
}

/*
 * primaryExpression
 * :	IDENTIFIER
 * |	literal
 * |	'(' expression ')'
 * |	mapExpression
 * |	listExpression
 * |    newExpression
 * ;
 *
 * literal
 * :	INTEGER_LITERAL
 * |	FLOATING_POINT_LITERAL
 * |	'true'
 * |	'false'
 * |	STRING_LITERAL
 * |	'null'
 * |    'this'
 * ;
 *
 * NOTE: The primaryExpression rule has no context. It simply forwards the
 * AST node it receives to the best matching child rule.
 *
 */
void zen_Parser_primaryExpression(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_PrimaryExpressionContext_t* context = zen_PrimaryExpressionContext_new(node);
    zen_TokenType_t la1 = zen_TokenStream_la(parser->m_tokens, 1);
    if (zen_Parser_isLiteralFollow(la1)) {
        zen_Token_t* literal = zen_TokenStream_lt(parser->m_tokens, 1);
        context->m_expression = zen_Parser_newTerminalNode(node, literal);
        /* Consume the literal token. */
        zen_TokenStream_consume(parser->m_tokens);
    }
    else {
        switch (la1) {
            case ZEN_TOKEN_IDENTIFIER: {
                zen_Token_t* identifier = zen_TokenStream_lt(parser->m_tokens, 1);
                context->m_expression = zen_Parser_newTerminalNode(node, identifier);
                /* Consume the identifier token. */
                zen_TokenStream_consume(parser->m_tokens);

                break;
            }

            case ZEN_TOKEN_LEFT_PARENTHESIS: {
                /* Consume and discard the '(' token. */
                zen_TokenStream_consume(parser->m_tokens);

                /* If expression fails, discard tokens until the ')' token is
                 * encountered.
                 */
                zen_Parser_pushFollowToken(parser, ZEN_TOKEN_RIGHT_PARENTHESIS);

                zen_ASTNode_t* expression = zen_ASTNode_new(node);
                context->m_expression = expression;
                zen_Parser_expression(parser, expression);

                /* Pop the ')' token from the follow set. */
                zen_Parser_popFollowToken(parser);

                /* Match and discard the ')' token. */
                zen_Parser_match(parser, ZEN_TOKEN_RIGHT_PARENTHESIS);

                break;
            }

            case ZEN_TOKEN_LEFT_BRACE: {
                zen_ASTNode_t* mapExpression = zen_ASTNode_new(node);
                context->m_expression = mapExpression;
                zen_Parser_mapExpression(parser, mapExpression);

                break;
            }

            case ZEN_TOKEN_LEFT_SQUARE_BRACKET: {
                zen_ASTNode_t* listExpression = zen_ASTNode_new(node);
                context->m_expression = listExpression;
                zen_Parser_listExpression(parser, listExpression);

                break;
            }

            case ZEN_TOKEN_KEYWORD_NEW: {
                zen_ASTNode_t* newExpression = zen_ASTNode_new(node);
                context->m_expression = newExpression;
                zen_Parser_newExpression(parser, newExpression);

                break;
            }

            case ZEN_TOKEN_KEYWORD_THIS: {
                zen_Token_t* keyword = zen_TokenStream_lt(parser->m_tokens, 1);
                context->m_expression = zen_Parser_newTerminalNode(node, keyword);
                /* Consume the this keyword. */
                zen_TokenStream_consume(parser->m_tokens);

                break;
            }

            default: {
                printf("[internal error] Control should not reach here.\n");

                break;
            }
        }
    }

    zen_StackTrace_exit();
}

bool zen_Parser_isPrimaryExpressionFollow(zen_TokenType_t type) {
    bool result = false;
    if (zen_Parser_isLiteralFollow(type)) {
        result = true;
    }
    else {
        switch (type) {
            case ZEN_TOKEN_KEYWORD_THIS:
            case ZEN_TOKEN_IDENTIFIER:
            case ZEN_TOKEN_LEFT_PARENTHESIS:
            case ZEN_TOKEN_LEFT_BRACE:
            case ZEN_TOKEN_LEFT_SQUARE_BRACKET:
            case ZEN_TOKEN_LEFT_ANGLE_BRACKET:
            case ZEN_TOKEN_KEYWORD_NEW: {
                result = true;
                break;
            }
        }
    }
    return result;
}

bool zen_Parser_isLiteral(zen_TokenType_t type) {
    bool result = false;
    switch (type) {
        case ZEN_TOKEN_INTEGER_LITERAL:
        case ZEN_TOKEN_FLOATING_POINT_LITERAL:
        case ZEN_TOKEN_KEYWORD_TRUE:
        case ZEN_TOKEN_KEYWORD_FALSE:
        case ZEN_TOKEN_STRING_LITERAL:
        case ZEN_TOKEN_KEYWORD_NULL: {
            result = true;
            break;
        }
    }
    return result;
}

bool zen_Parser_isLiteralFollow(zen_TokenType_t type) {
    return zen_Parser_isLiteral(type);
}

/*
 * mapExpression
 * :	'{' mapEntries? '}'
 * ;
 */
/*
 * TODO: We can add an arbitary ',' in the end of a map, list, or an array.
 *		 Simply use the isExpressionFollow() function.
 */
void zen_Parser_mapExpression(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

	zen_MapExpressionContext_t* context = zen_MapExpressionContext_new(node);

    /* Match and discard the '{' token. */
    zen_Parser_match(parser, ZEN_TOKEN_LEFT_BRACE);

    /* If expression fails, discard tokens until the '}' token is
     * encountered.
     */
    zen_Parser_pushFollowToken(parser, ZEN_TOKEN_RIGHT_BRACE);

    if (zen_Parser_isExpressionFollow(zen_TokenStream_la(parser->m_tokens, 1))) {
        zen_ASTNode_t* mapEntries = zen_ASTNode_new(node);
        context->m_mapEntries = mapEntries;
        zen_Parser_mapEntries(parser, mapEntries);
    }

    /* Pop the '}' token from the follow set. */
    zen_Parser_popFollowToken(parser);

    /* Match and discard the '}' token. */
    zen_Parser_match(parser, ZEN_TOKEN_RIGHT_BRACE);

    zen_StackTrace_exit();
}

/*
 * mapEntries
 * :	mapEntry (',' mapEntry)*
 * ;
 */
void zen_Parser_mapEntries(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_MapEntriesContext_t* context = zen_MapEntriesContext_new(node);

    zen_ASTNode_t* mapEntry = zen_ASTNode_new(node);
    jtk_ArrayList_add(context->m_mapEntries, mapEntry);
    zen_Parser_mapEntry(parser, mapEntry);

    while (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_COMMA) {
        /* Consume and discard the ',' token. */
        zen_TokenStream_consume(parser->m_tokens);

        mapEntry = zen_ASTNode_new(node);
        jtk_ArrayList_add(context->m_mapEntries, mapEntry);
        zen_Parser_mapEntry(parser, mapEntry);
    }

    zen_StackTrace_exit();
}

/*
 * mapEntry
 * :	expression ':' expression
 * ;
 */
void zen_Parser_mapEntry(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_MapEntryContext_t* context = zen_MapEntryContext_new(node);

    zen_ASTNode_t* keyExpression = zen_ASTNode_new(node);
    context->m_keyExpression = keyExpression;
    zen_Parser_expression(parser, keyExpression);

    /* Match and discard the ':' token. */
    zen_Parser_match(parser, ZEN_TOKEN_COLON);

    zen_ASTNode_t* valueExpression = zen_ASTNode_new(node);
    context->m_valueExpression = valueExpression;
    zen_Parser_expression(parser, valueExpression);

    zen_StackTrace_exit();
}

/*
 * listExpression
 * :    '[' expressions ']'
 * ;
 */
void zen_Parser_listExpression(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_ListExpressionContext_t* context = zen_ListExpressionContext_new(node);

    /* Match and discard the '[' token. */
    zen_Parser_match(parser, ZEN_TOKEN_LEFT_SQUARE_BRACKET);

    if (zen_Parser_isExpressionFollow(zen_TokenStream_la(parser->m_tokens, 1))) {
        /* If expression fails, discard tokens until the ')' token is
        * encountered.
        */
        zen_Parser_pushFollowToken(parser, ZEN_TOKEN_RIGHT_SQUARE_BRACKET);

        zen_ASTNode_t* expressions = zen_ASTNode_new(node);
        context->m_expressions = expressions;
        zen_Parser_expressions(parser, expressions);

        /* Pop the ']' token from the follow set. */
        zen_Parser_popFollowToken(parser);
    }

    /* Match and discard the ']' token. */
    zen_Parser_match(parser, ZEN_TOKEN_RIGHT_SQUARE_BRACKET);

    zen_StackTrace_exit();
}

/*
 * newExpression
 * :   'new' typeName functionArguments?
 * ;
 */
void zen_Parser_newExpression(zen_Parser_t* parser, zen_ASTNode_t* node) {
    zen_StackTrace_enter();

    zen_NewExpressionContext_t* context = zen_NewExpressionContext_new(node);

    /* Match and discard the 'new' token. */
    zen_Parser_match(parser, ZEN_TOKEN_KEYWORD_NEW);

    zen_ASTNode_t* typeName = zen_ASTNode_new(node);
    context->m_typeName = typeName;
    zen_Parser_typeName(parser, typeName);

    if (zen_TokenStream_la(parser->m_tokens, 1) == ZEN_TOKEN_LEFT_PARENTHESIS) {
        zen_ASTNode_t* functionArguments = zen_ASTNode_new(node);
        context->m_functionArguments = functionArguments;
        zen_Parser_functionArguments(parser, functionArguments);
    }

    zen_StackTrace_exit();
}

// TODO: recover(IMPORT_DECLARATION_RECOVERY_SET);
