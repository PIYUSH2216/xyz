#include <stdio.h>

#include "value.h"

Value value_integer(long value)
{
    Value result;

    result.type = VALUE_INTEGER;
    result.integer = value;

    return result;
}

Value value_float(double value)
{
    Value result;

    result.type = VALUE_FLOAT;
    result.floating = value;

    return result;
}

Value value_string(const char *value)
{
    Value result;

    result.type = VALUE_STRING;
    result.string = value;

    return result;
}

Value value_boolean(int value)
{
    Value result;

    result.type = VALUE_BOOLEAN;
    result.boolean = value;

    return result;
}

Value value_null(void)
{
    Value result;

    result.type = VALUE_NULL;

    return result;
}

void value_print(Value value)
{
    switch (value.type)
    {
        case VALUE_INTEGER:
            printf("%ld\n", value.integer);
            break;

        case VALUE_FLOAT:
            printf("%g\n", value.floating);
            break;

        case VALUE_STRING:
            printf("%s\n", value.string);
            break;

        case VALUE_BOOLEAN:
            printf("%s\n", value.boolean ? "true" : "false");
            break;

        case VALUE_NULL:
            printf("null\n");
            break;
    }
}
