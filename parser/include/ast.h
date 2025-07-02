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
    AST_VAR_ACCESS
} ASTNodeTypes;

typedef enum
{
    BINOP_PLUS,
    BINOP_MINUS,
    BINOP_TIMES,
    BINOP_DIVIDE
} BinopTypes;

typedef enum
{
    UNOP_PLUS,
    UNOP_MINUS
} UnopTypes;

typedef enum
{
    ASSIGNMENT_EQUAL
} AssignmentTypes;

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
void AST_free(ASTNode *node);