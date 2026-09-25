#include <stdio.h>
#include <stddef.h>

#include "evaluator.h"

static Value evaluate_binary(
    TokenType operator,
    Value left,
    Value right
)
{
	int left_value;
	int right_value;

	if (left.type == VALUE_INTEGER)
    		left_value = (left.integer != 0);
	else if (left.type == VALUE_BOOLEAN)
    		left_value = left.boolean;
	else
    		return value_null();

	if (right.type == VALUE_INTEGER)
    		right_value = (right.integer != 0);
	else if (right.type == VALUE_BOOLEAN)
    		right_value = right.boolean;
	else
    		return value_null();

    switch (operator)
    {
        case TOKEN_PLUS:
            return value_integer(left.integer + right.integer);

        case TOKEN_MINUS:
            return value_integer(left.integer - right.integer);

        case TOKEN_STAR:
            return value_integer(left.integer * right.integer);

        case TOKEN_SLASH:
            if (right.integer == 0)
            {
                fprintf(stderr,
                        "XYZ Runtime Error: Division by zero.\n");
                return value_null();
            }

            return value_integer(left.integer / right.integer);

        case TOKEN_PERCENT:
            if (right.integer == 0)
            {
                fprintf(stderr,
                        "XYZ Runtime Error: Modulo by zero.\n");
                return value_null();
            }

            return value_integer(left.integer % right.integer);

        case TOKEN_EQUAL_EQUAL:
            return value_boolean(left.integer == right.integer);

        case TOKEN_BANG_EQUAL:
            return value_boolean(left.integer != right.integer);

        case TOKEN_LESS:
            return value_boolean(left.integer < right.integer);

        case TOKEN_LESS_EQUAL:
            return value_boolean(left.integer <= right.integer);

        case TOKEN_GREATER:
            return value_boolean(left.integer > right.integer);

        case TOKEN_GREATER_EQUAL:
            return value_boolean(left.integer >= right.integer);
	case TOKEN_AND:
    	    return value_boolean(left_value && right_value);
	case TOKEN_OR:
    	    return value_boolean(left_value || right_value);
        default:
            return value_null();
    }
}
Value evaluate(
    ASTNode *node,
    Environment *environment
)
{
    if (node == NULL)
    {
        return value_null();
    }

    switch (node->type)
    {
        case AST_INTEGER:
            return value_integer(
                node->integer_value
            );

        case AST_FLOAT:
            return value_float(
                node->float_value
            );

        case AST_STRING:
            return value_string(
                node->string.value
            );

        case AST_BOOLEAN:
            return value_boolean(
                node->boolean_value
            );

        case AST_NULL:
            return value_null();

        case AST_VARIABLE:
        {
            Value value;

            if (environment_get(
                    environment,
                    node->variable.name,
                    node->variable.length,
                    &value
                ))
            {
                return value;
            }

            fprintf(
                stderr,
                "XYZ Runtime Error: Undefined variable '%.*s'.\n",
                node->variable.length,
                node->variable.name
            );

            return value_null();
        }

        case AST_VARIABLE_DECLARATION:
        {
            Value value = evaluate(
                node->variable_declaration.value,
                environment
            );

            if (!environment_define(
                    environment,
                    node->variable_declaration.name,
                    node->variable_declaration.name_length,
                    value
                ))
            {
                fprintf(
                    stderr,
                    "XYZ Runtime Error: Could not define variable '%.*s'.\n",
                    node->variable_declaration.name_length,
                    node->variable_declaration.name
                );

                return value_null();
            }

            return value;
        }

        case AST_BINARY:
        {
            Value left = evaluate(
                node->binary.left,
                environment
            );

            Value right = evaluate(
                node->binary.right,
                environment
            );

            return evaluate_binary(
                node->binary.operator,
                left,
                right
            );
        }

        default:
            return value_null();
    }
}
