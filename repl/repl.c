#include "include/repl.h"
#include <parser/include/ast.h>
#include "eval/include/env.h"
#include "eval/include/variable.h"
#include "parser/include/parser.h"
#include "eval/include/eval.h"
#include "repl/include/info.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <lexer/include/lexer.h>

void repl_welcome()
{
    printf("%s version %s released under %s license\n", LANG_NAME, LANG_VERSION, LANG_LICENSE);
}

int main()
{
    char line[MAX_CHARS_PER_LINE];
    Env env = env_init();

    repl_welcome();

    while (1)
    {
        printf(">>> ");
        if (!fgets(line, MAX_CHARS_PER_LINE, stdin))
            break;

        if (line[0] == '\n')
            continue;

        line[strcspn(line, "\n")] = '\0';

        char *source = malloc(strlen(line) + 2);
        source[0] = ' ';
        strcpy(source + 1, line);
        ASTNode *ast = parse(source);

        if (ast->type == AST_STATEMENT_LIST) {
            ASTNode *current = ast;
            while (current)
            {
                ShdValue result = eval(&env, current->statement_list.first);
                if (result.value_type == VALUE_INTEGER)
                    printf("%d\n", result.value_data.int_value);
                else
                    printf("%f\n", result.value_data.float_value);

                current = current->statement_list.next;
            }
        }
        else
        {
            ShdValue result = eval(&env, ast);
            if (result.value_type == VALUE_INTEGER)
                printf("%d\n", result.value_data.int_value);
            else
                printf("%f\n", result.value_data.float_value);
        }

        AST_free(ast);
        free(source);
    }

    return 0;
}