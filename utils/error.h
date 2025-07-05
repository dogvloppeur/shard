#pragma once

typedef enum
{
    SERR_DEFAULT,
    SERR_BINOP,
    SERR_UNOP,
    SERR_ASSIGNMENT,
    SERR_EXPECT
} SyntaxErrorTypes;

typedef enum
{
    VERR_UNDECLARED,
    VERR_REDECLARE,
    VERR_NOSPACE
} VariableErrorTypes;

typedef enum
{
    MERR_ZERODIV,
    MERR_TYPE
} MathErrorTypes;

typedef enum
{
    CERR_BREAK,
    CERR_CONTINUE
} ContextErrorTypes;

void error_illegal_char(char c, int line, int column);
void error_syntax(SyntaxErrorTypes type, char *s, int line);
void error_variable(VariableErrorTypes type, char *name);
void error_math(MathErrorTypes type, int left, int right, char *operator);
void error_ast();
void error_context(ContextErrorTypes context);