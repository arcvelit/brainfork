#pragma once

#include <stdint.h>

// Tokens
typedef enum 
{
    TOKEN_INCREMENT,
    TOKEN_DECREMENT,
    TOKEN_BRACKET_OPEN,
    TOKEN_BRACKET_CLOSED,
    TOKEN_PERIOD,
    TOKEN_MOVE_RIGHT,
    TOKEN_MOVE_LEFT,
    TOKEN_LINE_INDICATOR
} TokenType;


typedef struct 
{
    TokenType _type;
    char _char;
} Token;