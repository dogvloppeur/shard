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

2. Make the code (don't forget to install `make`)
```bash
cd shard
cd scripts
make
```
The executable is in `shard/shard`.

3. (optional) Installation
```bash
chmod +x install.sh
su                      # or sudo/doas...
./install.sh
```
You can now run *Shard* by simply typing `shard` in your terminal!

## Contributing
Everyone is free to contribute, but please read [the contribution guide](CONTRIBUTING.md) before.

## Syntax
a                           -> integer

a + b

a - b

a * b

a / b

a; b; c                     -> statement list