#ifndef XYZ_ENVIRONMENT_H
#define XYZ_ENVIRONMENT_H

#include "value.h"

#define MAX_VARIABLES 256

typedef struct {
    const char *name;
    int name_length;
    Value value;
} Variable;

typedef struct {
    Variable variables[MAX_VARIABLES];
    int count;
} Environment;

void environment_init(Environment *environment);

int environment_define(
    Environment *environment,
    const char *name,
    int name_length,
    Value value
);

int environment_get(
    Environment *environment,
    const char *name,
    int name_length,
    Value *value
);

#endif
