#include <string.h>

#include "environment.h"

void environment_init(Environment *environment)
{
    environment->count = 0;
}

int environment_define(
    Environment *environment,
    const char *name,
    int name_length,
    Value value
)
{
    if (environment->count >= MAX_VARIABLES)
        return 0;

    Variable *variable =
        &environment->variables[environment->count];

    variable->name = name;
    variable->name_length = name_length;
    variable->value = value;

    environment->count++;

    return 1;
}

int environment_get(
    Environment *environment,
    const char *name,
    int name_length,
    Value *value
)
{
    for (int i = 0; i < environment->count; i++)
    {
        Variable *variable = &environment->variables[i];

        if (variable->name_length == name_length &&
            strncmp(variable->name, name, name_length) == 0)
        {
            *value = variable->value;
            return 1;
        }
    }

    return 0;
}
