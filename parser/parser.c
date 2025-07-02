#include "include/parser.h"
#include "lexer/include/cursor.h"
#include "lexer/include/token.h"
#include "lexer/include/token_type.h"
#include "parser/include/ast.h"
#include "parser/include/checkers.h"
#include <stdio.h>
#include <stdlib.h>

ASTNode *parse(const char *source)
{
    Parser parser = {
        .cursor = cursor_init(1, 0, NULL, source, 0)
    };
    cursor_advance(&parser.cursor);
    parser.current_token = get_next_token(&parser.cursor);

    return parse_statement_list(&parser);
}

void parser_advance(Parser *parser)
{
    parser->current_token = get_next_token(&parser->cursor);
}

void parser_expect(Parser *parser, TokenType type)
{
    if (parser->current_token.type != (int)type)
    {
        fprintf(stderr, "Expected token type: %d\n", type);
        exit(1);
    }

    parser_advance(parser);
}

BinopTypes char_to_binop(char c)
{
    switch (c)
    {
        case '+': return BINOP_PLUS;
        case '-': return BINOP_MINUS;
        case '*': return BINOP_TIMES;
        case '/': return BINOP_DIVIDE;
        default:
            fprintf(stderr, "Unknow binary operator: %c\n", c);
            exit(1);
    }
}

UnopTypes char_to_unop(char c)
{
    switch (c)
    {
        case '+': return UNOP_PLUS;
        case '-': return UNOP_MINUS;
        default:
            fprintf(stderr, "Unknow unary operator: %c\n", c);
            exit(1);
    }
}

AssignmentTypes char_to_assignment(char c)
{
    switch (c)
    {
        case '=': return ASSIGNMENT_EQUAL;
        default:
            fprintf(stderr, "Unknow assignment operator: %c\n", c);
            exit(1);
    }
}

ASTNode *parse_factor(Parser *parser)
{
    if (parser->current_token.type == T_PLUS || parser->current_token.type == T_MINUS)
    {
        UnopTypes operator = char_to_unop(*parser->current_token.value);
        int line = parser->current_token.line;
        int column = parser->current_token.column;
        parser_advance(parser);

        ASTNode *operand = parse_factor(parser);
        return AST_new_unop(operator, operand, line, column);
    }

    if (parser->current_token.type == T_INTEGER)
    {
        int value = atoi(parser->current_token.value);
        ASTNode *node = AST_new_integer(value, parser->current_token.line, parser->current_token.column);
        parser_advance(parser);
        return node;
    }

    if (parser->current_token.type == T_FLOAT)
    {
        float value = atof(parser->current_token.value);
        ASTNode *node = AST_new_float(value, parser->current_token.line, parser->current_token.column);
        parser_advance(parser);
        return node;
    }

    if (parser->current_token.type == T_IDENTIFIER)
        return parse_var_access(parser);

    if (parser->current_token.type == T_LPAR)
    {
        parser_advance(parser);
        ASTNode *node = parse_expression(parser);
        parser_expect(parser, T_RPAR);
        return node;
    }

    fprintf(stderr, "Syntax error in factor\n");
    exit(1);
}

ASTNode *parse_term(Parser *parser)
{
    ASTNode *node = parse_factor(parser);

    while (parser->current_token.type == T_STAR || parser->current_token.type == T_SLASH)
    {
        BinopTypes operator = char_to_binop(*parser->current_token.value);
        parser_advance(parser);
        ASTNode *right = parse_factor(parser);
        ASTNode *new_node = AST_new_binop(operator, node, right, parser->current_token.line, parser->current_token.column);
        node = new_node;
    }

    return node;
}

ASTNode *parse_var_access(Parser *parser)
{
    if (parser->current_token.type != T_IDENTIFIER)
    {
        fprintf(stderr, "Expected identifier for variable access\n");
        exit(1);
    }

    char *name = parser->current_token.value;
    int line = parser->current_token.line;
    int column = parser->current_token.column;

    parser_advance(parser);

    return AST_new_var_access(name, line, column);
}

ASTNode *parse_var_assignment(Parser *parser)
{
    if (parser->current_token.type != T_IDENTIFIER)
    {
        fprintf(stderr, "Expected identifier for variable assignment\n");
        exit(1);
    }

    char *name = parser->current_token.value;
    int line = parser->current_token.line;
    int column = parser->current_token.column;

    parser_advance(parser);

    AssignmentTypes operator = char_to_assignment(*parser->current_token.value);

    parser_advance(parser);

    ASTNode *value = parse_expression(parser);

    return AST_new_var_assignment(name, operator, value, line, column);
}

ASTNode *parse_expression(Parser *parser)
{
    ASTNode *node = parse_term(parser);

    while (parser->current_token.type == T_PLUS || parser->current_token.type == T_MINUS)
    {
        BinopTypes operator = char_to_binop(*parser->current_token.value);
        parser_advance(parser);
        ASTNode *right = parse_term(parser);
        ASTNode *new_node = AST_new_binop(operator, node, right, parser->current_token.line, parser->current_token.column);
        node = new_node;
    }

    return node;
}

ASTNode *parse_var_declaration(Parser *parser)
{
    if (parser->current_token.type != T_IDENTIFIER)
    {
        fprintf(stderr, "Expected identifier after 'var'\n");
        exit(1);
    }

    char *name = parser->current_token.value;
    int line = parser->current_token.line;
    int column = parser->current_token.column;
    parser_advance(parser);

    ASTNode *value = NULL;

    if (parser->current_token.type == T_EQUAL)
    {
        parser_advance(parser);
        value = parse_expression(parser);
    }

    return AST_new_var_declaration(name, value, line, column);
}

ASTNode *parse_statement(Parser *parser)
{
    if (parser->current_token.type == T_KW_VAR)
    {
        parser_advance(parser);
        return parse_var_declaration(parser);
    }
    else if (parser->current_token.type == T_IDENTIFIER)
    {
        Token next_token = peek_next_token(&parser->cursor);

        if (is_token_assignment_operator(next_token.type))
            return parse_var_assignment(parser);
        else
            return parse_expression(parser);
    }
    else
        return parse_expression(parser);
}

ASTNode *parse_statement_list(Parser *parser)
{
    ASTNode *first = parse_statement(parser);
    ASTNode *head = AST_new_statement_list(first, NULL, parser->current_token.line, parser->current_token.column);
    ASTNode *current = head;

    while (parser->current_token.type == T_SEMI)
    {
        parser_advance(parser);
        if (parser->current_token.type == T_EOF)
            break;

        ASTNode *next_stmt = parse_statement(parser);
        current->statement_list.next = AST_new_statement_list(next_stmt, NULL, parser->current_token.line, parser->current_token.column);
        current = current->statement_list.next;
    }

    return head;
}