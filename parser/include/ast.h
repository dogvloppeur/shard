#pragma once

typedef enum
{
    AST_INTEGER,
    AST_BINOP,
    AST_STATEMENT_LIST
} ASTNodeTypes;

typedef enum
{
    BINOP_PLUS,
    BINOP_MINUS,
    BINOP_TIMES,
    BINOP_DIVIDE
} BinopTypes;

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
    };
};

ASTNode *AST_new_integer(int value, int line, int column);
ASTNode *AST_new_binop(BinopTypes operator, ASTNode *left, ASTNode *right);
ASTNode *AST_new_statement_list(ASTNode *first, ASTNode *next);
void AST_free(ASTNode *node);