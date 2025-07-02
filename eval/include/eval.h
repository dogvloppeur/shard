#pragma once

#include "eval/include/variable.h"
#include "parser/include/ast.h"
#include "env.h"

ShdValue eval(Env *env, ASTNode *node);