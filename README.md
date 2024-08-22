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
The program will create a C file with your transpiled program and generate an executable with the same name as your input file. The transpiler optimizes the increment and move calls to reduce the number of instructions.

## Behavior
Brainfork allows in-text comments without needing an indicator (#, //, etc.); write your comments wherever you want. However, avoid using the period ( . ) as punctuation.  See the `program.bf` file for an example of a comment.

## Limitations
The original Brainfuck language supports the comma operator ( , ) to provide a character input; brainfork does not support that yet, it will be treated as part of a comment.

## Demo
First, compile brainfork into an executable.
```sh
$ cd src
$ make
```
To interpret Brainfuck
```sh
$ brainfork -i program.bf
Hello, World!
```
To transpile Brainfuck and generate an executable
```sh
$ brainfork -t program.bf
$ program
Hello, World!
```
You do not have to use the `src/build` batch file at all, but it is **necessary to compile programs**.

## Environment
This was made with Windows in mind, using the 32-bit gcc compiler. There are no calls or conventions specific to Windows in the code, but it is not impossible to encounter compilation errors somewhere under different software conditions.


