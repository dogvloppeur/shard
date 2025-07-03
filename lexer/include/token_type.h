#pragma once

typedef enum
{
    T_INTEGER,
    T_PLUS,
    T_MINUS,
    T_STAR,
    T_SLASH,
    T_LPAR,
    T_RPAR,
    T_EOF,
    T_UNKNOWN,
    T_SEMI,
    T_FLOAT,
    T_IDENTIFIER,
    T_EQUAL,
    T_KW_VAR,
    T_KW_IF,
    T_KW_ELSE,
    T_LBRACE,
    T_RBRACE,
    T_EQEQUAL,
    T_NOTEQUAL,
    T_LESSTHAN,
    T_LESSTHANEQ,
    T_GREATTHAN,
    T_GREATTHANEQ,
    T_KW_AND,
    T_KW_OR,
    T_XOR,
    T_KW_NOT
} TokenType;

typedef struct
{
    const char *text;
    TokenType type;
} Keyword;

extern const char *token_type_names[];
extern const Keyword keywords[];

TokenType keyword_get_type(const char *identifier);