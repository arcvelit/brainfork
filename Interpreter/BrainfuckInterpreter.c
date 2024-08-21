#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tokens.h"

#define MEMORY_BUFFER_CAP 1024

typedef char byte;


void print_buffer(char* buff, const size_t len)
{
    const byte* end = buff + len;
    while (buff < end)
        putchar(*buff++);
}

FILE* get_file_handle(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("error: must provide a source file");
        exit(EXIT_FAILURE);
    }
    else if (argc > 2)
    {
        printf("warning: unnecessary argument '%s'\n", argv[2]);
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL) 
    {
        printf("error: could not open %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    return file;
}

void parse_to_tokens(Token* token_buff, FILE* file, const size_t len)
{
    char c; 

    while ((c = fgetc(file)) != EOF)
    {   
        Token token; 
        token._char = c;

        switch (token._char)
        {
            case '+':
                token._type = TOKEN_INCREMENT;
                break;
            case '-':
                token._type = TOKEN_DECREMENT;
                break;
            case '[':
                token._type = TOKEN_BRACKET_OPEN;
                break;
            case ']':
                token._type = TOKEN_BRACKET_CLOSED;
                break;
            case '.':
                token._type = TOKEN_PERIOD;
                break;
            case '>':
                token._type = TOKEN_MOVE_RIGHT;
                break;
            case '<':
                token._type = TOKEN_MOVE_LEFT;
                break;
            // For portability :p
            case '\n': 
            case '\r':
                token._type = TOKEN_LINE_INDICATOR;
                break; 
            default:
                printf("error: unsupported token '%s'", token._char);
                exit(EXIT_FAILURE);
        }

        *token_buff++ = token;
    }
}


int main(int argc, char *argv[])
{
    FILE* file = get_file_handle(argc, argv);

    byte memory_buffer[MEMORY_BUFFER_CAP];
    memset(&memory_buffer, 0, MEMORY_BUFFER_CAP);

    fseek (file, 0, SEEK_END);
    const uint32_t program_length = ftell(file);
    fseek (file, 0, SEEK_SET);

    Token* token_buffer = malloc(sizeof(Token) * program_length);

    parse_to_tokens(token_buffer, file, program_length);
    fclose(file);

    //for (int i = 0; i < program_length; i++)
    //    putchar(token_buffer[i]._char);
    
    //memcpy(&memory_buffer, &operation_buffer, op_buffer_length);
    //print_buffer(memory_buffer, MEMORY_BUFFER_CAP);
    //print_buffer(operation_buffer, op_buffer_length);

    free(token_buffer);
}