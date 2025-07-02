#include "include/repl.h"
#include <parser/include/ast.h>
#include "cli/include/info.h"
#include <stdio.h>
#include <lexer/include/lexer.h>

void repl_welcome()
{
    printf("%s version %s released under %s license\n", LANG_NAME, LANG_VERSION, LANG_LICENSE);
}