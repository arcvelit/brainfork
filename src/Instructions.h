#pragma once

#include <stdint.h>

typedef enum
{
    OPTION_COMPILE,
    OPTION_INTERPRET,
    OPTION_TRANSPILE
} Option;

typedef enum 
{
    OP_INCREMENT,
    OP_DECREMENT,
    OP_BRACKET_OPEN,
    OP_BRACKET_CLOSED,
    OP_PRINT,
    OP_MOVE_RIGHT,
    OP_MOVE_LEFT,
    OP_PROGRAM_TERMINATE
} Operation;

typedef struct 
{
    Operation _op;
    char _char;
    union 
    {
        uint64_t _point_to;
        uint64_t _position;
    };
} Instruction;

typedef struct 
{
    uint64_t pointer;
    Instruction* stack;
} LoopStack;
