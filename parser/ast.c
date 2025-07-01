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

ASTNode *AST_new_binop(BinopTypes operator, ASTNode *left, ASTNode *right)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    node->type = AST_BINOP;
    node->binop.operator = operator;
    node->binop.left = left;
    node->binop.right = right;

    return node;
}

ASTNode *AST_new_statement_list(ASTNode *first, ASTNode *next)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    node->type = AST_STATEMENT_LIST;
    node->statement_list.first = first;
    node->statement_list.next = next;

    return node;
}

ASTNode *AST_new_float(float value, float line, float column)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    node->type = AST_FLOAT;
    node->line = line;
    node->column = column;
    node->_float.value = value;

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