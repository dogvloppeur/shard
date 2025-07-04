#pragma once

#include "eval/include/variable.h"
#include <stdbool.h>

#define MAX_VARIABLES                       1024

typedef enum
{
    VEDIT_SET,
    VEDIT_ADD,
    VEDIT_SUB,
    VEDIT_MUL,
    VEDIT_DIV,
    VEDIT_AND,
    VEDIT_OR,
    VEDIT_XOR,
    VEDIT_BITWISE
} VariableEditModes;

typedef struct
{
    ShdVariable variables[MAX_VARIABLES];
    int variable_pointer;
} Env;

Env env_init();
bool env_find_variable(Env *env, char *name);
int env_find_variable_index(Env *env, char *name);
void env_add_variable(Env *env, ShdVariable variable);
void env_edit_variable(Env *env, VariableEditModes mode, ShdVariable variable);
ShdVariable *env_get_variable(Env *env, char *name);