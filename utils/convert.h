#pragma once

#include <parser/include/ast.h>
#include <lexer/include/token_type.h>

/*
    Converts a token type to a binary operation.
*/
BinopTypes token_type_to_binop(TokenType type);
/*
    Converts a token type to an unary operation.
*/
UnopTypes token_type_to_unop(TokenType type);
/*
    Converts a token type to an assignment operation.
*/
AssignmentTypes token_type_to_assignment(TokenType type);