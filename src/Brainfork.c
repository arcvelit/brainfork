#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "instructions.h"

#define MEMORY_BUFFER_CAP 1024
#define USAGE "error: usage is <option: -c -i -t> <filename>"

typedef char byte;

struct { uint64_t no_instructions;} stats = {0};



const char* string_dot_x_ext(const char* str, char ext) {
    char* new_str = strdup(str);
    char* last_dot = strrchr(new_str, '.');
    if (last_dot++) 
    {
        *last_dot++ = ext;
        *last_dot = '\0';
    }
    return new_str;
}

void push_loop_stack(Instruction** loop_stack, Instruction* instruction, uint64_t* loop_stack_pointer)
{
    loop_stack[(*loop_stack_pointer)++] = instruction;
}

Instruction* pop_loop_stack(Instruction** loop_stack, uint64_t* loop_stack_pointer)
{
    return loop_stack[--(*loop_stack_pointer)];
}

FILE* get_program_info(int argc, char* argv[], Option* option, const char** file_name)
{
    if (argc < 2)
    {
        printf(USAGE);
        exit(EXIT_FAILURE);
    }
    else if (argc > 3)
    {
        printf("warning: unnecessary argument '%s'\n", argv[3]);
    }

    // Get option
    if (strcmp(argv[1], "-c") == 0)
    {
        *option = OPTION_COMPILE;
    }
    else if (strcmp(argv[1], "-i") == 0)
    {
        *option = OPTION_INTERPRET;
    }
    else if (strcmp(argv[1], "-t") == 0)
    {
        *option = OPTION_TRANSPILE;
    }
    else 
    {
        printf(USAGE);
        exit(EXIT_FAILURE);
    }

    // Get file handle
    FILE *file = fopen(argv[2], "r");

    if (file == NULL) 
    {
        printf("error: could not open %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    // Identify name
    *file_name = argv[2]; 

    return file;
}

void parse_to_instructions(Instruction* instruction_buff, FILE* file, const size_t file_length)
{
    // Loop stack holds opening brackets
    Instruction** loop_stack = malloc(sizeof(Instruction*) * (file_length / 2 + 1));
    uint64_t loop_stack_pointer = 0;
    char c; 

    while ((c = fgetc(file)) != EOF)
    {   
        Instruction* instruction = malloc(sizeof(Instruction)); 
        instruction->_char = c;

        switch (instruction->_char)
        {
            case '+':
                instruction->_op = OP_INCREMENT;
                break;
            case '-':
                instruction->_op = OP_DECREMENT;
                break;
            case '[':
                instruction->_op = OP_BRACKET_OPEN;
                instruction->_position = stats.no_instructions;
                push_loop_stack(loop_stack, instruction, &loop_stack_pointer);
                break;
            case ']':
                instruction->_op = OP_BRACKET_CLOSED;
                if (loop_stack_pointer == 0)
                {
                    printf("error: misaligned loop brackets");
                    exit(EXIT_FAILURE);
                }
                Instruction* i = pop_loop_stack(loop_stack, &loop_stack_pointer);
                instruction->_point_to = i->_position;
                break;
            case '.':
                instruction->_op = OP_PRINT;
                break;
            case '>':
                instruction->_op = OP_MOVE_RIGHT;
                break;
            case '<':
                instruction->_op = OP_MOVE_LEFT;
                break;
            case '\n': 
            case '\r':
            case '\t':
            case  ' ':
            default: // Allow inline comments 
                continue;
        }

        *instruction_buff++ = *instruction;
        free(instruction);

        stats.no_instructions++;
    }

    free(loop_stack);
}

void run_interpreter(Instruction* instruction_buffer)
{
    byte memory_buffer[MEMORY_BUFFER_CAP];
    memset(&memory_buffer, 0, MEMORY_BUFFER_CAP);

    /* Instruction */ uint64_t program_counter = 0;
    /* Memory      */ uint64_t stack_pointer   = 0;

    while(program_counter < stats.no_instructions)
    {
        Instruction instruction = instruction_buffer[program_counter];

        switch (instruction._op)
        {
            case OP_INCREMENT:
                memory_buffer[stack_pointer]++;
                break;
            case OP_DECREMENT:
                memory_buffer[stack_pointer]--;
                break;
            case OP_BRACKET_OPEN:
                break;
            case OP_BRACKET_CLOSED:
                if (memory_buffer[stack_pointer] != 0)
                {
                    program_counter = instruction._point_to;
                    continue;
                }
                break;
            case OP_PRINT:
                putchar(memory_buffer[stack_pointer]);
                break;
            case OP_MOVE_RIGHT:
                stack_pointer = (stack_pointer + 1) % MEMORY_BUFFER_CAP;
                break;
            case OP_MOVE_LEFT:
                stack_pointer = (stack_pointer + MEMORY_BUFFER_CAP - 1) % MEMORY_BUFFER_CAP;
                break;
        }

        program_counter++;
    }
}

void run_compiler(Instruction* instruction_buffer, const char* file_name)
{
    printf("error: compiler not available yet");
    exit(EXIT_FAILURE);
    /*
        char command[512];
        snprintf(command, sizeof(command),
            "BuildNasm \"%s\"", "program.asm");

        int result = system(command);
    */
}

void fprintf_line(FILE* file, uint32_t indent_depth, const char* line)
{
    while (indent_depth-- > 0) fprintf(file, "\t");
    fprintf(file, "%s", line);
}

void run_transpiler(Instruction* instruction_buffer, const char* file_name)
{
    const char* file_name_dot_c = string_dot_x_ext(file_name, 'c');
    
    FILE* file_c = fopen(file_name_dot_c, "w");

    uint32_t indent_depth = 1;

    int64_t mov_accumulator = 0;
    int64_t inc_accumulator = 0;

    // Preprocessor and aliases
    fprintf(file_c, "#include <stdio.h>\n");
    fprintf(file_c, "#include <string.h>\n");
    fprintf(file_c, "#include <stdint.h>\n\n");
    fprintf(file_c, "#define MEMORY_BUFFER_CAP %d\n\n", MEMORY_BUFFER_CAP);
    fprintf(file_c, "typedef char byte;\n\n");

    // Functions
    fprintf(file_c, "uint64_t sp_mov(uint64_t* sp, uint64_t move)\n{\n\t*sp = (*sp + MEMORY_BUFFER_CAP + move) %% MEMORY_BUFFER_CAP;\n}\n\n");

    // Start of main
    fprintf(file_c, "int main()\n{\n");

    fprintf_line(file_c, indent_depth, "byte mbuff[MEMORY_BUFFER_CAP];\n");
    fprintf_line(file_c, indent_depth, "memset(&mbuff, 0, MEMORY_BUFFER_CAP);\n\n");

    fprintf_line(file_c, indent_depth, "uint64_t sp = 0;\n\n");

    for (uint64_t program_counter = 0; program_counter < stats.no_instructions; program_counter++)
    {
        Instruction instruction = instruction_buffer[program_counter];

        // Minimize increment instructions
        if (inc_accumulator != 0 && instruction._op != OP_INCREMENT && instruction._op != OP_DECREMENT)
        {
            fprintf_line(file_c, indent_depth, "mbuff[sp]+= ");
            fprintf(file_c, "%d;\n", inc_accumulator);
            inc_accumulator = 0;
        }

        // Minimize increment instructions
        if (mov_accumulator != 0 && instruction._op != OP_MOVE_LEFT && instruction._op != OP_MOVE_RIGHT)
        {
            fprintf_line(file_c, indent_depth, "sp_mov(&sp, ");
            fprintf(file_c, "%d);\n", mov_accumulator);
            mov_accumulator = 0;
        }
        
        switch (instruction._op)
        {
            case OP_INCREMENT:
                inc_accumulator++;
                break;
            case OP_DECREMENT:
                inc_accumulator--;
                break;
            case OP_BRACKET_OPEN:
                fprintf_line(file_c, indent_depth++, "while(mbuff[sp]) {\n");
                break;
            case OP_BRACKET_CLOSED:
                fprintf_line(file_c, --indent_depth, "}\n");
                break;
            case OP_PRINT:
                fprintf_line(file_c, indent_depth, "putchar(mbuff[sp]);\n");
                break;
            case OP_MOVE_RIGHT:
                mov_accumulator++;
                break;
            case OP_MOVE_LEFT:
                mov_accumulator--;
                break;
        }
    }

    // End of main
    fprintf(file_c, "}\n");
    fclose(file_c);

    // Call batch script 
    char command[512];
    snprintf(command, sizeof(command), "build -t \"%s\"", file_name_dot_c);
    system(command);
}

int main(int argc, char *argv[])
{
    // Setup instructions buffer
    Option option;
    const char* file_name;
    FILE* file = get_program_info(argc, argv, &option, &file_name);

    // Instruction buffer capacity may not be filled
    // use stats.no_instructions for its size
    fseek (file, 0, SEEK_END);
    const uint64_t file_length = ftell(file);
    fseek (file, 0, SEEK_SET);

    Instruction* instruction_buffer = malloc(sizeof(Instruction) * file_length);

    parse_to_instructions(instruction_buffer, file, file_length);
    fclose(file);

    /* TODO
     * make the compiler
     * 
     */

    switch (option)
    {
    case OPTION_INTERPRET:
        run_interpreter(instruction_buffer);
        break;
    case OPTION_COMPILE:
        run_compiler(instruction_buffer, file_name);
        break;
    case OPTION_TRANSPILE:
        run_transpiler(instruction_buffer, file_name);
        break;
    }

    free(instruction_buffer);
}