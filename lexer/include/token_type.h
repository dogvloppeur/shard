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
    T_SEMI
} TokenType;

extern const char *token_type_names[];