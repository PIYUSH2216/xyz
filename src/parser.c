#include <stdio.h>
#include "parser.h"

static void advance_parser(Parser *parser) {
    parser->previous = parser->current;
    parser->current = lexer_scan_token(&parser->lexer);
}

static void parser_error(Parser *parser, const char *message) {
    if (parser->had_error) {
        return;
    }

    parser->had_error = 1;

    fprintf(stderr,
            "XYZ Parser Error at line %d: %s\n",
            parser->current.line,
            message);
}

static int check(Parser *parser, TokenType type) {
    return parser->current.type == type;
}

static int consume(Parser *parser, TokenType type, const char *message) {
    if (check(parser, type)) {
        advance_parser(parser);
        return 1;
    }

    parser_error(parser, message);
    return 0;
}

static ASTNode *parse_primary(Parser *parser) {
    Token token = parser->current;

    /* Integer literal */
    if (check(parser, TOKEN_INTEGER)) {
        advance_parser(parser);

        char buffer[64];

        if (token.length >= (int)sizeof(buffer)) {
            parser_error(parser, "Integer literal is too long.");
            return NULL;
        }

        int i;

        for (i = 0; i < token.length; i++) {
            buffer[i] = token.start[i];
        }

        buffer[token.length] = '\0';

        long value = 0;

        if (sscanf(buffer, "%ld", &value) != 1) {
            parser_error(parser, "Invalid integer literal.");
            return NULL;
        }

        return ast_create_integer(value);
    }
	if (check(parser, TOKEN_FLOAT)) {
 	   Token token = parser->current;
	   advance_parser(parser);
 	   char buffer[64];
	   if (token.length >= (int)sizeof(buffer)) {
        	parser_error(parser, "Float literal is too long.");
        	return NULL;
    		}
		for (int i = 0; i < token.length; i++) {
        		buffer[i] = token.start[i];
    			}

    		buffer[token.length] = '\0';
		double value = 0.0;
		if (sscanf(buffer, "%lf", &value) != 1) {
        		parser_error(parser, "Invalid float literal.");
        		return NULL;
    			}

    	return ast_create_float(value);
	}
    /* Null literal */
    if (check(parser, TOKEN_NULL)) {
        advance_parser(parser);
        return ast_create_null();
    }

     /* String literal */
    if (check(parser, TOKEN_STRING)) {
    advance_parser(parser);

    return ast_create_string(
        token.start,
        token.length
    );
    }
    /* Boolean literal */
    if (check(parser, TOKEN_TRUE)) {
    advance_parser(parser);
    return ast_create_boolean(1);
    }

    if (check(parser, TOKEN_FALSE)) {
    advance_parser(parser);
    return ast_create_boolean(0);
    }
    parser_error(parser, "Expected an expression.");
    return NULL;
}
static ASTNode *parse_variable_declaration(Parser *parser) {
    advance_parser(parser); /* consume 'let' */

    Token name = parser->current;

    if (!consume(
            parser,
            TOKEN_IDENTIFIER,
            "Expected variable name after 'let'."
        )) {
        return NULL;
    }

    if (!consume(
            parser,
            TOKEN_EQUAL,
            "Expected '=' after variable name."
        )) {
        return NULL;
    }

    ASTNode *value = parse_primary(parser);

    if (value == NULL) {
        return NULL;
    }

    if (!consume(
            parser,
            TOKEN_SEMICOLON,
            "Expected ';' after variable declaration."
        )) {
        ast_free(value);
        return NULL;
    }

    return ast_create_variable_declaration(
        name.start,
        name.length,
        value
    );
}

void parser_init(Parser *parser, const char *source) {
    lexer_init(&parser->lexer, source);

    parser->current.type = TOKEN_EOF;
    parser->previous.type = TOKEN_EOF;

    parser->had_error = 0;

    advance_parser(parser);
}

ASTNode *parser_parse(Parser *parser) {
    ASTNode *program = ast_create_program();

    if (program == NULL) {
        parser_error(parser, "Could not create program AST.");
        return NULL;
    }

    while (!check(parser, TOKEN_EOF)) {

        if (check(parser, TOKEN_LET)) {
            ASTNode *statement = parse_variable_declaration(parser);

            if (statement == NULL) {
                ast_free(program);
                return NULL;
            }

            if (!ast_program_add(program, statement)) {
                parser_error(
                    parser,
                    "Could not add statement to program."
                );

                ast_free(statement);
                ast_free(program);
                return NULL;
            }

            continue;
        }

        parser_error(
            parser,
            "Expected a declaration or statement."
        );

        ast_free(program);
        return NULL;
    }

    return program;
}
