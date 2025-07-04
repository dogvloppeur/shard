#include <utils/info.h>
#include "include/repl.h"
#include <stdio.h>
#include <string.h>
#include "eval/include/env.h"
#include <stdlib.h>
#include "parser/include/ast.h"
#include "parser/include/parser.h"
#include <lexer/include/lexer.h>

#ifndef _MSC_VER
// strndup is not available on Windows, so we define it if missing
char *strndup(const char *s, size_t n) {
    size_t len = 0;
    while (len < n && s[len]) len++;
    char *result = (char *)malloc(len + 1);
    if (!result) return NULL;
    memcpy(result, s, len);
    result[len] = '\0';
    return result;
}
#endif

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

            eval_and_print_statements(&env, ast);

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

            eval_and_print_statements(&env, ast);

            AST_free(ast);
            free(source);
        }
        else if (strcmp(argv[1], "--help") == 0)
        {
            printf("no argument:\t\tREPL\n"
                   "--help\t\tDisplay this message\n"
                   "--version\t\tDisplay the version installed on your System\n"
                   "--license\t\tDisplay the name of the license\n"
                   "-c <code>\t\tRun code from the CLI\n"
                   "-f / --file\t\tExecute source code from a file\n");
        }
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

            eval_and_print_statements(&env, ast);

            AST_free(ast);
            free(source);
        }
        else if (strcmp(argv[1], "--version") == 0)
            printf("Version %s\n", LANG_VERSION);
        else if (strcmp(argv[1], "--license") == 0)
            printf("Released under %s license\n", LANG_LICENSE);
        else
        {
            fprintf(stderr, "Invalid option. Use --help for a list of available options.\n");
            exit(1);
        }
    }

    return 0;
}
