#include <stdio.h>

#include "parser.h"

static void print_ast(ASTNode *node, int indent) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < indent; i++) {
        printf("  ");
    }

    switch (node->type) {
        case AST_INTEGER:
            printf("Integer: %ld\n", node->integer_value);
            break;

	case AST_NULL:
    	    printf("Null\n");
            break;
	case AST_STRING:
    	    printf("String: %.*s\n",node->string.length,node->string.value);
	    break;
	case AST_BOOLEAN:
    	    printf("Boolean: %s\n",node->boolean_value ? "true" : "false");
	    break;
        case AST_VARIABLE_DECLARATION:
            printf("VariableDeclaration\n");

            for (int i = 0; i < indent + 1; i++) {
                printf("  ");
            }

            printf("name: %.*s\n",
                   node->variable_declaration.name_length,
                   node->variable_declaration.name);

            for (int i = 0; i < indent + 1; i++) {
                printf("  ");
            }

            printf("value:\n");

            print_ast(
                node->variable_declaration.value,
                indent + 2
            );
            break;

        default:
            printf("Unknown AST node\n");
            break;
    }
}

int main(void) {
    const char *source =
        "let x = false;";

    Parser parser;

    parser_init(&parser, source);

    ASTNode *ast = parser_parse(&parser);

    if (ast == NULL) {
        fprintf(stderr, "Failed to parse XYZ program.\n");
        return 1;
    }

    printf("XYZ AST\n");
    printf("=======\n");

    print_ast(ast, 0);

    ast_free(ast);

    return 0;
}
