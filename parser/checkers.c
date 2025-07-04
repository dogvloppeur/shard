#include "include/checkers.h"
#include "lexer/include/token_type.h"

bool is_token_assignment_operator(TokenType token)
{
    return (token == T_EQUAL || token == T_PLUSEQ || token == T_MINUSEQ || token == T_STAREQ || token == T_SLASHEQ
            || token == T_ANDEQ || token == T_OREQ || token == T_XOREQ || token == T_BITWEQ);
}