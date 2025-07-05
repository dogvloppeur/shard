#pragma once

#include <eval/include/variable.h>
#include <stdbool.h>

#define DEFAULT_MAX_VARIABLES               8

/*
    Every variable edition modes.
*/
typedef enum
{
    VEDIT_SET,          // =
    VEDIT_ADD,          // +=
    VEDIT_SUB,          // -=
    VEDIT_MUL,          // *=
    VEDIT_DIV,          // /=
    VEDIT_AND,          // &=
    VEDIT_OR,           // |=
    VEDIT_XOR,          // ^=
    VEDIT_BITWISE,      // ~=
    VEDIT_LSHIFT,       // <<=
    VEDIT_RSHIFT        // >>=
} VariableEditModes;

/*
    Every context types.
*/
typedef enum
{
    CONTEXT_GLOBAL,
    CONTEXT_LOOP
} ContextTypes;

/*
    Represents the environment.
*/
typedef struct
{
    ShdVariable *variables;
    int variable_count, max_variables;
    ContextTypes context;
} Env;

Env env_init();
bool env_find_variable(Env *env, char *name);
int env_find_variable_index(Env *env, char *name);
void env_add_variable(Env *env, ShdVariable variable);
void env_edit_variable(Env *env, VariableEditModes mode, ShdVariable variable);
ShdVariable *env_get_variable(Env *env, char *name);
ContextTypes env_get_context(Env *env);
void env_switch_context(Env *env, ContextTypes context);
void env_destroy(Env *env);