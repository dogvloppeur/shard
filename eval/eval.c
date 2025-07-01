#include "include/eval.h"
#include "parser/include/ast.h"
#include <stdlib.h>
#include <stdio.h>

float eval(ASTNode *node)
{
    if (!node)
    {
        fprintf(stderr, "Eval: NULL node\n");
        exit(1);
    }

    switch ((int)node->type)
    {
        case AST_INTEGER:
            return node->integer.value;

        case AST_BINOP:
        {
            float left = eval(node->binop.left);
            float right = eval(node->binop.right);

            switch ((int)node->binop.operator)
            {
                case BINOP_PLUS: return left + right;
                case BINOP_MINUS: return left - right;
                case BINOP_TIMES: return left * right;
                case BINOP_DIVIDE:
                    if (right == 0)
                    {
                        fprintf(stderr, "Division by zero\n");
                        exit(1);
                    }
                    return left / right;

                default:
                    fprintf(stderr, "Unknow binary operator\n");
                    exit(1);
            }
        }

        case AST_STATEMENT_LIST:
        {
            float result = 0;

            ASTNode *current = node;
            while (current)
            {
                result = eval(current->statement_list.first);
                current = current->statement_list.next;
            }

            return result;
        }

        case AST_FLOAT:
            return node->_float.value;

        default:
            fprintf(stderr, "Unknow AST node in eval\n");
            exit(1);
    }
}