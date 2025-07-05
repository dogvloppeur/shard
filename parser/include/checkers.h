#pragma once

#include <lexer/include/token_type.h>
#include <stdbool.h>

/*
    Check if the given token is an assignment operator.
*/
bool is_token_assignment_operator(TokenType token);
/*
    Check if the given token is a comparison operator.
*/
bool is_token_comparison_operator(TokenType token);