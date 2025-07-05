#include <stdlib.h>

#include <parser/include/ast.h>

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

ASTNode *AST_new_cond_if(ASTNode *condition, ASTNode *branch, int line, int column)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    node->type = AST_COND_IF;
    node->cond_if.condition = condition;
    node->cond_if.branch = branch;
    node->line = line;
    node->column = column;

    return node;
}

ASTNode *AST_new_cond_if_else(ASTNode *condition, ASTNode *then_branch, ASTNode *else_branch, int line, int column)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    node->type = AST_COND_IF_ELSE;
    node->cond_if_else.condition = condition;
    node->cond_if_else.then_branch = then_branch;
    node->cond_if_else.else_branch = else_branch;
    node->line = line;
    node->column = column;

    return node;
}

ASTNode *AST_new_cond_loop(CondLoopTypes type, ASTNode *condition, ASTNode *branch, int line, int column)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    node->type = AST_COND_LOOP;
    node->cond_loop.type = type;
    node->cond_loop.condition = condition;
    node->cond_loop.branch = branch;
    node->line = line;
    node->column = column;

    return node;
}

ASTNode *AST_new_uncond_loop(ASTNode *branch, int line, int column)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    node->type = AST_UNCOND_LOOP;
    node->uncond_loop.branch = branch;
    node->line = line;
    node->column = column;

    return node;
}

ASTNode *AST_new_loop_control(LoopControlTypes type, int line, int column)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    node->type = AST_LOOP_CONTROL;
    node->loop_control.type = type;
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

        case AST_FLOAT:
            break;

        case AST_BINOP:
            AST_free(node->binop.left);
            AST_free(node->binop.right);
            break;

        case AST_UNOP:
            AST_free(node->unop.value);
            break;

        case AST_STATEMENT_LIST:
            AST_free(node->statement_list.first);
            AST_free(node->statement_list.next);
            break;

        case AST_VAR_DECLARATION:
            AST_free(node->var_declaration.value);
            break;

        case AST_VAR_ASSIGNMENT:
            AST_free(node->var_assignment.value);
            break;

        case AST_VAR_ACCESS:
            break;

        case AST_COND_IF:
            AST_free(node->cond_if.condition);
            AST_free(node->cond_if.branch);
            break;

        case AST_COND_IF_ELSE:
            AST_free(node->cond_if_else.condition);
            AST_free(node->cond_if_else.then_branch);
            AST_free(node->cond_if_else.else_branch);
            break;

        case AST_COND_LOOP:
            AST_free(node->cond_loop.condition);
            AST_free(node->cond_loop.branch);
            break;

        case AST_UNCOND_LOOP:
            AST_free(node->uncond_loop.branch);
            break;

        default:
            break;
    }

    free(node);
}