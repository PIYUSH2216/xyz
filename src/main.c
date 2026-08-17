#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

static void print_ast(ASTNode *node, int indent) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < indent; i++) {
        printf("  ");
    }

    switch (node->type) {
        case AST_PROGRAM:
    	printf("Program\n");
	for (int i = 0; i < node->program.count; i++) {
        for (int j = 0; j < indent + 1; j++) {
            printf("  ");
        }
	printf("Statement %d:\n", i + 1);
        print_ast(
            node->program.statements[i],
            indent + 2
        );
    	}
	    break;
	case AST_INTEGER:
            printf("Integer: %ld\n", node->integer_value);
            break;
	case AST_FLOAT:
    	    printf("Float: %f\n", node->float_value);
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

static char *read_file(const char *path) {
    FILE *file = fopen(path, "rb");

    if (file == NULL) {
        fprintf(stderr, "XYZ Error: Could not open file '%s'.\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    if (size < 0) {
        fclose(file);
        fprintf(stderr, "XYZ Error: Could not determine file size.\n");
        return NULL;
    }

    char *source = malloc((size_t)size + 1);

    if (source == NULL) {
        fclose(file);
        fprintf(stderr, "XYZ Error: Not enough memory.\n");
        return NULL;
    }

    size_t bytes_read = fread(source, 1, (size_t)size, file);

    fclose(file);

    if (bytes_read != (size_t)size) {
        free(source);
        fprintf(stderr, "XYZ Error: Could not read file '%s'.\n", path);
        return NULL;
    }

    source[size] = '\0';

    return source;
}

int main(int argc, char *argv[]){
    if (argc != 2) {
    fprintf(stderr, "Usage: xyz <file.xyz>\n");
    return 1;
}

char *source = read_file(argv[1]);

if (source == NULL) {
    return 1;
}
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
    free(source);
    return 0;
}
