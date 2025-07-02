#include "include/checkers.h"
#include "lexer/include/token_type.h"

bool is_token_assignment_operator(TokenType token)
{
    return token == T_EQUAL;
}