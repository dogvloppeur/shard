#pragma once

#include <stddef.h>

/*
    Represents the cursor.
    line: the line where the cursor is
    column: the column where the cursor is
    current_char: the character the cursor is on
    source: the source code
    position: the cursor position
*/
typedef struct
{
    int line, column;
    char current_char, *source;
    size_t position;
} Cursor;

/*
    Initialize the cursor.
    return: the cursor
*/
Cursor cursor_init(int line, int column, char current_char, char *source, size_t position);
/*
    Update the cursor.
*/
void cursor_update(Cursor *cursor);
/*
    Advance the cursor.
*/
void cursor_advance(Cursor *cursor);