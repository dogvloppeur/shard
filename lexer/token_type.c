#include "include/token_type.h"
#include <stddef.h>
#include <string.h>

const char *token_type_names[] = {
    "T_INTEGER",
    "T_PLUS",
    "T_MINUS",
    "T_STAR",
    "T_SLASH",
    "T_LPAR",
    "T_RPAR",
    "T_EOF",
    "T_UNKNOWN",
    "T_SEMI",
    "T_FLOAT",
    "T_IDENTIFIER",
    "T_EQUAL",
    "T_KW_VAR"
};

const Keyword keywords[] = {
    {"var", T_KW_VAR}
};

TokenType keyword_get_type(const char *identifier)
{
    for (size_t i = 0; i < sizeof(keywords) / sizeof(Keyword); i++)
    {
        if (strcmp(identifier, keywords[i].text) == 0)
            return keywords[i].type;
    }

    return T_IDENTIFIER;
}