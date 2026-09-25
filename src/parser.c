#include <stdio.h>
#include "parser.h"

static ASTNode *parse_expression(Parser *parser);
static ASTNode *parse_or(Parser *parser);
static ASTNode *parse_and(Parser *parser);
static ASTNode *parse_comparison(Parser *parser);
static ASTNode *parse_additive(Parser *parser);
static ASTNode *parse_term(Parser *parser);
static ASTNode *parse_factor(Parser *parser);
static ASTNode *parse_primary(Parser *parser);

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

	if (check(parser, TOKEN_LEFT_PAREN)) {
    advance_parser(parser);

    ASTNode *expression = parse_expression(parser);

    if (expression == NULL) {
        return NULL;
    }

    if (!consume(
            parser,
            TOKEN_RIGHT_PAREN,
            "Expected ')' after expression."
        )) {
        ast_free(expression);
        return NULL;
    }

    return expression;
}
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

/* Variable reference */
if (check(parser, TOKEN_IDENTIFIER)) {
    advance_parser(parser);

    return ast_create_variable(
        token.start,
        token.length
    );
}

parser_error(parser, "Expected an expression.");
return NULL;

}


static ASTNode *parse_additive(Parser *parser)
{
    ASTNode *left = parse_term(parser);

    if (left == NULL)
        return NULL;

    while (check(parser, TOKEN_PLUS) ||
           check(parser, TOKEN_MINUS))
    {
        TokenType operator = parser->current.type;
        advance_parser(parser);

        ASTNode *right = parse_term(parser);

        if (right == NULL)
        {
            ast_free(left);
            return NULL;
        }

        ASTNode *binary =
            ast_create_binary(left, operator, right);

        if (binary == NULL)
        {
            ast_free(left);
            ast_free(right);
            return NULL;
        }

        left = binary;
    }

    return left;
}
static ASTNode *parse_comparison(Parser *parser)
{
    ASTNode *left = parse_additive(parser);

    if (left == NULL)
        return NULL;

    while (check(parser, TOKEN_EQUAL_EQUAL) ||
           check(parser, TOKEN_BANG_EQUAL) ||
           check(parser, TOKEN_LESS) ||
           check(parser, TOKEN_LESS_EQUAL) ||
           check(parser, TOKEN_GREATER) ||
           check(parser, TOKEN_GREATER_EQUAL))
    {
        TokenType operator = parser->current.type;
        advance_parser(parser);

        ASTNode *right = parse_additive(parser);

        if (right == NULL)
        {
            ast_free(left);
            return NULL;
        }

        ASTNode *binary =
            ast_create_binary(left, operator, right);

        if (binary == NULL)
        {
            ast_free(left);
            ast_free(right);
            return NULL;
        }

        left = binary;
    }

    return left;
}

static ASTNode *parse_and(Parser *parser)
{
    ASTNode *left = parse_comparison(parser);

    if (left == NULL)
        return NULL;

    while (check(parser, TOKEN_AND))
    {
        TokenType operator = parser->current.type;
        advance_parser(parser);

        ASTNode *right = parse_comparison(parser);

        if (right == NULL)
        {
            ast_free(left);
            return NULL;
        }

        ASTNode *binary =
            ast_create_binary(left, operator, right);

        if (binary == NULL)
        {
            ast_free(left);
            ast_free(right);
            return NULL;
        }

        left = binary;
    }

    return left;
}

static ASTNode *parse_or(Parser *parser)
{
    ASTNode *left = parse_and(parser);

    if (left == NULL)
        return NULL;

    while (check(parser, TOKEN_OR))
    {
        TokenType operator = parser->current.type;
        advance_parser(parser);

        ASTNode *right = parse_and(parser);

        if (right == NULL)
        {
            ast_free(left);
            return NULL;
        }

        ASTNode *binary =
            ast_create_binary(left, operator, right);

        if (binary == NULL)
        {
            ast_free(left);
            ast_free(right);
            return NULL;
        }

        left = binary;
    }

    return left;
}

static ASTNode *parse_expression(Parser *parser)
{
    return parse_or(parser);
}
         
static ASTNode *parse_term(Parser *parser) {
    ASTNode *left = parse_factor(parser);

    if (left == NULL) {
        return NULL;
    }

    while (check(parser, TOKEN_STAR) ||
           check(parser, TOKEN_SLASH) ||
           check(parser, TOKEN_PERCENT)) {

        TokenType operator = parser->current.type;

        advance_parser(parser);

        ASTNode *right = parse_factor(parser);

        if (right == NULL) {
            ast_free(left);
            return NULL;
        }

        ASTNode *binary =
            ast_create_binary(left, operator, right);

        if (binary == NULL) {
            ast_free(left);
            ast_free(right);
            return NULL;
        }

        left = binary;
    }

    return left;
}

static ASTNode *parse_factor(Parser *parser) {
    return parse_primary(parser);
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

    ASTNode *value = parse_expression(parser);

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
