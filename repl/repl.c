#include "include/repl.h"
#include "parser/include/ast.h"
#include "parser/include/parser.h"
#include "eval/include/eval.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char line[MAX_CHARS_PER_LINE];

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
        int result = eval(ast);

        if (ast->type == AST_STATEMENT_LIST) {
            ASTNode *current = ast;
            while (current) {
                int result = eval(current->statement_list.first);
                printf("%d\n", result);
                current = current->statement_list.next;
            }
        } else {
            int result = eval(ast);
            printf("%d\n", result);
        }

        AST_free(ast);
        free(source);
    }

    return 0;
}