#include "include/cursor.h"

Cursor cursor_init(int line, int column, char current_char, char *source, size_t position)
{
    Cursor cursor = {
        .line = line,
        .column = column,
        .current_char = current_char,
        .source = source,
        .position = position
    };

    return cursor;
}

void cursor_update(Cursor *cursor)
{
    switch (cursor->current_char)
    {
        case '\n':
            cursor->line++;
            cursor->column = 0;
            break;

        case '\r':
            cursor->line++;
            cursor->column = 0;
            break;

        case '\b':
            if (cursor->column > 0)
                cursor->column--;
            break;

        default:
            cursor->column++;
            break;
    }
}

void cursor_advance(Cursor *cursor)
{
    if (cursor->source[cursor->position] != '\0')
    {
        cursor->position++;
        cursor->current_char = cursor->source[cursor->position];
        cursor_update(cursor);
    }
    else
        cursor->current_char = '\0';
}