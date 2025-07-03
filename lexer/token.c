#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "include/token.h"
#include "include/token_type.h"
#include "lexer/include/cursor.h"

Token get_next_token(Cursor *cursor)
{
    while (cursor->current_char == ' ' || cursor->current_char == '\t' || cursor->current_char == '\n')
        cursor_advance(cursor);

    Token token = {
        .line = cursor->line,
        .column = cursor->column,
        .value = &cursor->source[cursor->position]
    };

    if (cursor->current_char == '\0')
    {
        token.type = T_EOF;
        token.length = 0;
        return token;
    }

    if (isdigit(cursor->current_char))
    {
        size_t start = cursor->position;
        while (isdigit(cursor->current_char))
            cursor_advance(cursor);
        
        if (cursor->current_char == '.' && isdigit(cursor->source[cursor->position + 1]))
        {
            cursor_advance(cursor);

            while (isdigit(cursor->current_char))
                cursor_advance(cursor);

            token.type = T_FLOAT;
        }
        else
        {
            token.type = T_INTEGER;
        }

        token.length = cursor->position - start;
        return token;
    }

    if (isalpha(cursor->current_char) || cursor->current_char == '_')
    {
        size_t start = cursor->position;

        while (isalnum(cursor->current_char) || cursor->current_char == '_')
            cursor_advance(cursor);

        size_t length = cursor->position - start;
        char *identifier = strndup(cursor->source + start, length);

        token.type = keyword_get_type(identifier);
        token.value = identifier;
        token.length = length;
        token.line = cursor->line;
        token.column = cursor->column;

        return token;
    }

    switch (cursor->current_char)
    {
        case '+':
            token.type = T_PLUS;
            token.length = 1;
            cursor_advance(cursor);
            return token;

        case '-':
            token.type = T_MINUS;
            token.length = 1;
            cursor_advance(cursor);
            return token;

        case '*':
            token.type = T_STAR;
            token.length = 1;
            cursor_advance(cursor);
            return token;

        case '/':
            token.type = T_SLASH;
            token.length = 1;
            cursor_advance(cursor);
            return token;

        case '(':
            token.type = T_LPAR;
            token.length = 1;
            cursor_advance(cursor);
            return token;

        case ')':
            token.type = T_RPAR;
            token.length = 1;
            cursor_advance(cursor);
            return token;

        case ';':
            token.type = T_SEMI;
            token.length = 1;
            cursor_advance(cursor);
            return token;

        case '=':
            cursor_advance(cursor);

            if (cursor->current_char == '=')
            {
                cursor_advance(cursor);
                token.type = T_EQEQUAL;
                token.length = 2;
                return token;
            }

            token.type = T_EQUAL;
            token.length = 1;
            return token;

        case '{':
            token.type = T_LBRACE;
            token.length = 1;
            cursor_advance(cursor);
            return token;

        case '}':
            token.type = T_RBRACE;
            token.length = 1;
            cursor_advance(cursor);
            return token;

        case '!':
            cursor_advance(cursor);

            if (cursor->current_char == '=')
            {
                cursor_advance(cursor);
                token.type = T_NOTEQUAL;
                token.length = 2;
                return token;
            }

            fprintf(stderr, "Illegal character '!'\n");
            token.type = T_UNKNOWN;

        case '<':
            cursor_advance(cursor);

            if (cursor->current_char == '=')
            {
                cursor_advance(cursor);
                token.type = T_LESSTHANEQ;
                token.length = 2;
                return token;
            }

            token.type = T_LESSTHAN;
            token.length = 1;
            return token;

        case '>':
            cursor_advance(cursor);

            if (cursor->current_char == '=')
            {
                cursor_advance(cursor);
                token.type = T_GREATTHANEQ;
                token.length = 2;
                return token;
            }

            token.type = T_GREATTHAN;
            token.length = 1;
            return token;

        case '^':
            token.type = T_XOR;
            token.length = 1;
            cursor_advance(cursor);
            return token;
    }

    token.type = T_UNKNOWN;
    token.length = 1;
    cursor_advance(cursor);
    return token;
}

Token peek_next_token(Cursor *cursor)
{
    Cursor tmp_cursor = *cursor;
    tmp_cursor.current_char = tmp_cursor.source[tmp_cursor.position];
    Token next_token = get_next_token(&tmp_cursor);
    return next_token;
}