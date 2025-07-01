#include "include/parser.h"
#include "lexer/include/cursor.h"
#include "lexer/include/token.h"
#include "lexer/include/token_type.h"
#include "parser/include/ast.h"
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

ASTNode *parse_factor(Parser *parser)
{
    if (parser->current_token.type == T_INTEGER)
    {
        int value = atoi(parser->current_token.value);
        ASTNode *node = AST_new_integer(value, parser->current_token.line, parser->current_token.column);
        parser_advance(parser);

        return node;
    }
    else if (parser->current_token.type == T_LPAR)
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
        ASTNode *new_node = AST_new_binop(operator, node, right);
        node = new_node;
    }

    return node;
}

ASTNode *parse_expression(Parser *parser)
{
    ASTNode *node = parse_term(parser);

    while (parser->current_token.type == T_PLUS || parser->current_token.type == T_MINUS)
    {
        BinopTypes operator = char_to_binop(*parser->current_token.value);
        parser_advance(parser);
        ASTNode *right = parse_term(parser);
        ASTNode *new_node = AST_new_binop(operator, node, right);
        node = new_node;
    }

    return node;
}

ASTNode *parse_statement(Parser *parser)
{
    return parse_expression(parser);
}

ASTNode *parse_statement_list(Parser *parser)
{
    ASTNode *first = parse_statement(parser);
    ASTNode *head = AST_new_statement_list(first, NULL);
    ASTNode *current = head;

    while (parser->current_token.type == T_SEMI)
    {
        parser_advance(parser);
        if (parser->current_token.type == T_EOF)
            break;

        ASTNode *next_stmt = parse_statement(parser);
        current->statement_list.next = AST_new_statement_list(next_stmt, NULL);
        current = current->statement_list.next;
    }

    return head;
}