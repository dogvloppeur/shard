#include "include/repl.h"
#include <parser/include/ast.h>
#include "parser/include/parser.h"
#include "eval/include/eval.h"
#include "repl/include/info.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void repl_welcome()
{
    printf("%s version %s released under %s license\n", LANG_NAME, LANG_VERSION, LANG_LICENSE);
}

int main()
{
    char line[MAX_CHARS_PER_LINE];

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
                float result = eval(current->statement_list.first);
                printf("%g\n", result);
                current = current->statement_list.next;
            }
        }
        else
        {
            float result = eval(ast);
            printf("%g\n", result);
        }

        AST_free(ast);
        free(source);
    }

    return 0;
}