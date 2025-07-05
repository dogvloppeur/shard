#include "include/env.h"
#include "eval/include/variable.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <utils/error.h>

Env env_init()
{
    Env env;

    env.variable_pointer = -1;
    env.context = CONTEXT_GLOBAL;

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
        error_variable(VERR_REDECLARE, variable.name);

    if (env->variable_pointer >= MAX_VARIABLES - 1)
        error_variable(VERR_NOSPACE, variable.name);

    env->variable_pointer++;
    env->variables[env->variable_pointer] = variable;
}

void env_edit_variable(Env *env, VariableEditModes mode, ShdVariable new_var)
{
    for (int i = 0; i <= env->variable_pointer; i++)
    {
        if (strcmp(env->variables[i].name, new_var.name) == 0)
        {
            ShdVariable *var = &env->variables[i];

            switch (mode)
            {
                case VEDIT_SET:
                    var->value = new_var.value;
                    break;

                case VEDIT_ADD:
                    if (var->value.value_type == VALUE_INTEGER)
                        var->value.value_data.int_value += new_var.value.value_data.int_value;
                    else if (var->value.value_type == VALUE_FLOAT)
                        var->value.value_data.float_value += new_var.value.value_data.float_value;
                    break;

                case VEDIT_SUB:
                    if (var->value.value_type == VALUE_INTEGER)
                        var->value.value_data.int_value -= new_var.value.value_data.int_value;
                    else if (var->value.value_type == VALUE_FLOAT)
                        var->value.value_data.float_value -= new_var.value.value_data.float_value;
                    break;

                case VEDIT_MUL:
                    if (var->value.value_type == VALUE_INTEGER)
                        var->value.value_data.int_value *= new_var.value.value_data.int_value;
                    else if (var->value.value_type == VALUE_FLOAT)
                        var->value.value_data.float_value *= new_var.value.value_data.float_value;
                    break;

                case VEDIT_DIV:
                    if (var->value.value_type == VALUE_INTEGER)
                    {
                        if (new_var.value.value_data.int_value == 0)
                            error_math(MERR_ZERODIV, NULL, NULL, NULL);
                        else
                            var->value.value_data.int_value /= new_var.value.value_data.int_value;
                    }
                    else if (var->value.value_type == VALUE_FLOAT)
                    {
                        if (new_var.value.value_data.float_value == 0.0f)
                            error_math(MERR_ZERODIV, NULL, NULL, NULL);
                        else
                            var->value.value_data.float_value /= new_var.value.value_data.float_value;
                    }
                    break;

                case VEDIT_AND:
                    if (var->value.value_type == VALUE_INTEGER)
                        var->value.value_data.int_value &= new_var.value.value_data.int_value;
                    else if (var->value.value_type == VALUE_FLOAT)
                        error_math(MERR_TYPE, NULL, NULL, NULL);
                    break;

                case VEDIT_OR:
                    if (var->value.value_type == VALUE_INTEGER)
                        var->value.value_data.int_value |= new_var.value.value_data.int_value;
                    else if (var->value.value_type == VALUE_FLOAT)
                        error_math(MERR_TYPE, NULL, NULL, NULL);
                    break;

                case VEDIT_XOR:
                    if (var->value.value_type == VALUE_INTEGER)
                        var->value.value_data.int_value ^= new_var.value.value_data.int_value;
                    else if (var->value.value_type == VALUE_FLOAT)
                        error_math(MERR_TYPE, NULL, NULL, NULL);
                    break;

                case VEDIT_BITWISE:
                    if (var->value.value_type == VALUE_INTEGER)
                        var->value.value_data.int_value = ~(var->value.value_data.int_value);
                    else if (var->value.value_type == VALUE_FLOAT)
                        error_math(MERR_TYPE, NULL, NULL, NULL);
                    break;

                case VEDIT_LSHIFT:
                    if (var->value.value_type == VALUE_INTEGER)
                        var->value.value_data.int_value <<= var->value.value_data.int_value;
                    else if (var->value.value_type == VALUE_FLOAT)
                        error_math(MERR_TYPE, NULL, NULL, NULL);
                    break;

                case VEDIT_RSHIFT:
                    if (var->value.value_type == VALUE_INTEGER)
                        var->value.value_data.int_value >>= var->value.value_data.int_value;
                    else if (var->value.value_type == VALUE_FLOAT)
                        error_math(MERR_TYPE, NULL, NULL, NULL);
                    break;

                default:
                    error_variable(VERR_NOSPACE, new_var.name);
            }
            return;
        }
    }

    error_variable(VERR_UNDECLARED, new_var.name);
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

ContextTypes env_get_context(Env *env)
{
    return env->context;
}

void env_switch_context(Env *env, ContextTypes context)
{
    env->context = context;
}