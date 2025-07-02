#include "eval/include/variable.h"
#include "include/repl.h"
#include <stdio.h>
#include <string.h>
#include "eval/include/env.h"
#include <stdlib.h>
#include "parser/include/ast.h"
#include "parser/include/parser.h"
#include "eval/include/eval.h"

int main(int argc, char *argv[])
{
    if (argc == 1)
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
    }
    else if (argc > 1)
    {
        if (strcmp(argv[1], "-c") == 0)
        {
            if (argc < 3)
            {
                fprintf(stderr, "No code to execute.\n");
                exit(1);
            }

            Env env = env_init();

            char *raw = argv[2];
            char *source = malloc(strlen(raw) + 2);
            source[0] = ' ';
            strcpy(source + 1, raw);

            ASTNode *ast = parse(source);

            if (ast->type == AST_STATEMENT_LIST)
            {
                ASTNode *current_node = ast;
                while (current_node)
                {
                    ShdValue result = eval(&env, current_node->statement_list.first);
                    if (result.value_type == VALUE_INTEGER)
                        printf("%d\n", result.value_data.int_value);
                    else if (result.value_type == VALUE_FLOAT)
                        printf("%f\n", result.value_data.float_value);

                    current_node = current_node->statement_list.next;
                }
            }
            else
            {
                ShdValue result = eval(&env, ast);
                if (result.value_type == VALUE_INTEGER)
                    printf("%d\n", result.value_data.int_value);
                else if (result.value_type == VALUE_FLOAT)
                    printf("%f\n", result.value_data.float_value);
            }
            AST_free(ast);
        }
        else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
            printf("no argument:\t\tREPL\n"
                    "-c <code>:\t\tRun code from the CLI\n"
                "-h / --help:\t\tDisplay this message\n"
            "-f / --file:\t\tExecute source code from a file\n");
        else if (strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--file") == 0)
        {
            FILE *file = fopen(argv[2], "r");
            if (!file)
            {
                fprintf(stderr, "Unable to open file.\n");
                exit(1);
            }

            fseek(file, 0, SEEK_END);
            long flength = ftell(file);
            rewind(file);

            char *source = malloc(flength + 2);
            source[0] = ' ';
            fread(source + 1, 1, flength, file);
            source[flength + 1] = '\0';

            fclose(file);

            Env env = env_init();
            ASTNode *ast = parse(source);

            if (ast->type == AST_STATEMENT_LIST)
            {
                ASTNode *current_node = ast;
                while (current_node)
                {
                    ShdValue result = eval(&env, current_node->statement_list.first);

                    if (result.value_type == VALUE_INTEGER)
                        printf("%d\n", result.value_data.int_value);
                    else if (result.value_type == VALUE_FLOAT)
                        printf("%f\n", result.value_data.float_value);

                    current_node = current_node->statement_list.next;
                }
            }
            else
            {
                ShdValue result = eval(&env, ast);

                if (result.value_type == VALUE_INTEGER)
                    printf("%d\n", result.value_data.int_value);
                else if (result.value_type == VALUE_FLOAT)
                    printf("%f\n", result.value_data.float_value);
            }
        }
        else
        {
            fprintf(stderr, "Invalid option. Use -h or --help for a list of available options.\n");
            exit(1);
        }
    }

    return 0;
}