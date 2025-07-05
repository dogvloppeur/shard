#pragma once

#include <lexer/include/cursor.h>

/*
    Represents a token.
    type: the type of the token
    line: the line where the token is
    column: the column where the token is
    length: the length of the token (in characters)
    value: the value of the token
*/
typedef struct
{
    int type, line, column, length;
    char *value;
} Token;

/*
    Get the next token using the cursor.
*/
Token get_next_token(Cursor *cursor);
/*
    Get the next token without consuming it.
*/
Token peek_next_token(Cursor *cursor);