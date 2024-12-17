#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "instructions.h"

#define MEMORY_BUFFER_CAP 1024
#define USAGE "USAGE: brainfork <option: -c -i -t> <filename>"

#define BF_UNUSED(VAR) (void)(VAR)

#define BF_GCC_COMPILE_FMT(f) "gcc %s.c -o %s", f, f
#define BF_CLANG_COMPILE_FMT(f) "clang %s.c -o %s", f, f
#define BF_MSVC_COMPILE_FMT(f) "cl %s.c /Fe%s", f, f

#if defined(_WIN32) || defined(_WIN64)
    #define FIND_COMMAND(cmd) "where " cmd
#else
    #define FIND_COMMAND(cmd) "command -v " cmd
#endif

#define BF_WRITE_COMMAND_BUF(BUF, FMT) snprintf(BUF, sizeof(BUF), FMT)

#define BF_MEM_ASSERT(PTR)                           \
    do {                                             \
        if (!(PTR)) {                                \
            fprintf(stderr, "FATAL: Out of memory"); \
            exit(EXIT_FAILURE);                      \
        }                                            \
    } while (0)

typedef char byte;

struct { size_t no_instructions;} stats = {0};


// Takes a `file.abc` and returns new string `file.[ext]`
char* strdup_dot_x(const char* _cstr, char ext) {
    if (!_cstr) return NULL; 

    char* last_dot = strrchr(_cstr, '.');
    if (last_dot++ && *last_dot != '\0')
    {
        int offset = last_dot - _cstr;
        char* new_str = strdup(_cstr);
        BF_MEM_ASSERT(new_str);

        new_str[offset++] = ext;
        new_str[offset]   = '\0';
        return new_str;
    }
    
    return NULL;
}

// Takes a `file.abc` and returns new string `file`
char* strdup_dot_x_strip(const char* _cstr) {
    if (!_cstr) return NULL;

    char* last_dot = strrchr(_cstr, '.');
    if (last_dot) {
        char* new_str = strdup(_cstr);
        BF_MEM_ASSERT(new_str);

        int offset = last_dot - _cstr;
        new_str[offset] = '\0';
        return new_str;
    }

    return NULL;
}

void push_loop_stack(LoopStack* loop_stack, Instruction instruction)
{
    loop_stack->stack[loop_stack->pointer++] = instruction;
}

Instruction pop_loop_stack(LoopStack* loop_stack)
{
    return loop_stack->stack[--loop_stack->pointer];
}

void allocate_loop_stack(LoopStack* loop_stack, size_t stack_size)
{
    loop_stack->stack = malloc(sizeof(Instruction) * stack_size);
    loop_stack->pointer = 0;
}

void free_loop_stack(LoopStack* loop_stack)
{
    free(loop_stack->stack);
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
        fprintf(stderr, "WARNING: Unnecessary argument '%s'\n", argv[3]);
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
        fprintf(stderr, "ERROR: Could not open `%s`", argv[2]);
        exit(EXIT_FAILURE);
    }

    // Identify name
    *file_name = argv[2]; 

    return file;
}

void parse_to_instructions(Instruction* instruction_buff, FILE* file, const size_t file_length)
{
    // Loop stack holds opening brackets
    LoopStack loop_stack;
    allocate_loop_stack(&loop_stack, file_length / 2 + 1);
    char c; 

    while ((c = fgetc(file)) != EOF)
    {   
        Instruction instruction = {0};
        instruction._char = c;

        switch (instruction._char)
        {
            case '+':
                instruction._op = OP_INCREMENT;
                break;
            case '-':
                instruction._op = OP_DECREMENT;
                break;
            case '[':
                instruction._op = OP_BRACKET_OPEN;
                instruction._position = stats.no_instructions;
                push_loop_stack(&loop_stack, instruction);
                break;
            case ']':
                instruction._op = OP_BRACKET_CLOSED;
                if (loop_stack.pointer == 0)
                {
                    fprintf(stderr, "ERROR: Misaligned loop brackets"); \
                    exit(EXIT_FAILURE);
                }
                Instruction i = pop_loop_stack(&loop_stack);
                instruction._point_to = i._position;
                break;
            case '.':
                instruction._op = OP_PRINT;
                break;
            case '>':
                instruction._op = OP_MOVE_RIGHT;
                break;
            case '<':
                instruction._op = OP_MOVE_LEFT;
                break;
            case '\n': 
            case '\r':
            case '\t':
            case  ' ':
            default: // Allow inline comments 
                continue;
        }

        *instruction_buff++ = instruction;

        stats.no_instructions++;
    }

    Instruction t = {0};
    t._op = OP_PROGRAM_TERMINATE;
    *instruction_buff = t;
    stats.no_instructions++;

    free_loop_stack(&loop_stack);
}

void run_interpreter(Instruction* instruction_buffer)
{
    byte memory_buffer[MEMORY_BUFFER_CAP];
    memset(&memory_buffer, 0, MEMORY_BUFFER_CAP);

    /* Instruction */ size_t program_counter = 0;
    /* Memory      */ size_t stack_pointer   = 0;

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
                if (memory_buffer[stack_pointer] == 0) break;
                program_counter = instruction._point_to;
                continue;
            case OP_PRINT:
                putchar(memory_buffer[stack_pointer]);
                break;
            case OP_MOVE_RIGHT:
                stack_pointer = (stack_pointer + 1) % MEMORY_BUFFER_CAP;
                break;
            case OP_MOVE_LEFT:
                stack_pointer = (stack_pointer + MEMORY_BUFFER_CAP - 1) % MEMORY_BUFFER_CAP;
                break;
            case OP_PROGRAM_TERMINATE:
                return;
        }

        program_counter++;
    }
}

void run_compiler(Instruction* instruction_buffer, const char* file_name)
{
    BF_UNUSED(instruction_buffer); 
    BF_UNUSED(file_name);

    printf("ERROR: Compiler not implemented");
    exit(EXIT_FAILURE);
}

void fprintf_line(FILE* file, int indent_depth, const char* line)
{
    while (indent_depth-- > 0) fprintf(file, "\t");
    fprintf(file, "%s", line);
}

int is_compiler_available(const char* compiler_command) 
{
    FILE* fp = popen(compiler_command, "r");
    if (fp == NULL) 
    {
        return 0;
    }
    fclose(fp);
    return 1;
}

void exec_bf(char* file_name) 
{
    char command[256];
    
    if (is_compiler_available(FIND_COMMAND("gcc"))) {
        BF_WRITE_COMMAND_BUF(command, BF_GCC_COMPILE_FMT(file_name));
    } else if (is_compiler_available(FIND_COMMAND("clang"))) {
        BF_WRITE_COMMAND_BUF(command, BF_CLANG_COMPILE_FMT(file_name));
    } else if (is_compiler_available(FIND_COMMAND("cl"))) {
        BF_WRITE_COMMAND_BUF(command, BF_MSVC_COMPILE_FMT(file_name));
    } else {
        fprintf(stderr, "ERROR: No C compiler found on system");
        exit(EXIT_FAILURE);
    }
    
    system(command);
}

void run_transpiler(Instruction* instruction_buffer, const char* file_name)
{
    char* file_name_dot_c = strdup_dot_x(file_name, 'c');    
    FILE* file_c = fopen(file_name_dot_c, "w");
    free(file_name_dot_c);

    int indent_depth = 1;

    long mov_accumulator = 0;
    long inc_accumulator = 0;

    // Preprocessor and aliases
    fprintf(file_c, "#include <stdio.h>\n");
    fprintf(file_c, "#include <string.h>\n");
    fprintf(file_c, "#include <stdint.h>\n\n");
    fprintf(file_c, "#define MEMORY_BUFFER_CAP %d\n", MEMORY_BUFFER_CAP);
    fprintf(file_c, "#define sp_mov(sp, inc) sp = (sp + MEMORY_BUFFER_CAP + inc) %% MEMORY_BUFFER_CAP\n\n");
    fprintf(file_c, "typedef char byte;\n\n");

    // Start of main
    fprintf(file_c, "int main()\n{\n");

    fprintf_line(file_c, indent_depth, "// BEGIN PROGRAM\n");
    fprintf_line(file_c, indent_depth, "byte mbuff[MEMORY_BUFFER_CAP];\n");
    fprintf_line(file_c, indent_depth, "memset(&mbuff, 0, MEMORY_BUFFER_CAP);\n\n");

    fprintf_line(file_c, indent_depth, "size_t sp = 0;\n\n");

    for (size_t program_counter = 0; program_counter < stats.no_instructions; program_counter++)
    {
        Instruction instruction = instruction_buffer[program_counter];

        // Minimize increment instructions
        if (inc_accumulator != 0 && instruction._op != OP_INCREMENT && instruction._op != OP_DECREMENT)
        {
            fprintf_line(file_c, indent_depth, "mbuff[sp]+= ");
            fprintf(file_c, "%ld;\n", inc_accumulator);
            inc_accumulator = 0;
        }

        // Minimize increment instructions
        if (mov_accumulator != 0 && instruction._op != OP_MOVE_LEFT && instruction._op != OP_MOVE_RIGHT)
        {
            fprintf_line(file_c, indent_depth, "sp_mov(sp, ");
            fprintf(file_c, "%ld);\n", mov_accumulator);
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
            case OP_PROGRAM_TERMINATE:
                fprintf_line(file_c, indent_depth, "// EXIT PROGRAM\n");
                break;
        }
    }

    // End of main
    fprintf(file_c, "}\n");
    fclose(file_c);

    // Run compilation
    char* file_name_strip = strdup_dot_x_strip(file_name);
    exec_bf(file_name_strip);
    free(file_name_strip);
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
    const size_t file_length = ftell(file);
    fseek (file, 0, SEEK_SET);

    Instruction* instruction_buffer = malloc(sizeof(Instruction) * (file_length + 1));

    parse_to_instructions(instruction_buffer, file, file_length);
    fclose(file);

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