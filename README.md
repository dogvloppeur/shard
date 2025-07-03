# The Shard programming language

## What is Shard?
*Shard* is an interpreted programming language in active development.

## Features
- Booleans and comparison operators
- If/else conditions
- Complete CLI
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
To check if Shard is installed, type
```bash
shard --version
```
in your terminal.

You can now run *Shard* by simply typing `shard` in your terminal!

## Contributing
Everyone is free to contribute, but please read [the contribution guide](CONTRIBUTING.md) before.

## Using the CLI
Once you have installed *Shard*, run `shard` in your terminal.
Now, you should be able to input code. This is what we call the *REPL*.

You can enter the REPL without giving an option.
To see a list of options, type
```bash
shard --help
```

To see some informations about *Shard*, type
```bash
shard --version             # to see your version
shard --license             # to see your license
```

To execute code without using the REPL, type
```bash
shard -c <your code>
```

To execute code from a source file, type
```bash
shard -f <filepath>
# or
shard --file <filepath>
```

## Language rules
Let's see the language rules:
1. A variable must be declared like this: `var name = value`
2. You cannot redeclare a variable
3. Instructions can follow each other with `;`
4. You can use the following symbols for maths: `+ - * / ( ) == != <= >= < >`
5. You can assign a value to a declared variable like this: `name = value`
6. To access a variable, just type its name: `name + 4`
7. The boolean operators are `and or not ^`