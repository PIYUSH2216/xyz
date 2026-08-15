#ifndef XYZ_LEXER_H
#define XYZ_LEXER_H

#include "token.h"

typedef struct {
    const char *source;
    const char *start;
    const char *current;
    int line;
} Lexer;

void lexer_init(Lexer *lexer, const char *source);

Token lexer_scan_token(Lexer *lexer);

#endif
