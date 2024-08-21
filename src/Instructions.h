#pragma once

#include <stdint.h>

typedef enum
{
    OPTION_COMPILE,
    OPTION_INTERPRET
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