#include <ctype.h>
#include <string.h>

#include "lexer.h"

static int is_at_end(Lexer *lexer) {
    return *lexer->current == '\0';
}

static char advance_char(Lexer *lexer) {
    return *lexer->current++;
}

static char peek_char(Lexer *lexer) {
    if (is_at_end(lexer)) {
        return '\0';
    }

    return *lexer->current;
}

static void skip_whitespace(Lexer *lexer) {
    while (1) {
        char c = peek_char(lexer);

        if (c == ' ' || c == '\r' || c == '\t') {
            advance_char(lexer);
        } else if (c == '\n') {
            lexer->line++;
            advance_char(lexer);
        } else {
            return;
        }
    }
}

static Token make_token(Lexer *lexer, TokenType type) {
    Token token;

    token.type = type;
    token.start = lexer->start;
    token.length = (int)(lexer->current - lexer->start);
    token.line = lexer->line;

    return token;
}

static Token error_token(const char *message, Lexer *lexer) {
    Token token;

    token.type = TOKEN_EOF;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = lexer->line;

    return token;
}

static int is_alpha(char c) {
    return isalpha((unsigned char)c) || c == '_';
}

static int is_digit(char c) {
    return isdigit((unsigned char)c);
}

static Token identifier(Lexer *lexer) {
    while (is_alpha(peek_char(lexer)) || is_digit(peek_char(lexer))) {
        advance_char(lexer);
    }

    int length = (int)(lexer->current - lexer->start);

    if (length == 3 && strncmp(lexer->start, "let", 3) == 0) {
        return make_token(lexer, TOKEN_LET);
    }

    if (length == 5 && strncmp(lexer->start, "const", 5) == 0) {
        return make_token(lexer, TOKEN_CONST);
    }

    if (length == 2 && strncmp(lexer->start, "fn", 2) == 0) {
        return make_token(lexer, TOKEN_FN);
    }

    if (length == 6 && strncmp(lexer->start, "return", 6) == 0) {
        return make_token(lexer, TOKEN_RETURN);
    }

    if (length == 2 && strncmp(lexer->start, "if", 2) == 0) {
        return make_token(lexer, TOKEN_IF);
    }

    if (length == 4 && strncmp(lexer->start, "else", 4) == 0) {
        return make_token(lexer, TOKEN_ELSE);
    }

    if (length == 5 && strncmp(lexer->start, "while", 5) == 0) {
        return make_token(lexer, TOKEN_WHILE);
    }

    if (length == 3 && strncmp(lexer->start, "for", 3) == 0) {
        return make_token(lexer, TOKEN_FOR);
    }

    if (length == 5 && strncmp(lexer->start, "break", 5) == 0) {
        return make_token(lexer, TOKEN_BREAK);
    }

    if (length == 8 && strncmp(lexer->start, "continue", 8) == 0) {
        return make_token(lexer, TOKEN_CONTINUE);
    }

    if (length == 4 && strncmp(lexer->start, "true", 4) == 0) {
        return make_token(lexer, TOKEN_TRUE);
    }

    if (length == 5 && strncmp(lexer->start, "false", 5) == 0) {
        return make_token(lexer, TOKEN_FALSE);
    }

    if (length == 4 && strncmp(lexer->start, "null", 4) == 0) {
        return make_token(lexer, TOKEN_NULL);
    }

    if (length == 6 && strncmp(lexer->start, "import", 6) == 0) {
        return make_token(lexer, TOKEN_IMPORT);
    }

    if (length == 5 && strncmp(lexer->start, "allow", 5) == 0) {
        return make_token(lexer, TOKEN_ALLOW);
    }

    if (length == 7 && strncmp(lexer->start, "explain", 7) == 0) {
        return make_token(lexer, TOKEN_EXPLAIN);
    }
    if (length == 3 && strncmp(lexer->start, "AND", 3) == 0) {
    return make_token(lexer, TOKEN_AND);
    }
    if (length == 2 && strncmp(lexer->start, "OR", 2) == 0) {
    return make_token(lexer, TOKEN_OR);
    }
    if (length == 5 && strncmp(lexer->start, "print", 5) == 0){
    return make_token(lexer, TOKEN_PRINT);
    }
    return make_token(lexer, TOKEN_IDENTIFIER);
}

static Token number(Lexer *lexer) {
    while (is_digit(peek_char(lexer))) {
        advance_char(lexer);
    }

    /* Check for fractional part */
    if (peek_char(lexer) == '.' &&
        is_digit(lexer->current[1])) {

        advance_char(lexer); /* consume '.' */

        while (is_digit(peek_char(lexer))) {
            advance_char(lexer);
        }

        return make_token(lexer, TOKEN_FLOAT);
    }

    return make_token(lexer, TOKEN_INTEGER);
}

static Token string(Lexer *lexer) {
    while (peek_char(lexer) != '"' && !is_at_end(lexer)) {
        if (peek_char(lexer) == '\n') {
            lexer->line++;
        }

        advance_char(lexer);
    }

    if (is_at_end(lexer)) {
        return error_token("Unterminated string.", lexer);
    }

    advance_char(lexer); /* Closing quote */

    return make_token(lexer, TOKEN_STRING);
}
void lexer_init(Lexer *lexer, const char *source) {
    lexer->source = source;
    lexer->start = source;
    lexer->current = source;
    lexer->line = 1;
}

Token lexer_scan_token(Lexer *lexer) {
    skip_whitespace(lexer);

    lexer->start = lexer->current;

    if (is_at_end(lexer)) {
        return make_token(lexer, TOKEN_EOF);
    }

    char c = advance_char(lexer);

    if (is_alpha(c)) {
        return identifier(lexer);
    }

    if (is_digit(c)) {
        return number(lexer);
    }

    switch (c) {

	case '"':
    	    return string(lexer);
        case '+':
            return make_token(lexer, TOKEN_PLUS);

        case '-':
            return make_token(lexer, TOKEN_MINUS);

        case '*':
            return make_token(lexer, TOKEN_STAR);

        case '/':
            return make_token(lexer, TOKEN_SLASH);

        case '%':
            return make_token(lexer, TOKEN_PERCENT);

        case '=':
            if (peek_char(lexer) == '=') {
                advance_char(lexer);
                return make_token(lexer, TOKEN_EQUAL_EQUAL);
            }

            return make_token(lexer, TOKEN_EQUAL);

        case '!':
            if (peek_char(lexer) == '=') {
                advance_char(lexer);
                return make_token(lexer, TOKEN_BANG_EQUAL);
            }

            return make_token(lexer, TOKEN_BANG);

        case '<':
            if (peek_char(lexer) == '=') {
                advance_char(lexer);
                return make_token(lexer, TOKEN_LESS_EQUAL);
            }

            return make_token(lexer, TOKEN_LESS);

        case '>':
            if (peek_char(lexer) == '=') {
                advance_char(lexer);
                return make_token(lexer, TOKEN_GREATER_EQUAL);
            }

            return make_token(lexer, TOKEN_GREATER);

        case '(':
            return make_token(lexer, TOKEN_LEFT_PAREN);

        case ')':
            return make_token(lexer, TOKEN_RIGHT_PAREN);

        case '{':
            return make_token(lexer, TOKEN_LEFT_BRACE);

        case '}':
            return make_token(lexer, TOKEN_RIGHT_BRACE);

        case ',':
            return make_token(lexer, TOKEN_COMMA);

        case '.':
            return make_token(lexer, TOKEN_DOT);

        case ';':
            return make_token(lexer, TOKEN_SEMICOLON);

        default:
            return error_token("Unexpected character.", lexer);
    }
}

