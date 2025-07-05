#pragma once

typedef enum
{
    AST_INTEGER,
    AST_BINOP,
    AST_STATEMENT_LIST,
    AST_FLOAT,
    AST_UNOP,
    AST_VAR_DECLARATION,
    AST_VAR_ASSIGNMENT,
    AST_VAR_ACCESS,
    AST_COND_IF,
    AST_COND_IF_ELSE,
    AST_COND_LOOP,
    AST_UNCOND_LOOP,
    AST_LOOP_CONTROL
} ASTNodeTypes;

typedef enum
{
    BINOP_PLUS,
    BINOP_MINUS,
    BINOP_TIMES,
    BINOP_DIVIDE,
    BINOP_ISEQUAL,
    BINOP_ISNOTEQUAL,
    BINOP_ISLESSER,
    BINOP_ISLESSEREQUAL,
    BINOP_ISGREATER,
    BINOP_ISGREATEREQUAL,
    BINOP_AND,
    BINOP_OR,
    BINOP_XOR,
    BINOP_NOT,
    BINOP_ANDBIT,
    BINOP_ORBIT,
    BINOP_LSHIFT,
    BINOP_RSHIFT
} BinopTypes;

typedef enum
{
    UNOP_PLUS,
    UNOP_MINUS,
    UNOP_NOT,
    UNOP_BITWISE
} UnopTypes;

typedef enum
{
    ASSIGNMENT_EQUAL,
    ASSIGNMENT_PLUSEQ,
    ASSIGNMENT_MINUSEQ,
    ASSIGNMENT_STAREQ,
    ASSIGNMENT_SLASHEQ,
    ASSIGNMENT_ANDEQ,
    ASSIGNMENT_OREQ,
    ASSIGNMENT_XOREQ,
    ASSIGNMENT_BITWEQ,
    ASSIGNMENT_LSHIFTEQ,
    ASSIGNMENT_RSHIFTEQ
} AssignmentTypes;

typedef enum
{
    CONDLOOP_WHILE,
    CONDLOOP_UNTIL
} CondLoopTypes;

typedef enum
{
    LOOPCTRL_BREAK,
    LOOPCTRL_CONTINUE
} LoopControlTypes;

typedef struct ASTNode ASTNode;

struct ASTNode
{
    ASTNodeTypes type;
    int line, column;

    union
    {
        struct
        {
            int value;
        } integer;

        struct
        {
            BinopTypes operator;
            ASTNode *left;
            ASTNode *right;
        } binop;

        struct
        {
            ASTNode *first;
            ASTNode *next;
        } statement_list;

        struct
        {
            float value;
        } _float;

        struct
        {
            UnopTypes operator;
            ASTNode *value;
        } unop;

        struct
        {
            char *name;
            ASTNode *value;
        } var_declaration;

        struct
        {
            char *name;
            AssignmentTypes operator;
            ASTNode *value;
        } var_assignment;

        struct
        {
            char *name;
        } var_access;

        struct
        {
            ASTNode *condition;
            ASTNode *branch;
        } cond_if;

        struct
        {
            ASTNode *condition;
            ASTNode *then_branch;
            ASTNode *else_branch;
        } cond_if_else;

        struct
        {
            CondLoopTypes type;
            ASTNode *condition;
            ASTNode *branch;
        } cond_loop;

        struct
        {
            ASTNode *branch;
        } uncond_loop;

        struct
        {
            LoopControlTypes type;
        } loop_control;
    };
};

ASTNode *AST_new_integer(int value, int line, int column);
ASTNode *AST_new_binop(BinopTypes operator, ASTNode *left, ASTNode *right, int line, int column);
ASTNode *AST_new_statement_list(ASTNode *first, ASTNode *next, int line, int column);
ASTNode *AST_new_float(float value, int line, int column);
ASTNode *AST_new_unop(UnopTypes operator, ASTNode *value, int line, int column);
ASTNode *AST_new_var_declaration(const char *name, ASTNode *value, int line, int column);
ASTNode *AST_new_var_assignment(const char *name, AssignmentTypes operator, ASTNode *value, int line, int column);
ASTNode *AST_new_var_access(const char *name, int line, int column);
ASTNode *AST_new_cond_if(ASTNode *condition, ASTNode *branch, int line, int column);
ASTNode *AST_new_cond_if_else(ASTNode *condition, ASTNode *then_branch, ASTNode *else_branch, int line, int column);
ASTNode *AST_new_cond_loop(CondLoopTypes type, ASTNode *condition, ASTNode *branch, int line, int column);
ASTNode *AST_new_uncond_loop(ASTNode *branch, int line, int column);
ASTNode *AST_new_loop_control(LoopControlTypes type, int line, int column);

void AST_free(ASTNode *node);