#include <stdlib.h>

#include "include/ast.h"

ASTNode *AST_new_integer(int value, int line, int column)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    
    node->type = AST_INTEGER;
    node->line = line;
    node->column = column;
    node->integer.value = value;

    return node;
}

ASTNode *AST_new_binop(BinopTypes operator, ASTNode *left, ASTNode *right, int line, int column)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    node->type = AST_BINOP;
    node->binop.operator = operator;
    node->binop.left = left;
    node->binop.right = right;
    node->line = line;
    node->column = column;

    return node;
}

ASTNode *AST_new_statement_list(ASTNode *first, ASTNode *next, int line, int column)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    node->type = AST_STATEMENT_LIST;
    node->statement_list.first = first;
    node->statement_list.next = next;
    node->line = line;
    node->column = column;

    return node;
}

ASTNode *AST_new_float(float value, int line, int column)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    node->type = AST_FLOAT;
    node->line = line;
    node->column = column;
    node->_float.value = value;

    return node;
}

ASTNode *AST_new_unop(UnopTypes operator, ASTNode *value, int line, int column)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    node->type = AST_UNOP;
    node->unop.operator = operator;
    node->unop.value = value;
    node->line = line;
    node->column = column;

    return node;
}

ASTNode *AST_new_var_declaration(const char *name, ASTNode *value, int line, int column)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    node->type = AST_VAR_DECLARATION;
    node->var_declaration.name = name;
    node->var_declaration.value = value;
    node->line = line;
    node->column = column;

    return node;
}

ASTNode *AST_new_var_assignment(const char *name, AssignmentTypes operator, ASTNode *value, int line, int column)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    node->type = AST_VAR_ASSIGNMENT;
    node->var_assignment.name = name;
    node->var_assignment.operator = operator;
    node->var_assignment.value = value;

    return node;
}

ASTNode *AST_new_var_access(const char *name, int line, int column)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    node->type = AST_VAR_ACCESS;
    node->var_access.name = name;
    node->line = line;
    node->column = column;

    return node;
}

void AST_free(ASTNode *node)
{
    if (!node) return;

    switch ((int)node->type)
    {
        case AST_INTEGER:
            break;
        
        case AST_BINOP:
            AST_free(node->binop.left);
            AST_free(node->binop.right);
            break;
    }

    free (node);
}