# The Shard programming language

## What is Shard?
*Shard* is an interpreted programming language in active development.

## Features
- Dynamic and strongly typed variables
- Division by zero handling
- Floating numbers support
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
make                    # or make clean && make if you want to rebuild
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

## Using the REPL
Once you have installed *Shard*, run `shard` in your terminal.
Now, you should be able to input code.

Let's see the language rules:
1. A variable must be declared like this: `var name = value`
2. You cannot redeclare a variable
3. Instructions can follow each other with `;`
4. You can use the following symbols for maths: `+ - * / ( )`
5. You can assign a value to a declared variable like this: `name = value`
6. To access a variable, just type its name: `name + 4`