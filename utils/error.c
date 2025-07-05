#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include "color.h"

void error_illegal_char(char c, int line, int column)
{
    fprintf(stderr, RED_BOLD "ERROR:" RESET " illegal character '%c' found at line %d and column %d\n", c, line, column);
    exit(1);
}

void error_syntax(SyntaxErrorTypes type, char *s, int line)
{
    switch ((int)type)
    {
        case SERR_BINOP:
            fprintf(stderr, RED_BOLD "ERROR:" RESET " syntax error: unknown binary operator \"%s\" at line %d\n", s, line);
            exit(1);

        case SERR_UNOP:
            fprintf(stderr, RED_BOLD "ERROR:" RESET " syntax error: unknown unary operator \"%s\" at line %d\n", s, line);
            exit(1);

        case SERR_ASSIGNMENT:
            fprintf(stderr, RED_BOLD "ERROR:" RESET " syntax error: unknown assignment operator \"%s\" at line %d\n", s, line);
            exit(1);

        case SERR_EXPECT:
            fprintf(stderr, RED_BOLD "ERROR:" RESET " syntax error: expected %s at line %d\n", s, line);
            exit(1);

        case SERR_DEFAULT:
            fprintf(stderr, RED_BOLD "ERROR:" RESET " syntax error at \"%s\" at line %d\n", s, line);
            exit(1);
    }
}

void error_variable(VariableErrorTypes type, char *name)
{
    switch ((int)type)
    {
        case VERR_UNDECLARED:
            fprintf(stderr, RED_BOLD "ERROR:" RESET " variable error: undeclared variable \"%s\"\n", name);
            exit(1);

        case VERR_REDECLARE:
            fprintf(stderr, RED_BOLD "ERROR:" RESET " variable error: cannot redeclare variable \"%s\"\n", name);
            exit(1);

        case VERR_NOSPACE:
            fprintf(stderr, RED_BOLD "ERROR:" RESET " variable error: not enough space to store variable \"%s\"\n", name);
            exit(1);
    }
}

void error_math(MathErrorTypes type, int left, int right, char *operator)
{
    switch((int)type)
    {
        case MERR_ZERODIV:
            fprintf(stderr, RED_BOLD "ERROR:" RESET " math error: division by zero\n");
            exit(1);

        case MERR_TYPE:
            fprintf(stderr, RED_BOLD "ERROR:" RESET " math error: invalid types\n");
            exit(1);
    }
}

void error_ast()
{
    fprintf(stderr, RED_BOLD "ERROR:" RESET " AST error: unknow AST node\n");
    exit(1);
}

void error_context(ContextErrorTypes context)
{
    switch ((int)context)
    {
        case CERR_BREAK:
            fprintf(stderr, RED_BOLD "ERROR:" RESET " context error: break outside of loop\n");
            exit(1);

        case CERR_CONTINUE:
            fprintf(stderr, RED_BOLD "ERROR:" RESET " context error: continue outside of loop\n");
            exit(1);
    }
}