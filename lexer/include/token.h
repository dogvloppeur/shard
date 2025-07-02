#pragma once

#include "cursor.h"

typedef struct
{
    int type, line, column, length;
    char *value;
} Token;

Token get_next_token(Cursor *cursor);
Token peek_next_token(Cursor *cursor);