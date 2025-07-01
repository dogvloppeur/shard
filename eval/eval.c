#include "include/eval.h"
#include "parser/include/ast.h"
#include <stdlib.h>
#include <stdio.h>

int eval(ASTNode *node)
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
            int left = eval(node->binop.left);
            int right = eval(node->binop.right);

            switch ((int)node->binop.operator)
            {
                case BINOP_PLUS: return left + right;
                case BINOP_MINUS: return left - right;
                case BINOP_TIMES: return left * right;
                case BINOP_DIVIDE: return left / right;
            }
        }

        case AST_STATEMENT_LIST:
        {
            int result = 0;

            ASTNode *current = node;
            while (current)
            {
                result = eval(current->statement_list.first);
                current = current->statement_list.next;
            }

            return result;
        }

        default:
            fprintf(stderr, "Unknow AST node in eval\n");
            exit(1);
    }
}