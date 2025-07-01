#!/bin/bash

set -e

D_BUILD="../build"
D_LEXER="../lexer"
D_PARSER="../parser"
D_EVAL="../eval"
D_REPL="../repl"

CC="gcc"

CFLAGS="-Wall -Wextra -g -I.."

mkdir -p "$D_BUILD"

$CC $CFLAGS -c -o "$D_BUILD/token_type.o" "$D_LEXER/token_type.c"
$CC $CFLAGS -c -o "$D_BUILD/token.o" "$D_LEXER/token.c"
$CC $CFLAGS -c -o "$D_BUILD/cursor.o" "$D_LEXER/cursor.c"
$CC $CFLAGS -c -o "$D_BUILD/lexer.o" "$D_LEXER/lexer.c"
$CC $CFLAGS -c -o "$D_BUILD/ast.o" "$D_PARSER/ast.c"
$CC $CFLAGS -c -o "$D_BUILD/parser.o" "$D_PARSER/parser.c"
$CC $CFLAGS -c -o "$D_BUILD/eval.o" "$D_EVAL/eval.c"
$CC $CFLAGS -c -o "$D_BUILD/repl.o" "$D_REPL/repl.c"

$CC $CFLAGS -o ../shard \
"$D_BUILD/token_type.o" \
"$D_BUILD/cursor.o" \
"$D_BUILD/token.o" \
"$D_BUILD/lexer.o" \
"$D_BUILD/ast.o" \
"$D_BUILD/parser.o" \
"$D_BUILD/eval.o" \
"$D_BUILD/repl.o"