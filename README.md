# Brainfork
A [Brainfuck](https://en.wikipedia.org/wiki/Brainfuck) interpreter and transpiler in C. 

## Compiled option ⚠️
Compiled option: -c  
Not available yet. The idea is to compile the program into assembly.

## Interpreted option ✅ 
Interpreted option: -i  
The program will read the contents of your Brainfuck program and print the output on the console.

## Transpiled option ✅ 
Transpiled option: -t  
The program will be translated into C code will try generating an executable. It will look for GCC, CLANG, and MSVC compilers. The transpiler optimizes the increment and move calls to reduce the number of instructions.

## Interactive run ✅
You may run Brainfuck interactively in the console. You can input these special commands on a new line:
* x | X : Quit
* r | R : Erase memory
* s | S : Reset cursor
* ! : Dump memory
* ,_ : Write char at cursor
* , : Erase memory at cursor

## Behavior
Brainfork allows in-text comments without needing an indicator (#, //, etc.); write your comments wherever you want. However, avoid using the period ( . ) as punctuation.  See the `program.bf` file for an example of a comment.

## Limitations
The original Brainfuck language supports the comma operator ( , ) to provide a character input; excepted in interactive mode, brainfork does not support that and it will be treated as part of a comment.

## Demo
To interpret Brainfuck
```console
$ brainfork -i program.bf
Hello, World!
```
To transpile Brainfuck and generate an executable
```console
$ brainfork -t program.bf
$ program
Hello, World!
```
To run Brainfuck interactively
```console
$ brainfork
# ,h
# >
# ,a
# <.>.<.>.
haha
```

## Environment
This was made to be as portable as possible on modern machines with C89 or later standards. However it was only built and tested with GCC.


