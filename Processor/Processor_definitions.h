#ifndef SPU_DEFINES
#define SPU_DEFINES

enum machine_code {

    CMD_HLT = 0x0,
    CMD_PUSH = 0x1,
    CMD_POP = 0x2,

    CMD_ADD = 0x3,
    CMD_SUB = 0x4,
    CMD_MUL = 0x5,
    CMD_DIV = 0x6,

    CMD_OUT = 0x7,
    CMD_JB = 0x8,

    CMD_AX = 0x1,
    CMD_BX = 0x2,
    CMD_CX = 0x3,
    CMD_DX = 0x4,

    CMD_CTRL_BITS_001 = 0x1,
    CMD_CTRL_BITS_010 = 0x2,
    CMD_CTRL_BITS_011 = 0x3,
    CMD_CTRL_BITS_101 = 0x5,
    CMD_CTRL_BITS_110 = 0x6,
    CMD_CTRL_BITS_111 = 0x7,

    NUMBER_MASK = 0x1,
    REGISTER_MASK = 0x2,
    RAM_MASK = 0x4

};

#include "Stack.h"
const size_t Registers_amount = 17;
const uint64_t RAM_Size = 1000;

struct SPU_data {

    int* cmd_code;
    size_t code_size;
    int IP;
    Stack_t stack;
    int registers[Registers_amount];
    int* RAM;
};

#endif
