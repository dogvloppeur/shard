#include "convert.h"

BinopTypes token_type_to_binop(TokenType type)
{
    switch ((int)type)
    {
        case T_PLUS: return BINOP_PLUS;
        case T_MINUS: return BINOP_MINUS;
        case T_STAR: return BINOP_TIMES;
        case T_SLASH: return BINOP_DIVIDE;
        case T_EQEQUAL: return BINOP_ISEQUAL;
        case T_NOTEQUAL: return BINOP_ISNOTEQUAL;
        case T_LESSTHAN: return BINOP_ISLESSER;
        case T_LESSTHANEQ: return BINOP_ISLESSEREQUAL;
        case T_GREATTHAN: return BINOP_ISGREATER;
        case T_GREATTHANEQ: return BINOP_ISGREATEREQUAL;
        case T_KW_AND: return BINOP_AND;
        case T_KW_OR: return BINOP_OR;
        case T_XOR: return BINOP_XOR;
        case T_ANDBIT: return BINOP_ANDBIT;
        case T_ORBIT: return BINOP_ORBIT;
        case T_LSHIFT: return BINOP_LSHIFT;
        case T_RSHIFT: return BINOP_RSHIFT;
    }
}

UnopTypes token_type_to_unop(TokenType type)
{
    switch ((int)type)
    {
        case T_PLUS: return UNOP_PLUS;
        case T_MINUS: return UNOP_MINUS;
        case T_KW_NOT: return UNOP_NOT;
        case T_BITWISE: return UNOP_BITWISE;
    }
}

AssignmentTypes token_type_to_assignment(TokenType type)
{
    switch ((int)type)
    {
        case T_EQUAL: return ASSIGNMENT_EQUAL;
        case T_PLUSEQ: return ASSIGNMENT_PLUSEQ;
        case T_MINUSEQ: return ASSIGNMENT_MINUSEQ;
        case T_STAREQ: return ASSIGNMENT_STAREQ;
        case T_SLASHEQ: return ASSIGNMENT_SLASHEQ;
        case T_ANDEQ: return ASSIGNMENT_ANDEQ;
        case T_OREQ: return ASSIGNMENT_OREQ;
        case T_XOREQ: return ASSIGNMENT_XOREQ;
        case T_BITWEQ: return ASSIGNMENT_BITWEQ;
        case T_LSHIFTEQ: return ASSIGNMENT_LSHIFTEQ;
        case T_RSHIFTEQ: return ASSIGNMENT_RSHIFTEQ;
    }
}