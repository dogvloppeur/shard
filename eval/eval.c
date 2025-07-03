#include "include/eval.h"
#include "include/variable.h"
#include "parser/include/ast.h"
#include "include/env.h"
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
                        {
                            fprintf(stderr, "Division by zero\n");
                            exit(1);
                        }
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
                    default:
                        fprintf(stderr, "Unknown binary operator\n");
                        exit(1);
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
                        {
                            fprintf(stderr, "Division by zero\n");
                            exit(1);
                        }
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
                    default:
                        fprintf(stderr, "Unknown binary operator\n");
                        exit(1);
                }

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
                    default:
                        fprintf(stderr, "Unknown unary operator\n");
                        exit(1);
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
                    default:
                        fprintf(stderr, "Unknown unary operator\n");
                        exit(1);
                }

                return result;
            }

            fprintf(stderr, "Invalid value type in unary operation\n");
            exit(1);
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
                case ASSIGNMENT_EQUAL: env_edit_variable(env, variable); break;
                default:
                    fprintf(stderr, "Unknown assignment operation\n");
                    exit(1);
            }

            return value;
        }

        case AST_VAR_ACCESS:
        {
            char *name = node->var_access.name;
            ShdVariable *variable = env_get_variable(env, name);

            if (!variable)
            {
                fprintf(stderr, "Undeclared variable %s\n", name);
                exit(1);
            }

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

        default:
            fprintf(stderr, "Unknown AST node in eval\n");
            exit(1);
    }
}