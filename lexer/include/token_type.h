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
    T_KW_NOT,
    T_ANDBIT,
    T_ORBIT,
    T_BITWISE,
    T_PLUSEQ,
    T_MINUSEQ,
    T_STAREQ,
    T_SLASHEQ,
    T_ANDEQ,
    T_OREQ,
    T_XOREQ,
    T_BITWEQ,
    T_LSHIFT,
    T_RSHIFT,
    T_LSHIFTEQ,
    T_RSHIFTEQ,
    T_KW_WHILE,
    T_KW_UNTIL,
    T_KW_FOREVER,
    T_KW_BREAK,
    T_KW_CONTINUE
} TokenType;

typedef struct
{
    const char *text;
    TokenType type;
} Keyword;

extern const char *token_type_names[];
extern const Keyword keywords[];

TokenType keyword_get_type(const char *identifier);