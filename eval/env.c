#include "include/env.h"
#include "eval/include/variable.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Env env_init()
{
    Env env;

    env.variable_pointer = -1;

    return env;
}

bool env_find_variable(Env *env, char *name)
{
    for (int i = 0; i <= env->variable_pointer; i++)
    {
        if (strcmp(env->variables[i].name, name) == 0)
            return true;
    }
    return false;
}

int env_find_variable_index(Env *env, char *name)
{
    for (int i = 0; i <= env->variable_pointer; i++)
    {
        if (strcmp(env->variables[i].name, name) == 0)
            return i;
    }
    return -1;
}

void env_add_variable(Env *env, ShdVariable variable)
{
    if (env_find_variable(env, variable.name))
    {
        fprintf(stderr, "Cannot redeclare variable %s\n", variable.name);
        exit(1);
    }

    if (env->variable_pointer >= MAX_VARIABLES - 1)
    {
        fprintf(stderr, "Not enough space for variable %s\n", variable.name);
        exit(1);
    }

    env->variable_pointer++;
    env->variables[env->variable_pointer] = variable;
}

void env_edit_variable(Env *env, ShdVariable new_var)
{
    for (int i = 0; i <= env->variable_pointer; i++)
    {
        if (strcmp(env->variables[i].name, new_var.name) == 0)
        {
            env->variables[i] = new_var;
            return;
        }
    }

    fprintf(stderr, "Undeclared variable %s\n", new_var.name);
    exit(1);
}

ShdVariable *env_get_variable(Env *env, char *name)
{
    for (int i = 0; i <= env->variable_pointer; i++)
    {
        if (strcmp(env->variables[i].name, name) == 0)
            return &env->variables[i];
    }

    return NULL;
}