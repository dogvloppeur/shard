#include <parser/include/checkers.h>
#include <lexer/include/token_type.h>

bool is_token_assignment_operator(TokenType token)
{
    return (token == T_EQUAL || token == T_PLUSEQ || token == T_MINUSEQ || token == T_STAREQ || token == T_SLASHEQ
            || token == T_ANDEQ || token == T_OREQ || token == T_XOREQ || token == T_BITWEQ || token == T_LSHIFTEQ
            || token == T_RSHIFTEQ);
}

bool is_token_comparison_operator(TokenType token)
{
    return (token == T_EQEQUAL || token == T_NOTEQUAL || token == T_LESSTHAN || token == T_LESSTHANEQ || token == T_GREATTHAN
            || token == T_GREATTHANEQ);
}