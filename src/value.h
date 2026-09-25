#ifndef XYZ_VALUE_H
#define XYZ_VALUE_H

typedef enum {
    VALUE_INTEGER,
    VALUE_FLOAT,
    VALUE_STRING,
    VALUE_BOOLEAN,
    VALUE_NULL
} ValueType;

typedef struct {
    ValueType type;

    union {
        long integer;
        double floating;
        const char *string;
        int boolean;
    };
} Value;

Value value_integer(long value);
Value value_float(double value);
Value value_string(const char *value);
Value value_boolean(int value);
Value value_null(void);

void value_print(Value value);

#endif
