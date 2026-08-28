#ifndef XYZ_AST_H
#define XYZ_AST_H

#include "token.h"

typedef enum {
    AST_PROGRAM,
    AST_INTEGER,
    AST_FLOAT,
    AST_STRING,
    AST_NULL,
    AST_BOOLEAN,
    AST_VARIABLE,
    AST_BINARY,
    AST_VARIABLE_DECLARATION
} ASTNodeType;

typedef struct ASTNode ASTNode;

ASTNode *ast_create_null(void);

struct ASTNode {
    ASTNodeType type;

    union {
	struct {
    	    ASTNode **statements;
    	    int count;
 	    int capacity;
	} program;
        long integer_value;
        double float_value;
        int boolean_value;

        struct {
            const char *value;
            int length;
        } string;

        struct {
            const char *name;
            int length;
        } variable;

        struct {
            ASTNode *left;
            TokenType operator;
            ASTNode *right;
        } binary;

        struct {
            const char *name;
            int name_length;
            ASTNode *value;
        } variable_declaration;
    };
};

ASTNode *ast_create_program(void);
int ast_program_add(ASTNode *program, ASTNode *statement);

ASTNode *ast_create_integer(long value);
ASTNode *ast_create_float(double value);
ASTNode *ast_create_boolean(int value);

ASTNode *ast_create_variable(
    const char *name,
    int length
);

ASTNode *ast_create_string(
    const char *value,
    int length
);

ASTNode *ast_create_variable_declaration(
    const char *name,
    int name_length,
    ASTNode *value
);
ASTNode *ast_create_binary(
    ASTNode *left,
    TokenType operator,
    ASTNode *right
);
void ast_free(ASTNode *node);

#endif
