#ifndef CDEF
#define CDEF

COMMANDS_DEF({ "HLT",  .command_num = CMD_HLT,  .args_amount = 0, SPU_Hlt)},
COMMANDS_DEF({ "PUSH", .command_num = CMD_PUSH, .args_amount = 3, SPU_Push)},
COMMANDS_DEF({ "POP",  .command_num = CMD_POP,  .args_amount = 3, SPU_Pop)},
COMMANDS_DEF({ "ADD",  .command_num = CMD_ADD,  .args_amount = 0, SPU_Add)},
COMMANDS_DEF({ "SUB",  .command_num = CMD_SUB,  .args_amount = 0, SPU_Sub)},
COMMANDS_DEF({ "MUL",  .command_num = CMD_MUL,  .args_amount = 0, SPU_Mul)},
COMMANDS_DEF({ "DIV",  .command_num = CMD_DIV,  .args_amount = 0, SPU_Div)},
COMMANDS_DEF({ "OUT",  .command_num = CMD_OUT,  .args_amount = 0, SPU_Out)},
COMMANDS_DEF({ "JB",   .command_num = CMD_JB,   .args_amount = 1, SPU_Jb)}

#endif
