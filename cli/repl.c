#include <cli/include/repl.h>
#include <parser/include/ast.h>
#include <utils/info.h>
#include <stdio.h>
#include <lexer/include/lexer.h>
#include <eval/include/variable.h>
#include <eval/include/env.h>
#include <eval/include/eval.h>
#include <stdlib.h>

void repl_welcome()
{
    printf("%s version %s released under %s license\n", LANG_NAME, LANG_VERSION, LANG_LICENSE);
}

void eval_and_print_statements(Env *env, ASTNode *node)
{
    if (node->type == AST_STATEMENT_LIST) {
        ASTNode *current = node;
        while (current)
        {
            ASTNode *stmt = current->statement_list.first;
            if (stmt->type == AST_STATEMENT_LIST)
            {
                eval_and_print_statements(env, stmt);
            }
            else
            {
                ShdValue result = eval(env, stmt);
                if (result.value_type == VALUE_INTEGER)
                    printf("%d\n", result.value_data.int_value);
                else
                    printf("%f\n", result.value_data.float_value);
            }
            current = current->statement_list.next;
        }
    }
    else
    {
        ShdValue result = eval(env, node);
        if (result.value_type == VALUE_INTEGER)
            printf("%d\n", result.value_data.int_value);
        else
            printf("%f\n", result.value_data.float_value);
    }
}
