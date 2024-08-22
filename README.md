# Brainfork
A Brainf*ck compiler, interpreter, and transpiler in C.

## How to use it
1. `cd src`
2. `.\make`
3. `.\Brainfork <option: -c -i -t> program.bf`

## Compiled option
Compiled option: -c  
Coming soon!

## Interpreted option
Interpreted option: -i  
The program will read the contents of you brainf*ck program and print the output on the console.

## Transpiled option
Transpiled option: -t  
The program will create a C file with your transpiled program and generate an executable with it. The transpiler optimizes the increment and move calls to reduce the number of instructions.

## Behavior
The interpreter allows in-text comments without the need of an indicator (#, //, etc.); write your comments wherever you want. Though, avoid using "." as punctuation... 

## Environment
This was made for windows, using the 32bit gcc compiler. Other versions may work.

## Demo
