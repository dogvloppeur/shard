#pragma once

#include <eval/include/env.h>
#include <parser/include/ast.h>

#define MAX_CHARS_PER_LINE                      1024

void repl_welcome();
void eval_and_print_statements(Env *env, ASTNode *node);