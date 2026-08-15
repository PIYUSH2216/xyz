#include <stdlib.h>
#include "ast.h"

ASTNode *ast_create_integer(long value) {
    ASTNode *node = malloc(sizeof(ASTNode));

    if (node == NULL) {
        return NULL;
    }

    node->type = AST_INTEGER;
    node->integer_value = value;

    return node;
}

ASTNode *ast_create_float(double value) {
    ASTNode *node = malloc(sizeof(ASTNode));

    if (node == NULL) {
        return NULL;
    }

    node->type = AST_FLOAT;
    node->float_value = value;

    return node;
}

ASTNode *ast_create_boolean(int value) {
    ASTNode *node = malloc(sizeof(ASTNode));

    if (node == NULL) {
        return NULL;
    }

    node->type = AST_BOOLEAN;
    node->boolean_value = value;

    return node;
}

ASTNode *ast_create_variable(const char *name, int length) {
    ASTNode *node = malloc(sizeof(ASTNode));

    if (node == NULL) {
        return NULL;
    }

    node->type = AST_VARIABLE;
    node->variable.name = name;
    node->variable.length = length;

    return node;
}

ASTNode *ast_create_string(const char *value, int length) {
    ASTNode *node = malloc(sizeof(ASTNode));

    if (node == NULL) {
        return NULL;
    }

    node->type = AST_STRING;
    node->string.value = value;
    node->string.length = length;

    return node;
}

ASTNode *ast_create_variable_declaration(
    const char *name,
    int name_length,
    ASTNode *value
) {
    ASTNode *node = malloc(sizeof(ASTNode));

    if (node == NULL) {
        return NULL;
    }

    node->type = AST_VARIABLE_DECLARATION;
    node->variable_declaration.name = name;
    node->variable_declaration.name_length = name_length;
    node->variable_declaration.value = value;

    return node;
}

ASTNode *ast_create_null(void) {
    ASTNode *node = malloc(sizeof(ASTNode));

    if (node == NULL) {
        return NULL;
    }

    node->type = AST_NULL;

    return node;
}

void ast_free(ASTNode *node) {
    if (node == NULL) {
        return;
    }

    switch (node->type) {
        case AST_BINARY:
            ast_free(node->binary.left);
            ast_free(node->binary.right);
            break;

        case AST_VARIABLE_DECLARATION:
            ast_free(node->variable_declaration.value);
            break;

        default:
            break;
    }

    free(node);
}
