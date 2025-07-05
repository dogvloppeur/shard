#pragma once

#include "parser/include/ast.h"
#include "lexer/include/token_type.h"

BinopTypes token_type_to_binop(TokenType type);
UnopTypes token_type_to_unop(TokenType type);
AssignmentTypes token_type_to_assignment(TokenType type);