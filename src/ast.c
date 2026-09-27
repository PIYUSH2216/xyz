#include <stdlib.h>
#include "ast.h"

ASTNode *ast_create_program(void) {
    ASTNode *node = malloc(sizeof(ASTNode));

    if (node == NULL) {
        return NULL;
    }

    node->type = AST_PROGRAM;
    node->program.statements = NULL;
    node->program.count = 0;
    node->program.capacity = 0;

    return node;
}

int ast_program_add(ASTNode *program, ASTNode *statement) {
    if (program == NULL || statement == NULL) {
        return 0;
    }

    if (program->type != AST_PROGRAM) {
        return 0;
    }

    if (program->program.count >= program->program.capacity) {
        int new_capacity =
            program->program.capacity == 0
                ? 8
                : program->program.capacity * 2;

        ASTNode **new_statements = realloc(
            program->program.statements,
            sizeof(ASTNode *) * new_capacity
        );

        if (new_statements == NULL) {
            return 0;
        }

        program->program.statements = new_statements;
        program->program.capacity = new_capacity;
    }

    program->program.statements[
        program->program.count
    ] = statement;

    program->program.count++;

    return 1;
}

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

ASTNode *ast_create_assignment(
    const char *name,
    int name_length,
    ASTNode *value
)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    if (node == NULL)
        return NULL;

    node->type = AST_ASSIGNMENT;
    node->assignment.name = name;
    node->assignment.name_length = name_length;
    node->assignment.value = value;

    return node;
}

ASTNode *ast_create_binary(
    ASTNode *left,
    TokenType operator,
    ASTNode *right
) {
    ASTNode *node = malloc(sizeof(ASTNode));

    if (node == NULL) {
        return NULL;
    }

    node->type = AST_BINARY;
    node->binary.left = left;
    node->binary.operator = operator;
    node->binary.right = right;

    return node;
}
ASTNode *ast_create_if_statement(ASTNode *condition,ASTNode *then_branch,ASTNode *else_branch){
    ASTNode *node = malloc(sizeof(ASTNode));
    if(node == NULL)
        return NULL;

    node->type = AST_IF_STATEMENT;
    node->if_statement.condition = condition;
    node->if_statement.then_branch = then_branch;
    node->if_statement.else_branch = else_branch;

    return node;
}
ASTNode *ast_create_while_statement(
    ASTNode *condition,
    ASTNode *body
)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    if (node == NULL)
        return NULL;

    node->type = AST_WHILE_STATEMENT;
    node->while_statement.condition = condition;
    node->while_statement.body = body;

    return node;
}

ASTNode *ast_create_print(ASTNode *expression)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    if (node == NULL)
        return NULL;

    node->type = AST_PRINT;
    node->print_statement.expression = expression;

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
	case AST_PROGRAM:
    	    for (int i = 0; i < node->program.count; i++) {
            ast_free(node->program.statements[i]);
    	    }
	    break;
	case AST_IF_STATEMENT:
    	    ast_free(node->if_statement.condition);
    	    ast_free(node->if_statement.then_branch);
    	    ast_free(node->if_statement.else_branch);
	    break;
	case AST_ASSIGNMENT:
    	    ast_free(node->assignment.value);
    	    break;
	case AST_WHILE_STATEMENT:
    	    ast_free(node->while_statement.condition);
    	    ast_free(node->while_statement.body);
    	    break;
	case AST_PRINT:
    	    ast_free(node->print_statement.expression);
    	    break;
    free(node->program.statements);
    break;
        default:
            break;
    }

    free(node);
}
