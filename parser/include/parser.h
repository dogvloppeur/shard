#pragma once

#include <lexer/include/cursor.h>
#include <lexer/include/token.h>
#include "ast.h"
#include <lexer/include/token_type.h>

typedef struct
{
    Cursor cursor;
    Token current_token;
} Parser;

ASTNode *parse(const char *source);
void parser_advance(Parser *parser);
void parser_expect(Parser *parser, TokenType type);
BinopTypes token_type_to_binop(TokenType type);
UnopTypes token_type_to_unop(TokenType type);
AssignmentTypes token_type_to_assignment(TokenType type);
ASTNode *parse_factor(Parser *parser);
ASTNode *parse_term(Parser *parser);
ASTNode *parse_var_access(Parser *parser);
ASTNode *parse_var_assignment(Parser *parser);
ASTNode *parse_expression(Parser *parser);
ASTNode *parse_var_declaration(Parser *parser);
ASTNode *parse_statement(Parser *parser);
ASTNode *parse_statement_list(Parser *parser);
ASTNode *parse_cond_if(Parser *parser);
ASTNode *parse_comparison(Parser *parser);
ASTNode *parse_bool(Parser *parser);