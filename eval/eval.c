#include "include/eval.h"
#include "include/variable.h"
#include "lexer/include/token_type.h"
#include "parser/include/ast.h"
#include "include/env.h"
#include "utils/error.h"
#include <stdlib.h>
#include <stdio.h>

ShdValue eval(Env *env, ASTNode *node)
{
    if (!node)
    {
        fprintf(stderr, "Eval: NULL node\n");
        exit(1);
    }

    switch ((int)node->type)
    {
        case AST_INTEGER:
        {
            ShdValue value;
            value.value_type = VALUE_INTEGER;
            value.value_data.int_value = node->integer.value;
            return value;
        }

        case AST_FLOAT:
        {
            ShdValue value;
            value.value_type = VALUE_FLOAT;
            value.value_data.float_value = node->_float.value;
            return value;
        }

        case AST_BINOP:
        {
            ShdValue left = eval(env, node->binop.left);
            ShdValue right = eval(env, node->binop.right);

            if (left.value_type == VALUE_INTEGER && right.value_type == VALUE_INTEGER)
            {
                int l = left.value_data.int_value;
                int r = right.value_data.int_value;
                ShdValue result = { .value_type = VALUE_INTEGER };

                switch ((int)node->binop.operator)
                {
                    case BINOP_PLUS: result.value_data.int_value = l + r; break;
                    case BINOP_MINUS: result.value_data.int_value = l - r; break;
                    case BINOP_TIMES: result.value_data.int_value = l * r; break;
                    case BINOP_DIVIDE:
                        if (r == 0)
                            error_math(MERR_ZERODIV, NULL, NULL, NULL);

                        result.value_data.int_value = l / r;
                        break;
                    case BINOP_ISEQUAL: result.value_data.int_value = l == r; break;
                    case BINOP_ISNOTEQUAL: result.value_data.int_value = l != r; break;
                    case BINOP_ISLESSER: result.value_data.int_value = l < r; break;
                    case BINOP_ISLESSEREQUAL: result.value_data.int_value = l <= r; break;
                    case BINOP_ISGREATER: result.value_data.int_value = l > r; break;
                    case BINOP_ISGREATEREQUAL: result.value_data.int_value = l >= r; break;
                    case BINOP_AND: result.value_data.int_value = l && r; break;
                    case BINOP_OR: result.value_data.int_value = l || r; break;
                    case BINOP_XOR: result.value_data.int_value = l ^ r; break;
                    case BINOP_ANDBIT: result.value_data.int_value = l & r; break;
                    case BINOP_ORBIT: result.value_data.int_value = l | r; break;
                    case BINOP_LSHIFT: result.value_data.int_value = l << r; break;
                    case BINOP_RSHIFT: result.value_data.int_value = l >> r; break;
                }

                return result;
            }
            else
            {
                float l = (left.value_type == VALUE_INTEGER) ? left.value_data.int_value : left.value_data.float_value;
                float r = (right.value_type == VALUE_INTEGER) ? right.value_data.int_value : right.value_data.float_value;
                ShdValue result = { .value_type = VALUE_FLOAT };

                switch ((int)node->binop.operator)
                {
                    case BINOP_PLUS: result.value_data.float_value = l + r; break;
                    case BINOP_MINUS: result.value_data.float_value = l - r; break;
                    case BINOP_TIMES: result.value_data.float_value = l * r; break;
                    case BINOP_DIVIDE:
                        if (r == 0.0f)
                            error_math(MERR_ZERODIV, NULL, NULL, NULL);

                        result.value_data.float_value = l / r;
                        break;
                    case BINOP_ISEQUAL: result.value_data.float_value = l == r; break;
                    case BINOP_ISNOTEQUAL: result.value_data.float_value = l != r; break;
                    case BINOP_ISLESSER: result.value_data.float_value = l < r; break;
                    case BINOP_ISLESSEREQUAL: result.value_data.float_value = l <= r; break;
                    case BINOP_ISGREATER: result.value_data.float_value = l > r; break;
                    case BINOP_ISGREATEREQUAL: result.value_data.float_value = l >= r; break;
                    case BINOP_AND: result.value_data.float_value = l && r; break;
                    case BINOP_OR: result.value_data.float_value = l || r; break;
                }

                if (node->binop.operator == BINOP_ANDBIT || node->binop.operator == BINOP_ORBIT || node->binop.operator == BINOP_XOR || node->binop.operator == BINOP_LSHIFT || node->binop.operator == BINOP_RSHIFT)
                    error_math(MERR_TYPE, NULL, NULL, NULL);

                return result;
            }
        }

        case AST_UNOP:
        {
            ShdValue value = eval(env, node->unop.value);

            if (value.value_type == VALUE_INTEGER)
            {
                int v = value.value_data.int_value;
                ShdValue result = { .value_type = VALUE_INTEGER };

                switch ((int)node->unop.operator)
                {
                    case UNOP_PLUS: result.value_data.int_value = v; break;
                    case UNOP_MINUS: result.value_data.int_value = -v; break;
                    case UNOP_NOT: result.value_data.int_value = !v; break;
                    case UNOP_BITWISE: result.value_data.int_value = ~v; break;
                }

                return result;
            }
            else if (value.value_type == VALUE_FLOAT)
            {
                float v = value.value_data.float_value;
                ShdValue result = { .value_type = VALUE_FLOAT };

                switch ((int)node->unop.operator)
                {
                    case UNOP_PLUS: result.value_data.float_value = v; break;
                    case UNOP_MINUS: result.value_data.float_value = -v; break;
                    case UNOP_NOT: result.value_data.float_value = !v; break;
                }

                if (node->unop.operator == UNOP_BITWISE)
                    error_math(MERR_TYPE, NULL, NULL, NULL);

                return result;
            }
        }

        case AST_STATEMENT_LIST:
        {
            ASTNode *current_stmt = node;
            ShdValue last_value = { .value_type = VALUE_INTEGER, .value_data.int_value = 0 };

            while (current_stmt)
            {
                last_value = eval(env, current_stmt->statement_list.first);
                current_stmt = current_stmt->statement_list.next;
            }

            return last_value;
        }

        case AST_VAR_DECLARATION:
        {
            char *name = node->var_declaration.name;
            ShdValue value;

            if (node->var_declaration.value != NULL)
                value = eval(env, node->var_declaration.value);
            else
            {
                value.value_type = VALUE_INTEGER;
                value.value_data.int_value = 0;
            }

            ShdVariable variable = { .name = name, .value = value };
            env_add_variable(env, variable);

            return value;
        }

        case AST_VAR_ASSIGNMENT:
        {
            char *name = node->var_assignment.name;
            ShdValue value = eval(env, node->var_assignment.value);

            ShdVariable variable = { .name = name, .value = value};

            switch ((int)node->var_assignment.operator)
            {
                case ASSIGNMENT_EQUAL: env_edit_variable(env, VEDIT_SET, variable); break;
                case ASSIGNMENT_PLUSEQ: env_edit_variable(env, VEDIT_ADD, variable); break;
                case ASSIGNMENT_MINUSEQ: env_edit_variable(env, VEDIT_SUB, variable); break;
                case ASSIGNMENT_STAREQ: env_edit_variable(env, VEDIT_MUL, variable); break;
                case ASSIGNMENT_SLASHEQ: env_edit_variable(env, VEDIT_DIV, variable); break;
                case ASSIGNMENT_ANDEQ: env_edit_variable(env, VEDIT_AND, variable); break;
                case ASSIGNMENT_OREQ: env_edit_variable(env, VEDIT_OR, variable); break;
                case ASSIGNMENT_XOREQ: env_edit_variable(env, VEDIT_XOR, variable); break;
                case ASSIGNMENT_BITWEQ: env_edit_variable(env, VEDIT_BITWISE, variable); break;
                case ASSIGNMENT_LSHIFTEQ: env_edit_variable(env, VEDIT_LSHIFT, variable); break;
                case ASSIGNMENT_RSHIFTEQ: env_edit_variable(env, VEDIT_RSHIFT, variable); break;
            }

            return value;
        }

        case AST_VAR_ACCESS:
        {
            char *name = node->var_access.name;
            ShdVariable *variable = env_get_variable(env, name);

            if (!variable)
                error_variable(VERR_UNDECLARED, name);

            return variable->value;
        }

        case AST_COND_IF:
        {
            ShdValue cond = eval(env, node->cond_if.condition);

            if ((cond.value_type == VALUE_INTEGER && cond.value_data.int_value != 0) ||
                (cond.value_type == VALUE_FLOAT && cond.value_data.float_value != 0.0f))
            {
                ASTNode *current = node->cond_if.branch;
                ShdValue last = { .value_type = VALUE_INTEGER, .value_data.int_value = 0 };
                while (current)
                {
                    last = eval(env, current->statement_list.first);
                    current = current->statement_list.next;
                }
                return last;
            }

            ShdValue dummy = { .value_type = VALUE_INTEGER, .value_data.int_value = 0 };
            return dummy;
        }

        case AST_COND_IF_ELSE:
        {
            ShdValue cond = eval(env, node->cond_if_else.condition);

            ASTNode *branch = NULL;

            if ((cond.value_type == VALUE_INTEGER && cond.value_data.int_value != 0) ||
                (cond.value_type == VALUE_FLOAT && cond.value_data.float_value != 0.0f))
            {
                branch = node->cond_if_else.then_branch;
            }
            else
            {
                branch = node->cond_if_else.else_branch;
            }

            ShdValue last = { .value_type = VALUE_INTEGER, .value_data.int_value = 0 };
            ASTNode *current = branch;
            while (current)
            {
                last = eval(env, current->statement_list.first);
                current = current->statement_list.next;
            }

            return last;
        }

        case AST_COND_LOOP:
        {
            env_switch_context(env, CONTEXT_LOOP);  // entrée dans boucle

            if (node->cond_loop.type == CONDLOOP_WHILE)
            {
                while (1)
                {
                    ShdValue cond = eval(env, node->cond_loop.condition);
                    if (!((cond.value_type == VALUE_INTEGER && cond.value_data.int_value != 0) ||
                        (cond.value_type == VALUE_FLOAT && cond.value_data.float_value != 0.0f)))
                        break;

                    ASTNode *current = node->cond_loop.branch;
                    ShdValue last = { .value_type = VALUE_INTEGER, .value_data.int_value = 0, .signal = SIGNAL_NORMAL };

                    while (current)
                    {
                        last = eval(env, current->statement_list.first);

                        if (last.signal == SIGNAL_BREAK)
                        {
                            env_switch_context(env, CONTEXT_GLOBAL);
                            return last;
                        }
                        else if (last.signal == SIGNAL_CONTINUE)
                            break;

                        current = current->statement_list.next;
                    }

                    if (last.signal == SIGNAL_CONTINUE)
                    {
                        continue;
                    }
                }
            }
            else if (node->cond_loop.type == CONDLOOP_UNTIL)
            {
                while (1)
                {
                    ShdValue cond = eval(env, node->cond_loop.condition);
                    if (!((cond.value_type == VALUE_INTEGER && cond.value_data.int_value == 0) ||
                        (cond.value_type == VALUE_FLOAT && cond.value_data.float_value == 0.0f)))
                        break;

                    ASTNode *current = node->cond_loop.branch;
                    ShdValue last = { .value_type = VALUE_INTEGER, .value_data.int_value = 0, .signal = SIGNAL_NORMAL };

                    while (current)
                    {
                        last = eval(env, current->statement_list.first);

                        if (last.signal == SIGNAL_BREAK)
                        {
                            env_switch_context(env, CONTEXT_GLOBAL);
                            return last;
                        }
                        else if (last.signal == SIGNAL_CONTINUE)
                            break;

                        current = current->statement_list.next;
                    }

                    if (last.signal == SIGNAL_CONTINUE)
                        continue;
                }
            }

            env_switch_context(env, CONTEXT_GLOBAL);

            ShdValue dummy = { .value_type = VALUE_INTEGER, .value_data.int_value = 0, .signal = SIGNAL_NORMAL };
            return dummy;
        }

        case AST_UNCOND_LOOP:
        {
            env_switch_context(env, CONTEXT_LOOP);

            while (1)
            {
                ASTNode *current = node->uncond_loop.branch;
                ShdValue last = { .value_type = VALUE_INTEGER, .value_data.int_value = 0, .signal = SIGNAL_NORMAL };

                while (current)
                {
                    last = eval(env, current->statement_list.first);

                    if (last.signal == SIGNAL_BREAK)
                    {
                        env_switch_context(env, CONTEXT_GLOBAL);
                        return last;
                    }
                    else if (last.signal == SIGNAL_CONTINUE)
                        break;

                    current = current->statement_list.next;
                }

                if (last.signal == SIGNAL_CONTINUE)
                    continue;
            }

            env_switch_context(env, CONTEXT_GLOBAL);

            ShdValue dummy = { .value_type = VALUE_INTEGER, .value_data.int_value = 0, .signal = SIGNAL_NORMAL };
            return dummy;
        }

        case AST_LOOP_CONTROL:
        {
            ShdValue result = { .value_type = VALUE_INTEGER, .value_data.int_value = 0, .signal = SIGNAL_NORMAL };

            switch ((int)node->loop_control.type)
            {
                case LOOPCTRL_BREAK:
                    if (env->context != CONTEXT_LOOP)
                        error_context(CERR_BREAK);
                    env->context = CONTEXT_GLOBAL;
                    result.signal = SIGNAL_BREAK;
                    break;

                case LOOPCTRL_CONTINUE:
                    if (env->context != CONTEXT_LOOP)
                        error_context(CERR_CONTINUE);
                    result.signal = SIGNAL_CONTINUE;
                    break;
            }

            return result;
        }

        default: error_ast();
    }
}