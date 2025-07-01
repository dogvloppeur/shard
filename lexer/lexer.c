#include <stdio.h>

#include "include/lexer.h"
#include "include/cursor.h"
#include "include/token.h"
#include "include/token_type.h"

extern const char *token_type_names[];

void lex(char *source)
{
    Cursor cursor = cursor_init(1, 0, NULL, source, 0);
    Token token;

    cursor_advance(&cursor);

    do
    {
        token = get_next_token(&cursor);
        printf("TOKEN: type=%s, value=%.*s (line %d, column %d)\n", token_type_names[token.type], token.length, token.value, token.line, token.column);
    } while (token.type != T_EOF);
}