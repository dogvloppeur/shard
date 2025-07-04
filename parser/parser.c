#include "include/parser.h"
#include "lexer/include/cursor.h"
#include "lexer/include/token.h"
#include "lexer/include/token_type.h"
#include "parser/include/ast.h"
#include "parser/include/checkers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils/error.h>
#include <utils/color.h>

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
        error_syntax(SERR_EXPECT, token_type_names[parser->current_token.type], parser->current_token.line);
    parser_advance(parser);
}

BinopTypes token_type_to_binop(TokenType type)
{
    switch ((int)type)
    {
        case T_PLUS: return BINOP_PLUS;
        case T_MINUS: return BINOP_MINUS;
        case T_STAR: return BINOP_TIMES;
        case T_SLASH: return BINOP_DIVIDE;
        case T_EQEQUAL: return BINOP_ISEQUAL;
        case T_NOTEQUAL: return BINOP_ISNOTEQUAL;
        case T_LESSTHAN: return BINOP_ISLESSER;
        case T_LESSTHANEQ: return BINOP_ISLESSEREQUAL;
        case T_GREATTHAN: return BINOP_ISGREATER;
        case T_GREATTHANEQ: return BINOP_ISGREATEREQUAL;
        case T_KW_AND: return BINOP_AND;
        case T_KW_OR: return BINOP_OR;
        case T_XOR: return BINOP_XOR;
        case T_ANDBIT: return BINOP_ANDBIT;
        case T_ORBIT: return BINOP_ORBIT;
    }
}

UnopTypes token_type_to_unop(TokenType type)
{
    switch ((int)type)
    {
        case T_PLUS: return UNOP_PLUS;
        case T_MINUS: return UNOP_MINUS;
        case T_KW_NOT: return UNOP_NOT;
        case T_BITWISE: return UNOP_BITWISE;
    }
}

AssignmentTypes token_type_to_assignment(TokenType type)
{
    switch ((int)type)
    {
        case T_EQUAL: return ASSIGNMENT_EQUAL;
        case T_PLUSEQ: return ASSIGNMENT_PLUSEQ;
        case T_MINUSEQ: return ASSIGNMENT_MINUSEQ;
        case T_STAREQ: return ASSIGNMENT_STAREQ;
        case T_SLASHEQ: return ASSIGNMENT_SLASHEQ;
        case T_ANDEQ: return ASSIGNMENT_ANDEQ;
        case T_OREQ: return ASSIGNMENT_OREQ;
        case T_XOREQ: return ASSIGNMENT_XOREQ;
        case T_BITWEQ: return ASSIGNMENT_BITWEQ;
    }
}

ASTNode *parse_factor(Parser *parser)
{
    if (parser->current_token.type == T_PLUS || parser->current_token.type == T_MINUS)
    {
        UnopTypes operator = token_type_to_unop(parser->current_token.type);
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
        ASTNode *node = parse_bool(parser);
        parser_expect(parser, T_RPAR);
        return node;
    }

    error_syntax(SERR_DEFAULT, parser->current_token.value, parser->current_token.line);
}

ASTNode *parse_term(Parser *parser)
{
    ASTNode *node = parse_factor(parser);
    while (parser->current_token.type == T_STAR || parser->current_token.type == T_SLASH)
    {
        BinopTypes operator = token_type_to_binop(parser->current_token.type);
        int line = parser->current_token.line;
        int column = parser->current_token.column;
        parser_advance(parser);
        ASTNode *right = parse_factor(parser);
        node = AST_new_binop(operator, node, right, line, column);
    }
    return node;
}

ASTNode *parse_expression(Parser *parser)
{
    ASTNode *node = parse_term(parser);
    while (parser->current_token.type == T_PLUS || parser->current_token.type == T_MINUS)
    {
        BinopTypes operator = token_type_to_binop(parser->current_token.type);
        int line = parser->current_token.line;
        int column = parser->current_token.column;
        parser_advance(parser);
        ASTNode *right = parse_term(parser);
        node = AST_new_binop(operator, node, right, line, column);
    }
    return node;
}

ASTNode *parse_comparison(Parser *parser)
{
    ASTNode *node = parse_expression(parser);
    while (is_token_assignment_operator(parser->current_token.type))
    {
        BinopTypes operator = token_type_to_binop(parser->current_token.type);
        int line = parser->current_token.line;
        int column = parser->current_token.column;
        parser_advance(parser);
        ASTNode *right = parse_expression(parser);
        node = AST_new_binop(operator, node, right, line, column);
    }
    return node;
}

ASTNode *parse_bool_factor(Parser *parser)
{
    if (parser->current_token.type == T_KW_NOT)
    {
        UnopTypes operator = token_type_to_unop(parser->current_token.type);
        int line = parser->current_token.line;
        int column = parser->current_token.column;
        parser_advance(parser);
        ASTNode *value = parse_bool_factor(parser);
        return AST_new_unop(operator, value, line, column);
    }

    if (parser->current_token.type == T_BITWISE)
    {
        UnopTypes operand = token_type_to_unop(parser->current_token.type);
        int line = parser->current_token.line;
        int column = parser->current_token.column;
        parser_advance(parser);
        ASTNode *value = parse_bool_factor(parser);
        return AST_new_unop(operand, value, line, column);
    }

    return parse_comparison(parser);
}

ASTNode *parse_bool(Parser *parser)
{
    ASTNode *node = parse_bool_factor(parser);
    while (parser->current_token.type == T_KW_AND || parser->current_token.type == T_KW_OR || parser->current_token.type == T_XOR || parser->current_token.type == T_ANDBIT || parser->current_token.type == T_ORBIT)
    {
        BinopTypes operator = token_type_to_binop(parser->current_token.type);
        int line = parser->current_token.line;
        int column = parser->current_token.column;
        parser_advance(parser);
        ASTNode *right = parse_bool_factor(parser);
        node = AST_new_binop(operator, node, right, line, column);
    }

    return node;
}

ASTNode *parse_var_access(Parser *parser)
{
    if (parser->current_token.type != T_IDENTIFIER)
        error_syntax(SERR_EXPECT, "identifier for variable access",parser->current_token.line);
    char *name = parser->current_token.value;
    int line = parser->current_token.line;
    int column = parser->current_token.column;
    parser_advance(parser);
    return AST_new_var_access(name, line, column);
}

ASTNode *parse_var_assignment(Parser *parser)
{
    if (parser->current_token.type != T_IDENTIFIER)
        error_syntax(SERR_EXPECT, "identifier for variable assignment", parser->current_token.line);

    char *name = parser->current_token.value;
    int line = parser->current_token.line;
    int column = parser->current_token.column;
    parser_advance(parser);
    AssignmentTypes operator = token_type_to_assignment(parser->current_token.type);
    parser_advance(parser);
    ASTNode *value = parse_bool(parser);
    return AST_new_var_assignment(name, operator, value, line, column);
}

ASTNode *parse_var_declaration(Parser *parser)
{
    if (parser->current_token.type != T_IDENTIFIER)
        error_syntax(SERR_EXPECT, "identifier after \"var\"", parser->current_token.line);

    char *name = parser->current_token.value;
    int line = parser->current_token.line;
    int column = parser->current_token.column;
    parser_advance(parser);
    ASTNode *value = NULL;
    if (parser->current_token.type == T_EQUAL)
    {
        parser_advance(parser);
        value = parse_bool(parser);
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
            return parse_bool(parser);
    }
    else if (parser->current_token.type == T_KW_IF)
        return parse_cond_if(parser);
    else
        return parse_bool(parser);
}

ASTNode *parse_statement_list(Parser *parser)
{
    ASTNode *first = parse_statement(parser);
    ASTNode *head = AST_new_statement_list(first, NULL, parser->current_token.line, parser->current_token.column);
    ASTNode *current = head;

    while (parser->current_token.type != T_EOF && parser->current_token.type != T_RBRACE)
    {
        if (parser->current_token.type != T_SEMI)
            error_syntax(SERR_EXPECT, "';' between statements", parser->current_token.line);

        parser_advance(parser);
        if (parser->current_token.type == T_EOF || parser->current_token.type == T_RBRACE)
            break;

        ASTNode *next_stmt = parse_statement(parser);
        current->statement_list.next = AST_new_statement_list(next_stmt, NULL, parser->current_token.line, parser->current_token.column);
        current = current->statement_list.next;
    }

    return head;
}

ASTNode *parse_cond_if(Parser *parser)
{
    int line = parser->current_token.line;
    int column = parser->current_token.column;

    parser_expect(parser, T_KW_IF);
    ASTNode *condition = parse_bool(parser);
    parser_expect(parser, T_LBRACE);
    ASTNode *then_branch = parse_statement_list(parser);
    parser_expect(parser, T_RBRACE);

    if (parser->current_token.type == T_KW_ELSE)
    {
        parser_advance(parser);
        parser_expect(parser, T_LBRACE);
        ASTNode *else_branch = parse_statement_list(parser);
        parser_expect(parser, T_RBRACE);

        return AST_new_cond_if_else(condition, then_branch, else_branch, line, column);
    }

    return AST_new_cond_if(condition, then_branch, line, column);
}