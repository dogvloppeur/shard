#pragma once

#include <stddef.h>

typedef struct
{
    int line, column;
    char current_char, *source;
    size_t position;
} Cursor;

Cursor cursor_init(int line, int column, char current_char, char *source, size_t position);
void cursor_update(Cursor *cursor);
void cursor_advance(Cursor *cursor);