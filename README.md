# The Shard programming language

## What is Shard?
*Shard* is an interpreted programming language in active development.

## Features
- Arithmetic expressions
- Operators precedences
- Statement lists

## Build
1. Clone this repository
```bash
git clone https://github.com/shard-language/shard
```

2. Execute the configuration script if you don't have GCC installed
```bash
cd shard
cd scripts
chmod +x config.sh
./config.sh
```

3. Execute the build script
```bash
chmod +x build.sh
./build.sh
```

The executable is in `shard/shard`.

## Syntax
a                           -> integer
a + b
a - b
a * b
a / b
a; b; c                     -> statement list